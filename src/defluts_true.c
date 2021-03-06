
/*$Header: /usr16/software/web/src/RCS/defluts_true.c,v 1.2 2018/12/07 17:03:31 leith Exp $*/
/*
C***********************************************************************
C
C defluts_true      Directcolor mods             Jul 2001 Erik Vogan   *
C                   Truecolor                    Sep 2011 ArDean Leith *
C
C***********************************************************************
C* AUTHOR: ArDean Leith                                                 
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
C***********************************************************************
C
C    defluts_true(dummy)
c
C    PURPOSE:  Handles 24 bit true_color colormap usage.
C              The color map has a grey ramp in 1...255.
c              Colors set  in ispicol[colorgo...colorend]
C
C    PARAMETERS:  None currently used
C
C    CALLED BY:   web_main
C
C    CALLS: 
C
C COLORGO  0      1     2      3      4    5        6       7
C          BLACK, BLUE, GREEN, CYAN,  RED, MAGENTA, YELLOW, WHITE
C    
C--********************************************************************
*/

#include "std.h"
#include "x.h"
#include "common.h"
#include "routines.h"

 /*************************  defluts_true *******************************/

Colormap defluts_true(int dummy) 
 {

 Colormap        mappriv, mapdef;
 int             i, it, idepth;
 XVisualInfo     visual_data;
 XColor          exact;
 XColor          color;
 Visual          *visualdef; 
 int             screen;

 Status          ires;

 screen    = DefaultScreen(idispl);
 visualdef = DefaultVisual(idispl, screen);
 mapdef    = DefaultColormapOfScreen(iscreen);
 idepth    = DefaultDepthOfScreen(iscreen);

 printf(" Web ----------------------\n" );
 printf(" Default Screen:     %d  \n",screen);
 printf(" Screen depth:       %d  \n",idepth);
 printf(" Default visual:     %p \n",(void *)visualdef);

 // Write out shared colormap info 
 //lut_info("shared",mapdef,24,0,256); 

 // Check depth of screen (number of bit planes) 
 if(idepth < 8)
    {fprintf(stderr,"*** Screen depth is too shallow!"); 
     exit(1); }

 // Get a truecolor visual for colormap use  ------------ TrueColor

 if (!XMatchVisualInfo(idispl, XDefaultScreen(idispl), idepth,
                       TrueColor, &visual_data))
    {fprintf(stderr,"*** No %d bit, TrueColor visual!",idepth);  
     exit(2);  }

 ncolors = visual_data.colormap_size;
 visual  = visual_data.visual;

#ifdef NEVER
 // Refers to truecolor colormap
 mappriv = XCreateColormap(idispl, RootWindowOfScreen(iscreen), 
                           visual, AllocNone);
 if (!mappriv)
    { fprintf(stderr,"*** Failed to find a truecolor colormap!");  
      exit(3);  }
#endif
 mappriv = mapdef; // Use default colormap

 printf(" Current colormap    %p \n",(void *)mappriv );
 printf(" # Colormap colors:  %d  \n",ncolors);
 
 //Write out private colormap info 
 //lut_info("Default",mapdef,24,0,256); 
 //lut_info("Private",mappriv,24,0,256); 

 mapgo    =  0;                      
 mapend   = 255;  
 colorgo  = 0;
 colorend = 7;
 imagego  = 0;
 imageend = 255;

 i        = colorgo;

 ires = XAllocNamedColor(idispl,mappriv,"black",&exact, &color);;
 if (!ires)
    {fprintf(stderr,"*** Failed to XAllocNamedColor: black! \n");  
     exit(2); }
 ispicoln[i]  = i;             // Used in colmen
 ispicol[i++] = color.pixel;
 //printf(" black Pixel:        %#8x \n",color.pixel);

 ires = XAllocNamedColor(idispl,mappriv,"blue",&exact, &color);;
 if (!ires)
    {fprintf(stderr,"*** Failed to XAllocNamedColor: blue! \n");  
     exit(2); }
 ispicoln[i]  = i;             // Used in colmen
 ispicol[i++] = color.pixel;
 //printf(" blue Pixel:        %#8x \n",color.pixel);

 ires = XAllocNamedColor(idispl,mappriv,"green",&exact, &color);;
 if (!ires)
    {fprintf(stderr,"*** Failed to XAllocNamedColor: green! \n");  
     exit(2); }
 ispicoln[i]  = i;             // Used in colmen
 ispicol[i++] = color.pixel;
 //printf(" green Pixel:        %#8x \n",color.pixel);

 ires = XAllocNamedColor(idispl,mappriv,"cyan",&exact, &color);;
 if (!ires)
    {fprintf(stderr,"*** Failed to XAllocNamedColor: cyan! \n");  
     exit(2); }
 ispicoln[i]  = i;             // Used in colmen
 ispicol[i++] = color.pixel;
 //printf(" cyan Pixel:        %#8x \n",color.pixel);

 ires = XAllocNamedColor(idispl,mappriv,"red",&exact, &color);;
 if (!ires)
    {fprintf(stderr,"*** Failed to XAllocNamedColor: red! \n");  
     exit(2); }
 ispicoln[i]  = i;             // Used in colmen
 ispicol[i++] = color.pixel;
 //printf(" red Pixel:        %#8x \n",color.pixel);

 ires = XAllocNamedColor(idispl,mappriv,"magenta",&exact, &color);;
 if (!ires)
    {fprintf(stderr,"*** Failed to XAllocNamedColor: magenta! \n");  
     exit(2); }
 ispicoln[i]  = i;             // Used in colmen
 ispicol[i++] = color.pixel;
 //printf(" magenta Pixel:        %#8x \n",color.pixel);

 ires = XAllocNamedColor(idispl,mappriv,"yellow",&exact, &color);;
 if (!ires)
    {fprintf(stderr,"*** Failed to XAllocNamedColor: yellow! \n");  
     exit(2); }
 ispicoln[i]  = i;             // Used in colmen
 ispicol[i++] = color.pixel;
 //printf(" yellow Pixel:        %#8x \n",color.pixel);

 ires = XAllocNamedColor(idispl,mappriv,"white",&exact, &color);;
 if (!ires)
    {fprintf(stderr,"*** Failed to XAllocNamedColor: white! \n");  
     exit(2); }
 ispicoln[i]  = i;             // Used in colmen
 ispicol[i++] = color.pixel;
 //printf(" white Pixel:        %#8x \n",color.pixel);

#ifdef DEBUG
 for (i = 0; i < ncolors; i=i+4)// Debug info
    {
    printf(" ispicol(%3d..%3d) : %#8x  %#8x   %#8x   %#8x \n",
             i,i+3, ispicol[i],   ispicol[i+1],  
                    ispicol[i+2], ispicol[i+3]) ; 
    }
#endif


 return mappriv;
}








#ifdef NEVER


 // Set colors in spider colors
 icol = colorgo;       // First spider color
 nl   = 2;             // # of shades 
 for (nr = 0; nr < nl; nr++)        // number of reds
    {
    for (ng = 0; ng < nl; ng++)     // number of greens
       {
       for (nb = 0; nb < nl; nb++)  // number of blues
          {
          ispicol[icol++] = nlooks[nr] << 16 | 
                            nlooks[ng] <<  8 | 
                            nlooks[nb];
          } 
       }
    }
     
#ifdef DEBUG
 // Debug info
 for (i = 0; i < ncolors; i=i+4)
    {
    printf(" ispicol(%3d..%3d) : %#8x  %#8x   %#8x   %#8x \n",
             i,i+3, ispicol[i],   ispicol[i+1],  
                    ispicol[i+2], ispicol[i+3]) ; 
    }
#endif


 // Write out private colormap info 
 //lut_info("Private",mappriv,24,0,256); 
#endif





