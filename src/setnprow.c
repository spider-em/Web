
/*
C++*********************************************************************
C
C setnprow.c    Extracted from mont2.c Aug. 2001 ArDean Leith
C
C **********************************************************************
C * AUTHOR:  ArDean Leith                                              *
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
C    setnprow
C
C    PURPOSE:         Find number of images that will fit on a row.
C
C    PARAMETERS:     
C
C    CALLED BY:       mont2
C
C***********************************************************************
*/

#include "common.h"
#include "routines.h"


 /* external variables used here */

 /* internal variables used elsewhere */

/************************ setnprow  ********************************/

 int setnprow(int nsam)

 {   
 int    marlefo;

 /* adjust marlefo for left side position */
 marlefo = marlef;
 if (placed) marlefo = ixuln;

 if (iskip > 1)
    nprow = (iwidex - marlefo) / ((nsam / iskip)  + marx);
 else if (iskip < -1)
    nprow = (iwidex - marlefo) / ((nsam * -iskip) + marx);
 else
    nprow = (iwidex - marlefo) / ((nsam         ) + marx);

 return nprow;
 }


 
