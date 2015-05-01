
/*$Header: /usr8/web/new/RCS/wid_seper.c,v 1.1 2011/05/31 12:21:58 leith Exp $*/
/*
C++*********************************************************************
C
C wid_sep.c                                      ArDean Leith  May 2011
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
C  wid_seper
C
C  PARAMETERS:    
C
C  PURPOSE:    Displays seperating label widget
C
C--*********************************************************************
*/

#include "x.h"
#include "std.h"
#include <Xm/LabelG.h>

#define MAX_ARGS     20

 // File scope variables

 // ***********************   wid_seper   ****************************
  
 Widget wid_seper(Widget iw_parent, Widget iw_seper,
                   char * label,  int ix, int iy)

 { 
 int           n;
 XmString      str_label;        /* String label */
 Arg           args[MAX_ARGS];   /* Arg list */

 n = 0;

 // Note: XmStringCreateLtoR handles newline char oK) if this
 // changed to XmStringCreate you will have to solve newline problem 

 str_label = XmStringCreateLtoR(label,XmSTRING_DEFAULT_CHARSET);
 XtSetArg(args[n], XmNlabelString, str_label); n++;

 XtSetArg(args[n], XmNbackground, 0);          n++;

 if (iw_seper <= (Widget) 0)
    {   // Create seper widget first 
 
    if (ix > 0 && iy > 0)
       { // Set label position 
       XtSetArg(args[n], XmNx, (Position) ix); n++;
       XtSetArg(args[n], XmNy, (Position) iy); n++;
       }
    else
       {  // Use default alignment 
       XtSetArg(args[n], XmNalignment, XmALIGNMENT_BEGINNING);  n++;
       }

    iw_seper = XmCreateLabelGadget(iw_parent,"iw_seper",args,n);
    }
 else
    {
    XtSetValues(iw_seper,args,n);
    }

 // Free the String (not doing this may result in a memory leak)
 XmStringFree(str_label);

 XtManageChild(iw_seper);

 return iw_seper;
 }

        
