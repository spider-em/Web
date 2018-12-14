
/*$Header: /usr16/software/web/src/RCS/redvol.c,v 1.14 2018/12/07 17:03:34 leith Exp $*/

/*
C***********************************************************************
C
C   REDVOL.C  -- Created NOV 13 1993
C             
C **********************************************************************
C *  AUTHOR: ArDean Leith                                              *
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
C  redvol(filedata,  want8, cptr, want32, fptr, nsam1, nsam2,   
c         nrow1, nrow2,  nslice1, nslice2,  nmint, nmaxt,  
c         fmin_spit fmax_spit, scal, offset)
C           
C
C  PURPOSE:     Reads spider image into 8 / or 32 bit array normalized 
C               from nmin to nmax.  Returns normaliztion constants also.
C
C  PARAMETERS:  filedatat
c               want8, want32      Want 8 bit buffer, want 32 bit buffer
c               cptr               Pointer to 8 bit volume    (sent/ret.)
c               nsam1,nsam2        Sample range               (sent)
c               nrow1,nrow2        Row range                  (sent)
c               nslice1,nslice2    Slice range                (sent)
c               nmint,nmaxt        Target normalization range (sent)
c               fmint,fmaxt        Desired file range         (sent)
c               scal, offset       Normalization paremeters   (returned)
c               iskipt             Skip (mag. ) factor        (sent)
c
C  RETURNS:     TRUE or FALSE 
C
C  NOTE:        For char variables char = 128 is same as char = -128
C               so no manipulation is necessary.
C               The old if(char > 127) char = char - 256 is NOT needed.
C
C--*********************************************************************
*/

#include "files.h"
#include "common.h"
#include "routines.h"

 /* Externally defined variables */

 /* Common variables used here */
 static int nsams, nrows, nslices;

 /*************************   redvol   ****************************/

 int redvol(FILEDATA *filedata, 
            int want8,    char  ** cptr,
            int want32,   float ** fptr,
            int nsam1,    int nsam2,   
            int nrow1,    int nrow2, 
            int nslice1,  int nslice2, 
            int nmint,    int nmaxt, 
            float fmint,  float fmaxt, 
            float * scal, float * offset,
            int iskipt)
 {
 int         nsam, nrow, irec, j, m, npixlin, miskip, npixcol;
 int         islice, irow, isam, ival, newmin, noff, nsamst, nrowst;
 int         npixpimg;
 float       *buf,  *fptrt,  *fptrtt, *fptrttt;
 char        *cptrt, *cptrtt, *cptrttt;
 float       buft;
 int       ibug,ival1;

 nsam    = filedata -> nsam;
 nrow    = filedata -> nrow;

 /* Find nsams, nrows in displayed image (do not expand nslices)*/
 dispsize(nsam1,nsam2,nrow1,nrow2,nslice1,nslice2,iskipt,
         &nsams, &nrows, &nslices);

 /* Find buf for redlin */
 buf    = filedata -> buf;

 newmin = (fmint != filedata-> fmin_spi || 
           fmaxt != filedata->fmax_spi) ;

 if (want8)
    {
    /* Find normalization constants for 8 bit output data */
    scale32to8(fmint, fmaxt, iskipt, imagego, imageend, scal, offset);

    if (!(*cptr)) 
       {
        /* allocate cbuf space first */
        if ((*cptr = (char *) malloc(nsams * nrows * nslices *
                      sizeof(char))) ==  (char *) 0)
            { 
            spout(" *** Unable to allocate memory in redvol.");
            return FALSE;
            }
       }
    }

 if (want32 && !(*fptr)) 
    {
    /* Allocate float space first */
    nsamst = nsam2 - nsam1 + 1;
    nrowst = nrow2 - nrow1 + 1;
    if ((*fptr = (float *) malloc(nsamst * nrowst * nslices *
                      sizeof(float))) == (float *) 0)
        { 
        spout(" *** Unable to allocate memory in redvol.");
        return FALSE;
        }
    }

 /* Set temporary data pointers */
 fptrt = *fptr;  /* points to start of current slice */
 cptrt = *cptr;  

 if (want32 && nsam1 == 1 && nsam2 == nsam &&
               nrow1 == 1 && nrow2 == nrow)
    {  /* Provides much faster input than code below */
    npixpimg = nsam * nrow;

    /* Fill fptr vol with 32 bit data */
    for (islice = nslice1; islice <= nslice2; islice++)
       {  /* Reads whole image at a time! */
       if (!redlin(filedata,fptrt,npixpimg,islice)) return FALSE;

       if (want8)
          {/* Also fill cptr volume with 8 bit data for this image */
          scaleimg(fptrt, nsam, nrow, iskipt, newmin, 
                   *scal, *offset, cptrt);
          cptrt += npixpimg;
          }
       fptrt += npixpimg;
       }                
    return TRUE;
    }


 /* Fill cptr volume with 8 bit data and fptr vol with 32 bit data */

 if (iskipt > -2 && iskipt < 2)
    {    /************************************************* Full size */

    for (islice = nslice1; islice <= nslice2; islice++)
       {
       for (irow = nrow1; irow <= nrow2; irow++)
          {
          irec = (islice - 1) * nrow + irow;
          if (!redlin(filedata,buf,nsam,irec)) return FALSE;
        
          if (newmin)
             { /* Want to alter min & max  definition*/
             if (want8 && want32)
                {  
                for (isam =nsam1-1; isam < nsam2; isam++)
                   {
                   ival     = (int) (buf[isam] * *scal + *offset);
                   *cptrt++ = MYMIN(MYMAX(ival,imagego),imageend);
                   *fptrt++ = buf[isam];
                   }  /* for isam           */
                }     /* if want8 && want32 */

             else 
                {  /* Want8 (can not have want32 only and newmin) */
                for (isam =nsam1-1; isam < nsam2; isam++)
                   {
                   ival = (int) (buf[isam] * *scal + *offset);
                   *cptrt++ = MYMIN(MYMAX(ival,imagego),imageend);

//#ifdef WEB_TRUE
//                   if (ival < 0 || ival > 255)
//                      {
//                      ibug  = MYMIN(MYMAX(ival,imagego),imageend);
//                      ival1 = (int) (buf[isam] * *scal + *offset);
//                      printf(" ival1: %d  cval: %d ibug: %d\n",
//                             ival1,ival,ibug);
//                      }
//#endif

                   }  /* For isam      */
                }     /* Else  (want8) */
             }        /* If newmin     */

          else
             {  /* Not newmin */
             if (want8 && want32)
                {  
                for (isam =nsam1-1; isam < nsam2; isam++)
                   {
                   *cptrt++ = (int) (buf[isam] * *scal + *offset);
                   *fptrt++ = buf[isam];
                   }  /* For isam   */
                }     /* If want8 && want32 */

             else if (want8)
                {  
                for (isam =nsam1-1; isam < nsam2; isam++)
                   {
                   *cptrt++ = (int) (buf[isam] * *scal + *offset);
                   }  /* For isam   */
                }     /* if want8   */

             else
                {  /* want32 */
                for (isam =nsam1-1; isam < nsam2; isam++)
                   {
                   *fptrt++ = buf[isam];
                   }  /* For isam   */
                }     /* If want32  */
             }        /* Not newmin */
          }           /* For irow   */
       }              /* For islice */
    }                 /* If iskipt  */


 else if (iskipt > 1)
    {    /************************************** Reduced size image */
    for (islice = nslice1; islice <= nslice2; islice++)
        {
       for (irow = nrow1; irow <= nrow2; irow += iskipt)
          {
          irec = (islice - 1) * nrow + irow;
          if (!redlin(filedata,buf,nsam,irec)) return FALSE;
          
          if (newmin)
             { /* Want to alter min & max  definition*/
             if (want8 && want32)
                {  
                for (isam =nsam1-1; isam < nsam2; isam += iskipt)
                   {
                   ival = (int)(int) (buf[isam] * *scal + *offset);
                   ival = MYMIN(MYMAX(ival,imagego),imageend);
                   *cptrt++ = ival;
                   *fptrt++ = buf[isam];
                   }  /* For isam           */
                }     /* If want8 && want32 */

             else if (want8)
                {  
                for (isam =nsam1-1; isam < nsam2; isam += iskipt)
                   {
                   ival = (int) (buf[isam] * *scal + *offset);
                   ival = MYMIN(MYMAX(ival,imagego),imageend);
                   *cptrt++ = ival;
                   }  /* For isam   */
                }     /* If want8   */
             }        /* If newmin  */
          else
             {  /* Not newmin */
             if (want8 && want32)
                {  
                for (isam =nsam1-1; isam < nsam2; isam += iskipt)
                   {
                   *cptrt++ = (int) (buf[isam] * *scal + *offset);
                   *fptrt++ = buf[isam];
                   }  /* For isam   */
                }     /* If want8 && want32 */

             else if (want8)
                {  
                for (isam =nsam1-1; isam < nsam2; isam += iskipt)
                   {
                   *cptrt++ = (int) (buf[isam] * *scal + *offset);
                   }  /* For isam      */
                }     /* If want8      */

             else
                { /* want32 only */ 
                for (isam =nsam1-1; isam < nsam2; isam += iskipt)
                   {
                   *fptrt++ = buf[isam];
                   }  /* For isam      */
                }     /* Else (want32) */
             }        /* Newmin        */
          }           /* For irow      */
       }              /* For islice    */
    }                 /* If iskipt > 1 */
   

 else if (iskipt < -1)
    {    /************************************* Magnified size image */
    miskip  = -iskipt;
    npixlin = 0;

    for (islice = nslice1; islice <= nslice2; islice++)
       {
       for (irow = nrow1; irow <= nrow2; irow++)
          {
          irec = (islice - 1) * nrow + irow;
          if (!redlin(filedata,buf,nsam,irec)) return FALSE;
          cptrtt  = cptrt + npixlin;
          fptrtt  = fptrt + npixlin;

          /* Npixlin & npixcol used to set corner of active image area*/
          npixlin += miskip * nsams;
          npixcol = 0; 

          if (newmin)
             { /* Want to alter min & max  definition*/
             if (want8 && want32)
                { 
                for (isam =nsam1-1; isam < nsam2; isam++)
                   {
                   buft    = buf[isam];
                   ival    = (int) (buft * *scal + *offset);
                   ival    = MYMIN(MYMAX(ival,imagego),imageend);
                   cptrttt = cptrtt + npixcol - 1;
                   fptrttt = fptrtt + npixcol - 1;
                   npixcol += miskip;

                   for (j=0; j < miskip; j++)
                      {
                      noff = 0;
                      cptrttt++;
                      fptrttt++;
                      for (m=0; m < miskip; m++)
                         {
                         *(cptrttt + noff) = ival;
                         *(fptrttt + noff) = buft;
                         noff += nsams;
                         }  /* For m              */
                      }     /* For j              */
                   }        /* For isam           */
                }           /* If want8 && want32 */

             else if (want8)
                {  
                for (isam =nsam1-1; isam < nsam2; isam++)
                   {
                   ival    = (int) (buf[isam] * *scal + *offset);
                   ival    = MYMIN(MYMAX(ival,imagego),imageend);
                   cptrttt = cptrtt + npixcol - 1;
                   npixcol += miskip;

                   for (j=0; j < miskip; j++)
                      {
                      noff = 0;
                      cptrttt++;
                      for (m=0; m < miskip; m++)
                         {
                         *(cptrttt + noff) = ival;
                         noff += nsams;
                         }  /* for m      */
                      }     /* for j      */
                   }        /* for isam   */
                }           /* for want8  */
             }              /* if newmin  */

          else
             { /* Not newmin */
             if (want8 && want32)
                {  
                for (isam =nsam1-1; isam < nsam2; isam++)
                   {
                   buft    = buf[isam];
                   ival    = (int) (buft * *scal + *offset);
                   cptrttt = cptrtt + npixcol - 1;
                   fptrttt = fptrtt + npixcol - 1;
                   npixcol += miskip;

                   for (j=0; j < miskip; j++)
                      {
                      noff = 0;
                      cptrttt++;
                      fptrttt++;
                      for (m=0; m < miskip; m++)
                         {
                         *(cptrttt + noff) = ival;
                         *(fptrttt + noff) = buft;
                         noff += nsams;
                         }  /* For m              */
                      }     /* For j              */
                   }        /* For isam           */
                }           /* If want8 && want32 */

             else if (want8)
                {  
                for (isam =nsam1-1; isam < nsam2; isam++)
                   {
                   ival    = (int) (buf[isam] * *scal + *offset);
                   cptrttt = cptrtt + npixcol - 1;
                   npixcol += miskip;

                   for (j=0; j < miskip; j++)
                      {
                      noff = 0;
                      cptrttt++;
                      for (m=0; m < miskip; m++)
                         {
                         *(cptrttt + noff) = ival;
                         noff += nsams;
                         }  /* For m      */
                      }     /* For j      */
                   }        /* For isam   */
                }           /* For want8  */

             else
                {     /* Want32 only */
                for (isam =nsam1-1; isam < nsam2; isam++)
                   {
                   buft    = buf[isam];
                   fptrttt = fptrtt + npixcol -1;
                   npixcol += miskip;

                   for (j=0; j < miskip; j++)
                      {
                      noff = 0;
                      fptrttt++;
                      for (m=0; m < miskip; m++)
                         {
                         *(fptrttt + noff) = buft;
                         noff += nsams;
                         }  /* For m          */
                      }     /* For j          */
                   }        /* For isam       */
                }           /* Else (want32)  */ 
             }              /* Not newmin     */
          }                 /* For irow       */
       }                    /* For islice     */
    }                       /* If iskipt      */

 return TRUE;
 }




