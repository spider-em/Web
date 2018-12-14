
/*$Header: /usr16/software/web/src/RCS/redvol8.c,v 1.6 2018/12/07 17:03:34 leith Exp $*/
/*
C**************************************************************************
C
C   REDVOL8.C  -- CREATED NOV 13 1992
C             
C **********************************************************************
C *  AUTHOR: ArDean Leith                                                  *
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
C **********************************************************************
C
C  REDVOL8(FILEDATA,CVOL,NSAM1,NSAM2,NROW1,NROW2,NSLICE1,NSLICE2,
C          FN,FNCON)
C           
C
C  PURPOSE:     READS SPIDER IMAGE INTO 8BIT ARRAY NORMALIZED FROM
C               NMIN TO NMAX.  RETURNS NORMALIZTION CONSTANTS ALSO.
C
C  PARAMETERS:  NSAM1,NSAM2     SAMPLE RANGE
C               NROW1,NROW2     ROW RANGE
C               NSLICE1,NSLICE2 SLICE RANGE

C  RETURNS:     
C
C  CALLED BY:   SURF
C
C  NOTES:       
C
C--*********************************************************************
*/

#include "files.h"
#include "routines.h"


 /* external function prototypes */

 /* externally defined variables */
 
 /* static variables for use here */


 /*************************   redvol8   ****************************/

 int redvol8(FILEDATA *filedata, unsigned char  * cptr,
         int nsam1, int nsam2, int nrow1, int nrow2, 
         int nslice1, int nslice2, int nmint, int nmaxt, 
         float * fn, float * fncon)

 {
 int             nsam, nrow, nslice, irec;
 int             islice, irow, isam ;
 float           * buf;
 unsigned char * cptrt;

 nsam   = filedata -> nsam;
 nrow   = filedata -> nrow;
 nslice = filedata -> nslice;

 /* find buf for redlin */
 buf    = filedata -> buf;

 /* load volume data into 8 bit vol */

 *fn     = (nmaxt - nmint ) / ((filedata-> fmax_spi) - (filedata -> fmin_spi));
 *fncon  = (-filedata -> fmin_spi * *fn) + nmint;

 /* fill volume with 8 bit data */
 irec   = 0;

 cptrt = cptr;

 for (islice = 0; islice < nslice; islice++)
    {
    for (irow = 0; irow < nrow; irow++)
       {
       if (!redlin(filedata,buf,nsam,++irec))
          return FALSE;

       for (isam =0; isam < nsam; isam++)
          {
          *cptrt++ =  (unsigned char) (buf[isam] * *fn + *fncon);
          }
       }
    }
 return TRUE;
 }




