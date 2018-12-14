
/*$Header: /usr16/software/web/src/RCS/wicolor.c,v 1.11 2018/12/07 17:03:35 leith Exp $*/

/*
 C**********************************************************************
 C
 C WICOLOR  -- Created                                    Nov  1989
 C             Converted to C                             Sept 1992 al 
 C             Added background                           May  2002 al
 C             Rewrite                                    Sept 2011 al 
 C             Cosmetic                                   July 2015 al 
 C             colort bug                                 Dec  2018 al 
 C                                   
 C**********************************************************************
 C=* AUTHOR: ArDean Leith                                              *
 C=* FROM: WEB - VISUALIZER FOR SPIDER MODULAR IMAGE PROCESSING SYSTEM *
 C=* Copyright (C) 1992-2018  Health Research Inc.                     *
 C=* Riverview Center, 150 Broadway, Suite 560, Menands, NY 12204.     *
 C=* Email: spider@health.ny.gov                                       *
 C=*                                                                   *
 C=* This program is free software; you can redistribute it and/or     *
 C=* modify it under the terms of the GNU General Public License as    *
 C=* published by the Free Software Foundation; either version 2 of    *
 C=* the License, or (at your option) any later version.               *
 C=*                                                                   *
 C=* This program is distributed in the hope that it will be useful,   *
 C=* but WITHOUT ANY WARRANTY; without even the implied warranty of    *
 C=* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU  *
 C=* General Public License for more details.                          *
 C=*                                                                   *
 C=* You should have received a copy of the GNU General Public License *
 C=* with this program. If not, see <http://www.gnu.org/licenses>      *
 C=*                                                                   *
 C*********************************************************************
 C
 C wicolor(icontx,color)
 C
 C PURPOSE:      Set color for graphics drawing
 C
 C PARAMETERS:   icontx    Current graphics context
 C               color     Web color number
 C
 C CALLED BY:    Many routines
 C
 C**********************************************************************
*/

#include "common.h"
#include "routines.h"

#ifdef WEB_TRUE

 /* Externally defined common variables */

 int wicolor(GC icontxt, int icolort)
 {
 
 if (icolort >= 0 && icolort < ncolors)
    {
    /* Set foreground to specified Web color */
    XSetForeground(idispl,icontxt, ispicol[icolort]);

#ifdef DEBUG
    printf(" Foreground color:%4d  Using:%#8x \n", 
             icolort, ispicol[icolort]);
#endif

    if (icontxt == icontx)
       {
       /* Record current foreground color in icontx GC */
       icolor  = icolort;
       icolorx = ispicol[icolort];
       }
    }
 
 else if (icolort  < -999999)    /* colort bug 2018 al */
    {
    /* Set background to specified Web color */
    icolort  = icolort + 1000000;
    if (icolort < 0)        icolort = -icolort;
    if (icolort >= MAXCOL)  icolort = 0;
    XSetBackground(idispl,icontxt, ispicol[icolort]);
     
#ifdef DEBUG
    printf(" Background color:%4d  Using:%#8x \n", 
             icolort, ispicol[icolort]);
#endif
    if (icontxt == icontx)
       {
       /* Record current background color in icontx GC */
       icolorb  = icolort;
       icolorxb = ispicol[icolort];
       }
    }
 else
    {
    /* ERROR */
    spout("*** Pgm error, wicolor got bad color"); return FALSE;   
    }

 return TRUE;
 }


/************************  wicolor  ********************************/


#else

#define NIMAGE  128
 
 int wicolor(GC icontxt, int color)
 {
 int icolort;

 
 if (color <= 0 && color > -NIMAGE)
    {
    /* Set foreground to specified Web image ramp color */
    icolort = -color;
    }

 else if (color >= 0 && color < MAXCOL)
    {
    /* Set foreground to specified Web color */
    icolort  = color;
    }
 
 else
    {
    /* Use default color: ?? */
    icolort = 131;
    }


 if (color < -999999) 
    {
    /* Set background to specified Web color */
    icolort  = color + 1000000;
    XSetBackground(idispl,icontxt, ispicol[icolort]);
     
#ifdef DEBUG
    printf(" Background color:%4d  Using:%#8x \n", 
             icolort, ispicol[icolort]);
#endif
    }
 else
    {
    /* Set foreground to specified Web color */
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



