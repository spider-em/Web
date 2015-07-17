
/*$Header: /usr8/web/src/RCS/witran.c,v 1.7 2015/06/11 13:27:14 leith Exp $*/

/*
 ***********************************************************************
 *
 * witran.c                          Ported from witran.for by Jing Su
 *
 ***********************************************************************
 C=* FROM: WEB - VISUALIZER FOR SPIDER MODULAR IMAGE PROCESSING SYSTEM *
 C=* Copyright (C) 1992-2015  Health Research Inc.                     *
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
 *  PURPOSE: Apply transformation to x,y and output xs,ys
 *
 ***********************************************************************
*/

#include "std.h"

// Internal function prototypes
void witran_rev(float *, float *, float,float, int, 
                float, float, float);
 
/***************************** witran ****************************/

void witran(float * x, float * y, float * xs, float * ys, int n,
            float gam, float the, float phi)
{
float        rgam, rthe, rphi, cgam, sgam, cphi, sphi, cthe;
float        cthecphi, cthesphi;
extern float xu0t, yu0t, xs0t, ys0t;
int          i;

float        xb,yb;    // temp!!!!!!!!!!!!!!!

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
            +((x[i] - xu0t) * sgam + (y[i] - yu0t) * cgam) * sphi + xs0t;

    ys[i] = -((x[i] - xu0t) * cgam - (y[i] - yu0t) * sgam) * cthesphi
            +((x[i] - xu0t) * sgam + (y[i] - yu0t) * cgam) * cphi + ys0t;

    witran_rev(&xb,&yb, xs[i], ys[i], n,  gam,the,phi);

    // printf("In: %f, %f --> %f, %f: \n"  ,x[i],y[i], xb,yb);
    }

return;
}

/*
 ***********************************************************************
 *
 *  PURPOSE: Apply transformation to xs,ys and output x,s
 *
 ***********************************************************************
*/

void witran_rev(float * x, float * y, float xs, float ys, int n,
                float gam, float the, float phi)
{
float          rgam, rthe, rphi, cgam, sgam, cphi, sphi, cthe;
float          cthecphi, cthesphi;
extern float   xu0t, yu0t, xs0t, ys0t;
int            i;
float          TXccss, TXsccs, TYsccc, TYccsc, TX, TY;

const float    pid =  (3.1415927 / 180.0);

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

#ifdef NEVER
for (i = 0; i < n; i++)
    {

    xs[i] =   ((x[i] - xu0t) * cgam - (y[i] - yu0t) * sgam) * cthecphi
           +  ((x[i] - xu0t) * sgam + (y[i] - yu0t) * cgam) * sphi + xs0t 

    xs[i] =   ((x[i] * cgam - xu0t * cgam) - (y[i] * sgam - yu0t * sgam)) * cthecphi
           +  ((x[i] * sgam - xu0t * sgam) + (y[i] * cgam - yu0t * cgam)  * sphi + xs0t 

    xs[i] =   ((x[i] * cgam * cthecphi     - xu0t * cgam * cthecphi)  
             - (y[i] * sgam * cthecphi     - yu0t * sgam * cthecphi)
             +((x[i] * sgam * sphi         - xu0t * sgam * sphi)      
             + (y[i] * cgam * sphi         - yu0t * cgam * sphi))    + xs0t

    xs[i] =    x[i] * cgam * cthecphi      - xu0t * cgam * cthecphi   
             - y[i] * sgam * cthecphi      + yu0t * sgam * cthecphi   
             + x[i] * sgam * sphi          - xu0t * sgam * sphi        
             + y[i] * cgam * sphi          - yu0t * cgam * sphi      + xs0t

    xs[i] =    x[i] * cgam * cthecphi      
             - y[i] * sgam * cthecphi  
             + x[i] * sgam * sphi         
             + y[i] * cgam * sphi         
             - xu0t * cgam * cthecphi   
             + yu0t * sgam * cthecphi  
             - xu0t * sgam * sphi        
             - yu0t * cgam * sphi    + xs0t

    TX =    - xu0t * cgam * cthecphi  
            + yu0t * sgam * cthecphi  
            - xu0t * sgam * sphi     
            - yu0t * cgam * sphi     + xs0t

    xs[i] =   x[i] * cgam * cthecphi  
            - y[i] * sgam * cthecphi 
            + x[i] * sgam * sphi      
            + y[i] * cgam * sphi + TX

              x[i] * cgam * cthecphi  
            - y[i] * sgam * cthecphi 
            + x[i] * sgam * sphi      
            + y[i] * cgam * sphi + TX = xs[i] 

    x[i] *  cgam * cthecphi - y[i] * sgam * cthecphi + x[i] * sgam * sphi + y[i] * cgam * sphi + TX = xs[i] 
    x[i] *  cgam * cthecphi                          + x[i] * sgam * sphi                           = xs[i] + y[i] * sgam * cthecphi - y[i] * cgam * sphi - TX

    x[i] *  cgam * cthecphi + x[i] * sgam * sphi =  xs[i] + y[i] *  sgam * cthecphi - y[i] * cgam * sphi - TX
    x[i] * (cgam * cthecphi + sgam * sphi)       = (xs[i] + y[i] *  sgam * cthecphi - y[i] * cgam * sphi - TX)

    x[i]                                         = (xs[i] + y[i] *  sgam * cthecphi - y[i] * cgam * sphi  - TX) / (cgam * cthecphi + sgam * sphi) 
    x[i]                                         = (xs[i] + y[i] * (sgam * cthecphi -        cgam * sphi) - TX) / (cgam * cthecphi + sgam * sphi) 

    TXccss = (cgam * cthecphi + sgam * sphi)
    TXsccs = (sgam * cthecphi - cgam * sphi)

    x[i] = (xs[i]  + y[i] * TXsccs - TX) / TXccss 

    x[i] = (xs[i] / TXccss  + y[i] * TXsccs / TXccss - TX / TXccss)

    // ------------------------------------------------

    ys[i] = -((x[i] - xu0t) * cgam                              - (y[i] - yu0t) * sgam) * cthesphi
           + ((x[i] - xu0t) * sgam                              + (y[i] - yu0t) * cgam) * cphi                       + ys0t 

    ys[i] = -((x[i] * cgam - xu0t * cgam)                       - (y[i] * sgam - yu0t * sgam)) * cthesphi
            +((x[i] * sgam - xu0t * sgam)                       + (y[i] * cgam - yu0t * cgam)) * cphi                + ys0t

    ys[i] = -((x[i] * cgam * cthesphi - xu0t * cgam * cthesphi) - (y[i] * sgam * cthesphi - yu0t * sgam * cthesphi)) 
            +((x[i] * sgam * cphi     - xu0t * sgam * cphi)     + (y[i] * cgam * cphi     - yu0t * cgam * cphi))     + ys0t

    ys[i] = -( x[i] * cgam * cthesphi - xu0t * cgam * cthesphi  -  y[i] * sgam * cthesphi + yu0t * sgam * cthesphi) 
            +( x[i] * sgam * cphi     - xu0t * sgam * cphi      +  y[i] * cgam * cphi     - yu0t * cgam * cphi )     + ys0t

    ys[i] = -  x[i] * cgam * cthesphi + xu0t * cgam * cthesphi  +  y[i] * sgam * cthesphi - yu0t * sgam * cthesphi  
            +  x[i] * sgam * cphi     - xu0t * sgam * cphi      +  y[i] * cgam * cphi     - yu0t * cgam * cphi       + ys0t


    - y[i] * sgam * cthesphi  - y[i] * cgam * cphi = 
            - x[i] * cgam * cthesphi + xu0t * cgam * cthesphi - yu0t * sgam * cthesphi 
            + x[i] * sgam * cphi     - xu0t * sgam * cphi     - yu0t * cgam * cphi    + ys0t - ys[i]


      y[i] * sgam * cthesphi + y[i] * cgam * cphi = 
            + x[i] * cgam * cthesphi - xu0t * cgam * cthesphi + yu0t * sgam * cthesphi 
            - x[i] * sgam * cphi     + xu0t * sgam * cphi     + yu0t * cgam * cphi    - ys0t + ys[i]

     y[i] * ( sgam * cthesphi + cgam * cphi) = 
            + x[i] * cgam * cthesphi - xu0t * cgam * cthesphi + yu0t * sgam * cthesphi 
            - x[i] * sgam * cphi     + xu0t * sgam * cphi     + yu0t * cgam * cphi    - ys0t + ys[i]

     y[i] * ( sgam * cthesphi + cgam * cphi) = 
            + x[i] * cgam * cthesphi - x[i] * sgam * cphi     
            - xu0t * cgam * cthesphi + yu0t * sgam * cthesphi 
            + xu0t * sgam * cphi     + yu0t * cgam * cphi    - ys0t 
            + ys[i]

    TY    = - xu0t * cgam * cthesphi 
            + yu0t * sgam * cthesphi 
            + xu0t * sgam * cphi     
            + yu0t * cgam * cphi    - ys0t 

    y[i] * ( sgam * cthesphi + cgam * cphi) = 
            + x[i] * cgam * cthesphi  
            - x[i] * sgam * cphi     
            + TY + ys[i]

    y[i] * (sgam * cthesphi + cgam * cphi) =   x[i] *  cgam * cthesphi - x[i] * sgam * cphi  + TY + ys[i]

    y[i] * (sgam * cthesphi + cgam * cphi) =   x[i] * (cgam * cthesphi -        sgam * cphi) + TY + ys[i]

    TYsccc = (sgam * cthesphi + cgam * cphi)
    TYccsc = (cgam * cthesphi - sgam * cphi)

    y[i] * TYsccc =  x[i] * TYccsc  + TY + ys[i]
    y[i]          = (x[i] * TYccsc  + TY + ys[i]) / TYsccc

    y[i] = (ys[i]          + x[i]                                                                                         * TYccsc           + TY) / TYsccc 


    // Substitute    x[i] =  (xs[i] / TXccss                    + y[i] * TXsccs / TXccss                   - TX / TXccss)

    y[i] = (ys[i]          + (xs[i] / TXccss                    + y[i] * TXsccs / TXccss                   - TX / TXccss) * TYccsc           + TY) / TYsccc

    y[i] = (ys[i]          +  xs[i] / TXccss * TYccsc           + y[i] * TXsccs / TXccss * TYccsc          - TX / TXccss  * TYccsc           + TY) / TYsccc

    y[i] =  ys[i] / TYsccc +  xs[i] / TXccss * TYccsc / TYsccc  + y[i] * TXsccs / TXccss * TYccsc / TYsccc - TX / TXccss  * TYccsc / TYsccc  + TY  / TYsccc


    y[i] - y[i] * TXsccs / TXccss * TYccsc / TYsccc  = ys[i] / TYsccc +  xs[i] / TXccss * TYccsc / TYsccc  - TX / TXccss  * TYccsc / TYsccc  + TY  / TYsccc

    y[i] * (1 -   TXsccs / TXccss * TYccsc / TYsccc) = ys[i] / TYsccc +  xs[i] / TXccss * TYccsc / TYsccc  - TX / TXccss  * TYccsc / TYsccc  + TY  / TYsccc


    y[i] = (ys[i] / TYsccc +  xs[i] / TXccss * TYccsc / TYsccc  - TX / TXccss  * TYccsc / TYsccc  + TY  / TYsccc) / (1 - TXsccs / TXccss * TYccsc / TYsccc)

   }
#endif

    TX =    - xu0t * cgam * cthecphi  
            + yu0t * sgam * cthecphi  
            - xu0t * sgam * sphi     
            - yu0t * cgam * sphi     + xs0t;

    TXccss = (cgam * cthecphi + sgam * sphi);
    TXsccs = (sgam * cthecphi - cgam * sphi);

    TY    = - xu0t * cgam * cthesphi 
            + yu0t * sgam * cthesphi 
            + xu0t * sgam * cphi     
            + yu0t * cgam * cphi    - ys0t; 

    TYsccc = (sgam * cthesphi + cgam * cphi);
    TYccsc = (cgam * cthesphi - sgam * cphi);

  //y[i]   = (ys[i] / TYsccc +  xs[i] / TXccss * TYccsc / TYsccc  - TX / TXccss  * TYccsc / TYsccc  + TY  / TYsccc) / (1 - TXsccs / TXccss * TYccsc / TYsccc)
    *y     = (ys    / TYsccc +  xs    / TXccss * TYccsc / TYsccc  - TX / TXccss  * TYccsc / TYsccc  + TY  / TYsccc) / (1 - TXsccs / TXccss * TYccsc / TYsccc);

  //x[i]   = (xs[i] + y[i] * TXsccs - TX) / TXccss; 
    *x     = (xs    + *y   * TXsccs - TX) / TXccss; 

 return ;
 }





