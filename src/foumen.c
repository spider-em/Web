/*
****************************************************************************
*
*  foumen.c                                    new:    Sept 30, 1998 al
*
C **********************************************************************
* *  AUTHOR:  ArDean Leith                                                 *
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
*
*    foumen()
*
*    PURPOSE:	Mask a power spectrum and create a fourier file for
*               filtering.
*
****************************************************************************
*/

#include <Xm/Text.h>
#include "common.h"
#include "routines.h"

#define MAXLOCS 50

 /* external subroutine prototypes */

 /* internal subroutine prototypes */
 static void  foumen_buts(Widget, XtPointer, XtPointer);
 static void  foumen_buta(Widget, XtPointer, XtPointer);
 static void  fou        (void);
 static void  fou_pop    (Widget, XEvent *, String *, Cardinal *);
 static void foufilt     (void);
                            
 /* internal file variables */
 static Widget       iw_foumen = (Widget) 0;
 static Widget       iw_fname, iw_radius;
 static float        radius;
 static FILEDATA *   filedatat = (FILEDATA *) NULL; 
 static int          ilocs = 0; 
 static int          ix[MAXLOCS],iy[MAXLOCS];
 static float        radii[MAXLOCS];
 static char         pwfilnam[81];
 static int          nsamf;

 /***************************  foumen **************************/

 void foumen(void)
 {
 Widget iw_rowcol, iw_pushs, iw_pushc, iw_pusha, iw_dum = (Widget) 0;
 Widget iw_labu, iw_labd = (Widget) 0;
 char   cval[11];

 if (iw_foumen <= (Widget) 0)
    {   /* fourier filter menu widget does not exist. create it */
    iw_foumen = wid_dialog(iw_win,0,"fourier filter options", -1,-1);

    /* set up rows and columns in the dialog widget */
    iw_rowcol = wid_rowcol(iw_foumen,'v',-1,-1);

    radius     = 5;

    iw_fname   = (Widget) 0;
    iw_radius  = (Widget) 0;
    iw_dum     = (Widget) 0;
   
    /* create a text box for filter file name input */
    strcpy(cval,pwfilnam);
    iw_fname = wid_textboxb(iw_rowcol,iw_fname,"Output Filter File:", cval,20);

    /* create a label box for radius changing info */
    iw_labu  = wid_labelg(iw_rowcol, 0, "Use:   up-arrow to increase radius", -1, -1);
    iw_labd  = wid_labelg(iw_rowcol, 0, "Use: down-arrow to decrease radius", -1, -1);

    /* create a text box for radius input */
    sprintf(cval,"%f",radius);
    iw_radius = wid_textboxb(iw_rowcol,iw_radius,"Mask radius:",cval,11);
                                              
    /* create a push button for stop, cancel & apply */
    iw_dum = wid_stdbut(iw_rowcol,iw_foumen,
                        &iw_pushs,&iw_pushc,&iw_pusha,"SA",
                        foumen_buts, fin_cb, foumen_buta,NULL);
     }
  
 XtManageChild(iw_foumen);
 return;
 }

/*********************** foumen_buts   ******************************/

void foumen_buts(Widget iw_temp, XtPointer data, XtPointer call_data)
  {
  /* remove popup button menu */
  showbutx("","","",TRUE);

  /* cancel menu and stop this routine */
  XtUnmanageChild(iw_foumen);

  /* cancel menu, stop masking mask button assignments */
  XtUninstallTranslations(iw_win);

  /* close filter file (if any) */
  if (filedatat != (FILEDATA *) NULL) closefile(filedatat);

  }
                                
 /*******************************   foumen_buta  *********************/
                                                               
 void foumen_buta(Widget iw_t, XtPointer itag, XtPointer callpar)
 {
 char *     string;

 /* get file name from relevant text widget */
 string = XmTextGetString(iw_fname);
 if (strlen(string) == 0)
    {
    if (string) XtFree(string);
    spout("*** No Filter file name entered");
    return;
    }
 strncpy(pwfilnam,string,80);

 /* probably got a valid file name, add datexc to pwfilnam if not there */
 if (!addext(pwfilnam, datexc, 80)) { return;}

 /* open the output filter file */

 nsamf = (nsam + 2 - nsam%2);
 filedatat = opennew(pwfilnam, nsamf, nrow, 1, 1, "u");

 /* get initial radius for mask */
 string = XmTextGetString(iw_radius);
 sscanf(string,"%f",&radius);
 if (string) XtFree(string);
    
 XtUnmanageChild(iw_foumen);

 /* must start fou program */
 ilocs = 0;
 fou();
               
 return;    
 }
                                  
 /********************************   fou   *************************/

 void fou(void)
    {
    showbutx("Set a mask location",
             "Create fourier filter from masks.",
             "Stop this routine.", FALSE);

    /* set the actions for right, left, and center buttons */
    actions(iw_win, "fou_pop",fou_pop,"123ud");
    }

 /*****************************   fou_pop   *************************/

  void fou_pop(Widget iw_t, XEvent *event, String *params,
                   Cardinal *num_params)
  {
   int    ixc, iyc, ixi, iyi, iradius, ixim, iyim;
   char   outstr[80];                     

    if (!(strcmp(*params,"1")))                /* button 1 pushed */
        {
        /* get mouse position ixc,iyc  */
        getloc(event,'M',&ixc,&iyc);

        /*  find location relative to image */
        ixi = ixc - ixul + 1;
        iyi = iyc - iyul + 1;

        if (ixi < 1 || ixi > nsam || iyi < 1 || iyi > nrow)
            {
            /*  outside of image. can not select this location */
            sprintf(outstr,"Outside row: %d col: %d", ixi,iyi);
            spout(outstr);
            }
        else
            {  /* inside of image. */
            /* draw a permanent circle at this position */
            iradius = radius;
            xorc(iwin,    icontx, TRUE, ixc, iyc, iradius);
            xorc(imagsav, icontx, TRUE, ixc, iyc, iradius);

            /* findmirror location */
            ixim = nsam - ixi + 1;
            iyim = nrow - iyi + 1;

            /* draw a permanent circle at mirrored position */
            xorc(iwin,    icontx, TRUE, ixim + ixul - 1, iyim + iyul - 1, iradius);
            xorc(imagsav, icontx, TRUE, ixim + ixul - 1, iyim + iyul - 1, iradius);

            /* record locations  on right half */
            if (ixi > (nsam / 2))
               { ix[ilocs] = ixi;  iy[ilocs] = iyi; radii[ilocs] = radius; }
            else
               { ix[ilocs] = ixim; iy[ilocs] = iyim; radii[ilocs] = radius; }
            ilocs++;

            if (ilocs >= MAXLOCS)
               { 
               spout("***All mask locations in use");
               foufilt();
               return;
               }
            }
        }

    else if (!(strcmp(*params,"2")))            /* button 2 pushed */
        {
        /* middle button saves filter */
        foufilt();
        }

    else if (!(strcmp(*params,"3")))            /* button 3 pushed */
        {
        /* right button stops the routine */
        foumen_buts(iw_win,(XtPointer)NULL,(XtPointer)NULL);
        }
    else if (!(strcmp(*params,"u")))            /* up-arrow pushed */
        {
        /* increments radius */
        radius = radius + 1.0;

        spouts("Radius: "); spouti((float)radius);
        }
    else if (!(strcmp(*params,"d")))            /* down-arrow pushed */
        {
        /* decrements radius (not to zero) */
        radius = radius - 1.0;
        if (radius <= 0.001) radius = 0.001;
        spouts("Radius: "); spouti((float)radius);
        }
  }

/*************************** foufilt  **************************/

  void foufilt(void)
  {                       
  int     iloc, ixmask, iymask, isam, irow, k, irowm, ipix, isamf;
  float * buf  = (float *) NULL;
  float * fbuf = (float *) NULL;
  float   gausscon, diff2;
  int     iycon;

  /* allocate memory for mask image */
  if ((buf = (float *) malloc(nsam * nrow * sizeof(float))) == 
     (float *) NULL) 
     {spout("*** Unable to malloc buf in foufilt."); return; }

  for (iloc = 0; iloc < nsam * nrow; iloc++)
     {buf[iloc] = 0.0;}

  /* go thru each pixel in right side of mask */
  for (isam = nsam / 2; isam < nsam; isam++)
     {
     for (irow = 0; irow < nrow; irow++)
        {
        iycon = irow * nrow;

        /* for each mask circle location from ilocs */
        for (iloc = 0; iloc < ilocs; iloc++)
           {
           k        = iycon + isam;
           gausscon = -1.0 / (radii[iloc] * radii[iloc]);
           ixmask   = ix[iloc];
           iymask   = iy[iloc];
           diff2    = (isam - ixmask) * (isam - ixmask) + 
                      (irow - iymask) * (irow - iymask);

           buf[k] = buf[k] + exp(gausscon * diff2);
           if (buf[k] > 1.0) buf[k] = 1.0;

           } /* end of: for (iloc .... */
        }    /* end of: for (irow .... */
     }       /* end of: for (isam .... */

#ifdef NEVER
/* ***************************/
  wrtvol(filedatat,buf);
/* ***************************/
#endif

  /* allocate memory for fourier filter image */
  if ((fbuf = (float *) malloc(nsamf * nrow * sizeof(float))) == 
     (float *) NULL) 
     {spout("*** Unable to malloc fbuf in foufilt."); return; }

  /* start at center line in mask */
  irowm = nrow / 2;
  isamf = 0;

  for (irow = 0; irow < nrow; irow++)
     {
     ipix  = irowm * nsam;
     for (isam = nsam / 2 ; isam < nsam; isam++)
        {  /* use right half of mask only */
        fbuf[isamf++] = buf[ipix + isam];
        fbuf[isamf++] = 0.0;
        }

     /* pad ultra high frequencies with zeros */
     fbuf[isamf++] = 0.0;
     if ( nsam%2 == 0) 
        {fbuf[isamf++] = 0.0;}

     /* previous line in mask */
     irowm--;
     if (irowm <= 0) irowm = nrow -1;
     }

  /* write the filter to output file */
  wrtvol(filedatat,fbuf);

  /* adjust iform */
  if (redlin(filedatat,fbuf,nsamf,0))
     {
     fbuf[4] = -11;
     if (nsam%2 == 0) fbuf[4] = -12.0;
     wrtlin(filedatat,&fbuf[0],nsamf,0);
     }

  /* free buffer space */
  if (buf)  free(buf);
  if (fbuf) free(fbuf);

  /* stop this routine */
  foumen_buts(iw_win,(XtPointer) NULL,(XtPointer) NULL);
  }

