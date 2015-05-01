 
/*$Header: /usr8/web/src/RCS/checkcom.c,v 1.3 2005/10/18 17:00:00 leith Exp $*/

/**********************************************************************
 *
 * checkcom.c
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
 * checkcom()
 * 
 * PURPOSE:	checks for first command or incompatible commands
 *
 * RETURNS:	ok flag
 * 
 * PARAMETERS:  none
 *
 ********************************************************************
*/

#include "common.h"

 /* external function prototypes */
 extern void   spout        (char *);
 extern void   fin_cb       (Widget, caddr_t, caddr_t);
 extern void   webclear     (Widget, caddr_t, caddr_t); 
 extern int    wicolor      (GC, int);

 static int    newrun = TRUE;

 /******************************************************************/
 
 void checkcom(Widget iw_dum, caddr_t cdum1, caddr_t cdum2)
 {

 /*** printf(" in checkcom now\n");   ***/


 if (newrun) 
    {  /* this is the initial command in web */
    newrun = FALSE;

    /* clear the screen of junk from previous runs */
    webclear(NULL,NULL,NULL);

    /* set initial foreground color */
    wicolor(icontx,icolor);

    }
 return;
 }

  
