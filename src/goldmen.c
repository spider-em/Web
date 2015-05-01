
/*$Header: /usr8/web/src/RCS/goldmen.c,v 1.18 2011/09/21 13:28:10 leith Exp $*/
/*
C++*********************************************************************
C
C  WID_GOLDMEN.FOR -- CREATED MAY 91
C  goldmen.c          NOV 92
C
C **********************************************************************
C    AUTHOR:  ArDean Leith
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
C
C    GOLDMEN()
C
C    PURPOSE:    Displays gold marker selecting menu
C
C    PARAMETERS: None   
C
C    CALLed BY:  web_com      
C
C--*********************************************************************
*/
#include "common.h"
#include "routines.h"

#include <Xm/ToggleBG.h>

 /* Internal function prototypes */
 void          goldmen_buts  (Widget, XtPointer, XtPointer);
 void          goldmen_butshf(Widget, XtPointer, XtPointer);
 void          goldmen_butm  (Widget, XtPointer, XtPointer);
 void          goldmen_butum (Widget, XtPointer, XtPointer);
 int           goldmen_mark  (void);
 void          goldmen_tgcb(Widget,caddr_t,caddr_t);
  
 /* External common variables */
 extern Widget   iw_dean;
 extern Widget   iw_gold;
 extern FILE *   fpdocg;
 extern char     docnamg[81];
 extern int      markg;
 extern int      iradg;

 /* Internal common variables */
 static Widget iw_goldmen = NULL;
 static Widget iw_num     = NULL;

 /***************************   goldmen   ****************************/

 void goldmen(void)

 {
 Widget  iw_rowcolv, iw_pushs, iw_pushc; 

 if (iw_goldmen == (Widget) 0)
    {   /* Create marker picking menu first */

    iw_goldmen = wid_dialog(iw_win, 0, "Marker picking menu", -1, -1);
    iw_rowcolv = wid_rowcol(iw_goldmen, 'v', -1, -1);

    wid_pushg(iw_rowcolv, 0, "Shift the image",
                  goldmen_butshf, NULL, -1,-1);

    wid_pushg(iw_rowcolv, 0, "Show current markers",
                  goldmen_butm, NULL, -1,-1);

    iw_num = wid_toggleg (iw_rowcolv,0,"Show marker numbers",
                             show_num,goldmen_tgcb,NULL,0,0);

    wid_pushg(iw_rowcolv, 0, "Erase markers",
                  goldmen_butum, NULL, -1,-1);

    /* Create box for stop or cancel  */
    wid_stdbut(iw_rowcolv, iw_goldmen, 
               &iw_pushs, &iw_pushc, &iw_pushc, "SC",
               goldmen_buts, fin_cb ,fin_cb, NULL);
    }
 XtManageChild(iw_goldmen);
 }

/************** Goldmen_tgcb ************************************/

void goldmen_tgcb(Widget iw_temp,caddr_t data,caddr_t call_data)
  {
  if (iw_num)
    show_num = XmToggleButtonGadgetGetState(iw_num);
  }

/***********  Shift button callback  *********************************/

 void goldmen_butshf(Widget iw_temp, XtPointer data, XtPointer call_data)
 {
 /*  Remove message */
 showbutx("","","",TRUE);

 /*  Remove the menu widget */
 XtUnmanageChild(iw_goldmen);

 /*  Cancel buttons */
 XtUninstallTranslations(iw_win);

 /* Check translations setting in shift??? */
 shift(MARKER);
 }

/***********  Markit button callback *********************************/

 void goldmen_butm(Widget iw_temp, XtPointer data, XtPointer call_data)
 {
 float   plist[1];
 int     unused;
 int     openit = TRUE;

/* Close doc file, recover and display markers from doc file */
 goldmen_mark();

 /* Reopen the doc file for output again */
 fpdocg = savdn1(&docnamg[0], datexc, &fpdocg, plist, 0, 
            &openit, TRUE, FALSE);
 }


/***********  goldmen_mark *********************************/

 int goldmen_mark(void)
 {
 char *  string;
 int     ixi, iyi, ix1, iy1, i;
 float * ptr;
 int     maxkeys = 9999, maxreg = 3;
 float   plist[1];
 int     unused;
 float * dbuf = NULL;
 int     markdoc;

 /* Vax does not like file opened twice */
 if (fpdocg) fclose(fpdocg); fpdocg = NULL;

 /* retrieve marker locations from doc file */
 if (unsdal(&docnamg[0], &unused, 0, plist, 0, &dbuf, &markdoc,
           maxkeys, maxreg) > 0) 
    {   /* Problem retrieving doc file, assume it does not exist */
    if (dbuf) free(dbuf); dbuf = NULL; XBell(idispl,50);
    spout("*** Unable to read marker doc. file.");
    return 0;
    }

 //printf(" maxkeys: %d   maxreg: %d   Markdoc: %d \n",maxkeys,maxreg,markdoc);

 /* Add markers to display*/
 ptr = dbuf;

 for (i= 0; i < markdoc; i++)
     {
     if ((*ptr) > 0) 
        {
        /*     This marker is in use now, display it */
        ixi = *(ptr+1);
        iyi = *(ptr+2);

        /*     Find location relative to screen */
        ix1 = ixi + ixul - 1;
        iy1 = iyi + iyul - 1;


        /*    Draw circle at this location if inside image area*/
        if (ix1 >= ixullmin && ix1 < ixullmin+nsam  && 
               iy1 >= iyullmin && iy1 < iyullmin+nrow)
           {
           xorc(iwin,   icontx,TRUE,ix1,iy1,iradg);
           xorc(imagsav,icontx,TRUE,ix1,iy1,iradg);

           /*   Write marker number at this location */
	   if (show_num)
	     {
	       string = itoa(i+1);
	       witext(icontx, string, ix1, iy1, 1, 0, -9, 2, FALSE);
	       free(string);
	     }
           }
        }
    ptr += maxreg;   // Next doc file line

    }
 if (dbuf) free(dbuf); dbuf = NULL; 

 /* Refresh screen  */
 expos_cb(NULL,NULL,NULL);

 return markdoc;
 }

/***********  Erase button callback *********************************/

 void goldmen_butum(Widget iw_temp, XtPointer data, XtPointer call_data)
 {
 int	ixs, iys;

 /* Erase markers by redrawing image from the Ximage stuff */

 /* Find the new location of the image */
 ixs = ixul - ixullmin;
 iys = iyul - iyullmin;

 /* Redraw the image */
 XPutImage(idispl,imagsav,icontx, imagel, nsam1,nrow1,
          ixul,iyul, nsam-ixs, nrow-iys);

 XPutImage(idispl,iwin   ,icontx, imagel, nsam1,nrow1,
          ixul,iyul, nsam-ixs, nrow-iys);

 neednewmap = TRUE;

 /*  Refresh screen  */
 expos_cb(NULL,NULL,NULL);
 }

/*********** Stop button callback **********************************/

 void goldmen_buts(Widget iw_temp, XtPointer data, XtPointer call_data)
 {
 /*  Remove message */
 showbutx("","","",TRUE);

 /*  Remove the menu widget */
 XtUnmanageChild(iw_goldmen);

/*  Remove the menu widget */
 XtUnmanageChild(iw_gold);

 /*  Remove the marker number input widget */
 XtUnmanageChild(iw_dean);

 /*  Cancel buttons,  stop this routine */
 XtUninstallTranslations(iw_win);

 /* Reset the values */
 ixul   = ixulrmin;
 iyul   = iyulrmin;

 /* Leave the Ximage storage alone, it will be emptied on next image */ 
 if (fpdocg) fclose(fpdocg); fpdocg = NULL;
 }

