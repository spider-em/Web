/*$Header: /usr16/software/web/src/RCS/visual_info.c,v 1.6 2018/12/07 17:03:34 leith Exp $*/
/*
C++*************************************************************************
C
C VISUAL_INFO                         
C
C **********************************************************************
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
C  VISUAL_INFO
C
C  PURPOSE:  GIVES MISC. INFO ON X-WINDOW VISUAL USEFUL FOR DEBUGGING
C--*******************************************************************
*/

#include "std.h"
#include "x.h"
#include "routines.h"

 void visual_info(Display *idispl, Visual *vis, 
                  int * depth, int * ncolors)
 {

 XVisualInfo   vTemplate;
 XVisualInfo   *visualList;
 int           visualsMatched, iclass;
 char          *class;
 int           nummap,i;
 Colormap      *maps;

 vTemplate.visualid = XVisualIDFromVisual(vis);

 visualList  = XGetVisualInfo(idispl,VisualIDMask,
                             &vTemplate, &visualsMatched);
 if (visualsMatched == 0)
    { printf("No such visual! \n"); return; }

 iclass = visualList[0].class;

 if (iclass == DirectColor)
    class = "DirectColor";       
 else if (iclass == GrayScale)
    class = "GreyScale";
 else if (iclass == PseudoColor)
    class = "PseudoColor";
 else if (iclass == StaticColor)
    class = "StaticGray";
 else if (iclass == TrueColor)
    class = "TrueColor";
 else if (iclass == GrayScale)
    class = "GrayScale";
 else
    class = "Unknown";

 if (visualList[0].visual != vis)
    printf("Visual: %p  not same as in caller: %p\n",(void *)visualList[0].visual,(void *)vis);

    printf(" Visual class:              %s (%d) \n",class,visualList[0].class );

   *depth = visualList[0].depth;
    printf(" Visual depth:          %6d     \n",    visualList[0].depth );

    *ncolors = visualList[0].colormap_size;
    printf(" Visual map size:        %6d     \n",    visualList[0].colormap_size);

    printf(" RGB masks:                 %#lX %#lX %#lX \n",visualList[0].red_mask,
                                                        visualList[0].green_mask,
                                                        visualList[0].blue_mask);
    printf("  \n");

 XFree((XVisualInfo *) visualList); 

}

