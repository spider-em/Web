/*$Header: /usr16/software/web/src/RCS/redlin8f.c,v 1.4 2018/12/07 17:03:33 leith Exp $*/
/*
 * redlin8.c
 *
 *************************************************************************
 * AUTHOR: A. LEITH 
 C=* FROM: WEB - VISUALIZER FOR SPIDER MODULAR IMAGE PROCESSING SYSTEM *
 C=* Copyright (C) 1992-2005  Health Research Inc.                     *
 C=*                                                                   *
 C=* HEALTH RESEARCH INCORPORATED (HRI),                               *   
 C=* ONE UNIVERSITY PLACE, RENSSELAER, NY 12144-3455.                  *
 C=*                                                                   *
 C=* Email:  spider@wadsworth.org                                      *
 C=*                                                                   *
 C=* This program is free software; you can redistribute it and/or     *
 C=* modify it under the terms of the GNU General Public License as    *
 C=* published by the Free Software Foundation; either version 2 of    *
 C=* the License, or (at your option) any later version.               *
 C=*                                                                   *
 C=* This program is distributed in the hope that it will be useful,   *
 C=* but WITHOUT ANY WARRANTY; without even the implied warranty of    *
 C=* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU *
 C=* General Public License for more details.                          *
 C=*                                                                   *
 C=* You should have received a copy of the GNU General Public License *
 C=* along with this program; if not, write to the                     *
 C=* Free Software Foundation, Inc.,                                   *
 C=* 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.     *
 C=*                                                                   *
 **************************************************************************
 *
 * redlin8(fileptr,buf,nsam,irec)
 * 
 * PURPOSE:     
 *
 * RETURNS:      TRUE IF OK, FALSE OTHERWISE
 * 
 * PARAMETERS:   fileptr
 *               buf
 *               nsam
 *               irec    
 *
 *********************************************************************/

#include "files.h"
#include "routines.h"

 int redlin8f(FILEDATA *fileptr, float *buf, int nsam, int irec)
 {

 FILE          * fp;
 int             ioff,i;
 size_t          igot;
 unsigned char * cptr;

 /* find number of header and record bytes to be skipped */
 if (irec != 0) 
    ioff  = fileptr -> offset + fileptr -> headbyt + (irec -1) * nsam;
 else
    ioff  = fileptr -> offset;


 /* find file pointer for this file */
 fp    = fileptr -> fp;

 if ((igot = fseek(fp,ioff,SEEK_SET)) != 0) 
     {       /* offset seek failed */
     printf("*** Seek failed in redlin8f: %zu \n",igot);
     printf(" igot: %zu, ioff: %d ,nsam: %d \n",igot,ioff,nsam);
     return FALSE;
     }

 /* offset seek was successful, read record number nsam */
 cptr = (unsigned char *) buf;
 igot = fread(cptr, sizeof(unsigned char), nsam, fp);

 if (feof(fp))
      {
      printf("*** End of file in redlin8f. \n");
      printf("igot: %zu, ioff: %d ,nsam: %d \n",igot,ioff,nsam);
      printf("headbyt: %d, irec: %d ,offset: %d \n",
              fileptr->headbyt,irec,fileptr->offset);
      return FALSE;
      }          

 if (ferror(fp))
      {
      printf("*** Error reading input file in redlin8f. \n");
      return FALSE;
      }

 /* convert from 8 bit raw to floating point */
 cptr = (unsigned char *) buf + nsam - 1;
 for (i= nsam-1; i >= 0; i--)
    {
    buf[i] = *cptr--;
    }

 return TRUE;
 }

