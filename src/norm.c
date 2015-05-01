
/*$Header: /usr8/web/src/RCS/norm.c,v 1.8 2007/11/28 16:18:20 leith Exp $*/
/*
 C++*******************************************************************
 C
 C NORM
 C
C **********************************************************************
 C *  SPIDER - MODULAR IMAGE PROCESSING SYSTEM.  AUTHOR: J.FRANK            *
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
 C NORM(FILEPTR)
 C
 C PURPOSE: FINDS MAX, MIN, AV AND STANDARD DEVIATION OF AN IMAGE
 C          PLACES THESE VALUES IN THE HEADER
 C
 C--*******************************************************************
*/

#include "files.h"
#include "routines.h"
#include <float.h>

 extern int   setprm(FILEDATA *, char *);

/********************************************************/

 int norm_new(FILEDATA *fileptr)
 { 

 int       nsam, nrow, nslice, islice, ipix, idum;
 double    fmint, fmaxt, av, av2, sig, b;
 float     pix, fdum;
 char  *   cdum;
 float *   buf;

 nsam   = fileptr -> nsam;
 nrow   = fileptr -> nrow;
 nslice = fileptr -> nslice;

 fmaxt   = FLT_MIN;
 fmint   = FLT_MAX;
 fdum   = 0.0;
 buf    = NULL;

 av     = 0.0;
 av2    = 0.0;

 for (islice = 1; islice <= nslice; islice++)
    {  /* operates slice by slice to conserve memory allocation */

    /* redvol will allocate buf on first call */
    if (!redvol(fileptr, FALSE, &cdum, TRUE, &buf, 1, nsam,
            1, nrow, islice, islice, idum, idum, fdum, fdum,
            &fdum,  &fdum, 1))  return FALSE;

    for (ipix = 0; ipix < nsam * nrow; ipix++)
       {  /* find min, max, etc. */
       b    = *(buf + ipix);
       fmaxt = MYMAX(b,fmaxt);
       fmint = MYMIN(b,fmint);

       av   = av + b;
       av2  = av2 + b * b;
       }

    }

#ifdef NEVER
 pix = nsam * nrow * nslice;

 sig = sqrt (( av2 - ((av * av) / pix)) / (pix - 1.0));
 av  = av / pix;

 fileptr -> fmin_spi = fmint;
 fileptr -> fmax_spi = fmaxt;
 fileptr -> av   = av;
 fileptr -> sig  = sig;
#endif

 printf("*** range: %f ... %f \n",fmint,fmaxt);; 
 free(buf); buf = NULL;

 return (setprm(fileptr,"u"));
} 



/********************************************************/

 int norm(FILEDATA *fileptr)
 { 

 int       nsam, nrow, nslice, irec, isam;
 double    fmint, fmaxt, av, av2, sig, b;
 float     pix, *buf;

 nsam   = fileptr -> nsam;
 nrow   = fileptr -> nrow;
 nslice = fileptr -> nslice;
 buf    = fileptr -> buf;

   
 if (redlin(fileptr,buf,nsam,1) != TRUE)
    return FALSE;

 fmaxt = *buf;
 fmint = fmaxt;

 av  = 0.0;
 av2 = 0.0;

 for (irec = 1; irec <= nslice * nrow; irec++)
    {
    if (redlin(fileptr,buf,nsam,irec) != TRUE)
       return FALSE;

    for (isam = 0; isam < nsam; isam++)
       {
       b    = *(buf + isam);
       fmaxt = MYMAX(b,fmaxt);
       fmint = MYMIN(b,fmint);

       av   = av + b;
       av2  = av2 + b * b;
       }

    }

 pix = nsam * nrow * nslice;

 sig = sqrt (( av2 - ((av * av) / pix)) / (pix - 1.0));
 av  = av / pix;

 fileptr -> fmin_spi = fmint;
 fileptr -> fmax_spi = fmaxt;
 fileptr -> av   = av;
 fileptr -> sig  = sig;


 return (setprm(fileptr,"u"));
} 
