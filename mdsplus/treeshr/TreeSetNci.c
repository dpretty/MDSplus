/*------------------------------------------------------------------------------

		Name: TreeSetNci

		Type:   C function

		Author:	Josh Stillerman
			MIT Plasma Fusion Center

		Date:   19-MAY-1988

		Purpose: Set the Characteristics of a Node.

------------------------------------------------------------------------------

	Call sequence:

  int TreeSetNci(int nid, NCI_ITM *nci_itm_ptr)
  int TreeFlushOff( int nid)
  int TreeFlushReset( int nid)

------------------------------------------------------------------------------
   Copyright (c) 1988
   Property of Massachusetts Institute of Technology, Cambridge MA 02139.
   This program cannot be copied or distributed in any form for non-MIT
   use without specific written approval of MIT Plasma Fusion Center
   Management.
---------------------------------------------------------------------------

	Description:


+-----------------------------------------------------------------------------*/
#ifdef _WIN32
#include <windows.h>
#include <io.h>
#endif
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <treeshr.h>
#include "treeshrp.h"
#include <ncidef.h>
#ifndef O_BINARY
#define O_BINARY 0
#endif
#ifndef O_RANDOM
#define O_RANDOM 0
#endif

static char *cvsrev = "@(#)$RCSfile: TreeSetNci.c,v $ $Revision: 1.25 $ $Date: 2000/12/01 14:32:56 $";

extern void *DBID;

extern int SetNciRemote();
extern int TreeFlushOffRemote();
extern int TreeFlushResetRemote();
extern int TreeTurnOnRemote();
extern int TreeTurnOffRemote();

int TreeSetNci(int nid, NCI_ITM *nci_itm_ptr) { return _TreeSetNci(DBID, nid, nci_itm_ptr);}
int TreeFlushOff(int nid) { return _TreeFlushOff(DBID, nid);}
int TreeFlushReset(int nid) { return _TreeFlushReset(DBID, nid);}
int TreeTurnOn(int nid) { return _TreeTurnOn(DBID, nid);}
int TreeTurnOff(int nid) { return _TreeTurnOff(DBID, nid);}

int TreeGetNciLw(TREE_INFO *info, int node_num, NCI *nci);

static int SetNodeParentState(PINO_DATABASE *db, NODE *node, NCI *nci, unsigned int state);

int       _TreeSetNci(void *dbid, int nid_in, NCI_ITM *nci_itm_ptr)
{
  PINO_DATABASE *dblist = (PINO_DATABASE *)dbid;
  NID *nid_ptr = (NID *)&nid_in;
  int       status;
  int       node_number;
  TREE_INFO *tree_info;
  NCI_ITM  *itm_ptr;
  NCI       nci;
/*------------------------------------------------------------------------------

 Executable:
*/
  if (!(IS_OPEN(dblist)))
    return TreeNOTOPEN;
  if (dblist->open_readonly)
      return TreeREADONLY;
  if (dblist->remote)
    return SetNciRemote(dbid,nid_in,nci_itm_ptr);
  nid_to_tree_nidx(dblist, nid_ptr, tree_info, node_number);
  if (!tree_info)
    return TreeNNF;
  if (tree_info->reopen)
    TreeCloseFiles(tree_info);
  status = TreeGetNciLw(tree_info, node_number, &nci);
  if (status & 1)
  {
    for (itm_ptr = nci_itm_ptr; itm_ptr->code != NciEND_OF_LIST && status & 1; itm_ptr++)
    {
      switch (itm_ptr->code)
      {

       case NciSET_FLAGS:
	nci.flags |= *(unsigned int *) itm_ptr->pointer;
	break;
       case NciCLEAR_FLAGS:
	nci.flags &= ~(*(unsigned int *) itm_ptr->pointer);
	break;
       case NciSTATUS:
	nci.status = *(unsigned int *) itm_ptr->pointer;
	break;
       default:
	status = TreeILLEGAL_ITEM;
	break;
      }
    }
    if (status & 1)
      status = TreePutNci(tree_info, node_number, &nci, 1);
  }
  return status;
}

int TreeSetNciItm(int nid, int code, int value)
{
  NCI_ITM itm[] = {{0,0,0,0},{0,0,0,0}};
  itm[0].buffer_length = (short)sizeof(int);
  itm[0].code = (short)code;
  itm[0].pointer = (void *)&value;
  return TreeSetNci(nid, itm);
}

int _TreeFlushOff(void *dbid, int nid)
{
  PINO_DATABASE *dblist = (PINO_DATABASE *)dbid;
  NID *nid_ptr = (NID *)nid;
  int       node_number;
  TREE_INFO *tree_info;
  if (!(IS_OPEN(dblist)))
    return TreeNOTOPEN;
  if (dblist->remote)
    return TreeFlushOffRemote(dbid,nid);
  nid_to_tree_nidx(dblist, nid_ptr, tree_info, node_number);
  if (!tree_info)
    return TreeNNF;
  tree_info->flush=0;
  return TreeNORMAL;
}

int _TreeFlushReset(void *dbid,  int nid)
{
  PINO_DATABASE *dblist = (PINO_DATABASE *)dbid;
  NID *nid_ptr = (NID *)nid;
  int       node_number;
  TREE_INFO *tree_info;
  if (!(IS_OPEN(dblist)))
    return TreeNOTOPEN;
  if (dblist->remote)
    return TreeFlushResetRemote(dbid,nid);
  nid_to_tree_nidx(dblist, nid_ptr, tree_info, node_number);
  if (!tree_info)
    return TreeNNF;
  if ((tree_info->flush == 0) && (dblist->shotid == -1)) {
    tree_info->flush = 1;
    TreeWait(tree_info);
  }
  return TreeNORMAL;
}   

/*------------------------------------------------------------------------------

		Name: TreeGetNciLw

		Type:   C function

		Author:	Josh Stillerman
			MIT Plasma Fusion Center

		Date:    22-MAR-1988

		Purpose: Routine which returns an characteristics record for a node
			locking the record so that it can be updated.

------------------------------------------------------------------------------

	Call sequence:
	       STATUS = TreeGetNciLw(info, node_num, nci)

------------------------------------------------------------------------------
   Copyright (c) 1988
   Property of Massachusetts Institute of Technology, Cambridge MA 02139.
   This program cannot be copied or distributed in any form for non-MIT
   use without specific written approval of MIT Plasma Fusion Center
   Management.
---------------------------------------------------------------------------

	Description:


+-----------------------------------------------------------------------------*/

int TreeGetNciLw(TREE_INFO *info, int node_num, NCI *nci)
{
  int       status;
  status = TreeNORMAL;

/******************************************
  If the tree is not open for edit then
  if the characteristic file is not open
  (or not open for write)
  open the characteristic file for writting
  if OK so far then
  fill in the rab and read the record
******************************************/
  if ((info->edit == 0) || (node_num < info->edit->first_in_mem))
  {
    if ((info->nci_file == 0) || (info->nci_file->put == 0))
    status = TreeOpenNciW(info, 0);
    if (status & 1)
    {
      status = TreeLockNci(info,0,node_num);
      if (status & 1)
      {
        char nci_bytes[42];
#ifdef _WIN32
	_lseek(info->nci_file->put,node_num * sizeof(nci_bytes),SEEK_SET);
	status = (_read(info->nci_file->put,nci_bytes,sizeof(nci_bytes)) == sizeof(nci_bytes)) ? TreeNORMAL : TreeFAILURE;
#else
	lseek(info->nci_file->put,node_num * sizeof(nci_bytes),SEEK_SET);
	status = (read(info->nci_file->put,nci_bytes,sizeof(nci_bytes)) == sizeof(nci_bytes)) ? TreeNORMAL : TreeFAILURE;
#endif
        if (status == TreeNORMAL)
          TreeSerializeNciIn(nci_bytes,nci);
	if (!(status & 1))
          TreeUnLockNci(info,0,node_num);
      }
    }
  }
  else
  {
  /********************************************
   Otherwise the tree is open for edit so
   the characteristics are just a memory reference
   away.
  *********************************************/

    memcpy(nci, info->edit->nci + node_num - info->edit->first_in_mem, sizeof(struct nci));
  }

  return status;
}

/*------------------------------------------------------------------------------

		Name: TreeOpenNciW

		Type:   C function

		Author:	Josh Stillerman
			MIT Plasma Fusion Center

		Date:   22-MAR-1988

		Purpose: Open a tree characteristics file for write access.

------------------------------------------------------------------------------

	Call sequence:
		       status = TreeOpenNciW(info_ptr)

------------------------------------------------------------------------------
   Copyright (c) 1988
   Property of Massachusetts Institute of Technology, Cambridge MA 02139.
   This program cannot be copied or distributed in any form for non-MIT
   use without specific written approval of MIT Plasma Fusion Center
   Management.
---------------------------------------------------------------------------

	Description:


+-----------------------------------------------------------------------------*/
int TreeOpenNciW(TREE_INFO *info, int tmpfile)
{
  int       status;

/****************************************************
  If there is no characterisitics file block allocated then
    Allocate one
*****************************************************/

  if (info->nci_file == 0)
  {
    status = ((info->nci_file = (struct nci_file *)malloc(sizeof(NCI_FILE))) != NULL) ? TreeNORMAL : TreeFAILURE;
    if (status & 1)
    {
      size_t len = strlen(info->filespec)-4;
      char *filename = strncpy(malloc(len+20),info->filespec,len);
      filename[len]='\0';
      strcat(filename,tmpfile ? "characteristics#" : "characteristics");
      memset(info->nci_file,0, sizeof(NCI_FILE));
      info->nci_file->get = open(filename,(tmpfile ? O_RDWR | O_CREAT | O_TRUNC  : O_RDONLY) | O_BINARY | O_RANDOM,0664);
      status = (info->nci_file->get == -1) ? TreeFAILURE : TreeNORMAL;
      if (info->nci_file->get == -1)
        info->nci_file->get = 0;
      if (status & 1)
      {
#ifdef vxWorks
        info->nci_file->put = open(filename,O_RDWR | O_BINARY | O_RANDOM, 0);
#else
        info->nci_file->put = open(filename,O_RDWR | O_BINARY | O_RANDOM);
#endif
        status = (info->nci_file->put == -1) ? TreeFAILURE : TreeNORMAL;
        if (info->nci_file->put == -1)
          info->nci_file->put = 0;
      }
      free(filename);
    }
  }
  else
  {
  /*******************************************
    Else the file was open for read access so
    close it and re-open it for write access.
  *******************************************/
    size_t len = strlen(info->filespec)-4;
    char *filename = strncpy(malloc(len+20),info->filespec,len);
    filename[len]='\0';
    strcat(filename,tmpfile ? "characteristics#" : "characteristics");
    if (info->nci_file->put)
      close(info->nci_file->put);
    info->nci_file->put = open(filename,(tmpfile ? O_RDWR | O_CREAT | O_TRUNC : O_RDWR) | O_BINARY | O_RANDOM, 0664);
    status = (info->nci_file->put == -1) ? TreeFAILURE : TreeNORMAL;
    if (info->nci_file->put == -1)
      info->nci_file->put = 0;
    free(filename);
  }
  if (status & 1)
  {
    if (info->edit)
    {
    info->edit->first_in_mem = lseek(info->nci_file->put,0,SEEK_END)/42;
    }
  /**********************************************
   Set up the RABs for buffered reads and writes
   and CONNECT it.
   If there is a problem then close it, free the
   memory and return.
  **********************************************/
  }
  else
    {
      free(info->nci_file);
      info->nci_file = NULL;
    }
  if (status & 1)
    TreeCallHook(OpenNCIFileWrite, info);
  return status;
}

/*------------------------------------------------------------------------------

		Name: TreePutNci

		Type:   C function

		Author:	Josh Stillerman
			MIT Plasma Fusion Center

		Date:    22-MAR-1988

		Purpose: Routine which puts an attribute record into a
			tree.

------------------------------------------------------------------------------

	Call sequence:
	       STATUS = TreePutNci(info_ptr, node_num, nci_ptr, flush)

------------------------------------------------------------------------------
   Copyright (c) 1988
   Property of Massachusetts Institute of Technology, Cambridge MA 02139.
   This program cannot be copied or distributed in any form for non-MIT
   use without specific written approval of MIT Plasma Fusion Center
   Management.
---------------------------------------------------------------------------

	Description:


+-----------------------------------------------------------------------------*/

int TreePutNci(TREE_INFO *info, int node_num, NCI *nci, int flush)
{
  int       status;
  status = TreeNORMAL;
/***************************************
  If the tree is not open for edit
****************************************/

  if ((info->edit == 0) ||
      (node_num < info->edit->first_in_mem))
  {

  /**********************
   Update the NCI record
  ***********************/

    status = TreeLockNci(info, 0, node_num);
    if (status & 1)
    {
      char nci_bytes[42];
      TreeSerializeNciOut(nci,nci_bytes);
#ifdef _WIN32
      _lseek(info->nci_file->put,sizeof(nci_bytes) * node_num, SEEK_SET);
      status = (_write(info->nci_file->put,nci_bytes,sizeof(nci_bytes)) == sizeof(nci_bytes)) ? TreeNORMAL : TreeFAILURE;
#else
      lseek(info->nci_file->put,sizeof(nci_bytes) * node_num, SEEK_SET);
      status = (write(info->nci_file->put,nci_bytes,sizeof(nci_bytes)) == sizeof(nci_bytes)) ? TreeNORMAL : TreeFAILURE;
#endif
      TreeUnLockNci(info, 0, node_num);
    }
  }

/****************************
  Else it is open for edit so
  the characteristics are just a
  memory reference away.
*****************************/

  else
    memcpy(info->edit->nci + (node_num - info->edit->first_in_mem),nci,sizeof(nci));
  return status;
}

/*------------------------------------------------------------------------------

		Name: TreeTurnOn

		Type:   C function

		Author:	Josh Stillerman
			MIT Plasma Fusion Center

		Date:   28-MAR-1988

		Purpose: Turn on a sub tree.

------------------------------------------------------------------------------

	Call sequence:
			int TreeTurnOn()
			status = TreeTurnOn(nid);

------------------------------------------------------------------------------
   Copyright (c) 1988
   Property of Massachusetts Institute of Technology, Cambridge MA 02139.
   This program cannot be copied or distributed in any form for non-MIT
   use without specific written approval of MIT Plasma Fusion Center
   Management.
---------------------------------------------------------------------------

	Description:


+-----------------------------------------------------------------------------*/

int _TreeTurnOn(void *dbid, int nid_in)
{
  PINO_DATABASE *dblist = (PINO_DATABASE *)dbid;
  NID *nid = (NID *)&nid_in;
  int       status;
  int       node_num;
  TREE_INFO *info;
  NCI       nci;
  NODE     *node;
  if (!(IS_OPEN(dblist)))
    return TreeNOT_OPEN;
  if (dblist->remote)
    return TreeTurnOnRemote(dbid,nid_in);
  nid_to_tree_nidx(dblist, nid, info, node_num);
  if (!info)
    return TreeNNF;
  status = TreeGetNciLw(info, node_num, &nci);
  if (~status & 1)
    return status;
  if (nci.flags & NciM_STATE)
  {
    bitassign(0,nci.flags,NciM_STATE);
    status = TreePutNci(info, node_num, &nci, 0);
    if (~status & 1)
      return status;
    if (!(nci.flags & NciM_PARENT_STATE))
    {
      nid_to_node(dblist, nid, node);
      if (node->child)
	status = SetParentState(dblist, child_of(node), 0);
      if (node->member)
	status = SetParentState(dblist, member_of(node), 0);
    }
    else
      status = TreePARENT_OFF;
  }
  else
  {
    status = TreeUnLockNci(info, 0, node_num);
    if (status & 1)
      status = TreeALREADY_ON;
  }
  return status;
}

/*------------------------------------------------------------------------------

		Name: TreeTurnOff

		Type:   C function

		Author:	Josh Stillerman
			MIT Plasma Fusion Center

		Date:   28-MAR-1988

		Purpose: Turn off a sub tree.

------------------------------------------------------------------------------

	Call sequence:
			int TreeTurnOff()
			status = TreeTurnOff(nid);

------------------------------------------------------------------------------
   Copyright (c) 1988
   Property of Massachusetts Institute of Technology, Cambridge MA 02139.
   This program cannot be copied or distributed in any form for non-MIT
   use without specific written approval of MIT Plasma Fusion Center
   Management.
---------------------------------------------------------------------------

	Description:


+-----------------------------------------------------------------------------*/

int _TreeTurnOff(void *dbid, int nid_in)
{
  PINO_DATABASE *dblist = (PINO_DATABASE *)dbid;
  NID 	    *nid = (NID *)&nid_in;
  int       status;
  int       node_num;
  TREE_INFO *info;
  NCI       nci;
  NODE     *node;
  if (!IS_OPEN(dblist))
    return TreeNOT_OPEN;
  if (dblist->remote)
    return TreeTurnOffRemote(dbid,nid_in);
  nid_to_tree_nidx(dblist, nid, info, node_num);
  if (!info)
    return TreeNNF;
  status = TreeGetNciLw(info, node_num, &nci);
  if (~status & 1)
    return status;
  if (!(nci.flags & NciM_STATE))
  {
    bitassign(1,nci.flags,NciM_STATE);
    status = TreePutNci(info, node_num, &nci, 0);
    if (~status & 1)
      return status;
    if (!(nci.flags & NciM_PARENT_STATE))
    {
      nid_to_node(dblist, nid, node);
      if (node->child)
	status = SetParentState(dblist, child_of(node), 1);
      if (node->member)
	status = SetParentState(dblist, member_of(node), 1);
    }
  }
  else
  {
    status = TreeUnLockNci(info, 0, node_num);
    if (status & 1)
      status = TreeALREADY_OFF;
  }
  return status;
}

/*------------------------------------------------------------------------------

		Name: SetParentState

		Type:   C function

		Author:	Josh Stillerman
			MIT Plasma Fusion Center

		Date:   28-MAR-1988

		Purpose: propagate the parent_inactive status of
		       a node through its' children

------------------------------------------------------------------------------

	Call sequence:

------------------------------------------------------------------------------
   Copyright (c) 1988
   Property of Massachusetts Institute of Technology, Cambridge MA 02139.
   This program cannot be copied or distributed in any form for non-MIT
   use without specific written approval of MIT Plasma Fusion Center
   Management.
---------------------------------------------------------------------------

	Description:


+-----------------------------------------------------------------------------*/

int SetParentState(PINO_DATABASE *db, NODE *node, unsigned int state)
{
  int       status;
  NCI       nci;
  NODE     *lnode;
  status = TreeNORMAL;
  for (lnode = node; lnode && (status & 1); lnode = brother_of(lnode))
  {
    status = SetNodeParentState(db, lnode, &nci, state);
    if ((status & 1) && (!(nci.flags & NciM_STATE)) && (lnode->child))
      status = SetParentState(db, child_of(lnode), state);
    if ((status & 1) && (!(nci.flags & NciM_STATE)) && (lnode->member))
      status = SetParentState(db, member_of(lnode), state);
  }
  return status;
}

/*------------------------------------------------------------------------------

		Name: SetNodeParentState

		Type:   C function

		Author:	Josh Stillerman
			MIT Plasma Fusion Center

		Date:   28-MAR-1988

		Purpose: Set the state of the PARENT_INACTIVE field of
			a node's attributes.

------------------------------------------------------------------------------

	Call sequence:
			int TREE$SET_NODE_PARENT_STATE()
			status = TREE$SET_NODE_PARENT_STATE(db_ptr, node_ptr, nci_ptr, state)

------------------------------------------------------------------------------
   Copyright (c) 1988
   Property of Massachusetts Institute of Technology, Cambridge MA 02139.
   This program cannot be copied or distributed in any form for non-MIT
   use without specific written approval of MIT Plasma Fusion Center
   Management.
---------------------------------------------------------------------------

	Description:


+-----------------------------------------------------------------------------*/

static int SetNodeParentState(PINO_DATABASE *db, NODE *node, NCI *nci, unsigned int state)
{
  TREE_INFO *info;
  int       node_num;
  int status;
  for (info = db->tree_info; 
       info && ((node < info->node) || (node > (info->node + info->header->nodes))); info = info->next_info);
  if (!info)
    return TreeNNF;
  node_num = node - info->node;
  status = TreeGetNciLw(info, node_num, nci);
  if (status & 1)

  {
    bitassign(state,nci->flags,NciM_PARENT_STATE);
    status = TreePutNci(info, node_num, nci, 0);
  }
  return status;
}

#if defined (_WIN32)

int TreeLockNci(TREE_INFO *info, int readonly, int nodenum)
{
	int status = LockFile((HANDLE)_get_osfhandle(readonly ? info->nci_file->get : info->nci_file->put), 
		nodenum * sizeof(NCI), 0, sizeof(NCI), 0) == 0 ? TreeFAILURE : TreeSUCCESS;
	if (!(status & 1) && (GetLastError() == ERROR_LOCK_VIOLATION))
		  status = TreeSUCCESS;
	return status;
}
int TreeUnLockNci(TREE_INFO *info, int readonly, int nodenum)
{
	int status = UnlockFile((HANDLE)_get_osfhandle(readonly ? info->nci_file->get : info->nci_file->put), 
		nodenum * sizeof(NCI), 0, sizeof(NCI), 0) == 0 ? TreeFAILURE : TreeSUCCESS;
	return status;
}
#else

#ifdef vxWorks 
int TreeLockNci(TREE_INFO *info, int readonly, int nodenum)
{ return TreeSUCCESS; }
int TreeUnLockNci(TREE_INFO *info, int readonly, int nodenum)
{ return TreeSUCCESS; }
#else

int TreeLockNci(TREE_INFO *info, int readonly, int nodenum)
{
  struct flock flock_info;
  flock_info.l_type = readonly ? F_RDLCK : F_WRLCK;
  flock_info.l_whence = SEEK_SET;
  flock_info.l_start = nodenum * sizeof(NCI);
  flock_info.l_len = sizeof(NCI);
  return (fcntl(readonly ? info->nci_file->get : info->nci_file->put,F_SETLKW, &flock_info) != -1) ? 
          TreeSUCCESS : TreeLOCK_FAILURE;
}

int TreeUnLockNci(TREE_INFO *info, int readonly, int nodenum)
{
  struct flock flock_info;
  flock_info.l_type = F_UNLCK;
  flock_info.l_whence = SEEK_SET;
  flock_info.l_start = nodenum * sizeof(NCI);
  flock_info.l_len = sizeof(NCI);
  return (fcntl(readonly ? info->nci_file->get : info->nci_file->put,F_SETLKW, &flock_info) != -1) ? 
          TreeSUCCESS : TreeLOCK_FAILURE;
}
#endif
#endif
