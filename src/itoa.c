
/*$Header: /usr16/software/web/src/RCS/itoa.c,v 1.3 2018/12/07 17:03:32 leith Exp $*/
/*
C **********************************************************************
C
C ITOA
C        
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
C itoa(ivalue)
C
C PURPOSE:  convert integer to a char string
C
C PARAMETERS:  
C      IVALUE    INPUT NUMBER
C
C **********************************************************************
*/

#include "std.h"
#include "routines.h"

 /************************* itoa *******************************/

 char * itoa(int ivalue)

 {
 int leni;
 char * cval;

 leni = numdig(ivalue,1) ;

 if ((cval = (char *) malloc((leni + 1) * sizeof(char))) == 
     (char *) NULL)  return (char *) 0;

 sprintf(cval,"%*d",leni,ivalue);
 return cval; 
 }  
