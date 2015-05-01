
/*$Header: /usr8/web/new/RCS/widpos.c,v 1.3 2005/10/18 16:59:40 leith Exp $*/
/*
 * widpos.c
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
 * widpos(ix *,iy *)
 * 
 * PURPOSE:	finds position for widget from position in draw window
 *
 * RETURNS:	void
 * 
 * PARAMETERS:  int *	ix		x coord of ulh corner
 *		int *	iy		y coord of ulh corner
 *
 ***********************************************************************
*/

#include "x.h"
#include "std.h"
#include "common.h"

 /***************************** widpos *****************************/

 void widpos(int * ix, int * iy)
 {
 Position  ixr, iyr;

 XtTranslateCoords(iw_win, (Position) *ix, (Position) *iy, &ixr, &iyr);

 *ix = ixr;
 *iy = iyr;
 }
