
/*$Header: /usr16/software/web/src/RCS/redlin8.c,v 1.5 2018/12/07 17:03:33 leith Exp $*/
/*
C***********************************************************************
C
C   REDLIN8.C  -- CREATED                                  MAR 4  1994
C                 MRC                                      NOV 27 2012     
C **********************************************************************
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
C **********************************************************************
C
c  redlin8(filedata,cbuf,nx,irec)
c           
C  PURPOSE:     Reads spider image into 8 bit array normalized from
c               nmin to nmax.  Returns normaliztion constants also.
c
C  PARAMETERS:  nx 
C
C  RETURNS:     T/F    
C
C  CALLED BY:   surf
C
C  NOTES:       
C
C--*********************************************************************
*/

#include "files.h"
#include "routines.h"

 /* External function prototypes */

 /* Externally defined variables */
 
 /* Static variables for use here */


 /*************************   redlin8   ****************************/

 int redlin8(FILEDATA *fileptr, unsigned char *buf, int nx, int irec)
 {

 FILE      *fp;
 int       ioff;
 size_t    igot;

if ((fileptr -> iform) > 99)
    {    // MRC file should start at lower left) 
    irec = ((fileptr -> nsam) * (fileptr -> nslice)) - irec + 1;
    }

 // Find number of header and record bytes to be skipped 
 if (irec != 0) 
    ioff  = fileptr -> offset + fileptr -> headbyt + (irec -1) * nx;
 else
    ioff  = fileptr -> offset;

 // Find file pointer for this file 
 fp    = fileptr -> fp;

 if ((igot = fseek(fp,ioff,SEEK_SET)) != 0) 
     {       // Offset seek failed 
     printf("*** seek failed in redlin8: %zu \n",igot);
     printf(" igot: %zu, ioff: %d ,nx: %d \n",igot,ioff,nx);
     return FALSE;
     }

 // Offset seek was successful, read record number nx 
 igot = fread(buf, sizeof(unsigned char), nx, fp);

 if (feof(fp))
      {
      printf("*** End of file in redlin8 \n");
      printf("igot: %zu, ioff: %d ,nx: %d \n",igot,ioff,nx);
      return FALSE;
      }          

 if (ferror(fp))
      {
      printf("*** Error reading input file in redlin8 \n");
      return FALSE;
      }

 return TRUE;
 }



