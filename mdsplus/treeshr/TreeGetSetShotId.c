/*------------------------------------------------------------------------------

		Name:   TreeGetCurrentShotId

		Type:   C function

		Author:	Thomas W. Fredian

		Date:   11-OCT-1989

		Purpose: Get current shot number

------------------------------------------------------------------------------

	Call sequence:

int TreeGetCurrentShotId(experiment,shot)

------------------------------------------------------------------------------
   Copyright (c) 1989
   Property of Massachusetts Institute of Technology, Cambridge MA 02139.
   This program cannot be copied or distributed in any form for non-MIT
   use without specific written approval of MIT Plasma Fusion Center
   Management.
------------------------------------------------------------------------------*/
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <mdsdescrip.h>
#include <mdsshr.h>
#include <libroutines.h>
#include <strroutines.h>
#include <stdlib.h>
#include <fcntl.h>
#include <ncidef.h>
#include <treeshr.h>
#include "treeshrp.h"
extern void TranslateLogicalFree();
extern int TreeGetCurrentShotIdRemote();
extern int TreeSetCurrentShotIdRemote();
extern char *MaskReplace();
#ifndef index
extern char *index();
#endif

extern char *TranslateLogical();

static char *cvsrev = "@(#)$RCSfile: TreeGetSetShotId.c,v $ $Revision: 1.15 $ $Date: 2008/11/18 17:32:23 $";

#define _ToLower(c) (((c) >= 'A' && (c) <= 'Z') ? (c) | 0x20 : (c))

static char *GetFileName(char *experiment,char **ctx)
{
  char *ans = 0;
  static char pathname[1024];
  static char *path;
  char *semi = 0;
  char *part;
  if (*ctx == NULL)
  {
    if (path != NULL)
      TranslateLogicalFree(path);
    strcpy(pathname,experiment);
    strcat(pathname,"_path");
    path = (char *)TranslateLogical(pathname);
    part = path;
  }
  else if (*ctx == pathname)
    return NULL;
  else
    part = *ctx;
  if (part != NULL)
  {
    char *delim = TREE_PATH_DELIM;
    char *tmp;
    if ((semi = (char *)index(part, ';')) != 0)
      *semi = '\0';
    strncpy(pathname,part,500);
    if (semi == 0) 
      *ctx = pathname;
    else
      *ctx = part + strlen(part) + 1;
    tmp = MaskReplace(pathname,experiment,0);
    strcpy(pathname,tmp);
    free(tmp);
    if (pathname[strlen(pathname)-1] == '+')
    {
      int i;
      for (i=strlen(pathname)-1;(i >= 0) && (pathname[i] != delim[0]);i--);
      if (i >= 0)
        pathname[i+1] = 0;
    }
    else
    {
      if (pathname[strlen(pathname)-1] != delim[0])
        strcat(pathname,TREE_PATH_DELIM);
    }
    strcat(pathname,"shotid.sys");
    ans = pathname;
  }
  return ans;
}

static int CreateShotIdFile(char *experiment)
{
  int fd = -1;
  char *ctx = 0;
  char *filename;
  while ((fd == -1) && (filename = GetFileName(experiment,&ctx)))
    fd = MDS_IO_OPEN(filename,O_RDWR | O_CREAT | O_TRUNC,0777);
  return fd;
}

static int OpenShotIdFile(char *experiment,int mode)
{
  int fd=-1;
  char *ctx = 0;
  char *filename;
  int found = 0;
  while ((filename = GetFileName(experiment,&ctx)) && !(found=(MDS_IO_EXISTS(filename))));
  if (found)
    fd = MDS_IO_OPEN(filename,mode,0);
  else if (mode == O_WRONLY)
    fd = CreateShotIdFile(experiment);
  return fd;
}


int       TreeGetCurrentShotId(char *experiment)
{
  int shot = 0;
  int status = 0;
  char *path = 0;
  char *exp = strcpy(malloc(strlen(experiment)+6),experiment);
  int i;
  int slen;
  for (i=0;exp[i] != '\0';i++)
    exp[i] = _ToLower(exp[i]);
  strcat(exp,"_path");
  path = TranslateLogical(exp);
  exp[strlen(experiment)]='\0';
  if (path && ((slen = strlen(path)) > 2) && (path[slen-1] == ':') && (path[slen-2] == ':'))
  {
    path[slen-2] = 0;
    status = TreeGetCurrentShotIdRemote(exp, path, &shot);
  }
  else
  {
    int fd = OpenShotIdFile(exp,O_RDONLY);
    if (fd != -1)
    {
      status = MDS_IO_READ(fd,&shot,sizeof(shot)) == sizeof(shot);
      MDS_IO_CLOSE(fd);
#ifdef WORDS_BIGENDIAN
      if (status & 1)
      {
        int lshot = shot;
        int i;
        char *optr = (char *)&shot;
        char *iptr = (char *)&lshot;
        for (i=0;i<4;i++) optr[i] = iptr[3-i];
      }
#endif
    }
  }
  if (path)
    TranslateLogicalFree(path);
  free(exp);
  return (status & 1) ? shot : 0;
}

int       TreeSetCurrentShotId(char *experiment, int shot)
{
  int status = 0;
  char *path = 0;
  char *exp = strcpy(malloc(strlen(experiment)+6),experiment);
  int slen;
  int i;
  for (i=0;exp[i] != '\0';i++)
    exp[i] = _ToLower(exp[i]);
  strcat(exp,"_path");
  path = TranslateLogical(exp);
  exp[strlen(experiment)]='\0';
  if (path && ((slen = strlen(path)) > 2) && (path[slen-1] == ':') && (path[slen-2] == ':'))
  {
    path[slen-2] = 0;
    status = TreeSetCurrentShotIdRemote(exp, path, shot);
  }
  else
  {
    int fd = OpenShotIdFile(exp,O_WRONLY);
    if (fd != -1)
    {
      int lshot = shot;
#ifdef WORDS_BIGENDIAN
      int i;
      char *optr = (char *)&lshot;
      char *iptr = (char *)&shot;
      for (i=0;i<4;i++) optr[i] = iptr[3-i];
#endif
      status = MDS_IO_WRITE(fd,&lshot,sizeof(shot)) == sizeof(shot);
      MDS_IO_CLOSE(fd);
    }
  }
  if (path)
    TranslateLogicalFree(path);
  free(exp);
  return status;
}

