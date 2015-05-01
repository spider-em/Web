
/*$Header: /usr8/web/src/RCS/wid_lutinfo.c,v 1.6 2011/07/20 16:45:02 leith Exp $*/
/*
C***************************************************************************
C
C    WID_LUTINFO
C                Directcolor mods by: Erik Vogan July 2001
C
C **********************************************************************
C *  AUTHOR: ArDean Leith                                                  *
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
C    WID_LUTINFO(MAP)
C
C    PURPOSE:        LISTS COLORMAP POSITIONS
C
C    PARAMETERS:
C
C    CALLED BY:      
C
C *********************************************************************
*/

/* emv - needed for display depths > 8 bits */
#include "common.h"

 extern Display *idispl;

 /************************  wid_lutinfo   **************************/

 int wid_lutinfo(Colormap map, int npix) 
 {

    int            i;
    XColor         color;

    printf(" Lut contents for first %d positions in map: %d \n",
           npix,map);
    printf(" --------------------------\n");

    for (i = 0; i < npix; i++)
       {
       color.pixel = ispicol[i];

       XQueryColor(idispl,map,&color);

       printf(" %5d -- %5u %5u %5u     ",i,color.red,color.green,color.blue);
       if (i%2 != 0) printf("\n");
       }
    return TRUE;
 }

