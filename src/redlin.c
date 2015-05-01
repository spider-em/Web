/*$Header: /usr8/web/src/RCS/redlin.c,v 1.17 2012/11/27 19:12:31 leith Exp $*/

/*
 ***********************************************************************
 *                                                                     *
 * redlin.c       MRC support                   Nov 2012  ArDean Leith * 
 *                                                                     *
 ***********************************************************************
 C=* AUTHOR: A. LEITH                                                  *
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
 ***********************************************************************
 *
 * redlin(fileptr,buf,nx,irec)
 * 
 * PURPOSE:      Read one 'record' from image/volume file     
 *
 * RETURNS:      True if ok, false otherwise
 * 
 * PARAMETERS:   fileptr   File info
 *               buf       Data array
 *               nx        Record or read length
 *               irec      Record number wanted 
 *
 **********************************************************************/

#include "files.h"

// Byte flipping
#define CONVERT_4( A, B )                        \
             (A) =                               \
            ((unsigned int)(B) >> 24) |          \
            ((unsigned int)(B) << 24) |          \
           (((unsigned int)(B) >> 8) & 0xff00) | \
           (((unsigned int)(B) << 8) & 0xff0000)

 int redlin8f( FILEDATA *fileptr, float *buf, int nx, int irec);
 int redlin16f(FILEDATA *fileptr, float *buf, int nx, int irec);

 extern int  nsam8,nrow8,nslice8; /* size of raw files           */


 // *************************** redlin *********************************

 int redlin(FILEDATA *fileptr, float *buf, int nx, int irec)
 {

 FILE         * fp;
 long           ioff;
 int            k1;
 size_t         igot;
 unsigned int * pt;

 extern int  nsam8,nrow8,nslice8; /* size of raw files */

 if ((fileptr -> iform) > 99)
    {    // MRC file should start at lower left) 
    irec = ((fileptr -> nsam) * (fileptr -> nslice)) - irec + 1;
    }

 if ((fileptr -> iform) == 8 || (fileptr -> iform) == 108)
    {    // 8-bit file (may be MRC or raw) 
    return(redlin8f(fileptr, buf, nx, irec));
    }
 else if ((fileptr -> iform) == 16 || (fileptr -> iform) == 116)
    {    // 16-bit integer file (may be MRC or raw)  
    return(redlin16f(fileptr, buf, nx, irec));
    }

 // Find number of header and record bytes to be skipped before irec 
 // fileptr -> offset currently unused, nov 2012 al

 if (irec != 0) 
    ioff  = (fileptr -> offset) + (fileptr -> headbyt) + (irec -1) * nx * 4;
 else
    ioff  = (fileptr -> offset);


 /*  Find file pointer for this file */
 fp = (fileptr -> fp);

 if ((igot = fseek(fp,ioff,SEEK_SET)) != 0) 
     {       /* Offset seek failed */
     printf("*** Seek failed in redlin: %d \n",igot);
     printf(" igot: % d, ioff: %d ,nx: %d \n",igot,ioff,nx);
     return FALSE;
     }

 /* Offset seek was successful, read nx floats from file */
 igot = fread(buf, sizeof(float), nx, fp);
 
 if ((fileptr -> flip) == 1)
   {
   /* Flip the bytes */
   pt = (unsigned int *) buf;
   for (k1 = 0; k1 < nx; k1++, pt++)
      { CONVERT_4(*pt,*pt);  }
   }

 if (feof(fp) && nsam8 > 0 && nrow8 > 0)
      {
      printf("*** End of file in redlin \n");
      printf("igot: %d, ioff: %d ,nx: %d irec: %d\n",
              igot,ioff,nx,irec);
      printf(" fp->nrow: %d, fp->offset: %d \n",
                fileptr->nrow, fileptr->offset);
      return FALSE;
      }          

 if (ferror(fp))
      {
      printf("*** Error reading input file in redlin \n");
      return FALSE;
      }

 return TRUE;
 }

