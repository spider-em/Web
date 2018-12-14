
/*$Header: /usr16/software/web/src/RCS/ztoxyn.c,v 1.7 2005/10/18 16:59:52 leith Exp $*/
/*
C++************************************************************************
C
C ZTOXYN -- CREATED MAR 90
C           CONVERTED TO C, JULY 92 al
C           ADDED Z AXIS SEPT 01 ArDean Leith
C           SIMPLIFIED MAY 02 al
C
C **********************************************************************
C * WEB.    AUTHOR: ARDEAN LEITH                                       *
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
C   ZTOXYN(LUNF,NSAM,NSAM1,NSAM2,NROW,NROW1,NROW2,
C           NSLICE,NSLICE1,NSLICE2,AXIS,NMAX,IRTFLG)
C
C    PURPOSE:         DIPLAYING MONTAGE FROM FILE SERIES ON X WORKSTATION
C                     NORMALIZED SLICE BY SLICE
C
C    CALLED BY:       MONT3
C
C    NOTES:
C
C     AXIS      NEW SECTION         NEW ROW            NEW SAMPLE
C     ---        -----------     -----------------    --------------
C      X       (ISAM-NSAM1+1)    (ISLICE-NSLICE1+1)   (IROW-NROW1+1)
C
C     LOC = (NEW SECTION-1)*NSAMS*NROWS + (NEW ROW-1)*NSAMS + NEW SAMPLE
C
C***********************************************************************
*/

#include "files.h"
#include "routines.h"
#include "common.h"

/**************************  ztoxyn  ********************************/

 int ztoxyn(FILEDATA *filedata, char *cbuf, char axis, int nmaxt, 
            int iskip, int nmint)
 {  
   
 int        nsamd, nrowd, nsliced;
 int        locin, locout, i, npixpsl, icon, miskip;
 int        iskipz, iskipr, icons, irect, ipane, iconr, iloc;
 int        isam, irow, islice, l, m, ival, igo;
 float      *buf, *rbuf, *fmint, *fmaxt, tmin, tmax;
 float      fn, fncon, fval; 

 /* set image size and window */

 if (axis == 'z')
     {        /* 3-d file with z slice */
     dispsize(nsam1,nsam2,nrow1,nrow2,nslice1,nslice2,iskip, &nsamd, &nrowd, &nsliced);
     }
 else if (axis == 'y')
     {        /* y slice */
     dispsize(nsam1,nsam2,nslice1,nslice2,nrow1,nrow2,iskip, &nsamd, &nrowd, &nsliced);
     }
 else if (axis == 'x')
     {        /* x slice */
     dispsize(nrow1,nrow2,nslice1,nslice2,nsam1,nsam2,iskip, &nsamd, &nrowd, &nsliced);
     }
 else
    { spout(" *** IN ZTOXYN: UNSUPORTED AXIS"); return FALSE; }

 if (iskip > -1 && iskip < 1) iskip = 1;
 miskip   = abs(iskip);
 npixpsl  = nrowd * nsamd;

 if (iskip >= 1)
    {   /* display at full or reduced size */
    if (axis == 'y')
       icon = -nrow1 * npixpsl - nslice1 * nsamd / iskip - nsam1 / iskip;  
    else if (axis == 'x')
       icon = -nsam1 * npixpsl - nslice1 * nsamd / iskip - nrow1 / iskip;
    }
 else if (iskip < -1)
    {   /* display magnified image */
    if (axis == 'y')
       icon = -nrow1 * npixpsl - nslice1 * miskip * nsamd - 1;
    else
       icon = -nsam1 * npixpsl - nslice1 * miskip * nsamd -
               nrow1 * miskip;
    }

 /* find skip factors for y & z axis */
 iskipz = MYMAX(1,iskip);

 /* allocate space for range arrays */
 if (((fmint = (float *) malloc(nsliced * sizeof(float))) == 
           (float *) 0) ||
     ((fmaxt = (float *) malloc(nsliced * sizeof(float))) == 
           (float *) 0) ||
     ((rbuf  = (float *) malloc(nsliced * nsamd * nrowd * sizeof(float))
           ) ==  (float *) 0))
    {  spout(" *** Unable to allocate memory ztoxyn."); return FALSE; }

 /* set up lpointer to redlin array */
 buf = filedata -> buf;

 for (i=0; i< nsliced; i++)
    {    /* zero the slice min/max values */
    fmint[i] = 10e10;  fmaxt[i] = -10e10;
    }

 for (islice=nslice1; islice<=nslice2; islice+=iskipz)
   {
   if (axis == 'y')
      {                                                          /* y axis slicing */
      if (iskip < 0)
         icons = islice * nsamd * miskip + icon;
      else 
         icons = islice * nsamd / iskip  + icon;

      for (irow=nrow1; irow <= nrow2; irow+=iskipz)
         {
         irect = (islice - 1) * nrow + irow;
         if (redlin(filedata,buf,nsam,irect) == FALSE) 
             {free(rbuf); free(fmint); free(fmaxt);return FALSE;}

         ipane = irow - nrow1;
         tmin  = fmint[ipane];
         tmax  = fmaxt[ipane];

         if (iskip >= 1)
            {  /* display full or reduced y slice */

            iconr = irow * npixpsl + icons;
            
            for (isam=nsam1; isam <= nsam2; isam+=miskip)
               {
               fval       = buf[isam-1];
               tmin       = MYMIN(fval,tmin);
               tmax       = MYMAX(fval,tmax);

               iloc       = isam / iskip + iconr;
               rbuf[iloc] = fval;
               }         
            }

         else if (iskip < -1) 
            {   /*  magnified image y slice */

            iloc = irow * npixpsl + icons;
            
            for (isam=nsam1; isam <= nsam2; isam++)
               {
               fval  = buf[isam-1];
               tmin  = MYMIN(fval,tmin);
               tmax  = MYMAX(fval,tmax);

               for (l = 0; l < miskip; l++)
                  {
                  iloc++;
                  for (m = 0; m < miskip; m++)
                     rbuf[iloc + m * nsamd] = fval;
                  }
               }
            }
            fmint[ipane] = tmin;
            fmaxt[ipane] = tmax;
         }
     }
      
   else if (axis == 'x')
      {                                                      /* axis = x */
      if (iskip >= 1)
         {   /* full or reduced size x slice */
         icons = islice * nsamd / iskip + icon;

         for (irow=nrow1; irow <= nrow2; irow+=miskip)
            {
            irect = (islice - 1) * nrow + irow;
            if (redlin(filedata,buf,nsam,irect) == FALSE) 
               {free(rbuf); free(fmint); free(fmaxt);return FALSE;}
            iconr = irow / iskip + icons;

            for (isam=nsam1; isam <= nsam2; isam+=miskip)
               {
               fval  = buf[isam];
               ipane = isam - nsam1;
               fmint[ipane]  = MYMIN(fval,fmint[ipane]);
               fmaxt[ipane]  = MYMAX(fval,fmaxt[ipane]);

               iloc = isam *npixpsl + iconr;
               rbuf[iloc] = fval;
               }         
            }
         }

      else if (iskip < -1)
         {   /* magnifed x slice */ 

         icons = islice * nsamd * miskip + icon;

         for (irow=nrow1; irow <= nrow2; irow++)
            {
            irect = (islice - 1) * nrow + irow;
            if (redlin(filedata,buf,nsam,irect) == FALSE) 
               {free(rbuf); free(fmint); free(fmaxt);return FALSE;}

            iconr = irow * miskip + icons;

            for (isam=nsam1; isam <= nsam2; isam++)
               {
               fval  = buf[isam];
               ipane = isam - nsam1;
               fmint[ipane]  = MYMIN(fval,fmint[ipane]);
               fmaxt[ipane]  = MYMAX(fval,fmaxt[ipane]);

               iloc = isam * npixpsl + iconr;

               for (l = 0; l < miskip; l++)
                  {
                  for (m = 0; m < miskip; m++)
                     rbuf[iloc + m * nsamd] = fval;
                  iloc++;
                  }
               }
            }
         }
      }

   else if (axis == 'z')
      {                                                                 /* axis  z  */
      iloc  = (islice - nslice1) * npixpsl;

      ipane = islice - nslice1;

      if (iskip >= 1)
         {   /* full or reduced size z slice */

         for (irow=nrow1; irow <= nrow2; irow+=miskip)
            {
            irect = (islice - 1) * nrow + irow;
            if (redlin(filedata,buf,nsam,irect) == FALSE) 
               {free(rbuf); free(fmint); free(fmaxt);return FALSE;}

            for (isam=nsam1; isam <= nsam2; isam+=miskip)
               {
               fval         = buf[isam];
               fmint[ipane] = MYMIN(fval,fmint[ipane]);
               fmaxt[ipane] = MYMAX(fval,fmaxt[ipane]);
               rbuf[iloc++] = fval;
               }
            }
         }

      else if (iskip < -1)
         {   /* magnifed z slice */ 

         for (irow=nrow1; irow <= nrow2; irow++)
            {
            irect = (islice - 1) * nrow + irow;
            if (redlin(filedata,buf,nsam,irect) == FALSE) 
               {free(rbuf); free(fmint); free(fmaxt);return FALSE;}

            for (isam=nsam1; isam <= nsam2; isam++)
               {
               fval         = buf[isam];
               ipane        = isam - nsam1;
               fmint[ipane] = MYMIN(fval,fmint[ipane]);
               fmaxt[ipane] = MYMAX(fval,fmaxt[ipane]);

               for (l = 0; l < miskip; l++)
                  {
                  for (m = 0; m < miskip; m++)
                     rbuf[iloc + m * nsamd] = fval;
                  iloc++;
                  }
               }
            iloc += nsamd * (miskip - 1);         
            }
         }
      }
   }

 /* normalize the floating point montage by slice & convert to 8 bits */
locout = 0;

for (islice=0; islice<nsliced; islice++)
    {
    if (abs(fmaxt[islice] - fmint[islice]) < 10e-10)
       {      /* blank slice */
       fn    = 0.0;
       fncon = nmint;
       }
    else
       {      /* FIND SCALE FACTORS FOR IMAGE DENSITY 0...NMAX  */
       fn    = (nmaxt - nmint) / (fmaxt[islice] - fmint[islice]);
       fncon = -fn  * fmint[islice] + nmint + 0.5;
       }

    /* FIND STARTING POSITION IN STORED ARRAY FOR THIS SLICE */
    igo   = islice * npixpsl;

    if (nmaxt < 128)
       {
       for (locin = igo; locin < (igo+npixpsl); locin++)
              cbuf[locout++] = rbuf[locin] * fn + fncon;
       }
    else
       {
       for (locin = igo; locin < (igo+npixpsl); locin++)
          {
          ival = rbuf[locin] * fn + fncon;
          if  (ival > 127) ival = ival - 256;
          cbuf[locout++] = ival;
          }
       }
    }

free(rbuf);
free(fmint);
free(fmaxt);

return TRUE;
}

