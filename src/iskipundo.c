/*$Header: /usr16/software/web/src/RCS/iskipundo.c,v 1.2 2018/12/07 17:03:32 leith Exp $*/
/*
C++**********************************************************************
C
C  iskipundo.c          June 2000 
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
C    iskipundo(ixi,iyi,iya,iya)
C
C    PURPOSE:     Converts image coordinates to current screen
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


 /************************  iskipundo   *******************************/

 int iskipundo(int ixi, int iyi,   int iskip,
               int ixul, int iyul, int* ixs, int *iys)

 {
 if (iskip < -1)
    {    /* magnified image */
    *ixs = ixul -(ixi * iskip + 1) + ixul;
    *iys = iyul -(iyi * iskip + 1) + iyul;
    }
 else if (iskip > 1)
    {     /* shrunken image */
    *ixs = ixul + (ixi - 1) / iskip + 1;
    *iys = iyul + (iyi - 1) / iskip + 1;
    }
 else
    {    /* regular scale image */
    *ixs  = ixul + ixi;
    *iys  = iyul + iyi;
    }

 return TRUE;
 }

