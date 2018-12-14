
/*$Header: /usr16/software/web/src/RCS/mont2.c,v 1.39 2018/12/07 17:03:32 leith Exp $*/

/*
C++********************************************************************
C
C MONT2 -- CREATED FROM WKMONT                    JUL 91 ArDean Leith
C          CONVERTED TO C                         JUL 92 ArDean Leith
C          ADDED "BIG MOVIE"                      SEP 01 ArDean Leith
C          REMOVED UNABLE TO OPEN MSG             DEC 12 ArDean Leith
C *********************************************************************
C=* AUTHOR: A. LEITH                                                  *
C=* FROM: WEB - VISUALIZER FOR SPIDER MODULAR IMAGE PROCESSING SYSTEM *
C=* Copyright 1985-2012  Health Research Inc.,                        *
C=* Riverview Center, 150 Broadway, Suite 560, Menands, NY 12204.     *
C=* Email:  spider@wadsworth.org                                      *
C=*                                                                   *
C=* This program is free software; you can redistribute it and/or     *
C=* modify it under the terms of the GNU General Public License as    *
C=* published by the Free Software Foundation; either version 2 of    *
C=* the License, or (at your option) any later version.               *
C=*                                                                   *
C=* This program is distributed in the hope that it will be useful,   *
C=* but without any warranty; without even the implied warranty of    *
c=* merchantability or fitness for a particular purpose.  See the GNU *
C=* General Public License for more details.                          *
C=* You should have received a copy of the GNU General Public License *
C=* along with this program. If not, see <http://www.gnu.org/licenses>*
C=*                                                                   *
C *********************************************************************
C
C    MONT2
C
C    PURPOSE:         DISPLAY MONTAGE FROM 2D FILE SERIES.
C
C    PARAMETERS:     
C
C    CALLED BY:       IMAGEMEN
C
C**********************************************************************
*/

#include "common.h"
#include "routines.h"
#include "mont.h"

 /* External routines */

 /* External variables used here */

 /* Internal variables used elsewhere */
 int          ndone;             /* used by cat.c & movie            */
 char         filego[81] = "\0"; /* name of first image in montage   */
 int          minfile, maxfile;
 int          ixulmon,iyulmon;   /* upper left corner of montage     */

/****************************  mont2  ********************************/

 void mont2(int nmint, int nmaxt)

 {   
 int          resetmar, iret, ifile;
 int          numfil, nlet,  toomany;
 FILEDATA   * filedatan;
 int          nsamn, nrown, nslicen, iformn;
 int          marlefo;
 XImage *     imagex    = NULL;
 char   *     imagec    = NULL;
 float  *     imagef    = NULL;
 float  *     imagefavg = NULL;
 size_t       slen;
 char         *avgname;
 int          keepimage, keepimagef, keepimagec;

 /* DO NOT FORCE FIRST IMAGE POSITION */
 force = FALSE;

 resetmar = FALSE;
 if (placed) 
   {
   marlefo  = marlef;
   marlef   = ixuln - marx;
   resetmar = TRUE;
   }

 islice  = 1;
 ndone   = 0;
 toomany = FALSE;

 /* Update the screen */
 expos_cb(NULL,NULL,NULL);

 keepimagef = (ioper == 20 && montaverage);
 keepimagec = (ioper == 24);
 keepimage  = (ioper == 24);

 /* Display initial image on a new line*/
 newline = TRUE;

 ndone++;
 if (keepimage) 
    {  /* Big movie wanted */
    showimage(filedata, 
        keepimagec, &imagec, 
        keepimage,  &imagex, 
        keepimagef, &imagef, FALSE,
        (float) filedata->fmin_spi, (float) filedata->fmax_spi,
        nmint, nmaxt);

        /* Record image list for moveie */
        ximagelist[ndone] = imagex;
    }
 else
    {
    /* Display initial image, don't keep image or buffer */
    imagedis(filedata, keepimage, &imagex, keepimagec, 
             keepimagef, &imagefavg, nmint, nmaxt);
    }
   
 /* Free up the file descriptor for this image */
 closefile(filedata); filedata = NULL;

 /* remember montage location */
 ixulmon = ixul;
 iyulmon = iyul;

 /* Force desired number of images/line now even if truncates */
 force = TRUE;

 /* Find following image names and display their files */
 if (ioper != 20 && ioper != 23 )
    {strcpy(filego,filnow);}

 /* Find nprow now if not set earlier (e.g. not set in docmontage) */
 if (nprow <= 0)
   {    /* Calculate nprow after opening first image */
   nprow = setnprow(nsam);
   }

 do
   {
   /* Get next file name */
   if (ioper == 20 || ioper == 23)
      {   /* docmontage or categorize from doc file --   */
          /* Get file names using docimgnums array */
      ifile = docimgnums[numnext++];
      if (ifile < minfile || ifile > maxfile) 
         { iret = 0;}
      else
         { iret   = filget(filego, filnext, 80, ifile);  }

      /* Set numfil so test of: numfil <= lastfile  always succeeds */
      numfil = lastfile;
      }
   else
      {   /* Sequential montage -- increment file name */
      iret = filinc(filnow, filnext, &nlet, &numfil); 
      }


   if (iret) 
      {   /* File number incrementing succeeded */
      if (numfil <= lastfile || lastfile < 0) 
         {
         /* Still want to display this next file so open it */
         /* disp: "q" signifies no Unable to open error msg wanted */
         
         if ((filedatan = openold(filnext, &nsamn, &nrown, &nslicen, 
              &iformn, "q")) == NULL)
            {   
	    /* Next file not found (may not exist if * was used) */
            /* Stop if lastfile indicates * was used for files */

            if (lastfile < 0) 
               {
               toomany = TRUE;
               /* Stop if it reaches the last file */ 
               }

            else 
               {
               strcpy(filnow,filnext);  /* Try opening next file */
               }
            }
         else
            {   /* Next image file found OK, set filnow to filnext */
            strcpy(filnow,filnext);
            filedata = filedatan;
            nsam     = nsamn;
            nrow     = nrown;
            nslice   = nslicen;
            islice   = 1;

            /* Normalize file if necessary */
            if ((filedata -> imami == 1) || norm(filedata) == TRUE)

               {   /* file normalized ok */
               if (fabs(filedata -> fmax_spi - filedata ->fmin_spi) < FLTZER)
                  { spout("*** BLANK IMAGE SKIPPED"); }

               else
                  {
                  newline = FALSE;
                  if (ndone % nprow == 0) newline = TRUE;

                  if (keepimage) 
                     {  /* Big movie wanted, keep the images */
                     imagec = NULL;
                     showimage(filedata, 
                        keepimagec, &imagec, 
                        keepimage,  &imagex, 
                        keepimagef, &imagef, FALSE,
                        (float) filedata->fmin_spi, (float) filedata->fmax_spi,
                        nmint, nmaxt);
                     /* record image list for movie */
                     ximagelist[ndone+1] = imagex;
                     }
                  else
                     {
                     /* Display this image, dont keep image or buffer */
                     imagedis(filedata, keepimage, &imagex,keepimagec,
                              keepimagef, &imagef,nmint,nmaxt);
                     }
                  }

               if (ioper == 20 && montaverage)
                  { /* Montage from doc file & want avg. image */
                  sumum(imagefavg, imagef, imagefavg , nsam * nrow);
                  free(imagef);
                  imagef = NULL;
                  }

               /* free up the file descriptor for this image */
               closefile(filedata); filedata = NULL;

               /* increment image counter */
               ndone++;

               }    /* ends: if ((filedata  */
            }       /* ends: if ((filedatan */
         }          /* ends: if (numfil     */
      }             /* ends: if (filinc     */
   }                /* ends: if (iret)      */

 while ((
        ((ioper == 20 || ioper == 23) && numnext < docimgtotal)) || 
        ((ioper != 20 && ioper != 23) && (numfil <= lastfile && !toomany)) || 
         (lastfile < 0 && !toomany));

 if (ioper == 20)
    {   /* Free storage for imgnums from doc file */
        /* do not free storage when doing categorize afterwards */
    if (docimgnums) free(docimgnums);
    docimgnums = NULL;
    }

 if (ioper == 20 && montaverage)
    {  /* Montage from doc. file with average image calculation */
       /* cancel waitcursor */
       waitcursor(iwin, FALSE, 79);

    /* Calulate floating point average image */
    average(imagefavg, imagefavg ,nsam * nrow, (float) ndone);

    /* store the floating point average image */
    if (strlen(avgnam) <= 0)
       spout("No file name specified for average file!");
    else
       {
       /* add datexc to avgnam if not already there */
       slen = strlen(avgnam) + strlen(datexc) + (size_t) 2;

       if (!(avgname = (char *) malloc(slen * sizeof(char))))
          {
          spout("*** Unable to allocate avgname memory in mont2!");
          return;
          }  
       strcpy(avgname,avgnam);
       if (strstr(avgname,datexc) != 
            (avgname + strlen(avgname) - strlen(datexc)))
          {   /* need to add datexc extension at end of avgname */
          if (!addext(avgname,datexc,slen)) return;
          }

       newline = TRUE;
       avgsave(avgname, nsam, nrow, imagefavg);

       /* display the average image */
       ioper = 1;    /* not a montage anymore */
       imagemen1(avgname, TRUE);
       if (avgname) free(avgname);

       /* cancel waitcursor */
       waitcursor(iwin, FALSE, 79);
         }
    if (imagefavg) free(imagefavg);
    imagefavg = NULL;

    }

 force = FALSE;

 /* RESET MARLEF TO DEFAULT, IF NECESSARY */
 if (resetmar)
      marlef =  marlefo;

 newline = FALSE;

 if (ioper == 11  || ioper == 23)
    {                    /* categorize images from montage now */
    catmen();
    }

 else if (ioper == 16)
    {                    /* show movie now */
    moviemen(TRUE); 
    }

 return;
 }


 
