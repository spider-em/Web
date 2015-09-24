
/*$Header: /usr8/web/src/RCS/pickmen.c,v 1.37 2015/09/22 16:34:15 leith Exp $*/

/*
 C++********************************************************************
 C                                                                     *
 C  pickmen.c   New                              May 93   ArDean Leith *
 C              Improved                         Jun 2011 ArDean Leith *
 C              Improved formatting              Jun 2015 ArDean Leith *
 C              shift(TILT), Cosmetic            Jul 2015 ArDean Leith *
 C              Altered particle deletion        Aug 2015 ArDean Leith *
 C              Improved                         Aug 2015 ArDean Leith *
 C              Stop without saving confirm      Sep 2015 ArDean Leith *
 C              Det_tilt return                  Sep 2015 ArDean Leith *
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
 C  pickmen()
 C
 C  PURPOSE:    Displays particle picking selecting menu
 C
 C  PARAMETERS: None   
 C
 C  CALLED BY:  pickpop 
 C
 C  CALL TREE:
  
 imagemen --> imagemen_cb --> imagemen1 --> showimag --> redvol
                                                            v   
 showimage <-- imagemen1 <-- imagemen_cb <-- imagemen <--  wipic 
    |
  redvol
    |                 | _buta  -->
  wipic               |
    |                 | _butsh --> shift --> pickp
 fitdocmen            |
 fitdocmen_buta       | _butel --> backmen
    |                 |
 pickp                | _butcl --> pickdisp
    |                 |
 fitdoc ->unsdal      | _buter --> backmen
    |
 pickpop -> pickmen --| _butl  --> fitdoc --> unsdal 
                      |              v          
                      |            pickdraw                  
                      |              ^                       pickbackmen 
                      | _butn  --> fitdoc ---> unsdal        rcaver 
                      |                                      |
                      |                                      _pop
                      |                                      |
                      | _butb  --> pickbackmen --> _buts --> pickbacksave
                      |              |
                      |              `-----------> _buta --> pickback
                      |
                      | _butdet   --> fitdoc
                      |               tiltang 
                      |               willsq 
                      |               labelg
                      |               spout
                      |
                      |_fitbutdraw --> fitdoc
                      |                witran
                      |                pickdraw -> fitmen 
                      |
                      |_fitbutsavang --> fitsav
                      |
                      |_fitmenadv -->

 C*********************************************************************
*/

#include "common.h"
#include "routines.h"

#include <Xm/ToggleBG.h>
#include <Xm/Text.h>

 // Internal function prototypes 
 void    pickmen_buts  (Widget, XtPointer, XtPointer);
 void    pickmen_butm  (Widget, XtPointer, XtPointer);
 void    pickmen_butsh (Widget, XtPointer, XtPointer);
 void    pickmen_butl  (Widget, XtPointer, XtPointer);
 void    pickmen_butn  (Widget, XtPointer, XtPointer);
 void    pickmen_butcl (Widget, XtPointer, XtPointer);
 void    pickmen_butel (Widget, XtPointer, XtPointer);
 void    pickmen_buter (Widget, XtPointer, XtPointer);
 void    pickmen_butb  (Widget, XtPointer, XtPointer);
 void    pickmen_butk  (Widget, XtPointer, XtPointer);
 void    pickmen_butdet(Widget, XtPointer, XtPointer);

 void    fit_butdraw2  (Widget, XtPointer, XtPointer);
 void    fit_butsavang (Widget, XtPointer, XtPointer);

 int     det_tilt      (int wantmsg, int wantlabel);
 void    pickmen_butst (Widget, XtPointer, XtPointer);
 void    pickmen_butct (Widget, XtPointer, XtPointer);
 void    pickmen_butat (Widget, XtPointer, XtPointer);
  
 
 // Externally defined global variables 
 extern FILEDATA *  filedatal;               // From: imagemen
 extern FILEDATA *  filedatar;               // From: imagemen
 extern int         maxpart;                 // From: fitdoc
 extern int         numm;                    // From: pickp
 extern float *     xim;                     // From: fitdoc
 extern float *     xu0;                     // From: fitdoc
 extern float *     yu0;                     // From: fitdoc
 extern float *     xs;                      // From: fitdoc
 extern float *     ys;                      // From: fitdoc
 extern float *     xs2;                     // From: fitdoc
 extern float *     ys2;                     // From: fitdoc
 extern float       phif, thetaf, gammaff;   // From: fitmen
 extern float       arealim;                 // From: fitmen
 extern int         fitted;                  // From: pickp
 extern Widget      iw_but_lefrit;           // From: pickp

 // Internally defined global variables 
 float              xu0t, yu0t, xs0t, ys0t;
 int                orgkey       = 1;
 Widget             iw_rowcolh0  = (Widget) 0;
 Widget             iw_parlabel  = (Widget) 0;
 Widget             iw_pickmen   = (Widget) 0;   // Used in: pickp_pop
 
 // Internal file scope variables 
 static Widget      iw_rowcolh;
 static Widget      iw_the, iw_phi, iw_gam;
 static Widget      iw_pickmen_stop = (Widget) 0;  

 /***********************   pickmen   ********************************/

 void pickmen(void)

 {
 Widget  iw_pushs, iw_pushc, iw_pusha; 
 Widget  iw_rowcolv;

 char    cval[10];

 if (iw_pickmen == (Widget) 0)
    {   // Create picking menu 

    iw_pickmen = wid_dialog(iw_win, 0, 
                         "Particle picking menu", -1, -1);

    iw_rowcolv = wid_rowcol(iw_pickmen, 'v', -1, -1);

    // Create label boxes for particle key ------------------- Particle number
    iw_rowcolh0 = wid_rowcol(iw_rowcolv, 'h', -1, -1);
    wid_labelg(iw_rowcolh0,0,"Next particle number:",-1,-1);
    sprintf(cval,"%4d",numm);
    iw_parlabel = wid_labelg(iw_rowcolh0,0,cval,-1,-1);

    // Create label boxes for angles ------------------------- Angles 
    iw_rowcolh = wid_rowcol(iw_rowcolv, 'h', -1, -1);

    wid_labelg(iw_rowcolh,0,"Tilt:",-1,-1);
    sprintf(cval,"%-.2f",thetaf);
    iw_the = wid_labelg(iw_rowcolh,0,cval,-1,-1);

    wid_labelg(iw_rowcolh,0,"Axis ang:",-1,-1);
    sprintf(cval,"%-.2f",phif);
    iw_phi = wid_labelg(iw_rowcolh,0,cval,-1,-1);
    sprintf(cval,"%-.2f",gammaff);
    iw_gam = wid_labelg(iw_rowcolh,0,cval,-1,-1);

   //Create pushbutton for fitting angles -------------------- Redetermine angles
    wid_pushg(iw_rowcolv, 0, "Redetermine tilt & axis angles",
                  pickmen_butdet, NULL, -1,-1);

    // Create push button for saving angles ------------------ Save angles
    wid_pushg(iw_rowcolv, 0, "Save angles in angles file",
                     fit_butsavang, NULL, -1,-1);

    // Create pushbutton for shifting image ------------------ Shift images
    wid_pushg(iw_rowcolv, 0, "Shift images",
                  pickmen_butsh, NULL, -1,-1);

    // Create pushbutton for enhancing left image ------------ Enhance left
    wid_pushg(iw_rowcolv, 0, "Enhance left image",
                  pickmen_butel, NULL, -1,-1);

    // Create pushbutton for enhancing right image ----------- Enhance right
    wid_pushg(iw_rowcolv, 0, "Enhance right image",
                  pickmen_buter, NULL, -1,-1);

    // Create pushbutton for backgrounding images ------------ Background
    wid_pushg(iw_rowcolv, 0, "Pick background windows",
                  pickmen_butb, NULL, -1,-1);

    // Create pushbutton for erasing locations/numbers ------- Erase
    wid_pushg(iw_rowcolv, 0, "Erase all labels",
                  pickmen_butcl, NULL, -1,-1);

    // Create pushbutton for showing locations --------------- Show locations
    wid_pushg(iw_rowcolv, 0, "Show particle locations",
                  pickmen_butl, NULL, -1,-1);

    // Create pushbutton for showing numbers ----------------- Show numbers
    wid_pushg(iw_rowcolv, 0, "Show particle numbers",
                  pickmen_butn, NULL, -1,-1);

    // Create push button for draw points -------------------- Draw fitted
    wid_pushg(iw_rowcolv, 0, "Draw fitted locations",
                     fit_butdraw2, NULL, -1,-1);

    // Create push button for advanced ----------------------- Advanced 
    wid_pushg(iw_rowcolv, 0, "Expert menu",
                     fitmen_adv, NULL, -1,-1);

    // Create box for stop  ---------------------------------- Stop
    wid_stdbut(iw_rowcolv, iw_pickmen, 
               &iw_pushs, &iw_pushc, &iw_pusha, "S",
               pickmen_buts, fin_cb ,fin_cb, NULL);
    }

 else
    {
    // Update label box for particle ------------------------- Particle number
    sprintf(cval,"%4d",numm);
    wid_labelg(iw_rowcolh,iw_parlabel,cval,-1,-1);

    // Update label boxes for angles ------------------------- Angles 
    sprintf(cval,"%-.2f",thetaf);
    wid_labelg(iw_rowcolh,iw_the,cval,-1,-1);

    sprintf(cval,"%-.2f",phif);
    wid_labelg(iw_rowcolh,iw_phi,cval,-1,-1);

    sprintf(cval,"%-.2f",gammaff);
    wid_labelg(iw_rowcolh,iw_gam,cval,-1,-1);
    }

 XtManageChild(iw_pickmen);
 }

 /************ Erase button callback **********************************/

 void pickmen_butcl(Widget iw_temp, XtPointer data, 
                                    XtPointer calldata)
 {
 /* Redisplay images at current shifted location */
 pickdisp(TRUE,TRUE);
 }

 /************ Shift button callback **********************************/

 void pickmen_butsh(Widget iw_temp, XtPointer data, 
                                    XtPointer calldata)
 {
 /*  Remove message */
 XtUnmanageChild(iw_but_lefrit);

 /*  Remove the menu widget */
 XtUnmanageChild(iw_pickmen);

 /*  Cancel buttons */
 XtUninstallTranslations(iw_win);

 /* Shift image using mouse for input */
 shift(TILT);
 }


 /************ Background window picking callback **********************/

 void pickmen_butb(Widget iw_temp, XtPointer data, 
                                   XtPointer calldata)
 {
 // Pick background windows using mouse 
 if ( !fitted)
    {
    spout("*** Need to determine tilt angles first!!");
    XBell(idispl,50); XBell(idispl,50);
    return;
    }

 /*  Remove button assignment messages */
 XtUnmanageChild(iw_but_lefrit);

 /*  Remove the picking menu widget */
 XtUnmanageChild(iw_pickmen);

 /*  Cancel buttons */
 XtUninstallTranslations(iw_win);

 /* Display menu for picking background windows using mouse */
 pickbackmen();
 }

/*************  Draw particle callback *********************************/

 void pickmen_butn(Widget iw_temp, XtPointer data, 
                                  XtPointer calldata)
 {
 /* Draw particle numbers at their location */
 pickdraw(TRUE, TRUE, FALSE, TRUE, FALSE, maxpart);
 }


/***********  Draw locations callback *********************************/

 void pickmen_butl(Widget iw_temp, XtPointer data, 
                                   XtPointer alldata)
 {
 // Draw locations on both sides
 pickdraw(TRUE, TRUE, TRUE, FALSE, FALSE, maxpart);
 }

/*************  Enhance left callback *********************************/

 void pickmen_butel(Widget iw_temp, XtPointer data, 
                                    XtPointer calldata)
 {
 backmen(TRUE);
 }

/*************  Enhance right callback *********************************/

 void pickmen_buter(Widget iw_temp, XtPointer data, 
                                    XtPointer calldata)
 {
 backmen(FALSE);
 }


/****************  Determine angles callback ***********************/

 void pickmen_butdet(Widget iw_temp, XtPointer data, 
                                     XtPointer calldata)
 {
 char   outmes[80];
 char   cval[10];
 int    iflagt, iflagw;
 int    iarea;

 spoutfile(TRUE);

 /* Retrieve tilted and untilted points */
 fitdoc(TRUE,FALSE,FALSE,FALSE);

 /* Determine theta tilt angle */
 iflagt = tiltang(xu0,yu0, xs,ys, maxpart, &thetaf, &iarea, arealim, TRUE);
 if (iflagt > 0)
    {  
    spout("*** Warning, can not calculate tilted angle. Try again");
    XBell(idispl,50); XBell(idispl,50);
    }
 if (iflagt < 0)
    { /* Some bad locations accepted */  XBell(idispl,50); }

 // Get origin location using present orgkey
 xu0t = xu0[orgkey-1];
 yu0t = yu0[orgkey-1];
 xs0t =  xs[orgkey-1];
 ys0t =  ys[orgkey-1];

 // willsq reads:   xu0, yu0, xs, ys, thetaf, & maxpart,  xu0t, yu0t, xs0t, ys0t,  and 
 //        returns: phif, gammaff & error flag 
 iflagw = willsq(xu0, yu0, xs, ys, maxpart, 
                thetaf, &gammaff, &phif);
 if (iflagw == 0)
   {   /* Succeeded, fitting is OK */
   fitted = TRUE;
   sprintf(outmes,"Tilt (theta): %5.2f   Gamma: %5.2f   Phi: %5.2f  Origin: (%7.2f,%7.2f)",
            thetaf,gammaff,phif, xs0t,ys0t);
   spout(outmes);
   }
 else
   { XBell(idispl,50); }

 spoutfile(FALSE);
    
 // Update label box for angles 
 sprintf(cval,"%-.2f",thetaf);
 wid_labelg(NULL,iw_the,cval,0,0);
 sprintf(cval,"%-.2f",phif);
 wid_labelg(NULL,iw_phi,cval,0,0);
 sprintf(cval,"%-.2f",gammaff);
 wid_labelg(NULL,iw_gam,cval,0,0);
 }


 /************* Stop button callback **********************************/

 void pickmen_buts(Widget iw_temp, XtPointer data, 
                                   XtPointer calldata )
 {
 Widget  iw_pushst, iw_pushct, iw_pushat; 
 Widget  iw_rowcolv;
 Widget  iw_label;

 if (iw_pickmen_stop == (Widget) 0)
    {   // Create confirmation menu 

    iw_pickmen_stop = wid_dialog(iw_win, 0, 
                         "End Particle picking", -1, -1);
    iw_rowcolv = wid_rowcol(iw_pickmen_stop, 'v', -1, -1);


    // Create label for box saving fit angles query
    iw_label = wid_labelg(iw_rowcolv, 0, "Save fit angles before stopping?", -1, -1);

    /* Create box for stop & start  */
    wid_stdbut_str(iw_rowcolv, iw_pickmen_stop, 
                   &iw_pushst, &iw_pushct, &iw_pushat, "SCA",
                   "SAVE ANGLES THEN STOP",
                   "CANCEL", 
                   "STOP WITHOUT SAVING ANGLES",
                   pickmen_butat, pickmen_butst, pickmen_butst, NULL);
    }

 XtManageChild(iw_pickmen_stop);
 }

 
 /************* Save angles then stop callback **********************************/

 void pickmen_butat(Widget iw_temp, XtPointer data, 
                                    XtPointer calldata )
 {
 // Save fit angles
 fit_butsavang(NULL,NULL,NULL);

 // Remove menu and clean up using final stop callback
 pickmen_butst(NULL,NULL,NULL);
 }

 /************* Cancel stop callback **********************************/

 void pickmen_butct(Widget iw_temp, XtPointer data, 
                                    XtPointer calldata )
 {
 //  Remove the confirmation widget and continue as before
 XtUnmanageChild(iw_pickmen_stop);
 }

 /************* Final stop button callback **********************************/

 void pickmen_butst(Widget iw_temp, XtPointer data, 
                                    XtPointer calldata )
 {
 //  Remove the confirmation widget
 XtUnmanageChild(iw_pickmen_stop);

/*  Remove picking button message */                                 
 XtUnmanageChild(iw_but_lefrit);                       

 /*  Remove the pickmen menu widget */
 XtUnmanageChild(iw_pickmen); 

 /*  Cancel buttons for this routine */
 XtUninstallTranslations(iw_win);

 /*  Restore default cursor */
 setacursor(0,-1,-1);

 /* Deallocate array storage */ 
 if (xim  != (float *)NULL) {free(xim);  xim = (float *) NULL;}
 if (xu0  != (float *)NULL) {free(xu0);  xu0 = (float *) NULL;}
 if (yu0  != (float *)NULL) {free(yu0);  yu0 = (float *) NULL;}
 if (xs   != (float *)NULL) {free(xs);    xs = (float *) NULL;}
 if (ys   != (float *)NULL) {free(ys);    ys = (float *) NULL;}
 if (xs2  != (float *)NULL) {free(xs2);  xs2 = (float *) NULL;}
 if (ys2  != (float *)NULL) {free(ys2);  ys2 = (float *) NULL;}

 /* Stop recording output in results file */
 spoutfile(FALSE);

 closefile(filedatal); filedatal = NULL;
 closefile(filedatar); filedatar = NULL;
 }


/****************  Find tilt angle ***********************/

 int det_tilt(int wantmsg, int wantlabel)
 {
 char   outmes[80];
 char   cval[10];
 int    iflag;
 int    iarea;

 /* Retrieve current locations & orgkey from doc files */
 fitdoc(TRUE,TRUE,FALSE,FALSE);

 /* Determine theta tilt angle */

 iflag = tiltang(xu0,yu0, xs,ys, maxpart, &thetaf, &iarea, arealim, FALSE);
 if (iflag > 0)
    {  
    spout("*** Warning, can not calculate tilted angle. Try again");
    XBell(idispl,50); XBell(idispl,50);
    }
 if (iflag < 0)
    { /* Some bad locations accepted */  
    XBell(idispl,50); 
    }

 // Get origin location using present orgkey
 xu0t = xu0[orgkey-1];
 yu0t = yu0[orgkey-1];
 xs0t =  xs[orgkey-1];
 ys0t =  ys[orgkey-1];

 /* Determine axis angle */
 // willsq reads:   xu0, yu0, xs, ys, thetaf, & maxpart,  xu0t, yu0t, xs0t, ys0t   
 //        returns: phif, gammaff & error iflag 

 iflag = willsq(xu0, yu0, xs, ys, maxpart, 
                thetaf, &gammaff, &phif);
 if (iflag == 0)
   {   /* Succeeded, fitting is OK */
   fitted = TRUE;

   if (wantmsg)
      {   // Always FALSE currently
      sprintf(outmes,"Tilt (theta): %5.2f   Gamma: %5.2f   Phi: %5.2f",
                     thetaf,gammaff,phif);
      spout(outmes);
      }
   }
 else
   { XBell(idispl,50); }
    
 if (wantlabel && iw_the != NULL)
   {    // Update angles label box 
   sprintf(cval,"%-.2f",thetaf);
   wid_labelg(NULL,iw_the,cval,0,0);
   sprintf(cval,"%-.2f",phif);
   wid_labelg(NULL,iw_phi,cval,0,0);
   sprintf(cval,"%-.2f",gammaff);
   wid_labelg(NULL,iw_gam,cval,0,0);
   }
 }


