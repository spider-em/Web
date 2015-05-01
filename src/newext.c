
/*$Header: /usr8/web/src/RCS/newext.c,v 1.8 2011/06/10 13:52:26 leith Exp $*/

/***********************************************************************
 *
 * newext.c
 *
 ***********************************************************************
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
 ***********************************************************************
 * 
 * PURPOSE:  Change datexc     
 * 
 * PARAMETERS:	Widget 		iw_temp         Pushbutton (not used)
 *		XtPointer       data            Label text string
 *		XtPointer       call_data       Not used 
 *
 ***********************************************************************
*/

#include "common.h"
#include "routines.h"

 /* Internal functions */
 void            newext_cb   (Widget, XtPointer,  XtPointer);

 /* Static file scope variables */
 static char       label_string[] = "Enter file extension";  
 static Widget     iw_text = (Widget) 0;
 static int        ixtext = 0;       /* x coord for text       */
 static int        iytext = 0;       /* y coord for text       */
 

/************************  newext  ***********************************/

 void newext(Widget iw_temp, XtPointer data, XtPointer call_data)
 {

 /* Create extension input box */
 iw_text = wid_text(iw_win, iw_text, label_string, (char *) NULL,
                       *newext_cb, NULL, ixtext, iytext);
 }


 /**********************  newext_cb  *********************************/

 void newext_cb(Widget iw_temp, XtPointer data, XtPointer call_data)
 {
 XmSelectionBoxCallbackStruct * cbs;
 
 // cbs receives the text box contents.
 cbs = (XmSelectionBoxCallbackStruct *) call_data;
 
 // datexc is 3 char long
 strncpy(datexc,extractstr(cbs -> value),3);
 
 }

