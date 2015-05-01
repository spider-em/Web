
/*$Header: /usr8/web/src/RCS/querylut.c,v 1.6 2011/09/26 13:16:29 leith Exp $*/
/*
C***********************************************************************
C
C querylut.c
C
C **********************************************************************
C    AUTHOR:  ArDean Leith
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
C    PURPOSE:        List colormap contents 
C
C    PARAMETERS:     Map depth
C                    Number of colors
C
C    CALLS:     
C
C--*********************************************************************
*/

#include "std.h"
#include "routines.h"

 extern Display   *idispl;

 /*************************  lut_info   *******************************/

 int lut_info(char * label, Colormap mapt,  int depth, int kgot, int ncolors) 
 {

 int           i;
 XColor        *colors;

 printf(" %s colormap:  %#x contents \n", label,mapt);
 
 if ((colors = (XColor *) malloc(ncolors*sizeof(XColor))) == (XColor *) 0)
    { spout(" *** Unable to allocate colors in querylut."); return FALSE; }

 /* Query colors used in lut  */

 for(i=kgot ; i < ncolors ; i++)
    {
    colors[i].pixel = i << 16 | i << 8 | i ;
    colors[i].flags = DoRed|DoGreen|DoBlue;
    //colors[i].red = colors[i].green = colors[i].blue = i << 8   | i ;
    }
 XQueryColors(idispl, mapt, colors, ncolors);
    
 for (i = kgot; i < ncolors; i=i+2)
    { 
    printf(" %4d: %8X -- %5u %5u %5u   %4d: %8X -- %5u %5u %5u\n", 
      i,  colors[i].pixel, colors[i].red,   colors[i].green,  colors[i].blue,
      i+1,colors[i+1].pixel,colors[i+1].red,colors[i+1].green,colors[i+1].blue);
    }
 printf(" --------------------------------------- \n");

 free (colors);

 return TRUE;
 }


 /*************************  copylut   *******************************/

 int copylut(Colormap mapin,  Colormap mapout,  int ncolors) 
 {

 int           i;
 XColor        *colors;
 if (ncolors <= 0)
    { spout(" *** No colors in copylut."); return FALSE; }

 if ((colors = (XColor *) malloc(ncolors*sizeof(XColor))) == (XColor *) 0)
    { spout(" *** Unable to allocate colors in copylut."); return FALSE; }

 for(i=0 ; i < ncolors ; i++)
    {
    colors[i].pixel = i << 16 | i << 8 | i ;
    colors[i].flags = DoRed|DoGreen|DoBlue;
    }

 // Query colors used in mapin 
 XQueryColors(idispl, mapin,  colors, ncolors);
 
 // Store colors used in mapout 
 if (!XStoreColors(idispl, mapout, colors, ncolors))
    { printf(" *** Unable to store colors in copylut."); return FALSE; }

 free (colors);;

 return TRUE;
 }

 /*************   
 for (i = 0; i < ncolors; i++)
      { printf(" %5d -- %5u %5u %5u \n",i,colors[i].red, 
                                          colors[i].green, 
                                          colors[i].blue);}
 printf(" --------------------------------------- \n");
 ********************/

