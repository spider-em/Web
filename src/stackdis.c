
/*
C++*********************************************************************
C
C stackdis.c -- CREATED Jan 1998 YL
C               Problem with categorize from doc file    Jul 2004 al
C               Problem with categorize from seq. stack  Apr 2012 al
C
C *********************************************************************
C *				                                       *
 C=* FROM: WEB - VISUALIZER FOR SPIDER MODULAR IMAGE PROCESSING SYSTEM *
 C=* Copyright (C) 1992-2012  Health Research Inc.                     *
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
C *********************************************************************
C
C    STACKDIS
C
C    PURPOSE:         DISPLAY STACK FILES.
C
C    PARAMETERS:     
C
C    CALLED BY:       STACKMEN
C
C***********************************************************************
*/

#include "common.h"
#include "routines.h"

 /* External routines */
 extern int   sumum(float * first, float * second, float * output, 
                    int npix);
 extern int   average(float * input,  float * output,
                      int npix, float divisor);
 extern int   avgsave(char * avgnam, int nsam, int nrow, float * avgdata);
 extern void  showmoviemen  (int);

 /* External variables */
 extern int   stkgo, stkend;       /* From stackmen for montage only*/
 extern int   *docimgnums;         /* Doc file image numbers */
 extern int   docimgtotal;         /* Number of images from doc file */
 extern int   montaverage;         /* Want montage average */
 extern char  avgnam[128];         /* Name for montage average file */
 extern int   ndone;               /* Used by cat.c & movie, from mont2 */
 extern char  *cmont3buf;          /* From mont3 */
 extern int   nsami,nrowi,nslicei; /* From mont3 */
 extern XImage * ximagelist[1000]; /* From showmovie */

 /* Externally used variables */
 int imagelist[1000];              /* Used by: cat.c */
 int nimage;                       /* Used by: cat.c */

/****************************  stackdis  ********************************/

 void stackdis(int nmint, int nmaxt)

 {   
 XImage *     imaget    = NULL;
 float  *     imagef    = NULL;
 float  *     imagefavg = NULL;
 char         *avgname;
 int          nsam, nrow, imgnum, imgot, nsams, nrows;
 int          keepgo, numdone, iloc, numvox;
 char	      savename[81];
 char *       strtmp;
 size_t       slen;
 int          keepimage, keepimagef, keepimagec;
 XImage *     imagex    = NULL;
 char   *     imagec    = NULL;
 char   *     cptr;
 
 /* DO NOT FORCE FIRST IMAGE POSITION */
 force = FALSE;
 
 nsam    = filedata->nsam;
 nrow    = filedata->nrow;
  
 /* Set the full image size for image display */
 nsam2 = nsam;  nrow2 = nrow;  nslicei = 1;      
 dispsize(nsam1,nsam2,nrow1, nrow2, 1,1,iskip, &nsami, &nrowi, &nslicei);

 keepimagef = FALSE;
 keepimagec = (ioper == 24);
 keepimage  = (ioper == 24);
 ndone      = 0;

 if (keepimage) 
    {  /* Montage operation is active */
    numvox = (stkend - stkgo + 1) * nsami * nrowi;
    if ((cmont3buf = (char *) malloc(numvox * sizeof(char))) == (char *) 0)
       {
       spout(" *** Can not allocate cmont3buf in stackdis."); 
       waitcursor(iwin, FALSE, 79); return;
       }
    }

 /* Find nprow now if not set earlier (e.g. not set in docmontage) */
 if (nprow <= 0  || (ioper == 11 && nstack == 2))
    { nprow = setnprow(nsam);  }

 /* Save the filname */
 strcpy(savename, filnow);

 if (ioper == 20 || ioper == 23)
    {  /* Use doc file list for images or stacked images*/
    numdone = 0;
    }
 else
    {  /* Use stack selection list from menu */

    /* Check the starting # and ending # */
    if (stkgo < 1 || stkgo > stkend)
       { spout(" Illegal image numbers!\n"); return;  }

    /* Starting image for sequential list */
    imgnum = stkgo-1;
    }

 //printf("In: stackdis; nstack: %d, imginstack: %d \n", nstack,imginstack); 

 /* Display the images */
 keepgo = 1;
 nimage = 0;
 do
    {
    if (ioper == 20  || ioper == 23)
       {  // -------------- Display stack montage from doc file list 
          // -------------- Categorize stack montage from doc file list 

       /* Get stacked image number from docimgnums array */
       if ( numdone + 1 > docimgtotal) 
          { break;}
       imgnum = docimgnums[numdone];
       }
    else
       { // -------------- Display stack montage using stack list menu 
       imgnum++;
       if (imgnum > stkend) 
          {break;}
       }
    numdone++;

    if ((imgot = getoldstack(filedata, imgnum, TRUE, TRUE)) <= 0) 
        break;

    if (ioper != 24 || (imgot >= stkgo && imgot <= stkend))
       {   /* Got a used (active) image */

        if (ioper == 11 )
          {
          /* Make a list of displayed images for cat.c use */
          if (nimage >= 1000)
             { spout(" Too many images in list, Limit: 1000!\n"); return;  }

          imagelist[nimage++] = imgnum;
          //printf("In: stackdis; nimage: %d,  list[0]: %d \n", nimage,imagelist[0]); 
          }

       if (names)
          { 
          /* Write the image # in stack */
          /* showimage strips off directory listing */
          sprintf(filnow,   "/@%d", imgot);

          /* Blank out the char after number */       
          /* *(filnow + numdig(imgot,1) + 1) = '0'; */
          } 
 
       if (montaverage)
          { /* -------------- Montage from doc file & want avg. image */
          if (numdone <= 1)
             {  /* Display the image, save fimage in imagefavg */	    
             imagedis(filedata, FALSE, &imaget,FALSE, 
                      TRUE, &imagefavg, nmint,nmaxt);
             }
          else
             {
             /* ------------ Display the image, save fimage in imagef */	    
             imagedis(filedata, FALSE, &imaget,FALSE, 
                      TRUE, &imagef, nmint,nmaxt);

             sumum(imagefavg, imagef, imagefavg , nsam * nrow);
             free(imagef);
             imagef = NULL;
             }
          }
       else
          { /* -----------------------------------  No average wanted */
          if (keepimage) 
              {  /* ------------------------------  Big movie wanted */
              iloc = ndone * nsami * nrowi;
              cptr = &cmont3buf[iloc];
              showimage(filedata, 
                  keepimagec, &cptr, 
                  keepimage,  &imagex, 
                  keepimagef, &imagef, FALSE,
                  (float) filedata->fmin_spi, (float) filedata->fmax_spi,
                  nmint, nmaxt);

                  /* record image list for moveie */
                  ximagelist[++ndone] = imagex;
              }
          else
              {
              /* ------------- Display the image, do not save fimage */	    
              imagedis(filedata, FALSE, &imaget,FALSE, 
                       FALSE, &imagef, nmint,nmaxt);
              }
          }

      } /* if (imgot > stkgo && imgot < stkend) */
    }
 while (keepgo);  /* do */

 /* Recover filnow */
 strcpy(filnow, savename);

 /* Free up the file descriptor for this image */
 closefile(filedata); filedata = NULL;

 if (ioper != 11 && ioper != 23 && ioper != 26 && ioper != 25)
    {   /* Free storage for imgnums from doc file */
        /* Do not free storage when doing categorize afterwards */
    if (docimgnums) free(docimgnums);
    docimgnums = NULL;
    }

 if (montaverage)
    {  /* Montage from doc. file with average image calculation */

    /* Calculate floating point average image */
    average(imagefavg, imagefavg ,nsam * nrow, (float) numdone);

    /* Store the floating point average image */
    if (strlen(avgnam) <= 0)
       spout("No file name specified for average file!");
    else
       {
       /* Add datexc to avgnam if not already there */
       slen = strlen(avgnam) + strlen(datexc) + (size_t) 2;

       if (!(avgname = (char *) malloc(slen * sizeof(char))))
          {
          spout("*** Unable to allocate avgname memory in stackdis!");
          return;
          }  
       strcpy(avgname,avgnam);
       if (strstr(avgname,datexc) != 
            (avgname + strlen(avgname) - strlen(datexc)))
          {   /* Need to add datexc extension at end of avgname */
          if (!addext(avgname,datexc,slen)) return;
          }

       newline = TRUE;
       avgsave(avgname, nsam, nrow, imagefavg);

       /* Display the average image */
       ioper = 1;    /* Not a montage anymore */
       imagemen1(avgname, TRUE);
       if (avgname) free(avgname);

       /* Cancel waitcursor */
       waitcursor(iwin, FALSE, 79);
         }
    if (imagefavg) free(imagefavg);
    imagefavg = NULL;
    }

 if (ioper == 11  || ioper == 23)
    {                    /* Categorize images from montage now */
    catmen();
    }

 else if (ioper == 24)
    { /* Show montage */
    showmoviemen(TRUE);
    } 
 return;
 }


 
