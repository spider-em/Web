
/*$Header: /usr8/web/new/RCS/witran.c,v 1.6 2005/10/18 16:59:41 leith Exp $*/

/*
 ***********************************************************************
 *
 * witran.c
 * ported from witran.for by Jing Su
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
 *  PROGRAM TO APPLY TRANSFORMATION TO X,Y AND OUTPUT XS,YS
 *
 ***********************************************************************
*/

#include "std.h"

/***************************** witran ****************************/

void witran(float * x, float * y, float * xs, float * ys, int n,
            float gam, float the, float phi)
{
float        rgam, rthe, rphi, cgam, sgam, cphi, sphi, cthe;
float        cthecphi, cthesphi;
extern float xu0t, yu0t, xs0t, ys0t;
int          i;

const float pid =  (3.1415927 / 180.0);

rthe     = the * pid;
rphi     = phi * pid;
rgam     = gam * pid;

cgam     = cos(rgam);
sgam     = sin(rgam);
cphi     = cos(rphi);
sphi     = sin(rphi);
cthe     = cos(rthe);

cthecphi = cthe * cphi;
cthesphi = cthe * sphi;

for (i = 0; i < n; i++)
    {
    xs[i] =  ((x[i] - xu0t) * cgam - (y[i] - yu0t) * sgam) * cthecphi
           + ((x[i] - xu0t) * sgam + (y[i] - yu0t) * cgam) * sphi + xs0t;

    ys[i] = -((x[i] - xu0t) * cgam - (y[i] - yu0t) * sgam) * cthesphi
           + ((x[i] - xu0t) * sgam + (y[i] - yu0t) * cgam) * cphi +ys0t;
    }

return;
}


