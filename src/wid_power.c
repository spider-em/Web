/*
 ***********************************************************************
 *  WID_POWER.FOR  -- CREATED FEB 93
 *  Translated to wid_power.c by          Mahieddine Ladjadj   5/24/94
 *               24 bit display bug fixed ArDean Leith         11/8/07
 *
 C *********************************************************************
 C * AUTHOR: ArDean Leith                                              *
 C=* FROM: WEB - VISUALIZER FOR SPIDER MODULAR IMAGE PROCESSING SYSTEM *
 C=* Copyright (C) 1992-2007  Health Research Inc.                     *
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
 C  int wid_power(perm, chktsiz)
 C
 C  PURPOSE:    Readsback window, stores in a image file
 C
 C  PARAMETERS: perm         Permanent or temporary spectrum
 C              chktsiz      Checks size of window
 C
 C  CALLED BY:  powerbut_pop()
 C
 C  CALLS:      power   X-Windows routines
*
************************************************************************
*/

#include "common.h"
#include "routines.h"

/********************************* wid_power()   **********************/

int wid_power(int perm, int *chktsiz)
   {
   register int  j, i, k;
   char	         outstr[80];
   static int    nsamwt, nrowwt;
   int           ik, nsamok, nrowok, it, irtflg, ipad, iplanes;
   XImage        *imagep;
   char          *lbuf;
   char          *excbuf;
   float *bufin, fmint, fmaxt, fmsc, scal, b;

   int           *jptr;

   /* Kludge to get around too many continuations */
   int nmax = 127, GOODMAX = 46;
   int good[] = {
	   8,  16,  20,  24,  32,  36,  40,  48,  60,  64,
	  72,  80,  96, 100, 108, 120, 128, 144, 160, 180, 
	 192, 200, 216, 240, 256, 288, 300, 320, 360, 384, 
	 400, 432, 480, 512, 576, 600, 640, 700, 720, 768,
	 800, 864, 900, 960, 1000, 1024
      };

   /* Set waitcursor */
   waitcursor(iwin, TRUE, 79);

   if (*chktsiz)
      {

      if (ixulw == 0)
         { spout(" Window location set to 5 "); ixulw = 5; }
      if (iyulw == 0)
         { spout(" Window location set to 5 "); iyulw = 5; }

      if (nsamw == 0)
       { spout(" Window columns set to 32 "); nsamw = 32; }
      if (nroww == 0)
       { spout(" Window set to a square "); nroww = nsamw; }

      nsamwt = nsamw;
      nrowwt = nroww;

      if (nsamwt > 1024)
       { spout(" Window columns reduced to 1024 "); nsamwt = 1024; }
      if (nrowwt > 1024)
       { spout(" Window rows  reduced to 1024 "); nrowwt = 1024; }


      nsamok = nrowok = FALSE;
      /*--------------------------- changed by YH */
      for (i = GOODMAX-1; i >0 ; i--)
          {
          it = good[i-1];	/* floor */
	  ik = good[i];		/* ceiling */
          if (!nsamok)
             { /* check column size */
             if (it <= nsamw)	/* get the correct column size */ 
                {
		nsamwt = ( (nsamw-it)>abs(ik-nsamw)? ik : it );
		nsamok = TRUE;
		if (nsamwt != nsamw) {
		    sprintf(outstr, " *** Column size reset from %d to %d.",nsamw,nsamwt);
		    spout(outstr);
		    }
                }
	     }
          if (!nrowok)
             { /* Check row size */
             if (it <= nroww) /* Get the correct row size */
                {
		nrowwt = ( (nroww-it)>abs(ik-nroww)? ik : it );
		nrowok = TRUE;
		if (nrowwt != nroww) {
		    sprintf(outstr, " *** Row size reset from %d to %d.",nroww,nrowwt);
		    spout(outstr);
		    }
                }
             }
          if (nsamok && nrowok) {
	     nsamw = nsamwt;
	     nroww = nrowwt;
	     break ;    /* get out of loop */
	     }
          }       /****      for (i = 0; i < GOODMAX; i++)  **/
          /*---------------- changed by YH */

      if (!nrowok && !nsamok)
          { spout(" *** Spectrum sizing failed ");
            waitcursor(iwin,FALSE,79); return 1; } 

      *chktsiz = FALSE;
      }                  /* if (*chktsiz)  */

   /* Read an Z-Pixmap from window  */
   imagep  = (XImage *) NULL;

   iplanes = pow(2,8) - 1;

   /* Read an  Z-Pixmap from window  */
   imagep = (XImage *) NULL;
   if ( (imagep = XGetImage(idispl, imagsav, ixulw, iyulw,
                (unsigned int) nsamwt, (unsigned int) nrowwt,
                (unsigned long) iplanes, (int) ZPixmap) ) == (XImage *) NULL)                
        {
        spout("*** XGetImage operation Failed ");
        waitcursor(iwin,FALSE,79);
        return 2;
        }

   /* Images are padded to units of four bytes by XGetImage  */ 
   /*   see nsamwt .vs. imagep->bytes_per_line               */
   ipad = (nsamwt % 4) ? 4 - (nsamwt % 4) : 0;
                                                   
   /* Run time allocation of memory space for 8 bit image */
   if ( (bufin = (float *) malloc(nsamwt * nrowwt * sizeof(float)) ) == (float *) NULL) 
       {
       spout(" *** wid_power.c CAN'T ALLOCATE MEMORY for bufin");
       waitcursor(iwin, FALSE, 79); XDestroyImage(imagep);
       return 5;
       }

    if ((imagep->depth) == 8) 
      {
      /* Convert image data (char) to floating point for power computations */
      /* Skip the pad on each scan line                                 */
      k = 0;
      for (i = 0; i < nrowwt; i++)
         {
         for (j = 0 ; j < nsamwt; j++, k += ipad) 
            {
            bufin[i*nsamwt+j] = (float) imagep->data[k++];
            /* if (i == (nrowwt -1)) {printf(" 0buf: %d:%f \n",k, bufin[i*nsamwt+j]);} */
            }
         }

      /* find maximum and minimum of power spectrum input */
      fmint = fmaxt = bufin[0];
      for (i=1; i < nsamwt * nrowwt; i++)
          {
          fmaxt = MYMAX(fmaxt,bufin[i]);
          fmint = MYMIN(fmint,bufin[i]);
          }
       /* printf("  8input: %f ... %f \n",fmint, fmaxt); */
       }
    else 
      {
      jptr = (int *) imagep->data;
      k    = 0;
      for (i = 0; i < nrowwt; i++)   /* For each row */
         {
         for (j = 0 ; j < nsamwt; j++) /* For each column */
            {
            bufin[k++] = *jptr++; 
            /* if (i == (nrowwt -1)) {printf(" 2buf: %d:%f \n",k, bufin[k]);} */
            }
         }

      /* find maximum and minimum of power spectrum input */
      fmint = fmaxt = bufin[0];
      for (i=1; i < nsamwt * nrowwt; i++)
          {
          fmaxt = MYMAX(fmaxt,bufin[i]);
          fmint = MYMIN(fmint,bufin[i]);
          }
       /* printf("  32input: %f ... %f \n",fmint, fmaxt); */
       }

/********************** START DEBUG *** 
   printf("  iplanes: %d \n",iplanes); 
   printf("  format: %d \n",imagep->format); 
   printf("  byte_order: %d \n",imagep->byte_order); 
   printf("  depth: %d \n",imagep->depth); 
   printf("  bitmap_unit: %d \n",imagep->bitmap_unit); 
   printf("  bitmap_bit_order: %d \n",imagep->bitmap_bit_order); 
   printf("  bits_per_pixel: %d \n",imagep->bits_per_pixel); 

 i = ((nrowwt * nsamwt)/2);  
 for (j = i ; j < (i+nsamwt); j++) 
  printf("  fbuf: %d:%f\n",j,bufin[j]); 
 ****** END DEBUG **** */

   /* Free space */
   XDestroyImage(imagep);
                                                 
   /* create power spectrum */
   irtflg = power(bufin,nsamwt,nrowwt);
   if (irtflg) {waitcursor(iwin, FALSE, 79);  return 3;}

   /* find maximum and minimum of power spectrum */
   fmint = fmaxt = bufin[0];
   for (i=1; i < nsamwt * nrowwt; i++)
       {
       fmaxt = MYMAX(fmaxt,bufin[i]);
       fmint = MYMIN(fmint,bufin[i]);
       }

   /* printf(" Spectrum: %f ... %f \n",fmint, fmaxt); */

   if(fabs((double)(fmaxt - fmint)) < (double) FLTZER)
       { spout(" Blank power spectrum skipped "); 
         free(bufin); 
         waitcursor(iwin, FALSE, 79); return 4; }

   /* Display power spectrum */

   /* conversion factor from floating point to logical *1 range */
   scal = ((float)(nmax - mapgo)) / (fmaxt - fmint);
   fmsc = - fmint * scal + mapgo + 0.5;
                                                                  
   /* run time allocation of memory space for 8 bit image */
   if ( (lbuf = (char *) malloc(nsamwt * nrowwt * sizeof(char)) ) == (char *) NULL) 
      {
      spout(" *** wid_power.c CAN'T ALLOCATE  MEMORY for lbuf");
      waitcursor(iwin, FALSE, 79); free(bufin);   return 5;
      }

   if (nmax < 128)
       {
       for (i = 0; i < nsamwt * nrowwt; i++)
           { lbuf[i] =  (char) ((bufin[i] * scal) + fmsc); }
       }
   else
       {
       for (i = 0; i < nrowwt * nsamwt; i++)
           {
           b = bufin[i];
           /* convert 0..255 integers to -128..127 logical *1 */
           lbuf[i] = (((b * scal) + fmsc) < 128   ? 
                       (b * scal) + fmsc          :
                       (b * scal) + fmsc - 256.0 );
           }
       } 

#if defined(WEB_24) || defined(WEB_TRUE)
 if (DefaultDepthOfScreen(iscreen) != 8)
    {
    int i, j, k, Bpp;

    Bpp = (int) pow(2, (DefaultDepthOfScreen(iscreen) / 8) - 1);

    excbuf = calloc(nsamwt * nrowwt * Bpp, sizeof(char));
    for (i=0; i<nrowwt; i++)
       for (j=0; j<nsamwt; j++)
         for (k=0; k<Bpp; k++)
            if (k == 3)
               excbuf[((i*nsamwt) + j) * Bpp + k] = 0;
            else
               excbuf[((i*nsamwt) + j)*Bpp + k] = lbuf[(i*nsamwt) + j];

    if ( (imagep = XCreateImage(idispl,visual,
            (unsigned int) DefaultDepthOfScreen(iscreen),
            (int) ZPixmap, 0, (char *) excbuf,
            (unsigned int) nsamwt, 
            (unsigned int) nrowwt, Bpp*4, nsamwt*Bpp) ) == (XImage *) NULL)
           {
           spout("***   XCreateImage failed ");
           waitcursor(iwin, FALSE, 79);
           free(bufin); free(excbuf); free(lbuf); return 2;
           }    }  
 else
#endif
    {
    /* Create image for  the power spectrum */
    imagep = (XImage *) NULL;
    if ( (imagep = XCreateImage(idispl, visual,
                   (unsigned int) DefaultDepthOfScreen(iscreen),
                   (int) ZPixmap, 0, (char *) lbuf,
                   (unsigned int) nsamwt,
                   (unsigned int) nrowwt, 8, nsamwt) ) == (XImage *) NULL)
           {
           spout("*** XCreateImage failed ");
           waitcursor(iwin, FALSE, 79);
           free(bufin); free(lbuf); return 2;
           }
    }


   /* Put image to the screen */
   XPutImage(idispl, iwin, icontx, imagep, 0, 0, ixulw, iyulw,
             (unsigned int) nsamwt, (unsigned int) nrowwt); 

   /* Make average image permanent */
   if (perm)
       XPutImage(idispl, imagsav, icontx, imagep, 0, 0, ixulw, iyulw,
                 (unsigned int) nsamwt, (unsigned int) nrowwt); 
   neednewmap = TRUE;

   /* Make changes visible */
   XFlush(idispl);

   /* Free up memory space (Note that lbuf/or excbuf is destroyed */
   XDestroyImage(imagep);
   free(bufin); 
   if (DefaultDepthOfScreen(iscreen) != 8) free(lbuf);

   waitcursor(iwin, FALSE, 79);
 
   return 0;  
   }             
