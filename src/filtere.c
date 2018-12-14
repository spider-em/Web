
/*$Header: /usr16/software/web/src/RCS/filtere.c,v 1.9 2018/12/07 17:03:31 leith Exp $*/

/***********************************************************************
 *
 * filtere.c
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
 * filtere()
 * 
 * PURPOSE:    convolves an floating point image with a kernal 
 *             performing an erosion or dilation at specified
 *             connectivity and threshold.
 *
 * RETURNS:    true or false
 *
 * PARAMETERS: 
 *
 ***********************************************************************
*/

#ifndef VMS
#include <float.h>
#endif
#include "std.h"
#include "routines.h"
#include "filter.h"

 /******************************  filtere **************************/

 int filtere(float * oldimage, int nsamt, int nrowt, 
             int erode, int coeff, float threshold, 
             float * newimage, float * fmint, float * fmaxt)
 {
 int     ix, iy,   nrowkd2, nsamkd2, it;
 float   oldval;
 int     icon3, icon4;
 int     ip, hit;
 float * endimage;
 float   fmino,fmaxo;

 nrowkd2 = 1;
 nsamkd2 = 1;
 fmino   = *fmint;
 fmaxo   = *fmaxt;

 *fmint = FLT_MAX;
 *fmaxt = FLT_MIN;

 /* convolve over all positions in the new image except border */

 for (iy = 1; iy < nrowt - 1; iy++)
    {
    icon3 = iy * nsamt;

    for (ix = 1; ix < nsamt - 1; ix++)
       {
       icon4 = icon3 + ix;
       oldval = oldimage[icon4];

       if (erode  && (oldval < threshold)) 
          {
          newimage[icon4] = fmino;
          *fmaxt          = MYMAX(*fmaxt,fmino);
          *fmint          = MYMIN(*fmint,fmino);
          }
  
       else if (!erode && (oldval >= threshold))
          {
          newimage[icon4] = oldval;
          *fmaxt          = MYMAX(*fmaxt,oldval);
          *fmint          = MYMIN(*fmint,oldval);
          }

       else
          {
          hit   = 0;
          /* convolve over all positions in the kernal */
          if (oldimage[icon4 - 1        ] >= threshold) hit++;
          if (oldimage[icon4 - nsamt - 1] >= threshold) hit++;
          if (oldimage[icon4 - nsamt    ] >= threshold) hit++;
          if (oldimage[icon4 - nsamt + 1] >= threshold) hit++;
          if (oldimage[icon4 + 1        ] >= threshold) hit++;
          if (oldimage[icon4 + nsamt + 1] >= threshold) hit++;
          if (oldimage[icon4 + nsamt    ] >= threshold) hit++;
          if (oldimage[icon4 + nsamt - 1] >= threshold) hit++;

          if (erode  && (hit > coeff))
             {
             newimage[icon4] = oldval;
             *fmaxt          = MYMAX(*fmaxt,oldval);
             *fmint          = MYMIN(*fmint,oldval);
             }
          else if (!erode && (hit > coeff))
             {
             newimage[icon4] = fmaxo;
             *fmaxt          = MYMAX(*fmaxt,fmaxo);
             *fmint          = MYMIN(*fmint,fmaxo);
             }
          else
             {
             newimage[icon4] = fmino;
             *fmaxt          = MYMAX(*fmaxt,fmino);
             *fmint          = MYMIN(*fmint,fmino);
             }

          }
        }
     }

  /* fill in border with minimum value */

  endimage = newimage + nsamt * nrowt - 1;

  /* fill top and bottem borders */
  for(ip = 0; ip < nsamt * nrowkd2; ip++)
     {
     *(newimage + ip) = fmino;
     *(endimage - ip) = fmino;
     }

  /* fill left and right borders */
   for(it = 0; it < nsamkd2; it++)
     {    
     for(ip = it; ip < nsamt*nrowt; ip=ip+nsamt)
        {
        *(newimage + ip) = fmino; 
        *(endimage - ip) = fmino;
        }
     }

  return TRUE;
}


