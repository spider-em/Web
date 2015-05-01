
/*$Header: /usr8/web/src/RCS/strtoint.c,v 1.5 2005/10/18 16:59:50 leith Exp $*/
/*
 * strtoint.c
 *
 **********************************************************************
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
 **********************************************************************
 *
 * 
 * 
 * PURPOSE:	retrieves string from widget and converts to integer
 *
 * RETURNS:	error flag (FALSE is error!)
 * 
 * PARAMETERS:  iw_it     widget to be interrogated
 *              message   error message
 *              ival      integer value (returned)
 *
 **********************************************************************
*/

#include "std.h"
#include "x.h"
#include <Xm/TextF.h>
#include <Xm/Text.h>


/* function prototypes */
 extern void spout(char *);
 extern void spouts(char *);

 /*********************** strtoint ******************************/
 
 int strtoint(Widget iw_it, char * message, int * ival)

 {
 char * string;

 if (!XtIsWidget(iw_it))
    {
    spout("*** iw_it not set, pgm. error in strtoint caller");
    return FALSE;
    }

 string = XmTextGetString(iw_it);

 if (sscanf(string,"%d",ival) <= 0)  
    {
    spouts("*** Illegal value for: "); spout(message);
    XtFree(string);
    return FALSE;
    } 

 XtFree(string);
 return TRUE;
 }

