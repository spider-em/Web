
/*$Header: /usr16/software/web/src/RCS/rdprx.c,v 1.4 2018/12/07 17:03:33 leith Exp $*/
/*
C++************************************************************************
C
C RDPRX
C
C **********************************************************************
C *  AUTHOR:  ArDean Leith                                                 *
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
C RDPRX
C
C PURPOSE:         GET INTEGER OR FLOATING POINT VALUE FROM TEXT WIDGET 
C
C PARAMETERS:      
C
C CALLED BY:       
C
C***********************************************************************
*/

#include <Xm/Text.h>
#include "std.h"
#include "x.h"
#include "routines.h"


 /******************  rdprx   *******************************/

 int rdprx(Widget iw_t, char type, int * ival, float * fval)

 {  
 
 char *string;

 if (iw_t <= (Widget)0) 
    {
    spout("*** pgm error, ivalid Widget in rdprx!");
    return FALSE;
    }

 string = XmTextGetString(iw_t);

 if (type == 'i' || type == 'I')
    {   /* want to return integer */
    *ival = atoi(string);
    }
 else
    {
    if (sscanf(string,"%f",fval) != 1)
       {
       spout("*** Invalid floating point input in rdprx!");
       XtFree(string); 
       return FALSE;
       }
    }
 XtFree(string);
 return TRUE;
 }
 
