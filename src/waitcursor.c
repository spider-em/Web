
/*$Header: /usr8/web/src/RCS/waitcursor.c,v 1.2 2005/10/18 16:59:59 leith Exp $*/
/*
C++*************************************************************************
C
C WAITCURSOR                        
C             CONVERTED TO C -- SEPT 92 al
C **********************************************************************
C *  AUTHOR: ARDEAN LEITH
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
C    WAITCURSOR(IWIN,WAIT,IWANT)
C
C    PARAMETERS:  IWIN          WINDOW WHERE CURSOR IS DESIRED       READS
C                 WAIT          LOGICAL FLAG TO START OR STOP WAIT   READS
C                 ICWANT        DESIRED TEMP CURSOR NUMBER           READS
C
C    PURPOSE:     SET TEMPORARY WAIT CURSOR FOR THIS WINDOW
C
C    CALLED BY:   
C
C***********************************************************************
*/

#include "x.h"
#include "std.h"
#include "common.h"

#define MAXCURSOR 75

 extern void setacursor (int, int, int);

 /************************************************************/

 void waitcursor(Window idum, int wait, int icwant)

 {
 static int lastcursor, lastcursorx;

 if (wait)
    {           
    /* install waiting cursor */
    if (icwant < 0 || icwant >= MAXCURSOR) icwant = 75;
    
    lastcursor  = icursor;
    lastcursorx = icursorx; 
   
    /* (in setacursor -1,-1 means do not change cursor colors) */
    setacursor(icwant, -1,-1);
    }

 else
    {
    /* replace wait cursor with last cursor before wait cursor */
    /* (-1,-1 means do not change cursor colors) */
    setacursor(lastcursor,-1,-1);
    }  
 
 XFlush(idispl);
 return;
 }


