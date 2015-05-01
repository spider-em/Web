
/*$Header: /usr8/web/src/RCS/wid_pulldown.c,v 1.6 2011/07/20 13:45:58 leith Exp $*/
/*
 * wid_pulldown.c
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
 * wid_pulldown(Widget iw_parent, char *label, int quick)
 * 
 * PURPOSE:    Creates pull-down menu
 *
 * RETURNS:    iw_button      Widget for the pulldown menu
 * 
 * PARAMETERS: iw_parent      Parent
 *             label          Label for menu item
 *             quick          Unused
 *
 ***********************************************************************
*/

#include <Xm/CascadeBG.h>
#include <Xm/CascadeB.h>
#include <Xm/RowColumn.h>
#include "common.h"

#define MAX_ARGS 3

 /********************* wid_pulldown *************************/

 Widget wid_pulldown(Widget iw_parent, char *label, int quick)
 { 
 Arg           args[MAX_ARGS];     /* Arg list */
 int           n;                  /* Arg count */
 Widget        iw_pull, iw_button;
 XmString      str_temp;
 Colormap      mapt;

 int           icolorxtf,icolorxtb;

 n = 0;

#ifdef WEB_BGFG
 XtSetArg(args[n], XmNbackground,  icolorxmb); n++;      
#endif

 // Create pulldown widget
 iw_pull = XmCreatePulldownMenu(iw_parent,"iw_menup",args,n);

 XtSetArg(args[n], XmNsubMenuId, iw_pull); n++;

 str_temp =  XmStringCreate(label, XmSTRING_DEFAULT_CHARSET); 
 XtSetArg(args[n], XmNlabelString, str_temp); n++;

 // Create cascade button widget
 iw_button = XmCreateCascadeButtonGadget(iw_parent,"iw_cascade",args,n);
 XtManageChild(iw_button);

 // Can free the string now 
 XmStringFree(str_temp);

 return iw_pull;
 }

//  ----- Not implemented yet ------------------------
 // XtSetArg(args[n], XmNmnemonic , quick); n++;

 
