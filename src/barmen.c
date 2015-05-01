
/*$Header: /usr8/web/src/RCS/barmen.c,v 1.9 2011/07/19 18:35:44 leith Exp $*/
/* 
C **********************************************************************
C *  AUTHOR: A. LEITH                                                  *
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
C    BARMEN
C
C    PARAMETERS:   
C
C    PURPOSE:      Makes super-widget to find bar length
C
C    CALLED BY:    WEB_MAIN
C
C--********************************************************************
*/

#include "common.h"
#include <Xm/Scale.h>
#include <Xm/ToggleBG.h>

#define MAX_ARGS 13

 /* External function prototypes */
 extern Widget wid_labelg   (Widget, Widget, char *, int, int);
 extern Widget wid_dialog   (Widget, Widget, char *, int, int);
 extern void   spout        (char *);
 extern void   fin_cb       (Widget, XtPointer, XtPointer);
 extern Widget wid_toggleg  (Widget, Widget, char *, int, void (*)(),
                                char *, int, int);
 extern Widget wid_pushg    (Widget, Widget, char *, void (*)(), 
                                char *,int,int);

 /* internal function prototypes */
 void          barmen_len_cb(Widget, XtPointer, XtPointer);
 void          bar_fin_cb   (Widget, XtPointer, XtPointer);

 /* Internal common variables */
 static Widget        iw_hun, iw_ones, iw_or;
 static int           vertical = TRUE;

 /* Common variables for external use */
 int                  idx = 50, idy = 0;
 Widget               iw_barmen;


 /********************   barmen   ****************************/

 void barmen(Widget iw_temp, XtPointer data, XtPointer call_data)

 {
 Widget iw_labhun, iw_labones, iw_pushf;
 int    iwid, ihi,inow,n;
 Arg    args[MAX_ARGS];    


 if (iw_barmen <= (Widget)0)
    {   /* Create bar length menu widget first */

    iw_barmen   = wid_dialog(iw_win, 0, "Bar length ", -1, -1);

    /* Create label box for 100 lengths  */
    iw_labhun  = wid_labelg(iw_barmen, 0,"length\n(100's)", 30, 5);

    /* Create scale widget for 100 lengths */
    iwid =  76;
    ihi  = 224;

    inow = idx;
    if (inow <= 0) inow = idy;
   
    n = 0;
#ifdef WEB_BGFGO
    XtSetArg(args[n], XmNbackground,  icolorxmb);    n++;
#endif
 
    XtSetArg(args[n], XmNwidth,         iwid);    n++;                    
    XtSetArg(args[n], XmNheight,        ihi);    n++;                
    XtSetArg(args[n], XmNscaleWidth,    iwid-30);    n++;            
    XtSetArg(args[n], XmNscaleHeight,   ihi-5);     n++;       
    XtSetArg(args[n], XmNminimum,       0);    n++;                
    XtSetArg(args[n], XmNmaximum,       15);    n++;             
    XtSetArg(args[n], XmNdecimalPoints, 0);    n++;            
    XtSetArg(args[n], XmNvalue,         0);    n++;                  
    XtSetArg(args[n], XmNorientation,   XmVERTICAL);    n++;       
    XtSetArg(args[n], XmNshowValue,     TRUE);    n++;           
    XtSetArg(args[n],XmNx,             20);     n++;          
    XtSetArg(args[n],XmNy,             56);    n++;           
 
    iw_hun = XmCreateScale(iw_barmen, "iw_hun", args, n);
    XtAddCallback(iw_hun,XmNvalueChangedCallback, 
                   (XtCallbackProc) barmen_len_cb,"1");
    XtManageChild(iw_hun);


    /* Create label box for 1 lengths  */
    iw_labones  = wid_labelg(iw_barmen, 0,"length\n(units)", 200, 5);

    /* Create scale widget for 1 lengths */   
    n = 0;
#ifdef WEB_BGFGO
    XtSetArg(args[n], XmNbackground,  icolorxmb);    n++;
#endif
    XtSetArg(args[n], XmNwidth,         iwid);    n++;                    
    XtSetArg(args[n], XmNheight,        ihi);     n++;                
    XtSetArg(args[n], XmNscaleWidth,    iwid-30); n++;            
    XtSetArg(args[n], XmNscaleHeight,   ihi-5);   n++;       
    XtSetArg(args[n], XmNminimum,       0);       n++;                
    XtSetArg(args[n], XmNmaximum,       100);     n++;             
    XtSetArg(args[n], XmNdecimalPoints, 0);       n++;            
    XtSetArg(args[n], XmNvalue,         inow);    n++;                  
    XtSetArg(args[n], XmNorientation,   XmVERTICAL);    n++;       
    XtSetArg(args[n], XmNshowValue,     TRUE);    n++;           
    XtSetArg(args[n], XmNx,             180);     n++;          
    XtSetArg(args[n], XmNy,             56);      n++;           

 
    iw_ones = XmCreateScale(iw_barmen, "iw_ones", args, n);
    XtAddCallback(iw_ones,XmNvalueChangedCallback, 
                    (XtCallbackProc) barmen_len_cb,"0");
    XtManageChild(iw_ones);

    /* Create toggle box for bar orientation  */
    iw_or = wid_toggleg(iw_barmen,0,"Vertical bar",vertical,
                        barmen_len_cb,NULL, 44,300);

    /*  Create push button for finished */
    iw_pushf = wid_pushg(iw_barmen, 0," FINISHED ", bar_fin_cb,
                        (char *) iw_barmen,  180,350);
    }

 XtManageChild(iw_barmen);
 }

 /*********** length callback *************************/

 void barmen_len_cb(Widget iw_temp, XtPointer data, XtPointer call_data)

 {
 int     len100, len1;

 XmScaleGetValue(iw_hun,  &len100);
 XmScaleGetValue(iw_ones, &len1);

 vertical = XmToggleButtonGadgetGetState(iw_or);

 if (vertical)
    {
    idx = 0;
    idy = len100 * 100 + len1;
    }
 else
    {
    idx = len100 * 100 + len1;
    idy = 0;
    }

 }

/*********** Finished callback *************************/

 void bar_fin_cb(Widget iw_temp, XtPointer data, XtPointer call_data)

 {
 int     len100, len1;

 XtUnmanageChild((Widget) data);
 
 }

