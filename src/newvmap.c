
/*
 * newvmap.c   New                               Sep 2011 ArDean Leith *
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
 * newvmap
 * 
 * PURPOSE:     Fills a virtual colormap from: 0...255 using
 *              mapgo...mapend ramp limits 
 *
 * RETURNS:     void
 * 
 * PARAMETERS:     
 *
 ***********************************************************************
*/

#include "x.h"
#include "std.h"

 // Functions used here

 // External common variables
 extern int            mapgo,mapend;      // Image start & end in colormap 

 // Internally defined common variables
 int                   vmap[256];          // Virtual colormap

 void newvmap(int mapgot,int mapendt, int * vmap)

  { // Create new virtual map

  int i;
        
  for (i=0; i < 256; i++)
     {
     if (i < mapgot)
         vmap[i] = 0;
     else if (i <= mapendt)
         {
         vmap[i] = (i - mapgot) * 255 / (mapendt - mapgot);
         //printf("vmap: [%d]: %d %d %d \n",i,vmap[i]);
         }
     else
         vmap[i] = 255;
     }
  //printf("  New vmap: \n");
  }

