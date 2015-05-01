
/*$Header: /usr8/web/src/RCS/monn.c,v 1.12 2005/10/18 17:00:04 leith Exp $*/
/*
C++************************************************************************
C
C MONN -- CREATED JAN 90 FROM WKMONN.FOR
C         SPEEDED UP APR 90
C         CONVERTED TO C IN JULY 92 al
C
C **********************************************************************
C *  AUTHOR:  ArDean Leith                                                 *
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
C    MONN
C
C    PURPOSE:       DISPLAY  MONTAGE FROM 3-D FILE SERIES. NORMALIZES
C                   EACH SLICE OF THE MONTAGE SEPARATELY.
C
C    PARAMETERS:      
C
C    CALLED BY:       WID_MONT2    MONT3
C
C****************************************************************************
*/

#include "common.h"
#include "routines.h"

#define  FLTZER 10e-10

/****************************  monn **********************************/ 

 int monn(GC icontxt, FILEDATA *fileptr,  int iwant,
          int nsami, int nrowi, int ndum, int nmint, int nmaxt)

 {   
 float         *buf, *rbuf;
 char          *cbuf;
 float         fmint, fmaxt, fval, fn, fncon;
 int           iloc, i, m;
 int           isam, irow, iconz, irect, miskip, irec1;

 buf = filedata -> buf;

 /* allocate space for buffers */
 if ((rbuf = (float *) malloc(nsami * nrowi * sizeof(float))) == 
             (float *) 0)
    {spout(" *** Can not allocate rbuf memory in monn."); return FALSE;}

 /* zero slice min and maximum values */
 fmint =  10e20;
 fmaxt = -10e-20;

 iloc  = -1;

 if (axis == 'z')
    {                      /*  Z SLICE WANTED (FASTEST SLICING AXIS) */

    islice = iwant;
    iconz  = (islice - 1) * nrow;

    if (iskip == 1)
       {    /* display z slice at full size */
       for (irow=nrow1; irow <= nrow2; irow++)
          {
          irect = irow + iconz;
 
          if (redlin(fileptr,buf,nsam,irect) == FALSE) 
             {free(rbuf); return FALSE;}

          for (isam=nsam1-1; isam < nsam2; isam++)
             {
             fval  = buf[isam];
             fmaxt = MYMAX(fmaxt,fval);
             fmint = MYMIN(fmint,fval);
             rbuf[++iloc] = fval;
             }
          }
       }

    else if (iskip > 1)
       {    /* display z slice at reduced size */

       for (irow = nrow1; irow <= nrow2; irow+=iskip)
          {
          irect = irow + iconz;
          if (redlin(fileptr,buf,nsam,irect) == FALSE) 
             {free(rbuf); return FALSE; }

          for (isam=nsam1-1; isam < nsam2; isam+=iskip)
             {
             fval  = buf[isam];
             fmaxt = MYMAX(fmaxt,fval);
             fmint = MYMIN(fmint,fval);
             rbuf[++iloc] = fval;
             }
          }
       }
    
    else if (iskip < -1)
       {  /* display z slice at magnified size */
       miskip   = -iskip;

       for (irow = nrow1; irow <= nrow2; irow++)
          { 
          irect = irow + iconz;
          if (redlin(fileptr,buf,nsam,irect) == FALSE) 
             {free(rbuf); return FALSE;}

          iloc = (irow -1) * nsami * miskip;

          for (isam=nsam1-1; isam < nsam2; isam++)
             {     /* check all pixels for min and max values */
             fval  = buf[isam];
             fmaxt = MYMAX(fmaxt,fval);
             fmint = MYMIN(fmint,fval);
             for (i = 0; i < miskip; i++)
                {
                for (m = 0; m < miskip; m++)
                   rbuf[iloc + m * nsami] = fval;
                iloc++;
                }
             }
          }
       }
    }

 else if (axis == 'y')
    {                                            /* y axis slicing */

    irow = iwant;

    if (iskip == 1)
       {   /* display y slice at normal size */
       for (islice=nslice1; islice<=nslice2; islice++)
          {
          irect = (islice - 1) * nrow + irow;

          if (redlin(fileptr,buf,nsam,irect) == FALSE) 
             {free(rbuf); return FALSE; }

          for (isam=nsam1-1; isam < nsam2; isam++)
             {
             fval  = buf[isam];
             fmaxt = MYMAX(fmaxt,fval);
             fmint = MYMIN(fmint,fval);
             rbuf[++iloc] = fval;
             }
          }
       }

    else if (iskip > 1)
       {   /* display y slice at reduced size */

       for (islice = nslice1; islice <= nslice2; islice=islice+iskip)
          {
          irect = (islice -1) * nrow + irow;
          if (redlin(fileptr,buf,nsam,irect) == FALSE) 
             {free(rbuf); return FALSE; }

          for (isam=nsam1-1; isam < nsam2; isam+=iskip)
             {
             fval  = buf[isam];
             fmaxt = MYMAX(fmaxt,fval);
             fmint = MYMIN(fmint,fval);
             rbuf[++iloc] = fval;
             }
          }
       }

    else if (iskip < -1)
       {  /* display y slice at magnified size */
       miskip   = -iskip;

       for (islice = nslice1; islice <= nslice2; islice++)
          { 
          irect = (islice - 1) * nrow + irow;
          if (redlin(fileptr,buf,nsam,irect) == FALSE) 
             {free(rbuf); return FALSE; }

          iloc = (islice - 1) * nsami * miskip;

          for (isam=nsam1-1; isam < nsam1; isam++)
             {   /* check all pixels for min and max values */
             fval  = buf[isam];
             fmaxt = MYMAX(fmaxt,fval);
             fmint = MYMIN(fmint,fval);
             for (i = 0; i < miskip; i++)
                {
                for (m = 0; m < miskip; m++)
                   rbuf[iloc + m * nsami] = fval;
                iloc++;
                }
             }
          }
       }
    }

 else if (axis == 'x')
    {                                      /* WANT X SLICE  (SLOW) */

    isam = iwant -1;

    if (iskip == 1)
       {   /* display x slice at normal size */

       for (islice=nslice1; islice<=nslice2; islice++)
          {
          irec1 = (islice - 1) * nrow;
          for (irow=nrow1; irow <=nrow2; irow++)
             {
             irect = irec1 + irow;
             if (redlin(fileptr,buf,nsam,irect) == FALSE) 
                {free(rbuf); return FALSE; }
             fval  = buf[isam];
             fmaxt = MYMAX(fmaxt,fval);
             fmint = MYMIN(fmint,fval);
             rbuf[++iloc] = fval;
             }
          }
       }

    else if (iskip > 1)
       {   /* display x slice at reduced size */

       for (islice = nslice1; islice <= nslice2; islice=islice+iskip)
          {
          irec1 = (islice -1) * nrow;
          for (irow=nrow1; irow<=nrow2; irow+=iskip)
             {
             irect = irec1 + irow;
 
             if (redlin(fileptr,buf,nsam,irect) == FALSE) 
                {free(rbuf); return FALSE; }

             fval  = buf[isam];
             fmaxt = MYMAX(fmaxt,fval);
             fmint = MYMIN(fmint,fval);
             rbuf[++iloc] = fval;
             }
          }       
       }

    else if (iskip < -1)
       {  /* display x slice at magnified size */
       miskip   = -iskip;

       for (islice = nslice1; islice <= nslice2; islice++)
          { 
          irec1 = (islice - 1) * nrow;

          iloc  = (islice - 1) * nsami * miskip;

          for (irow=nrow1; irow<=nrow2; irow++)
             {
             irect = irec1 + irow;
             if (redlin(fileptr,buf,nsam,irect) == FALSE) 
                {free(rbuf); return FALSE; }

             fval  = buf[isam];
             fmaxt = MYMAX(fmaxt,fval);
             fmint = MYMIN(fmint,fval);
             for (i = 0; i < miskip; i++)
                {
                for (m = 0; m < miskip; m++)
                   rbuf[iloc + m * nsami] = fval;
                iloc++;
                }
             }
          }
       }
    }
 
 /* allocate space for cbuf */
 if ((cbuf = (char *) malloc(nsami * nrowi * 
                 sizeof(char))) == (char *) 0)
     {      
     spout(" *** Unable to allocate cbuf memory in monn.c");
     if (rbuf) free(rbuf); return FALSE;
     }

 /* CHECK TO SEE IF THIS SLICE CAN BE NORMALIZED OK */
 if (fabs(fmaxt - fmint) < FLTZER)
    {    /* a blank slice */
    fn    = 0.0;
    fncon = nmint;
    }
 else
    {
    fn    = (float) (nmaxt - nmint) / (fmaxt - fmint);
    fncon = -fmint * fn + nmint + 0.5;
    }

 /* normalize data and convert to 8 bits */
 for (i=0; i < nsami*nrowi ; i++)
    cbuf[i] = rbuf[i] * fn + fncon;

 /* DISPLAY THIS IMAGE SLICE */
 wipic(icontxt,cbuf, ixul, iyul, nsami, nrowi, TRUE, FALSE, &imagel);

 if (rbuf) free(rbuf); rbuf = NULL;
 if (cbuf) free(cbuf); cbuf = NULL;

 return TRUE;
 }
