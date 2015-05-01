
/*$Header: /usr8/web/src/RCS/witext.c,v 1.6 2011/09/21 12:14:19 leith Exp $*/

/*
 * witext.c
 *
 ***********************************************************************
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
 ***********************************************************************
 *
 * witext(icontxt, text, ix, iy, itsize, just, inten, ipen, overprint)
 * 
 * PURPOSE:     text drawing routine 
 *              Positions text with lower left pixel at specified loc.    
 * 
 * PARAMETERS:	 
 *
 ***********************************************************************
*/

#include "common.h"
#include "routines.h"

 /*********************** witext *******************************/

 void witext(GC icontxt, char * text, int ix, int iy, 
             int itsize, int just, int inten, int ipen, int overprint)

 {
 int ici, nchar, ixj;

 if (inten >= 0)
    {  // Set color in icontxt using spider colors 
    if (ipen < 0)
       ici = -ipen;
    else
       ici = colorgo + ipen ;

    wicolor(icontxt,ici);
    }

 /* Justify text if desired */
 ixj = ix;

 if (just == 1)
    {    /* Centered text */
    ixj = ix + pixlen(icontx,text);
    }
 else if (just ==2)
    {    /* Left justified text */
    ixj = ixj - pixlen(icontx,text);
    }

 /* String command needs text length */
 nchar = strlen(text);

 if (overprint) 
    {    /*  Write text to screen (foreground & background)  */
    XDrawImageString(idispl, iwin, icontxt, ixj, iy, text, nchar);

    /* Write text to backing store */
    XDrawImageString(idispl, imagsav, icontxt, ixj, iy, text, nchar);
    }   

 else
    {   /* Write text to screen (foreground only) */
    XDrawString(idispl, iwin, icontxt, ixj, iy, text, nchar);

    /* Write text to backing store (foreground only) */
    XDrawString(idispl, imagsav, icontxt, ixj, iy, text, nchar);
    } 
  
 neednewmap = TRUE;

 }
