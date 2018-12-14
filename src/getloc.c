 
/*$Header: /usr16/software/web/src/RCS/getloc.c,v 1.4 2018/12/07 17:03:32 leith Exp $*/
/*
C++*************************************************************************
C
C getloc -- SEPT 92 al                                                
C **********************************************************************
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
C getloc
C
C PURPOSE:       finds location for key or mouse
C
C PARAMETERS:    
C
C CALLED BY:     movearea
C
C NOTE:          The event positions are integers in the structure!
C
C--*********************************************************************
*/

#include "x.h"
#include "std.h"
#include "routines.h"


 /************************* getloc  ***************************/
 
 void getloc(XEvent * event, char type, int *ix, int *iy)
 {
 if (type == 'm' || type == 'M')            
    {
    /* get mouse position */

    /*****************
    *ix   = event -> xmotion.x_root;
    *iy   = event -> xmotion.y_root;************* */


    *ix   = event -> xmotion.x;
    *iy   = event -> xmotion.y;
     }

 else if (type == 'b' || type == 'B')
    {
    /* get mouse position when key pressed*/

    /************
    *ix = event -> xkey.x_root;
    *iy = event -> xkey.y_root; ********/

    *ix = event -> xkey.x;
    *iy = event -> xkey.y;
   /* Using x_root and y_root yields high coordinates, but placement
       is done relative to iw_win 'looks' right.
       Using x and y yields expected numbers, but placement is done
       relative to the terminal screen (i.e., 0,0 is ulhs of screen)
       This may cause problems when the user inputs coords.
    */
    }
 }




