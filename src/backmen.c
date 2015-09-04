
/*$Header: /usr8/web/src/RCS/backmen.c,v 1.32 2015/09/01 17:52:41 leith Exp $*/

/*C++*******************************************************************
 C                                                                     *
 C backmen.c                                    Mar  1994 ArDean Leith *
 C               Directcolor mods               Jul  2001 Erik Vogan   *
 C               Truecolor mods                 Sep  2011 ArDean Leith *
 C               Improved                       Aug  2015 ArDean Leith *
 C                                                                     *
 C**********************************************************************
 C * AUTHOR: A. LEITH                                                  *
 C=* FROM: WEB - VISUALIZER FOR SPIDER MODULAR IMAGE PROCESSING SYSTEM *
 C=* Copyright (C) 1992-2015  Health Research Inc.                     *
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
 C**********************************************************************
 C
 C  PARAMETERS:   
 C
 C  PURPOSE:      Sets image contast not using luts
 C
 C  CALLED BY:    fitdocmen -> backmen  -> showimage --> redvol
 C                                             |           wipic
 C                                             |-------------'
 C                                           pickdisp
 C
 C
 C              
 C                backmen_buta -> showimage --> redvol
 C                                    |         wipic
 C                                    |----------'
 C                                pickdisp
 C
 C  NOTE:  Could be speeded up by caching floating point data from file
 C
 C--********************************************************************
*/

#include "common.h"
#include "routines.h"

#include <Xm/Scale.h>

 /* Internal function prototypes */
 void                backmen_buta(Widget, XtPointer, XtPointer);
 void                backmen_butm(Widget, XtPointer, XtPointer);

 /* External function prototypes */
 void                wid_contrast_but(Widget, XtPointer, XtPointer);

 /* Externally defined global variables */
 extern XImage     * imager;
 extern XImage     * imagel;
 extern FILEDATA   * filedatal;
 extern FILEDATA   * filedatar;
 extern char       * cptrduml;   // Data used by right image 
 extern char       * cptrdumr;   // Data used by left  image  
 extern int          vmap[256];  // Virtual colormap

 /* Internal file variables */
 static Widget       iw_backmen = (Widget) 0, iw_ngob, iw_nendb;
 static int          leftside;

 /************************   backmen   ********************************/

 void backmen(int leftsidet)

 { 
 static Widget   iw_rowcolh;
 Widget          iw_rowcolv;
 Widget          iw_dums, iw_pushc, iw_pusha;
 float         * fptrdum = NULL;

 if (iw_backmen == (Widget)0)
    {   /* Create background enhancing menu widget first */

    iw_backmen  = wid_dialog(iw_win, 0, "Contrast options", -1,-1);
    iw_rowcolv  = wid_rowcol(iw_backmen, 'v', -1,-1);

    /* Create label for scales */
    wid_labelg(iw_rowcolv, 0,
               "Lower and upper end of ramp:", -1,-1);

    /* Create a horizontal row-col widget for scales */
    iw_rowcolh = wid_rowcol(iw_rowcolv, 'h', -1,-1);

    /* Create scale widget for starting value ------------------ ngo */
    iw_ngob  = wid_scale(iw_rowcolh, 0, "", 
                         imagego,imageend,  ngo, 
                         180,50, -1,-1);

    /* Create scale widget for ending value -------------------- nend */
    iw_nendb = wid_scale(iw_rowcolh, 0, "", 
                         imagego,imageend,  nend, 
                         180,50, -1,-1);

    XtAddCallback(iw_ngob, XmNvalueChangedCallback, 
                           (XtCallbackProc)backmen_butm,"0");
    XtAddCallback(iw_nendb,XmNvalueChangedCallback, 
                           (XtCallbackProc)backmen_butm,"0");

    /* Create box for apply ----------------------------------- apply */
    iw_dums = wid_stdbut(iw_rowcolv, iw_backmen, 
                        &iw_dums, &iw_pushc, &iw_pusha,  "CA",
                        fin_cb,fin_cb ,backmen_buta, NULL);
    }
 else
    {   /* Enhance menu widget already exists, update it */

    /* Alter scale widget for starting value ------------------- ngo */
    iw_ngob = wid_scale(iw_rowcolh, iw_ngob, "", 
                       imagego,imageend,  ngo, 
                       180,50, -1,-1);

    /* Alter scale widget for ending value  -------------------- nend */
    iw_nendb = wid_scale(iw_rowcolh, iw_nendb, "", 
                       imagego,imageend,  nend, 
                       180,50, -1,-1);
    }

 /* Set wait cursor */
 waitcursor(iwin, TRUE, 79);

 if (leftsidet)
    {
    /* Free up ximage buffer, before creating a new one */
    if (imagel) 
       { XFree((void *)imagel); imagel = NULL; } 

    /* Show left image using original ramp */                 
    nsam2 = nsaml; nrow2 = nrowl;
    showimage(filedatal, TRUE,   &cptrduml, 
                         TRUE,   &imagel,
                         FALSE,  &fptrdum, FALSE,
                         (float) filedatal->fmin_spi, 
                         (float) filedatal->fmax_spi,
                         imagego, imageend);    

    /* Shift left image to shifted location & display */
    pickdisp(TRUE, FALSE);
    }
 else
    {
    /* Show right image using original ramp */                       
    nsam2 = nsamr; nrow2 = nrowr;

    /* Free up ximage buffer, before creating a new one */
    if (imager) 
       { XFree((void *)imager); imager = NULL; } 

    showimage(filedatar, TRUE,   &cptrdumr, 
                         TRUE,   &imager,
                         FALSE,  &fptrdum, FALSE,
                         (float) filedatar->fmin_spi, 
                         (float) filedatar->fmax_spi,   
                         imagego, imageend);    

    /* Shift right image to shifted location and display */      
    pickdisp(FALSE, TRUE);
    }

 leftside = leftsidet;

 /* Cancel waitcursor */
 waitcursor(iwin, FALSE, 79);

 XtManageChild(iw_backmen);

 }


/*********** Move scale  button callback *****************************/

 void backmen_butm(Widget iw_temp, XtPointer data, 
                                   XtPointer call_data)

 {
 int             nindex, ival, iflags, indx;  
 float           fval,ft;
 int             ngo_com, nend_com;  

 /* Get new ngo & nend */
 XmScaleGetValue(iw_ngob,  &ngo_com);
 XmScaleGetValue(iw_nendb, &nend_com);

 if (nend_com < ngo_com)
    { spout("*** Upper value must be > lower value"); return;}
 else if (ngo_com < imagego)                                            
    { spout("*** Lower value outside of image ramp"); return;}
 else if (nend_com > imageend)                                         
    { spout("*** Upper value outside of image ramp"); return;}

 // Use ngo to signal ramp info
 ngo  = ngo_com;
 nend = nend_com;
 
#ifdef WEB_TRUE
// WEB_TRUE ------------------------------------------- WEB_TRUE

 // Uses virtual cmap
 newvmap(ngo,nend,vmap);
 neednewmap = TRUE;

 // Applies virtual cmap
 expos_cb(NULL,NULL,NULL);

#else
// WEB_24 --------------------------------------------- WEB_24


 /* Set the colors in the image portion of the colormap, only */ 

 iflags = DoRed | DoGreen | DoBlue; 
 indx   = 0;
 ft     = 65535.0 / ((float) (nend - ngo));

 for (nindex = mapgo; nindex <= imageend; nindex++)
    {
    colort[indx].pixel = ispicol[nindex];
    colort[indx].flags = iflags;

    if (nindex <= ngo) 
       { ival = 0; }     /* Set this color to black */
    else if (nindex < nend)
       {                 
       /* Interpolate shade */
       fval = (float)(nindex - ngo) * ft;
       ival = fval;
       }
    else 
       { ival = 65535; } /* Set this color to white */

    colort[indx].red     = ival;
    colort[indx].green   = ival;
    colort[indx++].blue  = ival;
    }
 
 /* Put the colors in the desired colormap */
 XStoreColors(idispl, map, colort, indx);

// BOTH --------------------------------------------- BOTH
#endif

 XFlush(idispl);
 return;
 }



 /*********** Accept  button callback *****************************/

 void backmen_buta(Widget iw_temp, XtPointer data, 
                                   XtPointer calldata)

 {
 float      fmint, fmaxt;
 float      *fptrdum = NULL;

 /* Remove menu */
 XtUnmanageChild(iw_backmen);

 /* Set wait cursor */
 waitcursor(iwin, TRUE, 79);

 /* Set location for images */ 
 placed = TRUE;
 if (leftside)
    {
    ixuln  = ixull;  iyuln = iyull;
    nsam2 = nsaml; nrow2 = nrowl;

    /* Find new displayed levels of fmin_spi and fmax_spi for left image */
    fmint = (((float)(ngo - imagego) / (float)(imageend - imagego))) * 
            (filedatal->fmax_spi - filedatal->fmin_spi) + filedatal->fmin_spi;

    fmaxt = (((float)(nend - imagego) / (float)(imageend - imagego))) * 
            (filedatal->fmax_spi - filedatal->fmin_spi)+ filedatal->fmin_spi;
 
    /* Free up ximage buffer, before creating a new one */
    if (imagel) 
       { XFree((void *)imagel); imagel =  NULL;  } 

    /* Show left image using new ramp */
    showimage(filedatal, TRUE,  &cptrduml, 
                         TRUE,  &imagel,
                         FALSE, &fptrdum, FALSE,
                         fmint, fmaxt, imagego, imageend);

    /* Shift left image to shifted location and display */
    pickdisp(TRUE, FALSE);
    }
 else
    {
    /* Set location for right images */ 
    placed = TRUE;
    ixuln  = ixulr;  iyuln = iyulr;
    nsam2  = nsamr;  nrow2 = nrowr;

    /* Find new displayed levels of fmin_spi and fmax_spi for right image */
    fmint = (((float)(ngo - imagego) / (float)(imageend - imagego))) * 
            (filedatar->fmax_spi - filedatar->fmin_spi) + filedatar->fmin_spi;

    fmaxt = (((float)(nend - imagego) / (float)(imageend - imagego))) * 
            (filedatar->fmax_spi - filedatar->fmin_spi) + filedatar->fmin_spi;

    /* Free up ximage buffer, before creating a new one */
    if (imager) 
       {  XFree((void *)imager); imager = NULL;  } 
   
    /* Show right image using new ramp */
    showimage(filedatar, TRUE,  &cptrdumr, 
                         TRUE,  &imager, 
                         FALSE, &fptrdum, FALSE,
                         fmint, fmaxt, imagego, imageend);

    /* Shift right image to shifted location and display */      
    pickdisp(FALSE, TRUE);
    }

  //printf ("%d...%d   File Range: %7.2f ...%7.2f \n",imagego,imageend, 
  //          filedatar->fmin_spi,filedatar->fmax_spi);
  //printf ("%d...%d   Range: %7.2f ...%7.2f \n",ngo,nend, fmint,fmaxt);
 
 /* Apply normal uncontrasted ramp to whole image */
 ngo  = imagego;
 nend = imageend;

 XmScaleSetValue(iw_ngob,  ngo);
 XmScaleSetValue(iw_nendb, nend);

 /* Apply normal uncontrasted ramp to whole screen */
 backmen_butm(NULL,NULL,NULL);

 /* Cancel waitcursor */
 waitcursor(iwin, FALSE, 79);

 return; 
 }



