
/*$Header: /usr16/software/web/src/RCS/wrtlin.c,v 1.10 2018/12/07 17:06:44 leith Exp $*/
/*
 * wrtlin.c
 *
 ***********************************************************************
 C=* SPIDER - MODULAR IMAGE PROCESSING SYSTEM.  AUTHOR: J.FRANK 
 C=* FROM: WEB - VISUALIZER FOR SPIDER MODULAR IMAGE PROCESSING SYSTEM *
 C=* Copyright (C) 1992-2018  Health Research Inc.                     *
 C=* Riverview Center, 150 Broadway, Suite 560, Menands, NY 12204.     *
 C=* Email:  spider@health.ny.gov                                      *
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
 ***********************************************************************
 *
 * wrtlin(fileptr,buf,nsam,irec)
 * 
 * PURPOSE:     
 *
 * RETURNS:  Number of positions written into file
 * 
 * PARAMETERS:   fileptr
 *               buf
 *               nsam
 *               irec    
 *
 *********************************************************************/

#include "files.h"
#include "routines.h"

unsigned int *pt;
#define CONVERT_4( A, B )                        \
              (A) =                 \
            ((unsigned int)(B) >> 24) |          \
            ((unsigned int)(B) << 24) |          \
           (((unsigned int)(B) >> 8) & 0xff00) | \
           (((unsigned int)(B) << 8) & 0xff0000)


 int wrtlin(FILEDATA *fileptr, float *buf, int nsam, int irec)

 {
 FILE    *fp;
 int     ioff, itest, k1;
 size_t  igot;

 /* find offset of this line in the file */ 
 if (irec != 0) 
    ioff  = fileptr -> offset + fileptr -> headbyt + (irec -1) * nsam * 4;
 else
    ioff  = fileptr -> offset;

 /* find fileptr for this file */
 fp    = fileptr -> fp;

 if ((igot = fseek(fp,ioff,SEEK_SET)) != 0) 
    {
    printf("*** Seek failed in wrtlin: %zu \n",igot);
    printf("wrtlin; igot: %zu, ioff: %d ,nsam: %d \n",igot,ioff,nsam);
    return FALSE;
    }

 /* have set file position to the correct offset, put buf in file */

 if (fileptr -> flip == 1)
   {
   /* flip the bytes */
   pt = (unsigned int *) buf;
   for (k1 = 0; k1 < nsam; k1++, pt++)
      { CONVERT_4(*pt,*pt);  }
   }

    igot = fwrite(buf, sizeof(float), nsam, fp);
    itest = nsam;

 if ((int) igot == itest)
    return TRUE;

 else 
    {
    printf("*** Write error in wrtlin. \n");
    printf("wrtlin; fp: %p igot: %zu   ioff: %d nsam: %d \n",
                    (void *)fp,    igot,      ioff,    nsam);
    return FALSE;
    }
 }

