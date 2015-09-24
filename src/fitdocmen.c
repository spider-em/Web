
/*$Header: /usr8/web/src/RCS/fitdocmen.c,v 1.22 2015/09/18 16:25:21 leith Exp $*/

/*
 C**********************************************************************
 C                                                                     *
 C  fitdocmen.c    New                           May   93 ArDean Leith *
 C                 Improved                      Jun 2011 ArDean Leith *
 C                 Improved                      Jun 2015 ArDean Leith *
 C                 fitdoc call removed           Sep 2015 ArDean Leith *
 C                                                                     *
 C *********************************************************************
 C   AUTHOR:  ArDean Leith                                             *
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
 C  fitdocmen()
 C
 C  PURPOSE:    Inputs doc file name for particle picking and then
 C              returns to particle picker
 C
 C  PARAMETERS: none   
 C
 C  CALLed BY:  pickp      
 C
 C  CALLS:      
 C
 C**********************************************************************
*/

#include "common.h"
#include "routines.h"
#include <Xm/Text.h>

 // Internal subroutine prototypes 
 void           fitdocmen_butc(Widget, XtPointer, XtPointer);
 void           fitdocmen_buta(Widget, XtPointer, XtPointer);

 //  Global variables (see pickp) 
 int            nfd   = -1;            // Doc file number         
 int            iredu = 1;             // Image reduction factor  
 char           dfil1[12],dfil2[12];   // Doc file root names
 char           dfil3[12],dfil4[12];
 char           dfil5[12],dfil6[12];

 // External global variables used here ??
 extern float   phif, thetaf, gammaff; // Defined in: fitmen
 extern int     fitted;                // Defined in: pickp

 // Internal file scope variables 
 static Widget  iw_redu, iw_nfd;
 static Widget  iw_fitdocmen = (Widget)0;
 
 /*********************** fitdocmen ********************************/

 void fitdocmen(Widget iw_temp, XtPointer data, 
                                XtPointer call_data)

 {
 static Widget iw_rowcolv;
 Widget        iw_pushc, iw_pusha, iw_dums; 
 char          cval[8];

 if (iw_fitdocmen == (Widget)0)
    {   
    /*   Create marker doc file menu */

    iw_fitdocmen = wid_dialog(iw_win, 0, 
                   "Particle document file", -1,-1);

    iw_rowcolv = wid_rowcol(iw_fitdocmen, 'v', -1, -1);

    /*   Create text box for doc file number input */
    nfd++;
    sprintf(cval,"%4d",nfd);
    iw_nfd = wid_textboxb(iw_rowcolv,0,
                         "Doc file number:",cval,4);

    /*  Create a text box for reduction factor input */
    iw_redu = wid_textboxb(iw_rowcolv,0,
                     "Image reduction factor:","1",4);

    /* Create box for apply  */
    wid_stdbut(iw_rowcolv, iw_fitdocmen, 
               &iw_pushc, &iw_pushc, &iw_pusha, "CA",
       fitdocmen_butc, fitdocmen_butc ,fitdocmen_buta,NULL);

    }
 else
    {
    /*   Update text box for doc file number input */
    nfd++;
    sprintf(cval,"%4d",nfd);
    iw_nfd = wid_textboxb(iw_rowcolv,iw_nfd,
                     "Doc file number:",cval,4);
    }
 XtManageChild(iw_fitdocmen);
 }

/***********  Docfile accept button callback ***********************/

 void fitdocmen_buta(Widget iw_temp, XtPointer data, 
                                     XtPointer call_data)
 {
 float        fdum;

 /* Get image reduction factor */
 if (!rdprx(iw_redu,'i',&iredu,&fdum)) return;
 if (iredu < 1) 
    {spout("*** Reduction factor must be >= 1 !"); return;}

 /* Get doc file number: nfd */
 if (!rdprx(iw_nfd,'i',&nfd,&fdum)) return;
 if (nfd < 0 || nfd > 999) 
    {spout("*** Doc. file number range 0 ...999 !"); return;}

 // Create output doc file names
 filget("dcu***",dfil1,12,nfd);
 filget("dct***",dfil2,12,nfd);
 filget("dcb***",dfil3,12,nfd);
 filget("duc***",dfil4,12,nfd);
 filget("dtc***",dfil5,12,nfd);
 filget("dft***",dfil6,12,nfd);

 /*  Remove doc file menu */
 XtUnmanageChild(iw_fitdocmen);

 /* Set x-hairs cursor */
 setacursor(76,-1,-1);

 /* Initialize some variables for each run */
 fitted = FALSE;
 thetaf = 0.0;  gammaff = -90.0;  phif = -90.0;

 /* Start picking points now */
 pickp(TRUE);
 }

/*********** Docfile cancel button callback ***********************/

 void fitdocmen_butc(Widget iw_temp, XtPointer data, 
                                     XtPointer call_data)
 {
 /*  Remove the menu widget */
 XtUnmanageChild(iw_fitdocmen);

 /* Refresh screen to redraw images if damaged */
 expos_cb(NULL,NULL,NULL);
 }

