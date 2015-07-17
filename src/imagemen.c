
/*$Header: /usr8/web/src/RCS/imagemen.c,v 1.86 2015/06/16 15:36:08 leith Exp $*/

/*
 C++********************************************************************
 C
 C imagemen.c                                   Nov 1992 
 C             Added pixelmen_in call           Jun 2011  ArDean Leith                                                                    
 C             Keep up image menu widget        Nov 2012  ArDean Leith                                                                    
 C             Keep up more image menu widgets  Jun 2015  ArDean Leith                                                                    
 C *********************************************************************
 C=* AUTHOR: A. LEITH 
 C=* FROM: WEB - VISUALIZER FOR SPIDER MODULAR IMAGE PROCESSING SYSTEM *
 C=* Copyright 1985-2015  Health Research Inc.,                        *
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
 C  IMAGEMEN
 C
 C  PARAMETERS:   iw_temp, data, call_data (usual callback data)
 C
 C  PURPOSE:      Makes a file widget, interrogates file widget and
 c                calls image display routine
 C
 C  CALLED BY:    web_com
 C
 C--********************************************************************
*/

/* Common variables */
#include <Xm/PushBG.h>
#include <Xm/FileSB.h>

#include <unistd.h>

#include "common.h"
#include "routines.h"

 /* External function  prototypes */
 extern void dendromen  (char * );
 extern void ctfmen0    (char * );
 extern void showmovmen2(void);

 /* Internal function  prototypes */
 void       imagemen_cb    (Widget, XtPointer, XtPointer);
 void       imagemennol_cb (Widget, XtPointer, XtPointer);
 void       imagemen_cb_com(void);

 /* Externally defined common variables */
 extern char    outstr[80];
 extern float   erodethresh;  /* Used in filtermen, erodemen */
 extern int     docimgtotal;  /* From: docmontmen */

 /* Common variables defined here and used elsewhere */
 FILEDATA*  filedatal = NULL; /* Used by backmen, pickback, pickmen */
 FILEDATA*  filedatar = NULL; /* Used by backmen, pickback, pickmen */
 float*     fimage = NULL;    /* Used by filtermen, pixelmen_in also */
 char       filcolvol[81];    /* Name for color vol. file */

 char*      cptrduml = NULL; /* Data used by image right */
 char*      cptrdumr = NULL; /* and left */
 int        icattyp;

 /* File scope variables used here */
 static char   filnew[128];
 static int    imnum = 1;
 static char   ops[] = "123456789abcdefghijklmnop"; // 25 opers
 static char * prompts[] = 
      {
      "Image",                       "First Image",   
      "Volume",                      "Histogram",
      "Image for Marker Picking",    "Image for Masking",
      "Image for Profiling",         "Untilted Image",
      "Image for Annotating",        "Image for Convolution",
      "Image/Stack for Categorizing","Image with Reflections",
      "Recorded Image",              "Dendrogram Doc File",
      "Volume", 		     "Movie Montage",
      "Volume",                      " ",
      "Drill-n-fill Volume",         "Montage Doc File",
      "Power Spectrum",              "CTF Doc File",
      "Montage Doc File",	     "Movie Series",
      "Image for Pixel"             // Last is: "p" = 25
      };
 /* Note: i is shift which does not get a prompt */


/*************************  imagemen  ********************************/

 void imagemen(Widget iw_temp, XtPointer data, XtPointer call_data)
 { 

 char   *oper;
 char    pattern[86];
 char   *filename;
 char    ctemp[3];

 oper = data;

 if (filwant[0] != '\0')
    { /* Use new file pattern */
    strcpy(&pattern[0],&filwant[0]);

    if (datexc[0] != '\0')
       {
       /* Set default file filter */
       strcat(pattern,".");
       strcat(pattern,&datexc[0]);
       }
    filwant[0] = '\0';
    }
 else
    pattern[0] = '\0';

 /* Convert first oper character to integer */
 ioper = (int) (strchr(ops,*oper) - ops) + 1;

 /* Set name for file */
 filename = &filnow[0];


 if (ioper == 8 && imnum == 2)
    {      /*************************** Interactive particle picking */
    if (!listfi) 
       {
       iw_filesnol = wid_text(iw_win, iw_filesnol, "Tilted image",
                    filename, *imagemennol_cb, NULL, -1, -1);
       }
    else
       {
       /* Create a file selector box */
       iw_files =  wid_file(iw_win, iw_files , "Tilted image",
               filename, &pattern[0], *imagemen_cb, -1, -1);
       }
    } 

 else if (ioper == 17  && imnum == 2)
    {        /**************************** Surface color texture map */
    filename = &filcolvol[0];
    if (!listfi) 
       {
       iw_filesnol = wid_text(iw_win, iw_filesnol, "Color volume",
                    filename, *imagemennol_cb, NULL, -1, -1);
       }
    else
       {
       /* Create a file selector box */
       iw_files =  wid_file(iw_win, iw_files , "Color volume",
               filename, &pattern[0], *imagemen_cb, -1, -1);
       }
    }

 else if (ioper == 18)
    {	 /********************************************* Shift image */
    if (imagel)
	shift(IMAGE);
    else if (islice > 1) 
	spout("*** Shift doesn't work for volumes.");
    else 
	spout("*** Current Image does not exist.");
    } 

 else
    {   /************************************* All other operations */
    if (!listfi) 
       {
       iw_filesnol = wid_text(iw_win, iw_filesnol, prompts[ioper-1],
                    filename, *imagemennol_cb, NULL, -1, -1);
       }
    else
       {
       /* Create a file selector box */
       iw_files =  wid_file(iw_win, iw_files , prompts[ioper-1],
               filename, &pattern[0], *imagemen_cb, -1, -1);
       }
    }
 }



/******************** imagemen_cb ************************************/

 void imagemen_cb(Widget iw_temp, XtPointer data,  
                                  XtPointer call_data)
  { 
  char       dirnowt[128];
  char  *    string;

  XmFileSelectionBoxCallbackStruct *cbs = 
    (XmFileSelectionBoxCallbackStruct *) call_data;

  /* Get name of file returned from the file widget */
  string = extractstr(cbs->value);
  strcpy(filnew,string);
  XtFree(string);

  /* Get name of directory used in the file widget */
  string = extractstr(cbs->dir);
  strcpy(dirnowt,string);
  XtFree(string);

  if (strcmp(dirnow,dirnowt) != 0)
     {    /* New directory wanted */
     strcpy(dirnow,dirnowt);
     chdir(dirnow);
     }

  imagemen_cb_com();
  return;
  }

/******************** imagemennol_cb ************************************/

/* Used if !listfi (when do not want file selction widget */

 void imagemennol_cb(Widget iw_temp, XtPointer data,  
                                     XtPointer call_data)
  { 
  char  *           string;

  XmSelectionBoxCallbackStruct *cbs = 
    (XmSelectionBoxCallbackStruct *) call_data;

  /* Get name of file returned from the file widget */
  string = extractstr(cbs -> value); 
  strcpy(filnew,string);
  XtFree(string);

  imagemen_cb_com();

  return;
  }

/******************** imagemen_cb_com *************************/

 void imagemen_cb_com(void)
  { 
  int               sayrange;

  if (!listfi)
     XtUnmanageChild(iw_filesnol);
  else if ((ioper != 1) && (ioper != 2) && (ioper != 3) )     // new Nov 2012 al
     XtUnmanageChild(iw_files);

  expos_cb(NULL,NULL,NULL);

  sayrange = TRUE;
  if (ioper == 4) sayrange = FALSE;

  if (ioper == 20 ||       /* Display montage    from doc file menu */
      ioper == 23)         /* Display categorize from doc file menu */
    {  
    docmontmen0(filnew);   /* Retrieve doc file info */
    if (docimgtotal < 0)   /* Bad response  */
       { 
       spouts("*** Bad doc file: ");  spout(filnew);
       return;
       }
   
    docmontmen(filnew);    /* Show montage from doc menu */
    return;
    }
  else if ( ioper == 14)
    {                      /* Display dendrogram menu */
    dendromen(filnew);
    return;
    }
  else if ( ioper == 22)
    {                      /* Display CTF menu */
    ctfmen0(filnew);
    return;
    }

  else if (ioper == 17 && imnum == 2)
     {                    /* Display colored surface from volume */ 
     imnum = 1;

     strcpy(filcolvol,filnew);
     /* printf("filcolvol: %s, filnewt: %s \n",filcolvol,filnew); */

     /* Get menu for displaying the colored surface */   
     surfmen(ioper == 15, FALSE, ioper == 17);
     return;
     }
 
  imagemen1(filnew,sayrange);

  return;
  }

 /******************************** imagemen1 *************************/

 int imagemen1(char * filnewt, int sayrange)
 { 

  int         iform;
  char*       cptrdum = NULL;
  float*      fptrdum = NULL;
  XRectangle  rect;
  int         nsams, nrows;   /* Size of the image on screen*/

  /* Set waitcursor */
  waitcursor(iwin, TRUE, 79);

  /* Open the new existing file */
  filedata = openold(&filnewt[0],&nsam,&nrow,&nslice,&iform,"o");

  if (filedata == (FILEDATA *) -1)
     {  // Skip message
     waitcursor(iwin, FALSE, 79);
     if (ioper == 8)
         if (filedatal) closefile(filedatal);
     return FALSE;
     }
  else if (filedata == (FILEDATA *)FALSE)
     {
     spout("*** Unable to open image file");
     waitcursor(iwin, FALSE, 79);
     if (ioper == 8)
         if (filedatal) closefile(filedatal);
     return FALSE;
     }
   
  if (((filedata->nstack == 0)    && 
       (filedata->imami != 1)     && 
       (norm(filedata) == FALSE)) ||

      ((filedata->nstack > 0)     && 
       (filedata->imginstack > 0) && 
       (filedata->imami != 1)     && 
       (norm(filedata) == FALSE)))     {    
     spout("*** Normalization failed.");
     closefile(filedata);
     waitcursor(iwin, FALSE, 79);
     return FALSE;
     }
  
  if ((filedata->nstack > 0)     && 
      (filedata->imginstack == 0))
     {   /* Overall stack header referenced */ 
     
     fmin_spi = 0.0;
     fmax_spi = 0.0;
     }
  else
     { 
     fmin_spi = filedata->fmin_spi;
     fmax_spi = filedata->fmax_spi; 

     if (sayrange)
        {   /* List image range */
        sprintf(&outstr[0],"Range: %f ... %f",
                fmin_spi,fmax_spi);
        spout(&outstr[0]);
        }

     if ((fmax_spi - fmin_spi) < 10e-10)
        {     /* This is a blank image */
        spout("*** BLANK IMAGE SKIPPED");
        waitcursor(iwin, FALSE, 79);
        closefile(filedata);
        return FALSE;
        }
     }

  /* Set flag if present image file same as previous file */
  if (strcmp(&filnow[0],&filnewt[0]) == 0) 
     samefile = TRUE;
  else
     {
     samefile = FALSE;
     gotsurf  = FALSE;
     }

  /* Set name of present displayed image */
  strcpy(&filnow[0],&filnewt[0]);

  /* set default display area to full slice */
  ixullmin = marlef;      iyullmin = martop;
  ixlrlmax = iwidex;      iylrlmax = ihighx;
  ixulrmin = iwidex;      iyulrmin = martop;
  ixlrrmax = iwidex;      iylrrmax = ihighx; 
  if (ioper == 8)
     {
     ixlrlmax = iwidex/2;
     ixulrmin = iwidex/2+1;
     }

  nsam1    = 1;    nrow1   = 1;   
  nslice1  = 1;    nslice2 = 1;

  if (iskip > 1)    
    {                /* Shrink image by factor of iskip */ 
     if( (nsam/iskip) < (ixlrlmax-ixullmin+1) )   /* Try to display  */
        nsam2 = nsam;                             /* as much as the  */
     else                                         /* window can view */
        nsam2 = (ixlrlmax-ixullmin+1)*iskip;
     if( (nrow/iskip) < (iylrlmax-iyullmin+1) ) 
        nrow2 = nrow;
     else 
        nrow2 = (iylrlmax-iyullmin+1)*iskip;
    }
  else 
    {  
     nsam2   = MYMIN(nsam,ixlrlmax-ixullmin+1);
     nrow2   = MYMIN(nrow,iylrlmax-iyullmin+1);
    }

  /* Reset the image size parameters */
  dispsize( nsam1, nsam2, nrow1, nrow2, 1,1,iskip, 
            &nsams, &nrows, &islice);

  /* Set image display range */
  imagego    = mapgo;
  imageend   = mapend;

  //printf("In: imagemen1; ioper: %d, filedata->nstack: %d \n",ioper,filedata->nstack); 

  if ((ioper == 1 || ioper == 2 || ioper == 11) && 
      (filedata->nstack  > 0     && 
       filedata->imginstack == 0)) 
     {                /********************** Display   from stack file */
                      /********************** Categorize from stack file */
     /* Cancel waitcursor */
     waitcursor(iwin, FALSE, 79);

     /* Display stacked images selector */
     stackmen(filedata);      
     }

  else if (ioper == 1  && nslice == 1 )
     {                /************************* Display single image */

     /* Set the full image size for image display */
     nsam2 = nsam;   nrow2 = nrow;  islice = 1;      
     dispsize(nsam1, nsam2, nrow1, nrow2, 1,1,iskip, 
             &nsams, &nrows, &islice);

     imagedis(filedata,TRUE,&imagel,
              TRUE,FALSE,&fptrdum, 
              imagego,imageend);

     /* set the variables for image shifting */
     ixullmin = ixul;	ixlrlmax = MYMIN(ixul+nsams,ixlrlmax);
     iyullmin = iyul;	iylrlmax = MYMIN(iyul+nrows,iylrlmax);

     nsam2 = nsams;  nrow2 = nrows;  islice = 1;

     /* Record left image location and size */
     nsaml  = nsams;  nrowl  = nrows;
     nsam1l = nsam1;  nrow1l = nrow1;
     nsam2l = MYMIN(nsams,ixlrlmax-ixullmin+1);  
     nrow2l = MYMIN(nrows,iylrlmax-iyullmin+1);
     ixull  = ixul;   iyull  = iyul;
     ixulli = ixul;   iyulli = iyul;
     ixuln  = ixul;   iyuln = iyul;

     /* Free up the file descriptor */
     closefile(filedata);

     /* Cancel waitcursor */
     waitcursor(iwin, FALSE, 79);     }

  else if (ioper == 1 || 
          (ioper == 2 && nslice > 1))
     {                /***************** Display montage from volume */     

     /* Cancel waitcursor */
     waitcursor(iwin, FALSE, 79);

     montmen3();     
     }

  else if (ioper ==  2 || 
          (ioper == 11 && icattyp == 0))
     {                 /* *********** Display montage from 2d volume */     
                       /* *********** Categorize from seq. files     */     
     montmen2();

     /* Cancel waitcursor */
     waitcursor(iwin, FALSE, 79);
     }

  else if (ioper == 17 && imnum == 1)
     {                /***************** Get color volume name first */
     /* Cancel waitcursor */
     waitcursor(iwin, FALSE, 79);
     imnum = 2; 
     imagemen(NULL,"h",NULL);
     }

  else if (ioper ==  3  || ioper == 15)
     {                /**************** Display surface from volume */ 
     /* Cancel waitcursor */
     waitcursor(iwin, FALSE, 79);
     imnum = 1;

     if (nslice <=1)
        {
        spout("*** Can not show surfaces of 2-D images.");
        closefile(filedata);
        return FALSE;
        } 
     /* Display the surfaces or sird image of surface */   
     surfmen(ioper == 15, FALSE, ioper == 17);
     }

  else if (ioper ==  4)
     {            /*************************************** Histogram */ 
     hist(filedata);
     /* Cancel waitcursor */
     waitcursor(iwin, FALSE, 79);
     /* Filedata is closed in hist-->his */
     }

  else if (ioper == 5 || ioper == 6)
     {   /************************************************** Markers */
         /************************************************** Mask    */
     /* Locate markers from this image */ 
     /* Display single image */

     marker = 1;

     /* Set the full image size for image display */
     nsam2 = nsam;  nrow2 = nrow;  islice = 1;      
     dispsize( nsam1, nsam2, nrow1, nrow2, 1,1,iskip, 
               &nsams, &nrows, &islice);

     /* Keeps image in imagel */
     imagedis(filedata,TRUE, &imagel,
              TRUE,FALSE, &fptrdum, 
              imagego,imageend);

     /* Cancel waitcursor */
     waitcursor(iwin, FALSE, 79);

     /* Record left image location and size */
     ixullmin = ixul;	ixlrlmax=ixul+nsams;
     iyullmin = iyul;	iylrlmax=iyul+nrows;

     nsaml    = nsams;  nrowl  = nrows;
     nsam1l   = nsam1;  nrow1l = nrow1;
     nsam2l   = nsams;  
     nrow2l   = nrows;
     ixull    = ixul;   iyull  = iyul;
     ixulli   = ixul;   iyulli = iyul;

     if (ioper == 5)
        /* Locate markers from this image */ 
        {gold();}

     else if (ioper == 6)
        /* Mask this image */ 
        {mask();}

     /* Free up the file descriptor */
     closefilep(&filedata);
     }


  else if (ioper == 7)
     {         /******************************************** Profile */
     islice = 1;

     /* Display single image first*/
     imagedis(filedata,FALSE,&imagel,
              FALSE,FALSE, &fptrdum, 
              imagego,imageend);

     /* Cancel waitcursor */
     waitcursor(iwin, FALSE, 79);

     /* Profile this image */
     profile();

     /* Filedata closed in profilemen_buts */
     }

  else if (ioper == 8)
     {           /********************************* Particle picking */

     /* Set upper left corner */
     placed = TRUE;
     islice = 1;

     if (imnum == 1)  
        {          /* display  left image */

        /* Set left image visible display position */
        nsam2 = nsam;  nrow2 = nrow;  islice = 1;

        ixuln  = ixullmin;  iyuln  = iyullmin;  
        rect.x = ixuln;     rect.y = iyuln;
        rect.width  = ixlrlmax-ixullmin;
        rect.height = iylrlmax-iyullmin;

        XSetClipRectangles(idispl,icontx,0,0,&rect,1,Unsorted);

        /* Do not save floating point buffer for future use */
        showimage(filedata, TRUE,  &cptrduml, 
                            TRUE,  &imagel, 
                            FALSE, &fptrdum, TRUE,
                            (float) (filedata->fmin_spi), 
                            (float) (filedata->fmax_spi),
                            imagego, imageend);

        /* Save pointer to left filedata */
        filedatal = filedata;

        /* Record left image location and size */
        nsaml  = nsam;   nrowl  = nrow;
        nsam1l = nsam1;  nrow1l = nrow1;
        nsam2l = MYMIN(nsam,ixlrlmax-ixullmin+1);  
        nrow2l = MYMIN(nrow,iylrlmax-iyullmin+1);
        ixull  = ixuln;  iyull  = iyuln;
        ixulli = ixuln;  iyulli = iyuln;

        rect.width  = ixlrrmax-ixullmin;
        rect.height = iylrrmax-iyullmin;

        XSetClipRectangles(idispl,icontx,0,0,&rect,1,Unsorted);

        /* Cancel waitcursor */
        waitcursor(iwin, FALSE, 79);
        imnum = 2;

        /* Display second image */
        imagemen(NULL,"8",NULL);
        }
     else
        {
        /* Set right image visible display position */
        ixuln  = ixulrmin; iyuln  = iyulrmin; 
        nsam1  = 1;   nsam2   = nsam;
        nrow1  = 1;   nrow2   = nrow;

        islice = 1;

        /* Do not save floating point buffer for future use */
        showimage(filedata, TRUE,  &cptrdumr, 
                            TRUE,  &imager, 
                            FALSE, &fptrdum, TRUE,
                            (float) (filedata->fmin_spi), 
                            (float) (filedata->fmax_spi),
                            imagego, imageend);
                   
        /* Save pointer to right filedata */
        filedatar = filedata;

        nsamr  = nsam;   nrowr  = nrow;
        nsam1r = nsam1;  nrow1r = nrow1;
        nsam2r = MYMIN(nsam,ixlrrmax-ixulrmin+1);  
        nrow2r = MYMIN(nrow,iylrrmax-iyulrmin+1);
        ixulr  = ixuln;  iyulr  = iyuln;
        ixulri = ixuln;  iyulri = iyuln;

        /* Cancel waitcursor */
        waitcursor(iwin, FALSE, 79);
        imnum = 1;

        /* Retrieve existing doc files for this set of data */
        fitdocmen(NULL,NULL,NULL);

        /* Do not free up image file descriptors */
        }
     }
 else if (ioper == 9)
     {           /***************************************** Annotate */

     islice = 1;
     /* Display single image first*/
     imagedis(filedata,FALSE,&imagel,
              FALSE,FALSE, &fptrdum, 
              imagego,imageend);

     /* Cancel waitcursor */
     waitcursor(iwin, FALSE, 79);

     /* Annotate this image */
     annotemen();

     }

 else if (ioper == 10 || ioper == -10)
     {          /******************************************** Filter */
     islice = 1;

     /* Display single image from file first, do not keep image */
     /* Save floating point buffer for future use           */

     showimage(filedata, FALSE, &cptrdum,
                         FALSE, &imagel, 
                         TRUE,  &fimage, TRUE,
                         (float)filedata->fmin_spi, 
                         (float)filedata->fmax_spi,
                         imagego, imageend);
          

     /* Cancel waitcursor */
     waitcursor(iwin, FALSE, 79);

     if (ioper == 10)
        {
        erodethresh = filedata->fmax_spi;

        /* Display filter selection menu */
        filtermen();
        }
     else
        {
        /* Use previous filter type during repeat */
        }
     }

  else if (ioper == 12)
     {   /****************************************** Pick reflections */ 
     /* Display single image */
     islice = 1;
     imagedis(filedata,FALSE, &imagel,
              FALSE,FALSE, &fptrdum, 
              imagego,imageend);

     /* Cancel waitcursor */
     waitcursor(iwin, FALSE, 79);

     reflmen();

     /* Free up the file descriptor */
     closefile(filedata);
     }                  
 
  else if (ioper == 13)
     {   /****************************************** Show recording */ 

     /* Display single recorded image */
     islice = 1;
     imagedis(filedata,FALSE, &imagel,
              FALSE,FALSE, &fptrdum,
              (int) filedata ->fmin_spi, 
              (int)filedata ->fmax_spi);

     /* Cancel waitcursor */
     waitcursor(iwin, FALSE, 79);

     /* Free up the file descriptor */
     closefile(filedata);
     }                  
   else if (ioper ==  16)
     {   /************************************** Small Movie montage */
     /* Start again at upper left origin */
     ixul   = marlef;
     iyul   = martop;
     nbot   = -1;
     if (nslice > 1)
        {         /**************** Display montage from 3-d volume */    
        montmen3();
        }
     else
        {         /**************** Display montage from 2-d volume */     
        montmen2();
        }

     /* Cancel waitcursor */
     waitcursor(iwin, FALSE, 79);
     }
  else if (ioper ==  19)
     {                /******************* Drill-n-fill from volume */ 
     /* cancel waitcursor */
     waitcursor(iwin, FALSE, 79);

     if (nslice <= 1)
        {
        spout("*** Can not drill or fill 2-D images.");
        closefile(filedata);
        return FALSE;
        }
 
     /* Drill or fill a  volume */   
     dentmen();
     } 

  else if ((ioper ==  20 || ioper == 23) && 
            filedata->nstack > 0) 
     {           /***********  Montage    stack from doc file list */     
                 /************ Categorize stack from doc file list */
     stackdis(imagego, imageend);

     /* Cancel waitcursor */
     waitcursor(iwin, FALSE, 79);
     }

  else if (ioper ==  20 || ioper == 23)
     {           /********** Montage    images from doc file list */     
                 /********** Categorize images from doc file list */
     mont2(imagego, imageend);

     /* Cancel waitcursor */
     waitcursor(iwin, FALSE, 79);
     }

  else if (ioper == 21)
     {   /****************************************** Fourier mask */ 
     /* Display single image */
     islice = 1;
     imagedis(filedata,FALSE, &imagel,
              FALSE,FALSE,&fptrdum, 
              imagego,imageend);

     /* Cancel waitcursor */
     waitcursor(iwin, FALSE, 79);

     foumen();

     /* Free up the file descriptor */
     closefile(filedata);
     }
                  
   else if (ioper ==  24) 
     {                 /*********************** Big Movie display */
     if  (filedata->nstack > 0  && 
          filedata->imginstack == 0) 
        {       /* Display from stack file */

        /* Display stacked images selector */
        stackmen(filedata);      
        }

     else if (nslice > 1)
        {showmovmen3();}
     else
        {showmovmen2();}

     /* Cancel waitcursor */
     waitcursor(iwin, FALSE, 79);
     }
  
  else if (ioper == 25 )
     {   /************************************************** Pixel */

    /* Set the full image size for image display */
     nsam2 = nsam;  nrow2 = nrow;  islice = 1;      
     dispsize( nsam1, nsam2, nrow1, nrow2, 1,1,iskip, 
               &nsams, &nrows, &islice);

     /* Keeps image in imagel */
     imagedis(filedata,TRUE, &imagel,
              TRUE,TRUE, &fimage, 
              imagego,imageend);

     /* Cancel waitcursor */
     waitcursor(iwin, FALSE, 79);

     /* Record left image location and size */
     ixullmin = ixul;	ixlrlmax=ixul+nsams;
     iyullmin = iyul;	iylrlmax=iyul+nrows;

     nsaml    = nsams;  nrowl  = nrows;
     nsam1l   = nsam1;  nrow1l = nrow1;
     nsam2l   = nsams;  
     nrow2l   = nrows;
     ixull    = ixul;   iyull  = iyul;
     ixulli   = ixul;   iyulli = iyul;

     // Query pixels from this image  
     pixelmen_in_nod();

     /* Free up the file descriptor */
     closefilep(&filedata);
     }
  return TRUE;
  }

