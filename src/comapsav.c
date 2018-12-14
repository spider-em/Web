/*
************************************************************************
** comapsav.c		
**              WID_COMAPSAV.FOR ported to UNIX   8/93
**              24 bit visual support             Nov 2007 ArDean Leith
**
** PURPOSE: Analyses outlined comap, saves averages, etc...
**
** CALLED BY:  comapmen5.c comapmen3.c comapmen2.c
**
** WID_COMAPSAV.FOR ported to UNIX
**
C **********************************************************************
** *  AUTHOR:  Mahieddine Ladjadj                                      *
 C=* FROM: WEB - VISUALIZER FOR SPIDER MODULAR IMAGE PROCESSING SYSTEM *
 C=* Copyright (C) 1992-2007  Health Research Inc.                     *
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
*/

#include <Xm/Text.h>
#include <stdlib.h>
#include "common.h"
#include "routines.h"
#include "comap.h"


/********************** comapsav ***********************************/

void comapsav(int itag)
{
  register int  i, j1;
  XImage        *imagemask;
  FILEDATA      *ptrdoc;
  FILE          *fp;
  char          outstr[81], filnam[81];
  int           nlist, nrun, ixs, iys, iform, ixulw, iyulw;
  float         *plist, b, *ipoc;
  float         pmin, pmax, pmean, thup, thdown, pnormal, scal, fmsc;
  double        psig, davd, davd2;
  int           nlet, numfil, iloc, irec, nnmax = 127;   /* limited display */
  int           xxsiz, yysiz;
  static int    icountav, icountin, *ibin;
  static char   *lbuf;
  static char   *excbuf;
  static float  *avims, *avim;
  static int    ibmaxx, ibmaxy, ibminx, ibminy;

  if (itag == 4 || itag == 5)
      {  /* --------------------------------------------   clear and restart masking */
      gotav = gotbin  = gotavim = FALSE;

      /* free up the memory space */
      if (ibin) 
        { free(ibin); ibin = (int *) NULL; }
      if (avims)
        { free(avims); avims = (float *) NULL; }
      if (avim)
        { free(avim); avim = (float *) NULL; }
      if (lbuf)
        { free(lbuf); lbuf = (char *) NULL;  }
      if (msksav) 
        { XFreePixmap( idispl, msksav ); msksav = (Pixmap) NULL; }
 
     if (itag == 4)
         {/* Blank markers but not image, by clearing the overlays */
         //clearover(NULL,NULL,NULL); al may 2011
         corr_mask(1);
         }
      else 
         corr_mask(2);

      return;
      }   /* End of: if (itag == 4 || itag == 5)  */


  /* Set wait cursor */
  waitcursor(iwin, TRUE, 79);
 
  /* Find which images are within the polygon */
  if (!gotbin)
      {  /* Recover the overlay bitmap into an image structure */
      imagemask = (XImage *) 0;
      if ( (imagemask = XGetImage(idispl,msksav,ixmar,iymar,
                         (unsigned int) ixsize, (unsigned int)iysize,
                         (unsigned long)128, XYPixmap) ) == (XImage *) 0)
           {
           spout(" *** Error - Image formation failed"); 
           waitcursor(iwin, FALSE, 0);  return;
           }

      nowbin++;
      if (!gotbin)
          { /* Allocate memory space for ibin[] */
          if ( (ibin = (int *) calloc(nline, sizeof(int))) == (int *) NULL)
             {
             spout(" *** Error - comapsav.c can't allocate memory for ibin");
             waitcursor(iwin, FALSE, 0);   return; 
             }
          }
      icountin = 0;
      ibmaxx   = ibmaxy = -10000;
      ibminx   = ibminy =  10000;

      for (i = 0; i < nline; i++)
          {   /* Make list of included images */
          if (nkey[i] > 0)
             {    /* image inside map, find scaled location */
             ixs = (int) (bufx[i] * factxa + factxb);
             iys = (int) (bufy[i] * factya + factyb);

             if ( XGetPixel(imagemask,ixs,iys) > 0)    /* image is inside mask */
                {
                ibin[i] = nowbin;
                icountin++;
                ibmaxx = MYMAX(ibmaxx,ixs);
                ibminx = MYMIN(ibminx,ixs);
                ibmaxy = MYMAX(ibmaxy,iys);
                ibminy = MYMIN(ibminy,iys);
                /* printf(" Image: %d. #: %d at: (%d,%d) is: %d \n", 
                            i,nowbin,ixs,iys,XGetPixel(imagemask,ixs,iys)); */
                }
             }
          else
              ibin[i] = -1;
          }
      
      /* free-up image storage in Xserver */  
      XDestroyImage(imagemask);
      gotbin = TRUE;

      /* sprintf(outstr,"Images within mask: %d. Total images: %d", 
                          icountin,nline); spout(outstr); */
      }                   

  if (itag == 1) 
      {    /* ------------------------------------  save image listing in doc file. */
      nrun = 1;
      fp = (FILE *) NULL;
      nlist = MYMAX(MYMAX(itxrego,ityrego),labrego) + 2;

      /* find the name of the document file */
      nlet = strlen(docnammo);
      if (nlet==0)
          { sprintf(outstr,"*** Error - filename is empty! ");
            spout(outstr); waitcursor(iwin, FALSE, 0); return; }
      strcpy(filnam,docnammo);
      if (filnam[nlet] != '.')
                strcat(strcat(filnam,"."),datexc);

      j1 = 0;
      for (i = 0; i < nline; i++)
          {  /* only binned images inside map will be put in doc file */
          if (ibin[i] > 0 && nkey[i] >= 0)
              {  /* run time allocate of memory space for plist[]*/
              if ( (plist = (float *) calloc(nlist, sizeof(float)) )
                          == (float *) NULL )
                    {
                    spout(" *** Error - comapsav.c can't allocate memory for plist");
                    waitcursor(iwin, FALSE, 0);   return;
                    }
 
              plist[0]         = (float) ++j1;
              plist[1]         = (float) nkey[i];
              plist[itxrego+1] = bufx[i];
              plist[ityrego+1] = bufy[i];
              plist[labrego+1] = ibin[i];
              fp               = savdn1(filnam,datexc,&fp,plist,nlist,&nrun,0,TRUE);
              nrun             = 0;
              free(plist);    /* free memory space */
              }
          }

      /* set next filename for doc file */
      filnam[nlet] = '\0';
      strcpy(docnammo,filnam);

      /* increment the NEXT filename */
      if(filinc(docnammo,filnam,&nlet,&numfil) != 2)
           {
           sprintf(outstr,"*** Error - could not increment filename: %s",docnammo);
           spout(outstr); waitcursor(iwin, FALSE, 0); return;
           }
      filnam[nlet] = '\0';
      strcpy(docnammo,filnam);     
  
      if(fp) 
          {
          fclose(fp);
          sprintf(outstr,"Images within mask recorded in doc file: %d. Total images: %d",
                  icountin,nline); 
          }
      else
          {
          sprintf(outstr,"No images within mask.  No Doc. file saved. ");
          }
      spout(outstr);

      /* reset cursor and put up menu again */
      waitcursor(iwin, FALSE, 0);

      comapmen2();
      return;
      }       /*********************************************  itag = 1  */


  if ( (!gotav) && ((itag == 2) || (itag == 3)))
      {  /* -------------------------------------------------------  calculate average image  */
      icountav = 0;

      for (i = 0; i < nline; i++)
          {
          if (ibin[i] == nowbin)
              { /* skip images outside map boundary */
              if ( nkey[i] >= 0) 
                  {   /* construct file name */
                  filget(filnamm,filnam,81,nkey[i]);
                  strcat(strcat(filnam,"."),datexc);

                  /* open 'old' image file */
                  if ((ptrdoc = openold(filnam,&nsam,&nrow,&nslice,&iform,"o")) 
                             != (FILEDATA *) NULL )
                       {
                       if(nslice > 1) 
                           {
                           sprintf(outstr,"Error - Not 2D image. Nslice= %d\n",nslice);
                           spout(outstr);
                           }
 
                       /* add image from new file to new running total */
                       icountav++;
                       if (icountav == 1) 
                           {
                           /* allocate memory space for average image */
                           if ( (avim = (float *) calloc((nsam * nrow),
                                 sizeof(float)) ) == (float *) NULL)           {
                           spout(" *** Error - comapsav.c can't allocate memory for avim");
                           waitcursor(iwin, FALSE, 0); return;                 }
                           }

                       ipoc = avim ;
                       for (irec = 1; irec <= nrow; irec++)
                          {
                          if (redlin(ptrdoc,ptrdoc->buf,nsam,irec) == TRUE)
                             {
                             for (j1 = 0; j1 < nsam; j1++)
                                 { (*avim++) += ptrdoc->buf[j1];  }
                             }
                          else
                             {spout(" *** Error - Could not read buf for average, one value of AVIM lost"); }
                          }
                       avim = ipoc;
                       closefile(ptrdoc);
                       }
                  else
                       {
                       sprintf(outstr,"*** Error - Bad filename: %s",filnam);
                       spout(outstr);
                       }
                  }                    /*  if (nkey[i] > = 0)  */
              }                        /* if ibin[i]==nowbin  */
          }                            /* for (i = 0; i < nline; i++) */

      }     /******************    (!gotav && (itag == 2 || itag == 3)) */


  if ( (itag == 2) || ((itag == 3) && (!gotav)) )
      {
      if (icountav > 0) 
          {  /* at least one file in sum, scale the summed page */
          davd = davd2 = 0.0;
          pmin = 10e30;    pmax = -10e30;

          /* find min, max, avg, & standard dev. for average image */
          for (i = 0; i < nrow * nsam; i++)
              {
              pmax = MYMAX(avim[i],pmax);
              pmin = MYMIN(avim[i],pmin);
              davd += avim[i];
              davd2 += (double)(avim[i]) * (double)(avim[i]);
              }
          if ((pmax-pmin) < FLTZER)                      {
              spout("*** Blank average image skipped ");
              waitcursor(iwin, FALSE, 0); return;        }

          /* standard deviation */
          psig   = sqrt( (davd2 - (davd * davd) / (nsam * nrow) ) 
                      / (double)(nsam * nsam - 1.0) );
          pmean  = davd / (nsam * nrow);
          thup   = pmean + rangeu * psig;
          thdown = pmean - rangel * psig;
          if (pmax > thup)   pmax = thup;
          if (pmin < thdown) pmin = thdown;

          /* normalization for averaging */
          pnormal = 1.0 / ((float) icountav);

          /* conversion factor from floating point to logical *1 range */
          scal = ((float)(nnmax - imagego)) / (pmax - pmin);
          fmsc = - pmin * scal + imagego + 0.5;
                                                                  
          /* run time allocation of memory space for 8 bit image */
          if ( (lbuf = (char *) calloc((nsam * nrow),sizeof(char)) ) 
                     == (char *) NULL)                               {
              spout(" *** Error - comapsav.c can't allocate memory for lbuf");
              waitcursor(iwin, FALSE, 0);    return;                 }                         
                            
          /* run time allocation of memory space for saved image */
          if ( (avims = (float *) calloc((nsam * nrow),sizeof(float)) ) 
                      == (float *) NULL)                              {
              spout(" *** Error - comapsav.c can't allocate memory for avims");
              waitcursor(iwin, FALSE, 0);    return;                  } 
          if (nnmax < 128)
              {
              for (i = 0; i < nrow * nsam; i++)
                  {
                  b = avim[i];
                  if (b > thup)   b = thup;
                  if (b < thdown) b = thdown;
                  avims[i] = b * pnormal;
                  lbuf[i]  =  (b * scal) + fmsc;
                  }
              }
          else
              {
              for (i = 0; i < nrow * nsam; i++)
                  {
                  b = avim[i];
                  if (b > thup)   b = thup;
                  if (b < thdown) b = thdown;
                  avims[i] = b * pnormal;

                  /* convert 0..255 integers to -128..127 logical *1 */
                  lbuf[i] = (((b * scal) + fmsc) < 128   ? 
                              (b * scal) + fmsc            :
                              (b * scal) + fmsc - 256.0 );
                  }
              }
         
          gotav = TRUE;
          }                                /* (icountav > 0)  */

      if (itag == 2 && icountav > 0)      /* --------------------------- display the average image  */
          {  /* find the location for image on screen */
          ixulw = MYMAX( 0,
                (((ibmaxx + ibminx) /2.0) - (float) nsam /2.0)) + ixmar;
          if ((ixulw + nsam) > iwidex) ixulw = iwidex - nsam;
          iyulw = MYMAX( 0, 
                (((ibmaxy + ibminy) /2.0) - (float) nrow /2.0)) + iymar;
          if ((iyulw + nrow) > ihighx) iyulw = ihighx - nrow;

          /* no overlaping of average image and mask ? */
          if (!ovrlap)
              {
              /* shift by xxsiz and yysiz */
              xxsiz = (int) ((ibmaxx - ibminx + nsam) / 2.0) + 10;
              yysiz = (int) ((ibmaxy - ibminy + nrow) / 2.0) + 10;

              /* try to find a position left, top, bottom, then right */
              if ((ixulw - xxsiz) > 0)
                  ixulw -= xxsiz;                   /* left */
              else if ((iyulw - yysiz) > 0)
                  iyulw -= yysiz;                   /* top */
              else if ((iyulw + yysiz) < ihighx)
                  iyulw += yysiz;                   /* bottom */
              else if ((ixulw + xxsiz) < iwidex)
                  ixulw += xxsiz;                   /* right */
              else
                  spout("Can not move average image in any  direction !");

              spout(" *** NOTE: shift is proportional to size of MASK ");
              }

          /* create image for  the screen in window */

#if defined(WEB_24) || defined(WEB_TRUE)
          if (DefaultDepthOfScreen(iscreen) != 8)
             {
             int i, j, k, Bpp;    /* Bpp will be 4 */
             Bpp = (int) pow(2, (DefaultDepthOfScreen(iscreen) / 8) - 1);

             excbuf = calloc(nsam * nrow * Bpp, sizeof(char));
             for (i=0; i<nrow; i++)
                for (j=0; j<nsam; j++)
                   for (k=0; k<Bpp; k++)
                      if (k == 3)
                         excbuf[((i*nsam) + j) * Bpp + k] = 0;
                      else
                         excbuf[((i*nsam) + j) * Bpp + k] = lbuf[(i*nsam) + j];

             if ( (imagemask = XCreateImage(idispl,visual,
                           (unsigned int) DefaultDepthOfScreen(iscreen),
                           (int) ZPixmap, 0, (char *) excbuf,
                           (unsigned int) nsam, 
                           (unsigned int) nrow, Bpp*4, nsam*Bpp) )
                           == (XImage *) NULL)
               { spout(" *** Error - XCreateImage failed ");  }
             }  
          else
#endif
          {
          if ( (imagemask = XCreateImage(idispl,visual,
                              DefaultDepthOfScreen(iscreen),
                              (int) ZPixmap, 0, (char *) lbuf,
                              (unsigned int) nsam,
                              (unsigned int) nrow, 8, nsam) ) 
                                   == (XImage *) NULL)
              { spout(" *** Error -  XCreateImage failed ");  }
          }  

          /* Put image on screen */
          XPutImage(idispl,iwin,icontx,imagemask,0,0,ixulw,iyulw,nsam,nrow); 

          /* Make average image permanent */
          XPutImage(idispl,imagsav,icontx,imagemask,0,0,ixulw,iyulw,nsam,nrow); 

          neednewmap = TRUE;

          /* Make changes visible */
          XFlush(idispl);

          /* Free up memory space (excbuf cleared, lbuf space has to be saved */
          imagemask->data = (char *) NULL; 
          XDestroyImage(imagemask);
          }

      sprintf(outstr,"Images averaged: %d. Total images: %d", icountav,nline);  
      spout(outstr);
      }        /*********    (itag == 2 || (itag == 3 && (!gotav)) ) */


  if (itag == 3 && icountav > 0)
      {/*****                              ------------------------ Save the averaged image  itag=3 */
      /* find the filename for averaged image */
      nlet = strlen(imnamm);
      if (nlet==0)
         { sprintf(outstr,"*** Error - No file name input! ");
           spout(outstr); waitcursor(iwin, FALSE, 0); return;        }
   
      strcpy(filnam,imnamm);
      if (filnam[nlet] != '.')
          strcat(strcat(filnam,"."),datexc);

      iform = 1;      /* 2D image */    
      if ((ptrdoc = opennew(filnam,nsam,nrow,nslice,iform,"n"))
                 != (FILEDATA *)NULL)
            {
            iloc = 0;
            for (i = 1; i <= nrow; i++)
                {
                wrtlin(ptrdoc,&avims[iloc],nsam,i);
                iloc += nsam;
                }
            closefile(ptrdoc);

            /* increment the NEXT filename */
            filnam[nlet] = '\0';
            strcpy(imnamm,filnam);
            if(filinc(imnamm,filnam,&nlet,&numfil) != 2)
              { sprintf(outstr,"*** Error - Could not increment filename: %s",imnamm);
                spout(outstr); waitcursor(iwin, FALSE, 0); return;       }

            filnam[nlet] = '\0';
            strcpy(imnamm,filnam); 
            }
      else
            {
            sprintf(outstr,"*** Error - Filename: %s already exists.",imnamm);
            spout(outstr);
            }
      }          /*********   if (itag == 3 && icountav > 0) */
 
  /* unset cursor */
  waitcursor(iwin, FALSE, 0);

  /* put up menu again */
  comapmen2();

  return;
  }               
 /****** end of comapsav function ******/

