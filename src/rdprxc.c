
/*$Header: /usr8/web/src/RCS/rdprxc.c,v 1.5 2015/08/03 18:50:32 leith Exp $*/
/*
C++************************************************************************
C
C RDPRXC
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
C RDPRXC
C
C PURPOSE:         GET STRING VALUE FROM TEXT WIDGET 
C
C PARAMETERS:      
C
C CALLED BY:       
C
C***********************************************************************
*/

#include "std.h"
#include "x.h"
#include <Xm/Text.h>

extern void   spout         (char *);
extern void   spouts        (char *);
extern void   spouti        (int);


 /******************  rdprxc  *******************************/

 int rdprxc(Widget iw_t, char * astring, int slen)

 {  
 char * astringt;
 size_t slent;

 if (iw_t == (Widget) 0) 
    {
    spout("*** Pgm error, ivalid Widget in rdprxc!");
    return 0;
    }

 astringt = XmTextGetString(iw_t);
 slent    = strlen(astringt);

 /* slen is available length including null for astring */
 if (slent >= slen)
    {
    spouts("*** String length limited to: "); spouti(slen-1); spout(" ");
    XtFree(astringt);
    return 0;
    }

 strcpy(astring,astringt);

 XtFree(astringt);
 return (int) slent;
 }
 
