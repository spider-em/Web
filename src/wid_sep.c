
/*$Header: /usr8/web/new/RCS/wid_sep.c,v 1.2 2005/10/18 16:59:47 leith Exp $*/
/*
 * wid_sep.c
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
 * 
 * 
 * PURPOSE:	creates a separator widget
 *
 * RETURNS:	void
 * 
 * PARAMETERS:	Widget	iw_parent	parent widget
 *
 ***********************************************************************
*/
#include "std.h"
#include "x.h"
#include <Xm/SeparatoG.h>


 /**********************************************************************/

 void wid_sep(Widget iw_parent)
   {
   Widget iw_sep;	/* separator widget */

   iw_sep    = XmCreateSeparatorGadget(iw_parent, "sep", NULL, 0);

   XtManageChild(iw_sep);

   }