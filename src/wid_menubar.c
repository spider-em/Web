
/*$Header: /usr16/software/web/src/RCS/wid_menubar.c,v 1.2 2018/12/07 17:03:35 leith Exp $*/
/*
 * wid_menubar.c
 *
 ***********************************************************************
 C=* FROM: WEB - VISUALIZER FOR SPIDER MODULAR IMAGE PROCESSING SYSTEM *
 C=* Copyright (C) 1992-2018  Health Research Inc.                     *
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
 * wid_menubar(iw_parent, iw_menubar)
 * 
 * PURPOSE:    creates a menu bar widget  
 *
 * RETURNS:    iw_menubar     Widget for the menu bar
 * 
 * PARAMETERS: iw_parent      Parent widget
 *             iw_menubar     This widget identifier
 *
 ***********************************************************************
*/


#include <Xm/RowColumn.h>
#include "common.h"
#include "routines.h"

#define MAX_ARGS 3

/***********************  wid_menubar  ************************/

 Widget wid_menubar(Widget iw_parent)
 { 

 Arg           args[MAX_ARGS];   /* Arg list */
 Widget        iw_menubar;
 int           n;                /* Arg count */

 Colormap      mapt;
 int           icolorxtb,icolorxtf;
 
#ifdef NEVER
query_widget(" parent widget",iw_parent);
#endif

n=0;
#ifdef WEB_BGFG
  XtSetArg(args[n], XmNbackground,  icolorxmb); n++;      
#endif

// Create widget 
 iw_menubar = XmCreateMenuBar(iw_parent, "menub", args, n);

 if (iw_menubar == (Widget)0)
    { // Widget failed
    spout("*** wid_menubar cannot create a menu bar widget.");
    return iw_menubar;
    }

 XtManageChild(iw_menubar);

 return iw_menubar;
 }
 
