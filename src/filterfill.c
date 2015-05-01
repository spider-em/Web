
/*$Header: /usr8/web/src/RCS/filterfill.c,v 1.5 2005/10/18 17:00:02 leith Exp $*/

/***********************************************************************
 *
 * filterfill.c
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
 * filterfill()
 * 
 * PURPOSE:    seed fill
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
 
 void flood (int);

 static int     nsamtt, connect8;
 static float   thresht, fvalt;
 static float * newimaget;
 static float * oldimaget;
 static int   * visited;

 /**************************  filterfill *****************************/

 int filterfill(float * oldimage, int nsamt, int nrowt, 
             int ixseed, int iyseed, float thresh, float fval, 
             float * newimage, float * fmint, float * fmaxt,
             int connectivity)
 {
 int     nrowkd2, nsamkd2;
 float * endimage;
 int   * endimagei;
 float * fptr;
 int     ip, it, iloc;

 /* set border size */
 nrowkd2 = 1;
 nsamkd2 = 1;

 thresht   = thresh;
 fvalt     = fval;
 newimaget = newimage;
 oldimaget = oldimage;
 nsamtt    = nsamt;
 connect8  = TRUE;
 if (connectivity == 4) connect8 = FALSE;

 /* copy oldimage to newimage */
 memcpy((void *) newimage,(void *) oldimage, nsamt*nrowt*4);
 
 /* allocate visited space */
 if ((visited = (int *) calloc(nsamt * nrowt ,
                      sizeof(int))) == (int *) 0)
    {spout("*** Unable to allocate visited in filterfill."); 
     return FALSE;}

 /* fill top and bottem borders of visited to prevent going out */
 /* borders of the image */

 endimagei = visited + nsamt * nrowt - 1;

 for(ip = 0; ip < nsamt * nrowkd2; ip++)
     {
     *(visited   + ip) = TRUE;
     *(endimagei - ip) = TRUE;
     }

 /* fill left and right borders */
 for(it = 0; it < nsamkd2; it++)
     {    
     for(ip = it; ip < nsamt*nrowt; ip=ip+nsamt)
        {
        *(visited   + ip) = TRUE; 
        *(endimagei - ip) = TRUE;
        }
     }

 /* do a flood fill around seed location */

 iloc = (iyseed-1) * nrowt + ixseed - 1;
 flood(iloc);

 /* fill in border with fval value */
 endimage = newimage + nsamt * nrowt - 1;

 /* fill top and bottem borders */
 for(ip = 0; ip < nsamt * nrowkd2; ip++)
     {
     *(newimage + ip) = fvalt;
     *(endimage - ip) = fvalt;
     }

 /* fill left and right borders */
 for(it = 0; it < nsamkd2; it++)
     {    
     for(ip = it; ip < nsamt*nrowt; ip=ip+nsamt)
        {
        *(newimage + ip) = fvalt; 
        *(endimage - ip) = fvalt;
        }
     }

 /* check min and max on final image */
 for (fptr= newimage; fptr < newimage + nsamt * nrowt; fptr++)
      {
      if (*fptr < *fmint) *fmint = *fptr;
      if (*fptr > *fmaxt) *fmaxt = *fptr;
      }

 if (visited) 
     {free(visited); visited = NULL; }

 return TRUE;
 }

 /**************************  flood *****************************/

 void flood (int iloc)
 {

 if (!visited[iloc] && oldimaget[iloc] < thresht)
    {
    /* set pixel to visited */
    visited[iloc] = TRUE;

    /* set this pixel to fval */
    newimaget[iloc]  = fvalt;

    /* call flood on all 8 bordering pixels */
    flood(iloc - 1     );
    flood(iloc - nsamtt);
    flood(iloc + 1     );
    flood(iloc + nsamtt);

    if (connect8)
       {   /* use 8 fold connectivity */
       flood(iloc - nsamtt - 1);
       flood(iloc - nsamtt + 1);
       flood(iloc + nsamtt + 1);
       flood(iloc + nsamtt - 1);
       }
    }
 else
    {
    /* set pixel to visited */
    visited[iloc] = TRUE;
    }
 return;
 }


