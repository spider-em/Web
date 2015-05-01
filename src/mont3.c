
/*$Header: /usr8/web/src/RCS/mont3.c,v 1.25 2012/12/05 17:10:27 leith Exp $*/

/*
C++*********************************************************************
C
C MONT3.C -- CREATED JAN 90 FROM WKMONT
C            WIDGETIZED MAY 91
C            CONVERTED TO C JULY 92 al
C            ADDED INCORE MOVIE SEPT 2001 ArDean Leith
C
C **********************************************************************
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
C **********************************************************************
C
C    MONT3
C
C    PURPOSE:         DISPLAY MONTAGE FROM FILE SERIES.
C                     CAN NORMALIZE SLICE BY SLICE, ALSO.
C
C    VARIABLES:       NAMES      LOGICAL FLAG TO PRINT NAMES
C
C    CALLED BY:       
C
C***********************************************************************
*/

#include "common.h"
#include "routines.h"

#define MAXDIM 32000000

 /* External variables defined here */
 char      *cmont3buf  = NULL;
 int       nsami, nrowi, nslicei;

 /* External variables used here */
 extern int      ixulmon, iyulmon, ndone;
 extern int      endfile;           
 extern XImage * ximagelist[1000]; /* from showmovie */

/******************************  mont3 *******************************/
   
 int  mont3(FILEDATA *fileptr, int nmint, int nmaxt)
 {   
 int       maxvox, maxflt, numvox, resetmar, marlefo;
 int       irow, isam, nrec1, nrec2, miskip;
 int       nsliceu, j , ival,l, iy;
 int       numfil, nlet,  toomany,  file_exist = TRUE;
 int       nsamn, nrown, nslicen, iformn;
 FILEDATA  * filedatan;
 char      label[] = "    ";
 float     fn, fncon;
 float     *buf;
 int       iloc, nrowspn, irec, irec1, iret;

 /* max. no. of voxels in output buffer */
 maxvox = MAXDIM * 4;
 maxflt = MAXDIM;

 /* set image size and window */

 if (axis == 'z')
     {        /* 3-D file with z slice */
     dispsize(nsam1,nsam2,nrow1,nrow2,nslice1,nslice2,iskip, &nsami, &nrowi, &nslicei);
     }
 else if (axis == 'y')
     {        /* Y slice */
     dispsize(nsam1,nsam2,nslice1,nslice2,nrow1,nrow2,iskip, &nsami, &nrowi, &nslicei);
     }
 else if (axis == 'x')
     {        /* X slice */
     dispsize(nrow1,nrow2,nslice1,nslice2,nsam1,nsam2,iskip, &nsami, &nrowi, &nslicei);
     }
 miskip = abs(iskip);

 /* Numvox is number of pixels in whole montage of images */
 numvox  = nsami * nrowi * nslicei;
 
 toomany = FALSE;

 /* Find y-offset needed for name label */
 nrowspn = nrowi;
 if (names) nrowspn += wrtname(-1,-1,(char *) NULL, -1);

 resetmar = FALSE;

 if (placed == TRUE) 
    {   /* Position sent from caller */
    ixul     = ixuln;
    iyul     = iyuln;
    marlefo  = marlef;
    marlef   = ixuln - marx;
    resetmar = TRUE;
    }

 force = FALSE;
 filedata = fileptr;

 /* Set wait cursor */
 waitcursor(iwin, TRUE, 79);

 /* Find following image names and display them */
 do
   {
   if (file_exist)
     {
     /* conversion from floating point to byte range */
     fn      = (float) (nmaxt-nmint) / (fmax_spi - fmin_spi);
     fncon   = -fmin_spi * fn + nmint + 0.5;

     ndone   = 0;
     newline = TRUE;

     if (ioper == 24)
       {   /* Large movie  display of x, y or z slices ****************/

       if ((cmont3buf = (char *) malloc(numvox * sizeof(char))) == (char *) 0)
          {
          spout(" *** Can not allocate cmont3buf in mont3."); 
          waitcursor(iwin, FALSE, 79); return FALSE;
          }

       if (normit == TRUE)
         {   /* Normalize slice by slice (handles iskip OK) */
         iret = ztoxyn(fileptr,cmont3buf, axis,nmaxt,iskip,mapgo); 
         }
       else
         {  /* do not normalize by slice */    
         iret = ztoxy(fileptr, cmont3buf, axis, fn, fncon, iskip);
         }

       if (! iret) 
           { /* Something failed */
           if (cmont3buf) free(cmont3buf); closefile(fileptr); fileptr = NULL; 
           cmont3buf = NULL; waitcursor(iwin, FALSE, 79); return FALSE;
           }  

       for (islice=1; islice <= nslicei; islice++)
         {  /* Make XImages for values returned from 'ztoxy'  routine in cmont3buf */
         iloc = (islice -1 ) * nsami * nrowi;

         /* force wipic to allocate new XImage */
         if (imagel) 
            {imagel =  NULL;}

         wipic(icontx, &cmont3buf[iloc], ixul, iyul, nsami, nrowi, FALSE, 
               TRUE, &imagel); 

         /* Record image list for movie */
         ximagelist[++ndone] = imagel;
         }   /* for (islice=1; islice <= nslicei; islice++) */
       }

     else if (axis == 'z')
       {   /* 3d file with z slice (no need for incore speedup ********/  

       for (islice=nslice1; islice<=nslice2; islice++)
         {
         nrec1  = (islice -1) * nrow + nrow1;
         nrec2  = (islice -1) * nrow + nrow2;

         if (movie == FALSE)
           {
           /* Force a new line in montage if necessary */
           if (ndone%nprow == 0) newline = TRUE;
 
           locc(&ixul,&iyul,nsami,nrowspn,&ntop,&nbot,&nrit,
              marx,mary,marlef,martop,iwidex,ihighx,placed,
              newline,&nuscrn,force);
           newline = FALSE;
           }

         if (normit == TRUE)
           {       /* Normalize each z slice (handles iskip OK) */
           if (monn(icontx,fileptr,islice,nsami,nrowi,nslicei,
               nmint,nmaxt) ==  FALSE) 
              {waitcursor(iwin, FALSE, 79); return FALSE;}
           }
         else
           {       /* Just display (handles iskip OK) */
           if (widisp(icontx,fileptr,nsam,nsam1,nsam2,nrec1,nrec2,
                      fmin_spi,fmax_spi,ixul,iyul,nmint,nmaxt,iskip,FALSE,
                      &imagel,FALSE)  == FALSE)
             {waitcursor(iwin, FALSE, 79); return FALSE;}
           }

         if (names)
           {    /* Put slice number under each pane */
           wrtname(ixul,iyul+nrowi,(char *) NULL,islice); 
           }

         if (!ndone) 
            {ixulmon = ixul; iyulmon = iyul;}
         ndone  = ndone + 1;
         placed = FALSE;

         /* Force desired no. of images / line even if it truncates an image */
         force  = TRUE;
         }   /*for (islice=nslice1; islice<=nslice2; islice++) */
       }     /*  if (axis = z) */
       
     else if ((normit == FALSE && numvox < maxvox) ||
              (normit == TRUE  && numvox < maxflt))
       {   /* can use fast incore routine for display of x or y slices */

       if ((cmont3buf = (char *) malloc(numvox * sizeof(char))) == (char *) 0)
          {
          spout(" *** Can not allocate cmont3buf in mont3."); 
          waitcursor(iwin, FALSE, 79); return FALSE; 
          }

       if (normit == TRUE)
         {   /* Normalize slice by slice (handles iskip OK) */
         iret = ztoxyn(fileptr,cmont3buf,axis,nmaxt,iskip,mapgo); 
         }
       else
         {  /* Do not normalize by slice */    
         iret = ztoxy(fileptr, cmont3buf, axis, fn, fncon, iskip); 
          }
       if (! iret)
           {
           if (cmont3buf) free(cmont3buf); closefile(fileptr); fileptr = NULL; 
           cmont3buf = NULL; waitcursor(iwin, FALSE, 79); return FALSE;
           }  

       for (islice=1; islice <= nslicei; islice++)
         {  /* plot out values returned from ztoxy  routine in cmont3buf */
         if (movie == FALSE)
           {
           /* Force a new line in montage if necessary */
           if (ndone%nprow == 0) newline = TRUE;
 
           /* find image position */
           locc(&ixul,&iyul,nsami,nrowspn,&ntop,&nbot,&nrit,
               marx,mary,marlef,martop,iwidex,ihighx,placed,
               newline,&nuscrn,force);
           newline = FALSE;
           }

         iloc = (islice -1) * nsami * nrowi;
         wipic(icontx, &cmont3buf[iloc], ixul, iyul, nsami, nrowi, TRUE, 
             TRUE, &imagel); 

         if (names)
           {    /* put slice number under each pane */
           wrtname(ixul,iyul+nrowi,(char *) NULL,islice); 
           }

         if (!ndone)
            {ixulmon = ixul; iyulmon = iyul;}
         ndone  = ndone + 1;
         placed = FALSE;
         /* force desired no. of images / line even if it truncates image */
         force  = TRUE;
         }    /* for (islice=1; islice <= nslicei; islice++) */

       if (cmont3buf) free(cmont3buf);  cmont3buf = NULL; 
     }

   else if (axis == 'y')
     {   /* 3-D FILE WITH Y SLICE,CAN NOT USE FAST INCORE ROUTINE *****/

     if (normit == FALSE && 
         (cmont3buf = (char *) malloc(nsami * nrowi * sizeof(char))) == (char *) 0)
        {
        spout(" *** Can not malloc cmont3buf2 in mont3."); 
        waitcursor(iwin, FALSE, 79); return FALSE;
        }

     buf = filedata -> buf;
     for (irow=nrow1; irow<=nrow2; irow++)
       {
       if (movie == FALSE)
          {
          /* force a new line in montage if necessary */
          if (ndone%nprow == 0) newline = TRUE;
 
          /* find image position */
          locc(&ixul,&iyul,nsami,nrowspn,&ntop,&nbot,&nrit,
               marx,mary,marlef,martop,iwidex,ihighx,placed,
               newline,&nuscrn,force);
          newline = FALSE;
          }

       if (normit == TRUE)
          {   /* normalize slice by slice */
          if (monn(icontx,fileptr,irow,
              nsami,nrowi,nslicei,nmint,nmaxt) == FALSE)
             {waitcursor(iwin, FALSE, 79); return FALSE; }
          }
       else
          {           /* OVERALL FILE NORMALIZED Y SLICING */
          if (iskip >= 1)
             {        /* normal or reduced size display */

             /* find upper end of slice and sample series */
             nsliceu = nslice2 - ((nslice2 - nslice1 + 1) % iskip) - 1;

             iy      = iyul;
             for (islice=nslice1; islice <= nsliceu; islice=islice + iskip)
                {
                irec = (islice -1) * nrow + irow;
                if (redlin(fileptr,buf,nsam,irec) == FALSE) 
                   {if (cmont3buf) free(cmont3buf);  cmont3buf = NULL;
                    waitcursor(iwin, FALSE, 79); return FALSE;
                   }

                l = 0;
                for (isam=nsam1-1; isam<nsam2; isam = isam + iskip)
                  {
                  ival = buf[isam] * fn + fncon;
                  if (ival > 127) ival = ival - 256;
                  cmont3buf[l++] = ival;
                  }

                /* DISPLAY THIS LINE OF THE IMAGE */
                wipic(icontx, cmont3buf, ixul, iy++, nsami, 1, TRUE,
                      TRUE, &imagel);  
                }
             }

          else if (iskip < -1)
             {        /* magnified display */
             for (islice=nslice1; islice<=nslice2; islice++)
                {
                irec = (islice -1) * nrow + irow;
                if (redlin(fileptr,buf,nsam,irec) == FALSE) 
                   { if (cmont3buf) free(cmont3buf);  cmont3buf = NULL;
                        waitcursor(iwin, FALSE, 79); return FALSE;}
                iloc = 0;
                for (isam=nsam1-1; isam<nsam2; isam = isam + iskip)
                   {
                   ival = buf[isam] * fn + fncon;
                   if (ival > 127) ival = ival - 256;

                   for (j=1; j <= miskip; j++)
                       cmont3buf[iloc++] = ival;
                   }

                /* display this line -iskip times */
                for (j=1; j <= miskip; j++)
                   wipic(icontx, cmont3buf, ixul, iy++, nsami, 1, TRUE,
                      TRUE, &imagel);  
                }
             }
          }
         if (names)
           {    /* put slice number under each pane */
           wrtname(ixul,iyul+nrowi,(char *) NULL,islice); 
           }

       if (!ndone)
          {ixulmon = ixul; iyulmon = iyul;}
       ndone  = ndone + 1;
       placed = FALSE;
       /* force desired no. of images / line even if it truncates an image */
       force  = TRUE;
       }
    if (normit == FALSE) { free(cmont3buf); cmont3buf = NULL;} 
    }

 else if (axis == 'x')
    {    /* 3-D FILE WITH X SLICE (CAN NOT USE INCORE) ****************/

    spout("WARNING X AXIS SLICING IS SLOW!");

    if (normit == FALSE && (cmont3buf = (char *) malloc(nsami * nrowi * sizeof(char))) == (char *) 0)
       {
       spout(" *** Can not allocate cmont3buf2 in mont3."); 
       closefile(fileptr);  fileptr = NULL; 
       waitcursor(iwin, FALSE, 79); return FALSE;
       }

    buf = filedata -> buf;
    for (isam= nsam1; isam<=nsam2; isam++)
       { 
       if (movie == FALSE)
          {
          /* force a new line in montage if necessary */
          if (ndone%nprow == 0) newline = TRUE;
 
          /* find image position */
          locc(&ixul,&iyul, nsami,nrowspn,&ntop,&nbot,&nrit,
               marx,mary,marlef,martop,iwidex,ihighx,placed,
               newline,&nuscrn,force);
          newline = FALSE;
          }

       if (normit == TRUE)
          {   /* normalize slice by slice */
          if (monn(icontx,fileptr,isam,
              nsami,nrowi,nslicei,nmint,nmaxt) == FALSE) 
              {
              closefile(fileptr);  waitcursor(iwin, FALSE, 79); 
              fileptr = NULL; return FALSE;
              }
          }
       else
          {
          if (iskip >= 1)
             {  /* normal or reduced size display */

             /* find upper end of slice and sample series */
             nsliceu = nslice2 - ((nslice2 - nslice1 + 1) % iskip) - 
                       iskip + 1;

             iy = iyul;

             for (islice=nslice1; islice<=nsliceu; islice=islice + iskip)
                {
                irec1 = (islice -1) * nrow + irow;
                l = 0;
                for (irow=nrow1-1; irow<nrow2; irow = irow + iskip)
                  {
                  if (redlin(fileptr,buf,nsam,irec1+irow) == FALSE) 
                      { 
                      if (cmont3buf) free(cmont3buf);  cmont3buf = NULL; 
                      closefile(fileptr);  waitcursor(iwin, FALSE, 79); 
                      fileptr = NULL;   return FALSE;
                      }
                  ival = buf[isam-1] * fn + fncon;
                  if (ival > 127) ival = ival - 256;
                  cmont3buf[l++] = ival;
                  }

                /* DISPLAY THIS LINE OF THE IMAGE */
                wipic(icontx, cmont3buf, ixul, iy++, nsami, 1, TRUE,  
                      TRUE, &imagel);  
                }
             }

          else if (iskip < -1)
             {        /* magnified display */

             for (islice=nslice1; islice <= nslice2; islice++)
                {
                irec1 = (islice -1) * nrow + irow;
                iloc  = 0;
                iy    = iyul;

                for (irow=nrow1-1; irow<nrow2; irow++)
                   {
                   if (redlin(fileptr,buf,nsam,irec+irow) == FALSE) 
                       {
                       if (cmont3buf) free(cmont3buf);  cmont3buf = NULL; 
                       closefile(fileptr); fileptr = NULL;
                       waitcursor(iwin, FALSE, 79); return FALSE;
                       }

                   ival = buf[isam] * fn + fncon;
                   if (ival > 127) ival = ival - 256;

                   for (j=1; j<=miskip; j++)
                      cmont3buf[iloc++] = ival;
                   }

                /* display this line -iskip times */
                for (j=1; j <= miskip; j++)
                   wipic(icontx, cmont3buf, ixul, iy++, nsami, 1, TRUE,
                      TRUE, &imagel);  
                }
             }

         if (names)
           {    /* put slice number under each pane */
           wrtname(ixul,iyul+nrowi,(char *) NULL,islice); 
           }

          if (!ndone) 
            {ixulmon = ixul; iyulmon = iyul;}
          ndone  = ndone + 1;
          placed = FALSE;
          /* force desired no. of images / line even if it truncates an image */
          force  = TRUE;
          }
       } 
    if (normit == FALSE && cmont3buf) { free(cmont3buf);  cmont3buf = NULL;}
    }

    /* free up the file descriptor for this image */
    closefile(filedata); fileptr = NULL;

   }   /*  file exists  */

   /* get next file name */
   if (filinc(filnow, filnext, &nlet, &numfil) != FALSE) 
      {   /* file number incrementing succeeded */  
      if (numfil <= endfile || endfile < 0) 
         {     /* still want to display this next file so open it */
         if ((filedatan = openold(filnext, &nsamn, &nrown, &nslicen, 
              &iformn, "q")) == NULL)
            {   /* Next file not found (may not exist if * was used) */
                /* Stop if lastfile indicates * was used for files */
            file_exist = FALSE;
            if (endfile < 0) 
               {toomany = TRUE;}
            else        /* otherwise, skip and continue */
               {strcpy(filnow,filnext);}  /* try opening file on next loop */
            }
         else
            {   /* Next image file found OK, set filnow to filnext */
            file_exist = TRUE;
            filedata = fileptr = filedatan;
 
            if ((filedata->imami != 1) && (norm(filedata) == FALSE))
              {    
              spout("*** Normalization failed in mont3");
              closefile(filedata); fileptr = NULL;
              waitcursor(iwin, FALSE, 79);
              newline = FALSE;
              return FALSE;
              }

            strcpy(filnow,filnext);
            fmin_spi     = filedata->fmin_spi;
            fmax_spi     = filedata->fmax_spi; 
            nsam     = nsamn;
            nrow     = nrown;
            nslice   = nslicen;
            }       /* ends: if ((filedatan */
         }          /* ends: if (numfil     */
      }             /* ends: if (filinc     */

    force = FALSE;
    if (resetmar == TRUE) marlef = marlefo;

 }   /* ends: do { }while          */

 while ((numfil <= endfile && !toomany) || (endfile < 0 && !toomany));

 /*  CANCEL WAIT CURSOR */
 waitcursor(iwin, FALSE, 79);
 newline = FALSE;

 return TRUE;
 }


