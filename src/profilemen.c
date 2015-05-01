/*$Header: /usr8/web/src/RCS/profilemen.c,v 1.9 2005/10/18 16:59:44 leith Exp $*/
/*
*****************************************************************************
*  profilemen.c         6/16/93
*
*  wid_profilemen.for (in VAX) ported to UNIX
*
C **********************************************************************
* *  AUTHOR:  Mahieddine Ladjadj                                           *
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
*    PROFILEMEN()
*
*    PURPOSE:
*
*    PARAMETERS:
*
*
*
*****************************************************************************
*/

#include "common.h"
#include "routines.h"

#include <Xm/ToggleBG.h>

 /* internal functions */
 void profilemen_buts (Widget, caddr_t, caddr_t);
 void profilemen_buta (Widget, caddr_t, caddr_t);
 void specialline     (Widget, caddr_t, caddr_t);
 void spline_cb       (Widget, caddr_t, caddr_t);

 /* external common variables */
 extern int   first, horizbar, leavebar, normbar, permbar;
 extern int   spline;			/* special line flag */
 extern int   linenum;			/* the line number of the special line */
 extern GC icontxm;
 extern void  profile_pop(Widget, XEvent *, String *, Cardinal *);

 /* common variables */
 Widget iw_profilemen = (Widget) 0;
 Widget iw_rowcol, iw_horizbar, iw_leavebar, iw_permbar, iw_normbar;
 Widget iw_spline;			/* the widget for special line picking */
 Widget	iw_message;			/* the note message widget */

/************************   profilemen   ******************************/

void profilemen(void)
{
   Widget iw_pushc, iw_pusha, iw_pushs, iw_dum = (Widget) 0;

   if (iw_profilemen <= (Widget) 0)
     {
       /* create a dialog box */
       iw_profilemen = wid_dialog(iw_win,0,"profile_options",-1,-1);

       /* profile menu widget */
       iw_rowcol = wid_rowcol(iw_profilemen,'v',-1,-1);
  
       /* create a toggle box for horizbar */
       iw_horizbar = wid_toggleg(iw_rowcol,0,"horizontal",
                    horizbar,toggle2_cb,(caddr_t) &horizbar,-1,-1);

       /* create a toggle box for leavebar */
       iw_leavebar = wid_toggleg(iw_rowcol,0,"leave bar",
                    leavebar,toggle2_cb,(caddr_t) &leavebar,-1,-1);

       /* create a toggle box for permbar */
       iw_permbar = wid_toggleg(iw_rowcol,0,"permanent profile",
                    permbar,toggle2_cb,(caddr_t) &permbar,-1,-1);

       /* create a toggle box for normbar */
       iw_normbar = wid_toggleg(iw_rowcol,0,"normalize along bar only",
                    normbar,toggle2_cb,(caddr_t) &normbar,-1,-1); 

       /* create a toggle box for normbar */
       iw_spline = wid_toggleg(iw_rowcol,0,"Special line",
                    spline,specialline,(caddr_t) &spline,-1,-1); 

       /* create a push button for stop, cancel & apply */
       iw_dum = wid_stdbut(iw_rowcol,iw_profilemen,
                           &iw_pushs,&iw_pushc,&iw_pusha,"SA",
                           profilemen_buts,fin_cb,profilemen_buta,
                           NULL);
   }  
   XtManageChild(iw_profilemen);

   return;
 }

/*********************** specialline   ********************/

void specialline(Widget iw_temp, caddr_t data, caddr_t call_data)
{
    Widget	iw_linenum;

    spline  = XmToggleButtonGadgetGetState(iw_spline);

    /* draw the profile graph for the special line */
    if (spline && nsam > 1 && nrow > 1) {

   	/* create text input box */
    	iw_linenum = wid_text(iw_win, 0, 
	        "Line number:", 
		(char *) NULL, *spline_cb, NULL, 0, 0);
	}
}

/*********************** spline_cb   ********************/

void spline_cb(Widget iw_temp, caddr_t data, caddr_t call_data)
 {
 XmString    txt_string;
 char        *text;

 XmSelectionBoxCallbackStruct *cbs =
      (XmSelectionBoxCallbackStruct *) call_data;
 
 txt_string = cbs -> value;
 text = extractstr(cbs -> value);
 linenum = atoi(text);

 iw_message = wid_mess(iw_win, 0, 
    "Note: Click the left button\n      to draw the graph\n      after ACCEPT.", 
     -1, -1);
}		

/*********************** profilemen_buta   ********************/

void profilemen_buta(Widget iw_temp, caddr_t data, caddr_t call_data)
  {
     /* get  toggle states */
     normbar  = XmToggleButtonGadgetGetState(iw_normbar);
     permbar  = XmToggleButtonGadgetGetState(iw_permbar);
     horizbar = XmToggleButtonGadgetGetState(iw_horizbar);
     leavebar = XmToggleButtonGadgetGetState(iw_leavebar);

     /* cancel menu and stop this routine */
     if (iw_profilemen != NULL)
         XtUnmanageChild(iw_profilemen);

     /* remove message window */
     if (iw_message != (Widget)0)
         XtUnmanageChild(iw_message);
  }

/*********************** profilemen_buts   ********************/

void profilemen_buts(Widget iw_temp, caddr_t data, caddr_t call_data)
  {
     /* remove popup button menu */
     showbutx("","","",TRUE);

     /* cancel menu and stop this routine */
     if (iw_profilemen != NULL)
       XtUnmanageChild(iw_profilemen);

     /* stop masking mask button assignments */
     XtUninstallTranslations(iw_win);

     /* close image file */
     closefile(filedata);

     /* release GC created for profile.c */
     XFreeGC(idispl, icontxm);
  }
