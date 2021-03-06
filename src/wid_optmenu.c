
/*$Header: /usr16/software/web/src/RCS/wid_optmenu.c,v 1.6 2018/12/07 17:03:35 leith Exp $*/
/*
 * wid_optmenu.c
 *
 ***********************************************************************
 C=* FROM: WEB - VISUALIZER FOR SPIDER MODULAR IMAGE PROCESSING SYSTEM *
 C=* Copyright (C) 1992-2018  Health Research Inc.                     *
 C=* HEALTH RESEARCH INCORPORATED (HRI),                               *   
 C=* Riverview Center, 150 Broadway, Suite 560, Menands, NY 12204.     *
 C=* Email:  spider@health.ny.gov                                      *
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
 * wid_optmenu(Widget iw_parent, char *label)
 * 
 * PURPOSE:    creates option menu
 *
 * RETURNS:    iw_button      widget for the option menu
 * 
 * PARAMETERS: iw_parent      parent
 *             label          label for menu item
 *
 ***********************************************************************
*/

#include <Xm/RowColumn.h>
#include "common.h"
#include "routines.h"

 /************************************************************/

 Widget wid_optmenu(Widget iw_parent, char *label)
 { 
 Arg           args[4];   /* arg list */
 register      int  n;           /* arg count */
 Widget        iw_pull, iw_button;
 XmString      str_temp;

 iw_pull = XmCreatePulldownMenu(iw_parent,"iw_menup",NULL,0);

 n = 0;
#ifdef WEB_BGFGO
    XtSetArg(args[n], XmNbackground,  icolorxmb); n++;
#endif
 XtSetArg(args[n], XmNsubMenuId, iw_pull);        n++;

 str_temp =  XmStringCreate(label, XmSTRING_DEFAULT_CHARSET); 
 XtSetArg(args[n], XmNlabelString, str_temp); n++;

 iw_button = XmCreateOptionMenu(iw_parent,"iw_optmenu",args,n);
 XtManageChild(iw_button);

 /* can free the string now */
 XmStringFree(str_temp);

 return iw_pull;
 }
 
