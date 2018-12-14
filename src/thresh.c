
/*$Header: /usr16/software/web/src/RCS/thresh.c,v 1.4 2018/12/07 17:03:34 leith Exp $*/

/***********************************************************************
 *
 * thresh.c
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
 * thresh()
 * 
 * PURPOSE:    Thresholds an image
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

 /******************************  thresh ******************************/

 int thresh(float * oldimage, int nsamt, int nrowt, 
             float lower, float lowerval,
             float upper, float upperval,
             float * newimage, float * fmint, float * fmaxt)
 {
 float   oldval;
 int     iloc;

 *fmint = FLT_MAX;
 *fmaxt = FLT_MIN;

 /* convolve over all positions in the new image */

 for (iloc = 0; iloc < nrowt * nsamt; iloc++)
       {
       oldval = oldimage[iloc];

       if (oldval <= lower) 
          {
          newimage[iloc] = lowerval;
          *fmaxt         = MYMAX(*fmaxt,lowerval);
          *fmint         = MYMIN(*fmint,lowerval);
          }
  
       else if (oldval > upper)
          {
          newimage[iloc] = upperval;
          *fmaxt         = MYMAX(*fmaxt,upperval);
          *fmint         = MYMIN(*fmint,upperval);
          }

       else
          {
          newimage[iloc] = oldval;
          *fmaxt         = MYMAX(*fmaxt,oldval);
          *fmint         = MYMIN(*fmint,oldval);
          }
        }

 return TRUE;
 }


