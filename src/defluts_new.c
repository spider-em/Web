
/*$Header: /usr16/software/web/src/RCS/defluts_new.c,v 1.1 2011/05/05 12:04:47 leith Exp $*/
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
C    PURPOSE:        TO CREATE COLORMAPS AND SET THE SPIDER 
C                    COLOR LOOK-UP-TABLES IN THE MAPS.
C                    THE COLOR MAP HAS A GREY RAMP IN mapgo...mapend AND
C                    SPIDER COLOR SHADES IN colorgo...colorend.
C                    POSITIONS STARTING AT 0 ARE OFTEN USED BY WINDOW 
C                    MANAGER.  IF so, THE NUMBER OF IMAGE GREY LEVELS 
C                    SHOULD BE REDUCED TO FIT IN THE SPACE ABOVE THE 
C                    RESERVED NUMBERS AND BELOW NUMBER 127.
C                    (THIS MECHANISM IS USED TO PRESERVE OVERLAY IN 
C                    LAST PLANE).  THE COLORMAP SETS mapend - mapgo +1
C                    GREY LEVELS AND <=128 SPIDER COLORS AND ATTEMPTS
C                    TO PRESERVE BACKGROUND & FOREGROUND COLORS USED BY
C                    THE WINDOW MANAGER. (IT MAY NOT SUCCEED)
C
C    PARAMETERS:     NONE CURRENTLY USED
C
C    CALLED BY:      web_main
C
C    CALLS:     
C
C--********************************************************************
*/

#include "std.h"
#include "x.h"
#include "common.h"
#include "routines.h"

#define  MAXCOLT 256

 /*************************  defluts   *******************************/

 Colormap defluts(int dummy) 
 {

 Colormap        mappriv, mapdef;
 int             i, it, nramp, idepth, istart, tofree;
 unsigned short  ival;
 XVisualInfo     visual_data;
 XColor          colors[MAXCOLT];
 float           fval;
 unsigned long   cells[MAXCOLT];
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
 printf(" Default colormap:   %#X \n",mapdef );

 // Check depth of screen (number of bit planes) 
 if(idepth < 8)
    {fprintf(stderr,"*** Screen depth is too shallow!"); 
     return FALSE; }

#ifndef WEB_24
 // Get a 8-bit pseudocolor visual for colormap use ------- PseudoColor
 if (!XMatchVisualInfo(idispl, XDefaultScreen(idispl), 8,
                       PseudoColor, &visual_data)) 
    { fprintf(stderr,"*** No 8 bit, pseudocolor visual!"); 
      return FALSE;  }
 ncolors = 256;
#else
 // Get a directcolor visual for colormap use  ------------ DirectColor
 if (!XMatchVisualInfo(idispl, XDefaultScreen(idispl), depth,
                       DirectColor, &visual_data))
    {fprintf(stderr,"*** No %d bit, directcolor visual!",depth);  
     return FALSE;  }
 ncolors = visual_data.colormap_size;
#endif

 visual  = visual_data.visual;
 ncolors = visual_data.colormap_size;

 // Create new private colormap
 mappriv = XCreateColormap(idispl, RootWindowOfScreen(iscreen), 
                           visual, AllocNone);
 if (!mappriv)
    { fprintf(stderr,"*** Failed to create private colormap!");  
      return FALSE;  }

 printf(" ncolors:            %d  \n",ncolors);
 printf(" Private map:        %#x \n",mappriv );
 
 if (! XAllocColorCells(idispl, mappriv, TRUE, 
                        NULL, nplanes, cells, ncolors);
    { fprintf(stderr,"*** Failed to allocate %d colors!",ncolors);  
      return FALSE;  }

#ifdef WEB_24
 
 // Allocate colors in the Directcolor visual ------------ DirectColor  
 int nplanes = 0;
 if (! XAllocColorCells(idispl, mappriv, TRUE, 
                        NULL, nplanes, cells, ncolors);
    { fprintf(stderr,"*** Failed to alloc %d colors!",ncolors);  
      return FALSE;  }

 mapgo    = 64;   // Need >= 64 colors for ramp 
 mapend   = 127;  
 colorgo  = 128;
 colorend = 239;

 nramp    = mapend - mapgo + 1; // nramp is number of grey levels available 
 fval     = 65535.0 / (nramp - 1);

#else
 // Allocate colors --------------------------------------- PseudoColor

/* Make a colormap by looking at default map used positions */
 querylut(mapdef, &mapgo,&mapend, &colorgo,&colorend);

 /* Ensure that enough ramp (64) & colors (100) are available anyway */
 if (mapgo > 64)  mapgo = 64;  /* need >= 64 colors for ramp  */
 colorend = 239;

 /* query existing default colors used by window system */
 for (i=0; i <MAXCOLT; i++)
    {
    colors[i].pixel = i;
    ispicol[i]      = i;
    colors[i].flags = DoRed|DoGreen|DoBlue; 
    }
 XQueryColors(idispl, mapdef, colors, MAXCOLT);


 // set colors in the image portion of the colormap
    
 // nramp is number of grey levels available */
 nramp = mapend - mapgo + 1;
 fval = 65535.0 / (nramp - 1);

 for (i = mapgo; i <= mapend; i++)
    {
    ival            = (unsigned short)(i - mapgo) * fval;
    colors[i].red   = ival;
    colors[i].green = ival;
    colors[i].blue  = ival;    
    }

 // Set colors in the spider color portion of the colormap 

 for (i = colorgo; i <= colorend; i++)
    {
    it = i - colorgo;
    colors[i].red   = red[it];
    colors[i].green = green[it];
    colors[i].blue  = blue[it];
    }

  /* allocate all 256 colors in the visual requested */
  XAllocColorCells(idispl, mappriv, TRUE, NULL, 0, cells, MAXCOLT);

  /* place grey ramp and colors in the private colormap */
  XStoreColors(idispl,mappriv,colors,MAXCOLT);

 /* Now make some cells available for the window manager later use  */
 tofree = mapgo -1;
 if (tofree > 0)
   XFreeColors(idispl, mappriv, (cells+mapgo-tofree), tofree, 0);

 /* free colors above colorend */
 tofree = 255 - colorend;
 if (tofree > 0)
    XFreeColors(idispl, mappriv, (cells+colorend+1), tofree, 0);

#endif


 /* write out colormap info */
 /*  wid_lutinfo(mappriv,256); */

 return mappriv;
}

