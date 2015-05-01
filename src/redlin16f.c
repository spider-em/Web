/*$Header: /usr8/web/src/RCS/redlin16f.c,v 1.4 2012/11/27 19:12:54 leith Exp $*/
/*
 C   REDLIN8.C  -- CREATED                                  MAR    1994
 C                 MRC                                      NOV 27 2012     
 C**********************************************************************
 C=* AUTHOR: A. LEITH 
 C=* FROM: WEB - VISUALIZER FOR SPIDER MODULAR IMAGE PROCESSING SYSTEM *
 C=* Copyright 1985-2012  Health Research Inc.,                        *
 C=* Riverview Center, 150 Broadway, Suite 560, Menands, NY 12204.     *
 C=* Email:  spider@wadsworth.org                                      *
 C=*                                                                   *
 C=* This program is free software; you can redistribute it and/or     *
 C=* modify it under the terms of the GNU General Public License as    *
 C=* published by the Free Software Foundation; either version 2 of    *
 C=* the License, or (at your option) any later version.               *
 C=*                                                                   *
 C=* This program is distributed in the hope that it will be useful,   *
 C=* but without any warranty; without even the implied warranty of    *
 c=* merchantability or fitness for a particular purpose.  See the GNU *
 C=* General Public License for more details.                          *
 C=* You should have received a copy of the GNU General Public License *
 C=* along with this program. If not, see <http://www.gnu.org/licenses>*
 C=*                                                                   *
 C *********************************************************************
 *
 * redlin16f(fileptr,buf,nsam,irec)
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

 int redlin16f(FILEDATA *fileptr, float *buf, int nsam, int irec)
 {

 FILE          * fp;
 int             ioff,i;
 size_t          igot;
 short         * sptr;

if ((fileptr -> iform) > 99)
    {    // MRC file should start at lower left) 
    irec = ((fileptr -> nsam) * (fileptr -> nslice)) - irec + 1;
    }

// Find number of header and record bytes to be skipped 
 if (irec != 0) 
    ioff  = fileptr -> offset + fileptr -> headbyt + (irec -1) * nsam * 2;
 else
    ioff  = fileptr -> offset;


 // Find file pointer for this file 
 fp    = fileptr -> fp;

 if ((igot = fseek(fp,ioff,SEEK_SET)) != 0) 
     {    // Offset seek failed 
     printf("*** Seek failed in redlin16f: %d \n",igot);
     printf(" igot: % d, ioff: %d ,nsam: %d \n",igot,ioff,nsam);
     return FALSE;
     }

 // Offset seek was successful, read record number nsam 
 sptr = (short *) buf;
 igot = fread(sptr, sizeof(short), nsam, fp);

 if (feof(fp))
      {
      printf("*** End of file in redlin16f. \n");
      printf("igot: %d, ioff: %d ,nsam: %d \n",igot,ioff,nsam);
      printf("headbyt: %d, irec: %d ,offset: %d \n",
              fileptr->headbyt,irec,fileptr->offset);
      return FALSE;
      }          

 if (ferror(fp))
      {
      printf("*** Error reading input file in redlin16f. \n");
      return FALSE;
      }

 // Convert in-place from 16 bit to floating point 
 sptr = (short *) buf + nsam - 1;
 for (i= nsam-1; i >= 0; i--)
    {
    buf[i] = *sptr--;
    }

 return TRUE;
 }

