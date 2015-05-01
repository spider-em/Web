/*$Header $*/

/***********************************************************************
 *
 * pop_errmsg.c
 *
 ***********************************************************************
 * author : ArDean Leith
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
 * pop_errmsg()
 * 
 * PURPOSE:    .
 *
 * RETURNS:    
 *
 * PARAMETERS: 
 *
 ***********************************************************************
*/

#include "x.h"
#include <Xm/MessageB.h>

extern  Widget   iw_win;

/****************************** Str2XmString *************************/

XmString Str2XmString( char *string )
    {    
     XmString        motif_string;

     motif_string = XmStringCreateLtoR( string, 
                                XmSTRING_DEFAULT_CHARSET );

     return( motif_string );
     }       

/****************************** RemoveDialButton *********************/

void RemoveDialButton( Widget widget, int which_button )
   {      
   Widget  dead_widget;

   dead_widget = (Widget) XmMessageBoxGetChild( widget, which_button );

   XtUnmanageChild( dead_widget );
   }      
 
/******************************  pop_errmsg_buto *********************/

void pop_errmsg_buto(Widget widget, caddr_t client_data, 
                           XmAnyCallbackStruct * call_data)
   {
   XtRemoveGrab( widget );
   XtUnmanageChild( widget );
   }

/******************************  pop_errmsg *************************/

void pop_errmsg( char message[] )
   {
   Arg             args[2];
   static Widget   widget = (Widget) 0;
   XmString        motif_string;

   motif_string = Str2XmString( message );

   if (widget == 0)
      { 
      widget = (Widget) XmCreateErrorDialog(iw_win, "Error Message",
                                            NULL, 0 );

      RemoveDialButton(widget, XmDIALOG_HELP_BUTTON );
      RemoveDialButton(widget, XmDIALOG_CANCEL_BUTTON );
      XtAddCallback(widget, XmNokCallback, 
                        (XtCallbackProc) pop_errmsg_buto, NULL);
      }

   XtSetArg( args[0], XmNmessageString, motif_string );
   XtSetValues(widget, args, 1);

   XtAddGrab(widget, TRUE, FALSE);
   XtManageChild(widget);

   XmStringFree(motif_string );
   }      

