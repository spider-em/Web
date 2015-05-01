
/*$Header: /usr8/web/src/RCS/addext.c,v 1.3 2005/10/18 16:59:49 leith Exp $*/
/*
C++************************************************************************
C
C addext.c
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
C addext(char * filnam, char * datexc, int lennam)
C
C PURPOSE: Adds datexc onto filnam if not already there.
C           Returns TRUE if all OK.
C
C**************************************************************************
*/

#include "common.h"
#include "routines.h"

 int addext(char * filnam, char * datexc, int lennam)
 {
 char * locext;

 if (* datexc)
    { /* have an extension to add onto filnam */

    locext = strstr(filnam,datexc);

    if (locext == NULL || 
       (locext != (filnam + strlen(filnam) - strlen(datexc))
        && *(locext-1) != '.'))
       {   /* datexc not already present, must append datexc now */
       if ((strlen(filnam) + strlen(datexc) + 1) >= (size_t) lennam)
          {
          spouts("*** Filename too long to append extension:");
          spouts(filnam); spouts(" : "); spout(datexc);
          return FALSE;
          }
       strcat(filnam,"."); strcat(filnam,datexc);
       }
    }
 return TRUE;
 }


