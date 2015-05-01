
/*$Header: /usr8/web/src/RCS/defluts.c,v 1.15 2011/07/26 15:28:34 leith Exp $*/
/*
C***********************************************************************
C
C defluts             Directcolor mods by: Erik Vogan July 2001
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
C    DEFLUTS(DUMMY)
C
C    PURPOSE:  Create colormaps and set the spider 
c              color look-up-tables in the maps.
C              The color map has a grey ramp in MAPGO...MAPEND and
c              spider color shades in COLORGO...COLOREND.
C              Positions starting at 0 are often used by window 
c              manager.  If so, the number of image grey levels 
c              should be reduced to fit in the space above the 
c              reserved numbers and below number 127.
C              (This mechanism is used to preserve overlay in 
c              last plane).  The colormap sets MAPEND - MAPGO +1
c              grey levels and <=128 spider colors and attempts
c              to preserve background & foreground colors used by
c              the window manager. (It may not succeed)
C
C    PARAMETERS:  None currently used
C
C    CALLED BY:   web_main
C
C    CALLS: 
C
C        COLORGO          1   2      3           4    5         6      7
C    128 BLACK, BLUE, GREEN, BLUE_GREEN, RED, MAGENTA, YELLOW, WHITE
C    
C    128 BLACK,    dark blue  blue          BLUE
C    132 green     green      blue          blue
C    136 green     green      blue          CYAN
C    140 GREEN     green      blue          blue
C
C    144 red       red        magenta       magenta
C    148 brown     brown      bluish        bluish  
C    152 greenish  green      blueish       blueish             
C    156 green     green      blueish       blueish
c
C    160 red       red        magenta        magenta
C    164 orange    orangish   lite orange    lite magenta
C    168 yellow    yellow     bluish         bluish
C    172 greeenish greenish   lite blue      lite blue
C
C    176 RED       red        magenta        MAGENTA
C    180 orange    orange     pink           pink
C    184 orange    orange     pink orange    pink orange
C    188 YELLOW    yellow     lite yellow    WHITE
C
C--********************************************************************
*/

#include "std.h"
#include "x.h"
#include "common.h"
#include "routines.h"

 /*************************  defluts   *******************************/

Colormap defluts(int dummy) 
 {

 Colormap        mappriv, mapdef;
 int             i, it, nramp, idepth, tofree;
 unsigned short  ival;
 XVisualInfo     visual_data;
 XColor          colors[MAXCOL];
 float           fval;
 unsigned long   cells[MAXCOL];
 Visual          *visualdef; 
 int             screen;
 unsigned long   plane_mask; 

 int             icol;               // First spider color
 int             nl   = 2;           // # of shades 
 //unsigned long   nlooks[] = {0, 128, 192, 255};
 unsigned long   nlooks[] = {0, 255};
 int             nr,ng, nb ;

 int             nplanes = 0;
 Status          ires;
 
 screen    = DefaultScreen(idispl);
 visualdef = DefaultVisual(idispl, screen);
 mapdef    = DefaultColormapOfScreen(iscreen);
 idepth    = DefaultDepthOfScreen(iscreen);

 printf(" Web ----------------------\n" );
 printf(" Default Screen:     %d  \n",screen);
 printf(" Screen depth:       %d  \n",idepth);
 printf(" Default visual:     %#X \n",visualdef);
 printf(" Default colormap:   %#X \n",mapdef );


 // Write out private colormap info 
 //lut_info("shared",mapdef,24,0,256); 

 // Check depth of screen (number of bit planes) 
 if(idepth < 8)
    {fprintf(stderr,"*** Screen depth is too shallow!"); 
     exit(1); }

 // Get a directcolor visual for colormap use  ------------ DirectColor

 if (!XMatchVisualInfo(idispl, XDefaultScreen(idispl), idepth,
                       DirectColor, &visual_data))
    {fprintf(stderr,"*** No %d bit, directcolor visual!",idepth);  
     exit(2);  }

 ncolors = visual_data.colormap_size;
 visual  = visual_data.visual;

 // Create new private colormap
 mappriv = XCreateColormap(idispl, RootWindowOfScreen(iscreen), 
                           visual, AllocNone);
 if (!mappriv)
    { fprintf(stderr,"*** Failed to create private colormap!");  
      exit(3);  }
 printf(" Private map:        %#x \n",mappriv );
 printf(" # of colors:        %d  \n",ncolors);
 

 ires = XAllocColorCells(idispl, mappriv, TRUE, 
                         NULL, nplanes, cells, ncolors);
 if (!ires)
    { fprintf(stderr,"*** Failed to allocate %d colors!\n",
      ncolors);  exit(4);  }

 // Allocate colors in the Directcolor visual   
 mapgo    =  64;                    // Need >= 64 levels for ramp 
 mapend   = 127;  
 colorgo  = 128;
 colorend = 252;

 nramp    = mapend - mapgo + 1;     // # of grey levels  
 fval     = 65535.0 / (nramp - 1);

 // Query existing default colormap colors 
 for (i=0; i <256; i++)
    {
    colors[i].pixel = cells[i];
    ispicol[i]      = cells[i];
    colors[i].flags = DoRed|DoGreen|DoBlue; 
    ispicoln[i]     = i;     // Used in colmen
    }
 ires = XQueryColors(idispl, mapdef, colors, 256);
 if (!ires)
    {fprintf(stderr,"*** Failed to query colors! \n");  
     exit(2); }

#ifdef DEBUG
 // Debug info
 for (i = 0; i < ncolors; i=i+4)
    {
    printf(" ispicol(%3d..%3d) : %#8x  %#8x   %#8x   %#8x \n",
             i,i+3, ispicol[i],   ispicol[i+1],  
                    ispicol[i+2], ispicol[i+3]) ; 
    }
#endif

 // Set colors in the image portion of the colormap 
 for (i = mapgo; i <= mapend; i++)
    {
    ival            = (unsigned short)(i - mapgo) * fval;
    colors[i].red   = ival;
    colors[i].green = ival;
    colors[i].blue  = ival;    
    colors[i].flags = DoRed|DoGreen|DoBlue;
    }

 // Place grey ramp and colors in the private colormap 
 ires = XStoreColors(idispl,mappriv,colors,ncolors);
 if (!ires)
    {fprintf(stderr,"*** Failed to store colors! \n");  
     exit(2);  }

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

 // Make cells available for the window manager  
 //tofree = 1,2          : bad slider  
 //tofree = 4            : bad widget
 //tofree = 63,8,127,255 : works OK
 //tofree = 256          : invisible widget
 //tofree = 257,500      : crashes bad free
 //tofree = 64           : crashes in contrast
   tofree = 63;
 //  tofree = 31;

 // Free colors below image ramp 
 plane_mask = 0 ; 

 if (tofree > 0)
    {
    XFreeColors(idispl,  mappriv,(cells+1), tofree, plane_mask);  
    //XFreeColors(idispl,mappriv, cells,    tofree, plane_mask); // bad 
    if (!ires)
       {fprintf(stderr,"*** Failed to free lower colors! \n");  
        exit(2);  }
    //printf(" Freed cells:       %3d,   From: %#8x  Using mask: %#x \n",
    //            tofree,cells[1], plane_mask);
    } 
 XFlush(idispl);

 // Free colors above colorend 
 tofree = 128;
tofree = 0;

 if (tofree > 0)
    {
    ires = XFreeColors(idispl, mappriv, (cells+colorgo), 
                       tofree, 0);
    if (!ires)
      {fprintf(stderr,"*** Failed to free upper colors! \n");  exit(2);  }

    //printf(" Freed cells:        %3d,  From: %#8x  Using mask: %#x \n",
    //               tofree,cells[colorgo], plane_mask);
    }
 XFlush(idispl);

 // Write out private colormap info 
 //lut_info("Private",mappriv,24,0,256); 

 return mappriv;
}




 /*************************  defluts    *******************************/

 
Colormap defluts_tryout(int dummy) 
 {

 Colormap        mappriv, mapdef;
 int             i, it, nramp,  ncolors, tofree;
 unsigned short  ival;
 XVisualInfo     visual_data;
 XColor          colors[256];
 float           fval;
 unsigned long   cells[256];
 Visual          *visualdef; 
 int             screen;
 
 int red[] = {
      0, 24247, 28834, 33421, 38008, 42595, 47182, 51769, 56356, 65535,
  19660, 24247, 28834, 33421, 38008, 42595, 47182, 51769, 56356, 65535,
      0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
  28784, 12850,     0,     0, 24415, 16962, 27499, 49087, 36751, 12850,
  32639, 12079,  8995,  8995, 12850,     0,  8995, 65535,     0, 36494,
  42405, 62708, 52428, 56283, 60138,     0, 12079, 20303,  8995, 12850,
  27499, 16962, 32639, 36751,  8995,     0, 39321, 12079, 12079, 21588,
  21588, 43176, 43176, 49344, 49344, 40863, 65535, 36494, 52428, 56283,
  39321, 37779, 48316, 60138, 65535, 20303, 56283, 65535, 52428, 28527,
  36494, 56283, 55512, 44461, 28784, 28784, 20303, 40863, 55512, 65535,
  65535, 37779,     0,     0,     0,     0,     0,     0,     0,     0,
      0,     0,     0,     0,     0,     0,     0,     0 
 };
 int green[] = {
      0, 24247, 28834, 33421, 38008, 42595, 47182, 51769, 56356, 65535,
      0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
  19660, 24247, 28834, 33421, 38008, 42595, 47182, 51769, 56356, 65535,
      0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
  56283, 52428,     0,     0, 40863, 16962,  8995, 55512, 36751, 12850,
      0, 12079,  8995,  8995, 39321, 32639, 27499, 32639, 65535,  8995,
  10794, 42148, 32639, 56283, 60138, 65535, 20303, 20303, 36494, 52428,
  36494, 28527, 65535, 48316, 36494, 65535, 52428, 20303, 20303, 21588,
  21588, 43176, 43176, 49344, 49344, 40863,     0,  8995, 12850, 28784,
  12850, 28784, 36751, 44461,     0, 12079, 28784,     0, 12850, 16962,
  27499, 37779, 49087, 60138, 37779, 56283, 12079, 24415, 55512, 65535,
  65535, 56283,     0,     0,     0,     0,     0,     0,     0,     0,
      0,     0,     0,     0,     0,     0,     0,     0 
 };
 int blue[] = {
      0, 24247, 28834, 33421, 38008, 42595, 47182, 51769, 56356, 65535,
      0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
  19660, 24247, 28834, 33421, 38008, 42595, 47182, 51769, 56356, 65535,
  37779, 39321,     0, 65535, 40863, 28527, 36494, 55512, 48316, 52428,
  65535, 20303, 36494, 36494, 52428, 65535, 36494,     0, 65535,  8995,
  10794, 24672, 12850, 28784, 44461,     0, 12079, 12079,  8995, 12850,
   8995, 16962,     0, 36751, 27499, 32639, 12850, 20303, 20303, 21588,
  21588, 43176, 43176, 49344, 49344, 24415, 65535, 27499, 12850, 56283,
  52428, 56283, 36751, 60138,     0, 12079, 37779, 32639, 39321, 16962,
   8995, 28784, 55512, 60138, 56283, 56283, 20303, 40863, 49087, 65535,
      0, 28784,     0,     0,     0,     0,     0,     0,     0,     0,
      0,     0,     0,     0,     0,     0,     0,     0 
 };

 screen    = DefaultScreen(idispl);
 visualdef = DefaultVisual(idispl, screen);
 mapdef    = DefaultColormapOfScreen(iscreen);
 idepth    = DefaultDepthOfScreen(iscreen);

 printf("\n" );
 printf(" Default Screen:     %d   ---------------------- \n",screen);
 printf(" Default depth:      %d  \n",idepth);
 printf(" Default visual:     %#X \n",visualdef);
 printf(" Default colormap:   %#X \n",mapdef );

 /* find depth of screen (number of bit planes) */
 if((DefaultDepthOfScreen(iscreen)) <= 8)
    {fprintf(stderr,"*** Screen depth is too shallow!"); return FALSE; }

 /* get a directcolor visual for colormap use */
 if (!XMatchVisualInfo(idispl, XDefaultScreen(idispl),
                       DefaultDepthOfScreen(iscreen),
                       DirectColor, &visual_data))
      { fprintf(stderr,"*** No %d bit, directcolor visual!",
                          DefaultDepthOfScreen(iscreen));   return FALSE; }

 visual  = visual_data.visual;
 ncolors = visual_data.colormap_size;
 mapdef  = DefaultColormapOfScreen(iscreen);

 mappriv = XCreateColormap(idispl, RootWindowOfScreen(iscreen),
                              visual, AllocNone);
 if (mappriv == mapdef)
   {fprintf(stderr,"*** Can not create private colormap!"); return FALSE; }

 mapgo    = 64;  /* Need >= 64 colors for ramp  */
 mapend   = 127;  
 colorgo  = 128;
 colorend = 239;

 ncolors  = 256;
 nramp    = mapend - mapgo + 1; //  nramp is number of grey levels available 
 fval     = 65535.0 / (nramp - 1);

 int  nplanes = 0;
 
 // allocate 256 colors in the visual requested 
 XAllocColorCells(idispl, mappriv, TRUE, NULL, nplanes, cells, ncolors);

 /* query existing default colors used by window system */
 for (i=0; i <256; i++)
    {
    //printf(" Cell(%3d): %#x \n",i, cells[i]);
    colors[i].pixel = cells[i];
    ispicol[i]      = cells[i];
    colors[i].flags = DoRed|DoGreen|DoBlue; 
    }
 XQueryColors(idispl, mapdef, colors, 256);

 for (i = 0; i < ncolors; i=i+4)
    {
    printf(" ispicol(%3d..%3d) : %#8x  %#8x   %#8x   %#8x \n",
             i,i+3, ispicol[i],   ispicol[i+1],  
                    ispicol[i+2], ispicol[i+3]) ; 
    }

 // Set colors in the image portion of the colormap ----------
 colorgo  = 128;
 colorend = 128+64-1;

#ifdef NVER
 for (i = mapgo; i <= mapend; i++)
    {
    ival            = (unsigned short)(i - mapgo) * fval;
    colors[i].red   = ival;
    colors[i].green = ival;
    colors[i].blue  = ival;    
    colors[i].flags = DoRed|DoGreen|DoBlue;
    }

 // Set colors in the spider color portion of the colormap ----

 for (i = colorgo; i <= colorend; i++)
    {
    it              = i - colorgo;
    colors[i].red   = red[it];
    colors[i].green = green[it];
    colors[i].blue  = blue[it];
    colors[i].flags = DoRed|DoGreen|DoBlue;
    }
#endif
  // Place grey ramp and colors in the private colormap 
  XStoreColors(idispl,mappriv,colors,256);

 unsigned long  plane_mask = 0 ; 
 // Make some cells available for the window manager later use 
 //tofree = 256=invisible widget; // works: mapgo -1,8,127,255,;  1,2=bad slider  4=bad widget
 //tofree = 257,500; // crashes bad free
 tofree = 64; 
 //tofree = 255; 

 //if (tofree > 0)
   //XFreeColors(idispl, mappriv, (cells+mapgo-tofree), tofree, 0);
   XFreeColors(idispl, mappriv, (cells+1), tofree, plane_mask);     //WORKS!
   // FAILSXFreeColors(idispl, mappriv, cells, tofree, plane_mask);
   //XFreeColors(idispl, mappriv, cells,tofree, plane_mask);
 
 printf(" Freeing %3d cells: %#x  Using mask: %#x  \n",
                  tofree,cells[1], plane_mask);
 XFlush(idispl);

#ifdef NVR
 // Free colors above colorend 
 tofree = 255 - colorend;
 tofree = 64;
 if (tofree > 0)
    XFreeColors(idispl, mappriv, (cells+colorend+1), tofree, 0);
#endif

 // Write out colormap info 
 //lut_info("Private",mappriv,24,0,256); 

 return mappriv;
}





