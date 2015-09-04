
/*$Header: /usr8/web/src/RCS/fitmen.c,v 1.34 2015/09/01 17:53:12 leith Exp $*/

/*
 C**********************************************************************
 C                                                                     *
 C fitmen                                                              *
 C         In: fit_butfit  moved Unmanage        Dec 2004 ArDean Leith *
 C         Improved                              Jun 2011 ArDean Leith *
 C         Particle number selector              Aug 2015 ArDean Leith *
 C                                                                     * 
 C**********************************************************************
 C *  AUTHOR:  ArDean Leith                                            *
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
 C    fitmen
 C
 C    PURPOSE:         Display fitmen menu
 C
 C    CALLED BY:       fitorigin, pickmen, pickp.
 C
 C    CALL TREE:
 C                    | _fit_butfit  --> get text theta
 C                                       willsg
 C                                       spout
 C
 C                    | _fit_buttilt --> get text arealim
 C                                       tiltang
 C                                       fitmen_adv
 C
 C*********************************************************************
*/

#include "common.h"
#include "routines.h"
#include <Xm/Text.h>

 // Internal function prototypes 
 void          fit_buttilt  (Widget, XtPointer, XtPointer);
 void          fit_butfit   (Widget, XtPointer, XtPointer);
 void          fit_butdraw2 (Widget, XtPointer, XtPointer);
 void          fit_butsavang(Widget, XtPointer, XtPointer);
 void          fitmen_butok (Widget, XtPointer, XtPointer);
 void          fitmen_butk  (Widget, XtPointer, XtPointer);

 // Externally defined global variables 
 extern int    maxpart;
 extern float  xu0t,  yu0t,  xs0t,  ys0t;    // From: fitorigin
 extern float  * xim, * xu0, * yu0; 
 extern float  * xs,  * ys, * xs2, * ys2; 
 extern int    fitted;                       // From: pickp
 extern int    orgkey;                       // From: pickmen
 extern int    numm;                         // From: pickp
 extern Widget iw_parlabel;                  // From: pickmen
 extern Widget iw_rowcolh0;                  // From: pickmen
	       
 // Global variables defined here, used here & elsewhere 
 float         phif          = 0.0;
 float         thetaf        = 0.0;
 float         gammaff       = 0.0;
 float         arealim       = 5000;         // Min. triangle area
 Widget        iw_fitmen_adv = (Widget) 0;   // Used in: pickp 

 // File scope variables used here
 static Widget iw_area, iw_orgkey, iw_xorg, iw_yorg;
 static Widget iw_thetaf;
 static Widget iw_partext;


 /****************************  fitmen_adv   ***********************/

 void fitmen_adv(void)

 { 
 static Widget iw_rowcolv, iw_rowcolh1, iw_rowcolh2, iw_rowcolh3;
 Widget        iw_pushs, iw_dums; 

 char   cval[40];
 
 if (iw_fitmen_adv == (Widget)0)
    {   // Create fit menu widget first 

    iw_fitmen_adv  = wid_dialog(iw_win, 0, 
                    "Angle fitting menu", -1, -1);

    iw_rowcolv = wid_rowcol(iw_fitmen_adv, 'v', -1, -1);


    // Create text box for particle key  --------------------- Key number
    sprintf(cval,"%4d",numm);
    iw_partext = wid_textboxb(iw_rowcolv,0,
                            "Next particle number:",cval,4);
    XtAddCallback(iw_partext,XmNvalueChangedCallback,
                           (XtCallbackProc) fitmen_butk,"0");

    // Create horizontal rowcol for origins   */
    iw_rowcolh1  = wid_rowcol(iw_rowcolv, 'h', -1, -1);

    // Create text box for origin key ----------------------  Origin key 
    sprintf(cval,"%4d",orgkey);
    iw_orgkey = wid_textboxb(iw_rowcolh1,0,
                         "Key number for origin:",cval,4);
    //XtAddCallback(iw_orgkey,XmNvalueChangedCallback,
    XtAddCallback(iw_orgkey,XmNactivateCallback,
                           (XtCallbackProc) fitmen_butok,"0");

    // Create horizontal rowcol for origins   */
    iw_rowcolh2  = wid_rowcol(iw_rowcolv, 'h', -1, -1);

    // Create text box for x origin ------------------------- X origin 
    sprintf(cval,"%f",xu0t);
    iw_xorg = wid_textboxb(iw_rowcolh2,0,
                           "X origin:",cval,10);

    // Create text box for y origin ------------------------- Y origin 
    sprintf(cval,"%f",yu0t);
    iw_yorg = wid_textboxb(iw_rowcolh2,0,
                           "Y origin:",cval,10);

    // Create horizontal rowcol for angles   */
    iw_rowcolh3  = wid_rowcol(iw_rowcolv, 'h', -1, -1);

    // Create text box for thetaf ---------------------------- Thetaf 
    sprintf(cval,"%f",thetaf);
    iw_thetaf = wid_textboxb(iw_rowcolh3,0,
                           "Theta:",cval,10);

    // Create text box for arealim --------------------------- Arealim 
    sprintf(cval,"%f",arealim);
    iw_area = wid_textboxb(iw_rowcolv,iw_area,
                           "Min. triangle area:",cval,10);

    // Create push button for tilt angle det. ---------------- Tilt 
    wid_pushg(iw_rowcolv, 0, "Determine theta",
                     fit_buttilt, NULL, -1,-1);

    // Create push button for fitting angles ----------------- Fit 
    wid_pushg(iw_rowcolv, 0, "Fit angles",
                     fit_butfit, NULL, -1,-1);

    // Create push button for draw points -------------------- Draw 
    wid_pushg(iw_rowcolv, 0, "Draw fitted locations",
                     fit_butdraw2, NULL, -1,-1);

    // Create push button for saving angles ------------------ Save ang. 
    wid_pushg(iw_rowcolv, 0, "Save angles",
                     fit_butsavang, NULL, -1,-1);

    // Create box for cancel --------------------------------- Cancel 
    iw_dums = wid_stdbut(iw_rowcolv, iw_fitmen_adv, 
                        &iw_pushs, &iw_dums, &iw_dums,"C",
                        fin_cb,fin_cb ,fin_cb, NULL);
    }

 else
    {
    // Update text box for particle -------------------------  Particle key
    sprintf(cval,"%4d",numm);
    iw_partext = wid_textboxb(iw_rowcolv,iw_partext,
                       "Next particle number:",cval,4);
    // Update label box for particle number
    wid_labelg(iw_rowcolh0,iw_parlabel,cval,-1,-1);

    // Update text box for origin key -------------------------  Origin key 
    sprintf(cval,"%4d",orgkey);
    iw_orgkey = wid_textboxb(iw_rowcolv,iw_orgkey,
                       "Key number for origin:",cval,4);

    // Update text box for x origin -------------------------- X origin 
    sprintf(cval,"%f",xu0t);
    iw_xorg = wid_textboxb(iw_rowcolh2,iw_xorg,
                        "X origin:",cval,10);

    // Update text box for y origin -------------------------- Y origin 
    sprintf(cval,"%f",yu0t);
    iw_yorg = wid_textboxb(iw_rowcolh2,iw_yorg,
                        "Y origin:",cval,10);

    // Update text box for thetaf ---------------------------- Thetaf 
    sprintf(cval,"%f",thetaf);
    iw_thetaf = wid_textboxb(iw_rowcolh3,iw_thetaf,
                       "Theta:",cval,10);

    // Update text box for arealim --------------------------  Arealim 
    sprintf(cval,"%f",arealim);
    iw_area = wid_textboxb(iw_rowcolv,iw_area,
                      "Min triangle area:",cval,10);
    }

 XtManageChild(iw_fitmen_adv);
 }  


 /*********** Particle number changed callback ************************/

 void fitmen_butk(Widget iw_temp, XtPointer data, 
                                   XtPointer calldata)
 {
 char * string = NULL;
 char   cval[40];

 /* Find particle number */
 string = XmTextGetString(iw_partext);
 sscanf(string,"%d",&numm);
 if (string) free(string);

 if (numm < 1) 
    { spout("*** Particle number must be > 0"); return; }

 // Update label box for particle number
 sprintf(cval,"%4d",numm);
 wid_labelg(iw_rowcolh0,iw_parlabel,cval,-1,-1);
 }

/***********  Origin key changed callback ***********************/

 void fitmen_butok(Widget iw_temp, XtPointer data, 
                                   XtPointer call_data)
 {

 char *   string;
 char     outmsg[80];

 /* Retrieve points from doc file */
 spoutfile(TRUE);
 if (fitdoc(FALSE) != 0)
    {
    spout("*** Can not fit angles yet!"); XBell(idispl,50);
    spoutfile(FALSE);
    return;
    }
 spoutfile(FALSE);

 string = XmTextGetString(iw_orgkey);
 sscanf(string,"%d",&orgkey);
 if (string) free(string);

 if (orgkey > maxpart  || orgkey < 1)
    {sprintf(outmsg,"Bad origin key: %d, Max key available: %d", 
             orgkey,maxpart);
    spout(outmsg); XBell(idispl,50);
    return;
    }

 // Set origin location
 xu0t = xu0[orgkey-1];
 yu0t = yu0[orgkey-1];
 xs0t =  xs[orgkey-1];
 ys0t =  ys[orgkey-1];

 // Update text boxes
 XtUnmanageChild(iw_fitmen_adv); 
 fitmen_adv();

 }



/****************  Fitting callback ***********************/

 void fit_butfit(Widget iw_temp, XtPointer data, 
                                 XtPointer calldata)
 {
 char * string;
 char   outmes[80];
 int    iflag;

 /* Query xu0t */
 string = XmTextGetString(iw_xorg);
 sscanf(string,"%f",&xu0t);
 if (string) free(string);

 /* Query yu0t */
 string = XmTextGetString(iw_yorg);
 sscanf(string,"%f",&yu0t);
 if (string) free(string);
    
 /* Query theta */
 string = XmTextGetString(iw_thetaf);
 sscanf(string,"%f",&thetaf);
 if (string) free(string);

 /* Fit tilt angles to the selected points */
 spoutfile(TRUE);

 /* Routine: willsq returns phif, gammaff, & error flag */
 iflag = willsq(xu0, yu0, xs, ys, maxpart, 
                thetaf, &gammaff, &phif);
 if (iflag == 0)
   {   /* Succeeded, fitting is OK */
   fitted = TRUE;
   sprintf(outmes,"Tilt (theta): %5.2f  Gamma:%5.2f Phi:%5.2f  Origin: (%7.2f,%7.2f)",
                  thetaf, gammaff,phif, xs0t,ys0t);
   spout(outmes);
   }
 else
   { XBell(idispl,50); }

 spoutfile(FALSE);

 /* Restart the fitting menu */
 fitmen_adv(); 
 }


/***********  Determine tilt callback *******************************/

 void fit_buttilt(Widget iw_temp,  XtPointer data, 
                                   XtPointer calldata )
 {
 char *  string;
 int     flag;
 int     iarea;

 /* Query arealim */
 string = XmTextGetString(iw_area);
 sscanf(string,"%f",&arealim);
 if (string) free(string);

 spoutfile(TRUE);

 /* Determine theta tilt angle */
 flag = tiltang(xu0,yu0, xs,ys, maxpart, &thetaf, &iarea, arealim, TRUE);
 if (flag > 0)
    {  
    spout("*** Warning, can not calculate tilted angle. Try again");
    XBell(idispl,50); XBell(idispl,50);
    }
 if (flag < 0)
    { /* Some bad locations accepted */ 
    XBell(idispl,50);
    }

 spoutfile(FALSE);

 /* Restart the fitting menu so values of angles will change */
 fitmen_adv(); 
 }


/***********  fit_butdraw2 button callback *************************/

 void fit_butdraw2(Widget iw_temp,  XtPointer data, 
                                    XtPointer calldata )
 {
 if (thetaf == 0.0)
    {
    spout("*** Do not have tilt angle yet.");
    XBell(idispl,50); return;
    }

 if (! fitted)
    {
    spout("*** Do not have fit angles yet.");
    XBell(idispl,50); return;
    }

 /* Calculate fitted positions using  gamma, theta & phi */
 spoutfile(TRUE);
 witran(xu0, yu0, xs2, ys2, maxpart, gammaff, thetaf, phif);
 spoutfile(FALSE);

 /* Draw fitted positions now */
 pickdraw(FALSE, FALSE, TRUE, FALSE, TRUE, maxpart);
 }

/***********  Save angles callback *********************************/

 void fit_butsavang(Widget iw_temp,  XtPointer data, 
                                     XtPointer calldata )
 {
 spoutfile(TRUE);

 fitsav(maxpart);

 spoutfile(FALSE);
 }


