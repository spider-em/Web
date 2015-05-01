
/*$Header: /usr8/web/new/RCS/ztoxy.c,v 1.6 2005/10/18 17:00:05 leith Exp $*/
/*
C++************************************************************************
C
C ZTOXY -- CREATED JULY 92 al
C          ADDED Z AXIS SEPT 01 ArDean Leith
C          SIMPLIFIED MAY 02 al
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
C    ZTOXY(NSAM,NSAM1,NSAM2,NROW,NROW1,NROW2,
C           NSLICE,NSLICE1,NSLICE2,AXIS,FN,FNCON)
C
C    PURPOSE:         DIPLAYING MONTAGE FROM FILE SERIES ON X WORKSTATION
C
C    RETURNS:         ERROR FLAG
C
C    CALLED BY:       MONT3
C
C    NOTES:
C
C     AXIS      NEW SECTION         NEW ROW            NEW SAMPLE
C     ---        -----------     -----------------    --------------
C      X       (ISAM-NSAM1+1)    (ISLICE-NSLICE1+1)   (IROW-NROW1+1)
C
C
C     LOC = (NEW SECTION-1)*NSAMS*NROWS + (NEW ROW-1)*NSAMS + NEW SAMPLE
C
C
C23456789012345678901234567890123456789012345678901234567890123456789012
C***********************************************************************
*/

#include "files.h"
#include "routines.h"
#include "common.h"

/**********************************************************/

 int ztoxy(FILEDATA *filedata, char *lbuf, 
            char axis, float fn, float fncon, int iskip)
 {  

 int        nsamd, nrowd, nsliced, icon;
 int        npixpsl, miskip, iskipz, icons, irect;
 int        iconr, ival, iloc, l, m, iloct; 
 int        isam, irow, islice;
 float      *buf;

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
    {
    spout(" *** IN ZTOXY: UNSUPPORTED AXIS");
    return FALSE;
    }

 buf      = filedata -> buf;
 if (iskip > -1 && iskip < 1) iskip = 1;
 npixpsl  = nrowd * nsamd;
 miskip   = abs(iskip);

 if (iskip >= 1)
    {   /* display at full or reduced size */

    if (axis == 'y')
       {
       icon = -nrow1 * npixpsl - nslice1 * nsamd / iskip -  nsam1 / iskip ;  
       }
    if (axis == 'x')
       {
       icon = -nsam1 * npixpsl - nslice1 * nsamd / iskip -  nrow1 / iskip;
       }
    }
 else if (iskip < -1)
    {   /* display magnified image */
    if (axis == 'y')
       icon = -nrow1 * npixpsl - nslice1 * miskip * nsamd - 1;
    else
       icon = -nsam1 * npixpsl - nslice1 * miskip * nsamd -
               nrow1 * miskip;
    }

 /* find skip factors for z axis */
 iskipz = MYMAX(1,iskip);
 
 for (islice=nslice1; islice<=nslice2; islice+=iskipz)
   {
   if (axis == 'y')
      {                                                     /* y axis */
      if (iskip < 0)
         icons = islice * nsamd * miskip + icon;
      else 
         icons = islice * nsamd / iskip  + icon;

      for (irow=nrow1; irow <= nrow2; irow+=iskipz)
         {
         irect = (islice - 1) * nrow + irow;
         if (redlin(filedata,buf,nsam,irect) == FALSE)  return FALSE;

         if (iskip >= 1)
            {  /* display full or reduced size y slice */

            iconr = irow * npixpsl + icons;
            for (isam=nsam1; isam <= nsam2; isam+=miskip)
               {
               ival  = buf[isam-1] * fn + fncon;
               if (ival >= 128) ival = ival - 256;

               iloc = isam / iskip + iconr;

/***************bug ************/
   if (iloc < 0 || iloc > nsamd * nrowd * nsliced -1)
    printf("illegal iloc: %d, irow: %d, isam: %d \n",iloc,irow,isam);

/********************/

               lbuf[iloc] = ival;
               }         
            }
            
         else if (iskip < -1) 
            {   /* display magnified image y slice */

            iloc = irow * npixpsl + icons;
            
            for (isam=nsam1-1; isam < nsam2; isam++)
               {
               ival  = buf[isam] * fn + fncon;
               if (ival >= 128) ival = ival - 256;

               for (l = 0; l < miskip; l++)
                  {
                  iloc++;
                  for (m = 0; m < miskip; m++)
                     {
/***************bug ************/
   iloct = iloc + m * nsamd;
   if (iloct < 0 || iloct > nsamd * nrowd * nsliced - 1) 
    {
    printf("illegal iloct: %d, irow: %d, isam: %d \n",iloct,irow,isam);
    printf("icons: %d, iconr: %d, npixpsl: %d \n",icons,iconr,npixpsl);
    printf("l: %d, m: %d  \n",l,m);
    }
/********************/
                     lbuf[iloc + m * nsamd] = ival;
                     }
                  }
               }      
            }
         }
      }
      
   else if (axis == 'x')
      {                                                   /*  x axis */ 
      if (iskip >= 1)
         {   /* full or reduced size x slice */
         icons = islice * nsamd / iskip + icon;

         for (irow=nrow1; irow <= nrow2; irow+=miskip)
            {
            irect = (islice - 1) * nrow + irow;
            if (redlin(filedata,buf,nsam,irect) == FALSE)  return FALSE;
            iconr = irow / iskip + icons;

            for (isam=nsam1; isam <= nsam2; isam++)
               {
               ival  = buf[isam-1] * fn + fncon;
               if (ival >= 128) ival = ival - 256;

               iloc = isam * npixpsl + iconr;
/***************bug ************/
   if (iloc < 0 || iloc > nsamd * nrowd * nsliced -1) {
    printf("illegal iloc: %d, irow: %d, isam: %d \n",iloc,irow,isam);
    printf("icons: %d, iconr: %d, npixpsl: %d \n",icons,iconr,npixpsl);
     }
/********************/
               lbuf[iloc] = ival;
               }         
            }
         }
      else if (iskip < -1)
         {   /* magnifed x slice */ 

         icons = islice * nsamd * miskip + icon;

         for (irow=nrow1; irow <= nrow2; irow++)
            {
            irect = (islice - 1) * nrow + irow;
            if (redlin(filedata,buf,nsam,irect) == FALSE) return FALSE;

            iconr = irow * miskip + icons;

            for (isam=nsam1; isam <= nsam2; isam++)
               {
               ival  = buf[isam-1] * fn + fncon;
               if (ival >= 128) ival = ival - 256;

               iloc = isam * npixpsl + iconr;
 
               for (l = 0; l < miskip; l++)
                  {
                  for (m = 0; m < miskip; m++)
                     {
        
/***************bug ************/
   iloct = iloc + m * nsamd;
   if (iloct < 0 || iloct > nsamd * nrowd * nsliced -1) 
    {
    printf("illegal iloct: %d, irow: %d, isam: %d \n",iloct,irow,isam);
    printf("icons: %d, iconr: %d, npixpsl: %d \n",icons,iconr,npixpsl);
    printf("l: %d, m: %d nsamd: %d ,icon %d \n",l,m,nsamd,icon);
    }
/********************/
                     lbuf[iloc + m * nsamd] = ival;
                     }
                  iloc++;
                  }
               }
            }
         }
      }
 

   else if (axis == 'z')
      {                                                  /*  z axis  */ 
      iloc  = (islice - nslice1) * npixpsl;

      if (iskip >= 1)
         {   /* full size z slice or  reduced size z slice */

         for (irow=nrow1; irow <= nrow2; irow+=miskip)
            {
            irect = (islice - 1) * nrow + irow;
            if (redlin(filedata,buf,nsam,irect) == FALSE)  
               {return FALSE;}

            for (isam=nsam1; isam <= nsam2; isam++)
               {
               ival       = buf[isam-1] * fn + fncon;
               if (ival >= 128) ival = ival - 256;
               lbuf[iloc++] = ival;
               }         
            }
         }
      else if (iskip < -1)
         {   /* magnifed z slice */ 

         for (irow=nrow1; irow <= nrow2; irow++)
            {
            irect = (islice - 1) * nrow + irow;
            if (redlin(filedata,buf,nsam,irect) == FALSE) 
               {return FALSE;}

            for (isam=nsam1; isam <= nsam2; isam++)
               {
               ival  = buf[isam-1] * fn + fncon;
               if (ival >= 128) ival = ival - 256;

               for (l = 0; l < miskip; l++)
                  {
                  for (m = 0; m < miskip; m++)
                     {lbuf[iloc + m * nsamd] = ival;}
                  iloc++;
                  }
               }
            iloc += nsamd * (miskip - 1);         
            }
         }
      }
   }

 return TRUE;
 }       

