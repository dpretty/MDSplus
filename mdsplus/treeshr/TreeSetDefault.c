#include <treeshr.h>
#include "treeshrp.h"

static char *cvsrev = "@(#)$RCSfile: TreeSetDefault.c,v $ $Revision: 1.4 $ $Date: 2010/08/04 19:01:11 $";

extern void **TreeCtx();

int TreeSetDefault(char *path, int *nid) { return _TreeSetDefault(*TreeCtx(),path,nid);}
int TreeSetDefaultNid(int nid_in) { return _TreeSetDefaultNid(*TreeCtx(),nid_in);}
int TreeGetDefaultNid(int *nid_in) { return _TreeGetDefaultNid(*TreeCtx(),nid_in);}

int _TreeSetDefault(void *dbid, char *path, int *nid)
{
  int       status;
  status = _TreeFindNode(dbid, path, nid);
  if (status & 1)
    status = _TreeSetDefaultNid(dbid, *nid);
  return status;
}

int _TreeSetDefaultNid(void *dbid, int nid_in)
{
  PINO_DATABASE *dblist = (PINO_DATABASE *)dbid;
  NID *nid = (NID *)&nid_in;
  if (IS_OPEN(dblist))
  {
    NODE     *node_ptr;
	if (dblist->remote)
		return SetDefaultNidRemote(dblist,nid_in);
    nid_to_node(dblist, nid, node_ptr);
    if (node_ptr)
    {
      dblist->default_node = node_ptr;
      return TreeNORMAL;
    }
    else
      return TreeNNF;
  }
  else
    return TreeNOT_OPEN;
}

int _TreeGetDefaultNid(void *dbid, int *nid_in)
{
  PINO_DATABASE *dblist = (PINO_DATABASE *)dbid;
  NID *nid = (NID *)nid_in;
  if (!(IS_OPEN(dblist)))
    return TreeNOT_OPEN;
  if (dblist->remote)
	  return GetDefaultNidRemote(dblist,nid_in);
  node_to_nid(dblist, (dblist->default_node), nid);
  return TreeNORMAL;
}
