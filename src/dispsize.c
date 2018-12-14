
/*$Header: /usr16/software/web/src/RCS/dispsize.c,v 1.8 2018/12/07 17:03:31 leith Exp $*/

/***********************************************************************
 *
 * dispsize.c
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
 * dispsize()
 * 
 * PURPOSE:    finds size of subimage given window from original image
 *
 * RETURNS:    true or false
 *
 * PARAMETERS: 
 *
 ***********************************************************************
*/

 /****************************  dispsize  *****************************/
 
#include <math.h>
#include "routines.h"

 void dispsize(int nsam1t,  int nsam2t, 
               int nrow1t,  int nrow2t, 
               int nslice1t,int nslice2t, int iskipt,
               int * nsams, int * nrows, int * nslices)

 { 
 /* find number of rows in displayed image */
 if (iskipt > 1) 
    {
    *nsams   = ceil((float)(nsam2t - nsam1t + 1) / (float) iskipt);
    *nrows   = ceil((float)(nrow2t - nrow1t + 1) / (float) iskipt);
    *nslices = (nslice2t - nslice1t + 1) ;
    }
 else if (iskipt < -1)
    {
    *nsams   = (nsam2t     - nsam1t + 1) * -iskipt;
    *nrows   = (nrow2t     - nrow1t + 1) * -iskipt;
    *nslices = (nslice2t - nslice1t + 1) ;
    }
 else
    {
    *nsams   = (nsam2t   - nsam1t   + 1);
    *nrows   = (nrow2t   - nrow1t   + 1);
    *nslices = (nslice2t - nslice1t + 1);
    } 

 return;
}

