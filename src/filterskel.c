
/*$Header: /usr8/web/src/RCS/filterskel.c,v 1.2 2005/10/18 16:59:59 leith Exp $*/

/***********************************************************************
 *
 * filterskel.c
 *
 ***********************************************************************
 * author : ArDean Leith
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
 * filterskel()
 * 
 * PURPOSE:    findis skeleton of a binary image.
 *
 * RETURNS:    true or false
 *
 * PARAMETERS: 
 *
 ***********************************************************************
*/

#include "std.h"
#ifndef VMS
#include "float.h"
#endif
#include "routines.h"
#include "common.h"

 static char*    cimage     = NULL;

 /******************************  filtere *****************************/

 int filterskel(float * oldimage, int nsamt, int nrowt, 
             float * newimage, float * fmint, float * fmaxt)
 {
 int     ix, iy,   ipixel;
 int     icon3, iloc, diffpixel, iok;
 float * tmpimage;
 float * eroded;
 float * dilated;
 float * ploc;
 float   fmino,fmaxo;

 fmino = * fmint;
 fmaxo = * fmaxt;

 /* allocate tmpimage, eroded, dilated */
 if ((tmpimage = (float *) malloc(nsamt * nrowt *
                      sizeof(float))) == (float *) 0)
    {spout(" *** Unable to allocate tmpimage in filterskel.");
     return FALSE;}

 if ((eroded = (float *) malloc(nsamt * nrowt *
                      sizeof(float))) == (float *) 0)
    {spout(" *** Unable to allocate eroded in filterskel."); 
     return FALSE;}

 if ((dilated = (float *) malloc(nsamt * nrowt *
                      sizeof(float))) == (float *) 0)
    {spout(" *** Unable to allocate dilated in filterskel."); 
     return FALSE;}

 /* copy oldimage to tmpimage */
 memcpy((void *) tmpimage,(void *) oldimage, nsamt*nrowt*4);
 
 /* set newimage to zero */
 for (ploc = newimage; ploc < newimage + (nsamt * nrowt); ploc++)
     {
     *ploc = 0.0;
     }

 /* convolve over all positions in the new image except border */
 diffpixel = TRUE;

 while (diffpixel) 
    {  
    diffpixel = FALSE;

    iok = filtere(tmpimage, nsamt, nrowt, TRUE, 7, *fmaxt,
                  eroded , fmint, fmaxt);

    iok = filtere(eroded, nsamt, nrowt, FALSE, 0, *fmaxt,
                  dilated , fmint, fmaxt);

    for (iy = 1; iy < nrowt - 1; iy++)
       {
       icon3 = iy * nrowt;

       for (ix = 1; ix < nsamt - 1; ix++)
          {
          iloc           = icon3 + ix;
          ipixel         = tmpimage[iloc] - dilated[iloc];
          newimage[iloc] = (int) newimage[iloc] | ipixel;
          /*  if (ipixel > 0) diffpixel = TRUE;  */
          if (eroded[iloc] > 0.0) diffpixel = TRUE;
          tmpimage[iloc] = eroded[iloc];
          }
       }

    /***
     showbimage(newimage, FALSE, &cimage, FALSE, &imagel, 
                nsamt, nrowt, fmino, fmaxo);
    **********/
    } 

 *fmint = fmino;
 *fmaxt = fmaxo;

 if (tmpimage) free(tmpimage); tmpimage = NULL;
 if (eroded)   free(eroded);   eroded = NULL;
 if (dilated)  free(dilated);  dilated = NULL;

 return TRUE;
 }


