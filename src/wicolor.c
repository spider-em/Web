
/*$Header: /usr8/web/src/RCS/wicolor.c,v 1.9 2011/09/14 15:38:45 leith Exp $*/
/*
C***************************************************************************
C
C WICOLOR  -- CREATED           NOV  1989
C             CONVERTED TO C    SEPT 1992 al 
C             ADDED BACKGROUND  MAY  2002 al
C             REWRITE           SEPT 2011 al 
C                                   
C **********************************************************************
 C=* AUTHOR: ArDean Leith                                              *
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
C WICOLOR(ICONTX,COLOR)
C
C PURPOSE:      SET COLOR FOR GRAPHICS DRAWING
C
C PARAMETERS:   ICONTX    PRESENT GRAPHICS CONTEXT
C               COLOR     SPIDER COLOR NUMBER
C
C CALLED BY:    MANY ROUTINES
C
C--*********************************************************************
*/
 
 /************************  wicolor  ********************************/

#ifdef WEB_TRUE

#include "common.h"

 int wicolor(GC icontxt, int icolort)
 {
 
 if (icolort >= 0 && icolort < ncolors)
    {
    /* Set foreground to specified spider color */
    XSetForeground(idispl,icontxt, ispicol[icolort]);

#ifdef DEBUG
    printf(" Foreground color:%4d  Using:%#8x \n", 
             icolort, ispicol[icolort]);
#endif

    if (icontxt == icontx)
       {
       /* Record current color in icontx GC */
       icolor  = icolort;
       icolorx = ispicol[icolort];
       }
    }
 
 else if (colort < -999999) 
    {
    /* Set background to specified spider color */
    icolort  = icolort + 1000000;
    XSetBackground(idispl,icontxt, ispicol[icolort]);
     
#ifdef DEBUG
    printf(" Background color:%4d  Using:%#8x \n", 
             icolort, ispicol[icolort]);
#endif
    if (icontxt == icontx)
       {
       /* Record current color in icontx GC */
       icolorb  = icolort;
       icolorxb = ispicol[icolort];
       }
    }
 else
    {
    /* ERROR */
    spout("*** Pgm error, wicolor get bad color"); return FALSE;   
    }

 return TRUE;
 }

#else
 /************************  wicolor  ********************************/


#include "common.h"

#define NIMAGE  128
 
 int wicolor(GC icontxt, int color)
 {
 int icolort;
 
 if (color <= 0 && color > -NIMAGE)
    {
    /* Set foreground to specified spider image ramp color */
    icolort = -color;
    }

 else if (color >= 0 && color < MAXCOL)
    {
    /* Set foreground to specified spider color */
    icolort  = color;
    }
 
 else
    {
    /* Use default color: ?? */
    icolort = 131;
    }


 if (color < -999999) 
    {
    /* Set background to specified spider color */
    icolort  = color + 1000000;
    XSetBackground(idispl,icontxt, ispicol[icolort]);
     
#ifdef DEBUG
    printf(" Background color:%4d  Using:%#8x \n", 
             icolort, ispicol[icolort]);
#endif
    }
 else
    {
    /* Set foreground to specified spider color */
    XSetForeground(idispl,icontxt, ispicol[icolort]);

#ifdef DEBUG
    printf(" Foreground color:%4d  Using:%#8x \n", 
             icolort, ispicol[icolort]);
#endif

    if (icontxt == icontx)
       {
       /* Record current color in icontx GC */
       icolor  = icolort;
       icolorx = ispicol[icolort];
       }
    }

 return TRUE;
 }

#endif



