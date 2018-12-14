/*$Header: /usr16/software/web/src/RCS/iskipadj.c,v 1.6 2018/12/07 17:03:32 leith Exp $*/
/*
C++**********************************************************************
C
C  iskipadj.c          Sept 99 
C
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
C    iskipadj()
C
C    PURPOSE:     Converts screen coordinates to actual image
C                 coordinates.  For use with images displayed at
C                 different sizes using variable: iskip.
C
C    PARAMETERS: none   
C
C    CALLED BY:  catmen_buta     
C
C--*********************************************************************
*/
#include "common.h"
#include "routines.h"


 /************************  iskipadj   *******************************/

 int iskipadj(int ixs,int iys, int nsam, int nrow, int iskip,
              int* ixi, int *iyi)

 {
 if (iskip < -1)
    {    /* magnified image */
    *ixi  = (ixs  + 1) / -iskip ;
    *iyi  = (iys  + 1) / -iskip ;
    }
 else if (iskip > 1)
    {     /* shrunken image */
    *ixi  = (ixs -1) * iskip + 1;
    *iyi  = (iys -1) * iskip + 1;
    }
 else
    {    /* regular scale image */
    *ixi  = ixs;
    *iyi  = iys;
    }

 if (*ixi < 1 || *ixi > nsam || *iyi < 1 || *iyi > nrow)
    {    /* cursor is outside of image, only want inside */
   return FALSE;
    }
 return TRUE;
 }

