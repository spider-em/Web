
/*$Header: /usr16/software/web/src/RCS/extractstr.c,v 1.4 2018/12/07 17:03:31 leith Exp $*/
/*
 C++********************************************************************
 C
 C extractstr -- CREATED July 92
 C                                                                     
 C *********************************************************************
 C=* FROM: WEB - VISUALIZER FOR SPIDER MODULAR IMAGE PROCESSING SYSTEM *
 C=* Copyright (C) 1992-2005  Health Research Inc.                     *
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
 C *********************************************************************
 C
 C    extractstr
 C
 C    PARAMETERS:   compstring           compound string
 C
 C    RETURNS:      STRING               ASCII STRING
 C
 C    PURPOSE:      GETS ASCII STRING FROM A COMPOUND STRING
 C
 C    CALLS:        MOTIF
 C
 C    CALLED BY:    
 C
 C--*******************************************************************
*/

#include "x.h"
#include "std.h"
#include "routines.h"

char *extractstr(XmString compstring)

 {

 XmStringContext    context;
 XmStringCharSet    charset;
 XmStringDirection  direction;
 Boolean            separator, it;
 static char        *string;

 it = XmStringInitContext (&context,compstring);
 if (it == TRUE)
    {
    /* extract segment from the string */
    it = XmStringGetNextSegment (context,&string,
			  &charset,&direction,&separator);
    if (it == FALSE)
       spout("*** Can not get segment in extractstr");
    }
 else
    spout("*** Can not initialize context in extractstr");

 XmStringFreeContext (context);

 return ((char *) string);
}
