
/***********************************************************************
 *
 * scale32to8.c
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
 * scale32to8(float fmint, float fmaxt, int iskip,
 *            int   nmint, int   nmaxt,
 *            float * scal, float * offset)
 * 
 * PURPOSE:  finds factor to scale a floating point into 8 bit 
 *           over range nmint..nmaxt.
 *
 * RETURNS:  true or false
 *
 * PARAMETERS: 
 *
 ***********************************************************************
*/

#include "std.h"
#include "routines.h"

 /****************************  scale32to8  *****************************/
 
 int scale32to8(float fmint, float fmaxt, int iskip,
              int   nmint, int   nmaxt,
              float * scal, float * offset)
 { 

 if (fabs(fmaxt - fmint) < FLTZER)
    { spout("*** Blank image"); return FALSE; }

 /* CONVERSION FACTOR FROM FLOATING POINT TO LOGICAL * 1 RANGE */
 *scal   = (float) (nmaxt - nmint) / (fmaxt - fmint);
 *offset = -fmint * (*scal) + nmint + 0.5;

 return TRUE;
}

