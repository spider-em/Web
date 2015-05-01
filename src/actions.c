
/*$Header: /usr8/web/src/RCS/actions.c,v 1.6 2005/10/18 16:59:53 leith Exp $*/
/*
 * actions.c
 *
 **********************************************************************
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
 **********************************************************************
 *
 * 
 * 
 * PURPOSE:     adds actions to iw_t's translation table for remaping
 *		the specified mouse buttons
 *
 * RETURNS:
 * 
 * PARAMETERS:	iw_t		widget whose translation table is
 *				to be modified
 * 		keys		string consisting of:
 *				   1 = left mouse button click
 *                                 2 = center mouse button click
 *                                 3 = right mouse button click
 *
 **********************************************************************
*/

#include "std.h"
#include "x.h"


extern XtAppContext app_context;        /* application context   */

/* function prototypes */
extern void action  (Widget, char *, char *, char *);

/************************  actions ***********************************/

void actions(Widget iw_t, char *act_name, void (*act)(), char *keys)
{
   char *compare;              /* string used in comparisons */

   struct XtActionsRec         /* action list                */
      {
      String       string;    
      XtActionProc act_prop;
      } acts[1]; 

   /* setup action list */
   acts[0].string   = act_name;
   acts[0].act_prop = act;

   /* remove all previous translations from iw_t */
   XtUninstallTranslations(iw_t);

   /* add new actions to context */
   XtAppAddActions(app_context, (XtActionList) acts, XtNumber(acts));

   /* set up action on mouse movement */
   compare = strchr(keys, 'M');
   if (compare != NULL)
      {
      action(iw_t, "<MotionNotify> : ", act_name, "(M)");
      }

   /* set up action on button 1 down (left button) */
   compare = strchr(keys, '1');
   if (compare != NULL) 
      {
      action(iw_t, "<Btn1Down> : ", act_name, "(1)");
      }

   /* set up action on button 2 (middle button) */
   compare = strchr(keys, '2');
   if (compare != NULL) 
      {
      action(iw_t, "<Btn2Down> : ", act_name, "(2)");
      }

   /* set up action on button 3 (right button) */
   compare = strchr(keys, '3');
   if (compare != NULL) 
      {
      action(iw_t, "<Btn3Down> : ", act_name, "(3)");
      }

   /* set up action on button 1 up (left button) */
   compare = strchr(keys, 'L');
   if (compare != NULL) 
      {
      action(iw_t, "<Btn1Up> : ", act_name, "(L)");
      }

   /* set up action on key up-arrow  */
   compare = strchr(keys, 'u');
   if (compare != NULL) 
      {
      action(iw_t, "<Key>Up : ", act_name, "(u)");
      }

   /* set up action on key down-arrow  */
   compare = strchr(keys, 'd');
   if (compare != NULL) 
      {
      action(iw_t, "<Key>Down : ", act_name, "(d)");
      }

}
