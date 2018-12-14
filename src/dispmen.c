
/*$Header: /usr16/software/web/src/RCS/dispmen.c,v 1.7 2018/12/07 17:03:31 leith Exp $*/
/*
C++************************************************************************
C
C dispmen -- created Feb 94
C
C **********************************************************************
C *  AUTHOR:  ArDean Leith                                                 *
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
C    dispmen
C
C    PURPOSE:         SET VARIOUS PARAMETERS RELATING TO DISPLAY
C
C    VARIABLES:       
C
C    CALLED BY:       WEB_COM
C      
C***********************************************************************
*/

#include <Xm/ToggleBG.h>
#include "common.h"
#define MAX_ARGS     3
#include "routines.h"


 /* internal function prototypes */
 static void    dispmen_buta(Widget, void *, void *);

 /* common variables */

 /* local file variables */
 static Widget  iw_wantscroll;
 static int     wantscroll = TRUE;
 static Widget  iw_dispmen = (Widget)0;

 /*************************  dispmen  ********************************/

 void dispmen(Widget iw_temp, void * data, void * call_data)
 { 
 Widget   iw_rowcol, iw_dums, iw_dum, iw_pushc, iw_pusha;


 if (iw_dispmen <= (Widget)0)
    {   /* create widget first */

    iw_dispmen = wid_dialog(iw_win,iw_dispmen,
                             "display options",20,20);
    iw_rowcol   = wid_rowcol(iw_dispmen,'v',-1,-1);

    /* create toggle box ------------------------------ wantscroll    */
    iw_wantscroll = wid_togmen(iw_rowcol,0,"Scrolling data window",
                             wantscroll,0,0);

    /* create accept boxes  ----------------------------------- apply */
    iw_pushc = (Widget) 0;
    iw_pusha = (Widget) 0;
    iw_dum   = wid_stdbut(iw_rowcol, iw_dispmen, &iw_dums, &iw_pushc, 
                      &iw_pusha,"CA",fin_cb,fin_cb,dispmen_buta, NULL);
    }

 XtManageChild(iw_dispmen);
 }


 /************ accept button callback *********************************/

 void dispmen_buta(Widget iw_temp, void * data, void * call_data)
 {
 int    n, scrolling;
 Arg      args[MAX_ARGS]; 


 /* find if there has been any change in demand for scrolling widget */
 scrolling    = wantscroll;
 wantscroll   = XmToggleButtonGadgetGetState(iw_wantscroll);

 /* remove the menu widget */
 XtUnmanageChild(iw_dispmen);

 if (wantscroll && !scrolling)
    {  /* replace scroll window */

    XtManageChild(iw_scrollpar);

    n = 0;
    XtSetArg(args[n], XmNbottomWidget, iw_scrollpar);        n++;
    XtSetArg(args[n], XmNbottomAttachment, XmATTACH_WIDGET); n++;
    XtSetValues(iw_win, args, n);  
    }

 else if (!wantscroll && scrolling)
    {  /* remove scroll window   */
    n = 0;
    XtSetArg(args[n], XmNbottomAttachment, XmATTACH_FORM); n++;
    XtSetValues(iw_win, args, n);  
    XtUnmanageChild(iw_scrollpar);
    }

 }


