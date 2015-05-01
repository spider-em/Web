
/*$Header: /usr8/web/src/RCS/numdig.c,v 1.4 2005/10/18 16:59:47 leith Exp $*/
/*
C **********************************************************************
C
C NUMDIG
C        CONVERTED TO C OCT 92 al
C
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
C NUMDIG(IVALUE,MINVAL)
C
C PURPOSE:  FIND NUMBER OF DIGITS IN IVALUE (adds one if negative)
C           THE NUMBER IS >=1 AND >= MINVAL.
C
C PARAMETERS:  
C      IVALUE    INPUT NUMBER
C      MINVAL    MIN. VALUE RETURNED FOR IVALUE
C
C **********************************************************************
*/

#include "std.h"

 int numdig(int ivalue, int minval)
 {
 int leni, lent, minus;

 minus = 0;
 if (ivalue < 0)
    {
    minus  = 1;
    ivalue = - ivalue;
    }

 leni = 1;
 lent = 9;

 while(ivalue > lent)
    {
    lent = 9 * pow((double)10.0,(double)leni) + lent;
    leni++;
    }

 leni = leni + minus;
 return (MYMAX(leni,minval));
 }  
