
/*$Header: /usr16/software/web/src/RCS/filter.c,v 1.7 2018/12/07 17:03:31 leith Exp $*/

/***********************************************************************
 *
 * filter.c
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
 * filter()
 * 
 * PURPOSE:    convolves an floating point image with a kernal.
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

 /* external function prototypes */

 /* external common variables */
 

 /************************************************************/

 int filter(float * oldimage, int nsamt, int nrowt, 
            float * kernal,   int nsamk, int nrowk, 
            float * newimage, float * fmint, float * fmaxt)
 {
 int     ix, iy,  ixk, iyk, nrowkd2, nsamkd2, it;
 float   val,valk,vali, ksum;
 int     icon1, icon2, icon3, icon4, icon5;
 int     ixo,iyo;
 int     ip;
 float * pf;
 float * endimage;

 if (nsamk % 2 == 0 || nrowk % 2 == 0)
    {
    spout("*** Filter kernal size must be odd!");
    return FALSE;
    }

 nrowkd2 = nrowk / 2;
 nsamkd2 = nsamk / 2;

 /* sum up kernal factors */
 ksum = 0.0;
 for (pf = kernal; pf < kernal + nsamk * nrowk; pf++)
    ksum += *pf;

/*******************
 printf("ksum: %f \n",ksum);
********/

 *fmint = FLT_MAX;
 *fmaxt = FLT_MIN;

 /* convolve over all positions in the new image except border */

 for (iy = nrowkd2; iy < nrowt - nrowkd2; iy++)
    {
    icon5 = iy * nsamt;
    icon3 = iy - nrowkd2;

    for (ix = nsamkd2; ix < nsamt - nsamkd2; ix++)
       {
       icon2 = ix - nsamkd2;
       val   = 0.0;

       /* convolve over all positions in the kernal */
       for (iyk = 0; iyk < nrowk; iyk++)
          {
          icon1    = iyk * nrowk;
          /* iyo   = icon3 + iyk; */
          /* icon4 = icon2 + iyo * nrowt; */
          icon4    = icon2 + (icon3 + iyk) * nsamt;

          for (ixk = 0; ixk < nsamk; ixk++)
             {
             /* get current kernal value     */
             /* valk = kernal[icon1 + ixk];  */
             /* ixo = ix - nsamkd2 + ixk;    */
             /* get current image value      */
             /* vali = oldimage[icon4 + ixo];*/

             val += kernal[icon1 + ixk] * oldimage[icon4 + ixk];
             }
          }
       newimage[icon5 + ix] = val;

       *fmaxt = MYMAX(*fmaxt,val);
       *fmint = MYMIN(*fmint,val);
       }
     }

  /* fill in border with minimum value */

  endimage = newimage + nsamt * nrowt - 1;

  /* fill top and bottem borders */
  for(ip = 0; ip < nsamt * nrowkd2; ip++)
     {
     *(newimage + ip) = *fmint;
     *(endimage - ip) = *fmint;
     }

  /* fill left and right borders */
   for(it = 0; it < nsamkd2; it++)
     {    
     for(ip = it; ip < nsamt*nrowt; ip=ip+nsamt)
        {
        *(newimage + ip) = *fmint; 
        *(endimage - ip) = *fmint;
        }
     }

  return TRUE;
}


