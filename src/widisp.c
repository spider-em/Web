
/*$Header: /usr16/software/web/src/RCS/widisp.c,v 1.16 2018/12/07 17:03:35 leith Exp $*/

/*
 C++********************************************************************
 C
 C    WIDISP
 C *********************************************************************
 C * AUTHOR:  ArDean Leith                                             *
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
 C *********************************************************************
 C
 C widisp(icontx,filedata,nrow1,nrow2,fmax_spi,fmin_spi,ixul,iyul,iskip)
 C
 C    PURPOSE:    TO DISPLAY AN IMAGE FROM fp AT IXUL,IYUL
 C
 C    PARAMETERS:    ICONTX       GRAPHICS CONTEXT
 C                   NSAM         NUMBER OF PIXELS ON A LINE
 C                   NSAM1        COL IN FILE OF IMAGE START
 C                   NSAM2        COL IN FILE OF IMAGE START
 C                   NROW1        ROW (RECORD) IN FILE OF IMAGE START
 C                   NROW2        ROW OF END OF IMAGE
 C                   fmax_spi         MAX. VALUE OF IMAGE
 C                   fmin_spi         MIN. VALUE OF IMAGE
 C                   IXUL         POSITION OF LEFT SIDE OF IMAGE ON SCREEN
 C                   IYUL         POSITION OF TOP OF IMAGE ON SCREEN
 C                   NMAX         MAX VALUE OF INDEX WRITTEN TO SCREEN
 C                   ISKIP        INCREMENT FOR DO LOOP TO SKIP PIXELS
 C
 C    CALLED BY:     mont3 image
 C
 C--*******************************************************************
*/

#include "routines.h"
#include "common.h"


 /****************************  widisp  *****************************/
 
 int widisp(GC icontx, FILEDATA *filedata, int nsam, int nsam1,int nsam2, 
            int nrow1, int nrow2, double fmin_spi, double fmax_spi,
            int ixul,  int iyul,  int nmint,   int    nmaxt, int iskip, 
            int keepimage, XImage ** imaget, int keepimagec)
 { 

 float  *buf;
 char   *cbuf;
 int    nsams, nrows, npix, ival,miskip;
 int    j, m, k,irect;
 float  scal, fm;

 if (fabs(fmax_spi - fmin_spi) < FLTZER)
    { spout("*** BLANK IMAGE SKIPPED"); return FALSE; }

 buf = filedata -> buf;

 /* find number of rows in displayed image and allocate buffer space */
 if (iskip > 1) 
    {
    nsams = 1 + (nsam2 - nsam1) / iskip;
    nrows = 1 + (nrow2 - nrow1) / iskip;
    }
 else if (iskip < -1)
    {
    miskip = -iskip;
    nsams = (nsam2 - nsam1 + 1) * miskip;
    nrows = (nrow2 - nrow1 + 1) * miskip;
    }
 else
    {
    nsams = (nsam2 - nsam1 + 1);
    nrows = (nrow2 - nrow1 + 1);
    } 
    
 if ((cbuf = (char *) malloc(nsams * nrows * sizeof(char))) == (char *) 0)    
    { 
    spout("*** Unable to allocate cbuf memory in widisp.");
    return FALSE;
    }

 /* CONVERSION FACTOR FROM FLOATING POINT TO LOGICAL * 1 RANGE */
 scal = (float) (nmaxt - nmint) / (fmax_spi - fmin_spi);
 fm   = -fmin_spi * scal + nmint + 0.5;

 npix = 0;  

 if (iskip > -2 && iskip < 2)
    {    /* display full size */

    for (irect = nrow1; irect <= nrow2; irect++)
       {
       if (redlin(filedata,buf,nsam,irect) != TRUE) 
          { free(cbuf);  return FALSE; }

       if (nmaxt < 128) 
           {   /* no need to convert numbers > 128 */   
           for (k = nsam1 - 1; k < nsam2; k++)
              {
              ival = (*(buf + k) * scal) + fm;
              *(cbuf + npix++) = ival;
              }
           }
       else
           {   /* need to convert numbers > 128 */   
           for (k = nsam1 - 1; k < nsam2; k++)
              {
              ival = (*(buf + k) * scal) + fm;
              /* convert 0..255  integers to -128...127 */
              if (ival > 127) ival = ival - 256;
              *(cbuf + npix++) = ival;
              }
           }
       }
    }

 else if (iskip > 1)
    {    /* display reduced size image */

    for (irect = nrow1; irect <= nrow2; irect = irect + iskip)
       {
       if (redlin(filedata,buf,nsam,irect) != TRUE) 
          { free(cbuf); return FALSE; }

       if (nmaxt < 128) 
           {   /* no need to convert numbers > 128 */   
           for (k = nsam1 - 1; k < nsam2; k = k + iskip)
              {
              ival = (*(buf + k) * scal) + fm;
              *(cbuf + npix++) = ival;
              }
           }
       else
           {   /* need to convert numbers > 128 */   
           for (k = nsam1 - 1; k < nsam2; k = k + iskip)
              {
              ival = (*(buf + k) * scal) + fm;
              /* convert 0..255  integers to -128...127 */
              if (ival > 127) ival = ival - 256;
              *(cbuf + npix++) = ival;
              }
           }
       }
    }


 else if (iskip < -1)
    {    /* display magnified size image */

    for (irect = nrow1; irect <= nrow2; irect++)
       {
       if (redlin(filedata,buf,nsam,irect) != TRUE) 
          { free(cbuf);   return FALSE; }
       if (nmaxt < 128) 
           {   /* no need to convert numbers > 128 */   
           for (k = nsam1 - 1; k < nsam2; k++)
              {
              ival = (*(buf + k) * scal) + fm;

              for (j = 0; j < miskip ; j++)
                 {
                 for (m = 0; m < miskip; m++)
                   *(cbuf + npix + m * nsams + j) = ival;
                 }
              npix = npix + miskip;
              }
           }
       else
           {   /* need to convert numbers > 128 */   
           for (k = nsam1 - 1; k < nsam2; k++)
              {
              ival = (*(buf+k)) * scal + fm;
              /* convert 0..255  integers to -128...127 */
              if (ival > 127) ival = ival - 256;

              for (j = 0; j < miskip ; j++)
                 {
                 for (m = 0; m < miskip; m++)
                   *(cbuf + npix + m * nsams + j) = ival;
                 }
              npix = npix + miskip;
              }
           }
        npix = (irect - nrow1 + 1) * miskip * nsams + nsam1- 1;
        }
    }

 /* display the image */
 wipic(icontx,cbuf,ixul,iyul,nsams,nrows,TRUE,keepimage,imaget);
 
 /* free up the character buffer unless it is still wanted */
 if (!keepimagec && cbuf)
     {
     free(cbuf);
     cbuf = NULL;
     }	

 /* record ximage for possible later reference */
 filedata -> ximage = *imaget;

 return TRUE;
}

