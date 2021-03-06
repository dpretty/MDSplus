#include "mdsip_connections.h"
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include "zlib.h"
static int SendBytes(int id, void *buffer, size_t bytes_to_send, int options) {
  char *bptr = (char *)buffer;
  IoRoutines *io = GetConnectionIo(id);
  if (io) {
    int tries = 0;
    while ((bytes_to_send > 0) && (tries < 10)) {
      ssize_t bytes_sent;
      bytes_sent = io->send(id, bptr, bytes_to_send, options);
      if (bytes_sent <= 0) {
	if (errno != EINTR)
	  return 0;
	tries++;
      }
      else {
	bytes_to_send -= bytes_sent;
	bptr += bytes_sent;
	tries = 0;
      }
    }
    if (tries >= 10) {
      char msg[256];
      sprintf(msg,"\rsend failed, shutting down connection %d",id);
      perror(msg);
      DisconnectConnection(id);
      return 0;
    }
    return 1;
  }
  return 0;
}

int SendMdsMsg(int id, Message *m, int oob) {
  unsigned long len = m->h.msglen - sizeof(m->h);
  unsigned long clength = 0; 
  Message *cm = 0;
  int status;
  int do_swap = 0; /*Added to handle byte swapping with compression*/
 
  if (len > 0 && GetCompressionLevel() > 0 && m->h.client_type != SENDCAPABILITIES) {
    clength = len;
    cm = (Message *)malloc(m->h.msglen + 4);
  }
  if (!oob) FlushConnection(id);
  if (m->h.client_type == SENDCAPABILITIES)
    m->h.status = GetCompressionLevel();
  if ((m->h.client_type & SwapEndianOnServer) != 0)
  {
    if (Endian(m->h.client_type) != Endian(ClientType()))
    {
      FlipData(m);
      FlipHeader(&m->h);
      do_swap = 1; /* Recall that the header field msglen needs to be swapped */
    }
  }
  else
    m->h.client_type = ClientType();
  if (clength && compress2(cm->bytes+4,&clength,m->bytes,len,GetCompressionLevel()) == 0 && clength < len)
  {
    cm->h = m->h;
    cm->h.client_type |= COMPRESSED;
    memcpy(cm->bytes,&cm->h.msglen,4);
    cm->h.msglen = clength + 4 + sizeof(MsgHdr);
/*If byte swapping required, swap msglen */
    if(do_swap)
 	FlipBytes(4,(char *)&cm->h.msglen);	
 /* status = SendBytes(id, (char *)cm, cm->h.msglen, oob);*/
/* now msglen is swapped, and cannot be used as byte counter */
    status = SendBytes(id, (char *)cm, clength + 4 + sizeof(MsgHdr), oob);
  }
  else
    status = SendBytes(id, (char *)m, len + sizeof(MsgHdr), oob);
  if (clength) free(cm);
  return status;
}
