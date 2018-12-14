
/*$Header: /usr16/software/web/src/RCS/his.c,v 1.11 2018/12/07 17:03:32 leith Exp $*/

/*
 C++********************************************************************
 C
 C  WID_HIS.FOR 
 C  his.c                    NOV 92 al
 C
 C *********************************************************************
 C=* FROM: WEB - VISUALIZER FOR SPIDER MODULAR IMAGE PROCESSING SYSTEM *
 C=* Copyright (C) 1992-2005  Health Research Inc.                     *
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
 C *********************************************************************
 C
 C    his(filedata,jhist)
 C
 C    PURPOSE:     Compute nhist place histogram from image records, 
 C                 store histogram in the histogram records
 C
 C    PARAMETERS:  filedata      file info
 C                 jhist         histogram data
 C
 C--*******************************************************************
*/

#include "files.h"
#include "common.h"
#include "routines.h"
#include "filter.h"

#define  NHIST 128

/**************************  his   ***************************/

 int his(FILEDATA *filedata, int * jhist)

 {
 float *   buf;
 char      outstr[80];
 float     hdiff, ff, b, av, hmin, sig, tmode, binsiz;
 double    y, N, dav;
 int       i, k, j, jhismax, nhist, irec;
 int   *   iptr;

 buf = filedata -> buf;

 if (filedata -> iform <= 0 || filedata -> iform > 3) 
    {
    /* can not do histogram on fourier files */
    spout("*** can not do histogram on this file format");
    return FALSE;
    }

 else
    {
    /*  must calculate histogram from image data */

    hdiff = filedata -> fmax_spi - filedata -> fmin_spi;
    hmin  = filedata -> fmin_spi;
    ff    = (NHIST - 1) / hdiff;

    y     = 0.0;
    dav    = 0.0;

    /*  zero the histogram */
    for ( iptr = jhist; iptr < jhist + NHIST; iptr++)
       *iptr = 0;

    for (irec = 1; irec <= nrow * nslice; irec++)
       {
       redlin(filedata, buf, nsam, irec);

       for (k = 0; k < nsam; k++)
          {
          b  = buf[k];

          dav = dav + b;
          y  = y + b * b;

          /* j is histogram bin */
          j = (b - hmin) * ff + 1.5;

          if (j < 0) 
             /*  off histogram at bottem */
             (*jhist)++;

          else if (j >= NHIST) 
             /* off histogram at top */
             (*(jhist + NHIST-1))++;

          else
             (*(jhist+j))++;
          }
       }

    /* update image statistics in the header of the file */
    N = (double) (nsam * nrow * nslice);
    sig = sqrt( (y - (dav*dav/N)) / (double)(N - 1.0) );

    av  = dav / N;

    filedata -> sig   = sig;
    filedata -> av    = av;

    setprm(filedata, "u");
    }

 /* find maximum frequency occuring in histogram (jhismax) and 
    location of maximum frequency (j) */

 jhismax = 0;
 j       = 0;
 for (iptr = jhist; iptr < jhist + NHIST; iptr++)
    {
    if (*iptr >= jhismax) 
      {
      jhismax = *iptr;
      j  = (int) (iptr - jhist);
      }
    }


 /* convert j to corresponding image intensity */
 binsiz = hdiff / (NHIST - 1);
 nhist  = NHIST;
 tmode  = hmin + j * binsiz;

 sprintf(outstr,"range: %f ... %f    bins: %d,   binsize: %f",
                      fmin_spi,   fmax_spi,     nhist,       binsiz);
 spout(outstr);

 sprintf(outstr,"mean: %f   mode: %f    sd: %f", av, tmode, sig);
 spout(outstr);

 closefile(filedata);
 return TRUE;
 }

