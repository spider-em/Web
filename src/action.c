
/*$Header: /home/dean/Software/web/src/RCS/action.c,v 1.6 2025/03/13 20:22:36 dean Exp $*/
/*
 * action.c
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
 * PURPOSE:	Adds one action to the translation table of iw_t
 *              of form:  <event act_name params>
 *
 * RETURNS:	void
 * 
 * PARAMETERS:  Widget		iw_t
 *		char		*event
 *		char		*act_name
 *		char		*params
 *
 *
 * CALLED BY:   actions
 *
 ***********************************************************************
*/

#include "std.h"
#include "x.h"
#include "routines.h"

extern Display *idispl;

/**********************************************************************/
void action(Widget iw_t, char *event, char *act_name, char *params)
   {
   char tmp_act_name[80];
   char tmp_event[120];

   if (((strlen(act_name)                                 ) >= (size_t) 80) ||
       ((strlen(params) + strlen(act_name)                ) >= (size_t) 80) ||
       ((strlen(params) + strlen(act_name) + strlen(event)) >= (size_t)120))
       { 
       spout("*** Pgm. error:  overlength action in action.c, tell Dean");
       XBell(idispl,50); XBell(idispl,50); XBell(idispl,50); XBell(idispl,50);
       return;
       }

   strcpy(tmp_event, event);
   strcpy(tmp_act_name, act_name);

   XtOverrideTranslations(iw_t, XtParseTranslationTable
                         (strcat(tmp_event,
                                 strcat(tmp_act_name, params))));
   }
