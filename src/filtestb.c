
/*$Header: /usr16/software/web/src/RCS/filtestb.c,v 1.3 2005/10/18 16:59:48 leith Exp $*/

/*
C++*********************************************************************
C
C filtestb
C
C **********************************************************************
C *  AUTHOR:  ArDean Leith                                                 *
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
C    filtestb
C
C    PURPOSE:         replaces outvals with distance to nearest inval
C
C    CALL TREE:
C                  
C    imagemen -- imagemen_cb  -- imagemen1-- filtermen --> filtestb 
C
C***********************************************************************
*/

#include "common.h"
#include "routines.h"

 /* external global variables used here */

 /* common global used elsewhere */

 /* common variables used here */


/******************************  filtestb *****************************/

 int filtestb(float * oldimage, int nsamt, int nrowt,
              float   in,        float out, 
              float * newimage, float * fmint, float * fmaxt)
 {
 int     iloc, nsizei, nsizeo, xot, yot, ipointo,ipointi, inval, outval;
 int     * xo = 0, * yo = 0, * xi = 0, * yi = 0, * ic = 0;
 float   dist, distmin, distmax;

 /* this could be speeded up but it is a one shot pgm, so not worth it*/
 nsizei  = -1;
 nsizeo  = -1;

 ipointo = -1;
 ipointi = -1;

 for (iloc = 0; iloc < nsamt * nrowt; iloc++)
     {
     /* fill newimage with negatives  */
     newimage[iloc] = -1.0;

     if (oldimage[iloc] == out) 
        {
        if ((++ipointo) > nsizeo)
           {  /* must get more array memory  */
           nsizeo += 1000;

           if (((xo = (int *) realloc(xo,nsizeo * sizeof(int))) == (int) 0) ||
               ((yo = (int *) realloc(yo,nsizeo * sizeof(int))) == (int) 0) ||
               ((ic = (int *) realloc(ic,nsizeo * sizeof(int))) == (int) 0))
                { 
                spout("*** Unable to alloc outer arrays in filtestb.c.");
                return FALSE;
                }
           }
        xo[ipointo] = iloc / nsamt;
        yo[ipointo] = iloc % nsamt;
        ic[ipointo] = iloc;
        }

     else if (oldimage[iloc] == in)
        {
        if ((++ipointi) > nsizei)
           {  /* must get more array memory  */
           nsizei += 1000;
           if (((xi = (int *) realloc(xi,nsizei * sizeof(int))) == (int *) 0) ||
               ((yi = (int *) realloc(yi,nsizei * sizeof(int))) == (int *) 0))

                { 
                spout("*** Unable to realloc inner space in filtestb.c.");
                return FALSE;
                }
          }

        xi[ipointi] = iloc / nsamt;
        yi[ipointi] = iloc % nsamt;
        }
     }

 distmax = (nsamt + nrowt) * (nsamt + nrowt);
 *fmaxt  = -distmax;

 for (outval =0; outval <= ipointo; outval++)
     {
     distmin = distmax;

     xot     = xo[outval]; 
     yot     = yo[outval];

     for (inval =0; inval <= ipointi; inval++)
         {
         dist = ((xot - xi[inval]) * (xot - xi[inval]) +
                 (yot - yi[inval]) * (yot - yi[inval]));

         if (dist < distmin) distmin = dist;
         }
     distmin              = sqrt(distmin);
     newimage[ic[outval]] = distmin;
     *fmaxt               = MYMAX(*fmaxt,distmin);
     }

  * fmint = -1.0;

  if (xo) free(xo);
  if (xi) free(xi);
  if (yo) free(yo);
  if (yi) free(yi);
  if (ic) free(ic);

  return TRUE;
  }


