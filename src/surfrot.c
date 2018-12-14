
/*$Header: /usr16/software/web/src/RCS/surfrot.c,v 1.12 2018/12/07 17:03:34 leith Exp $*/
/*
C***********************************************************************
C
C   surfrot.c  Created FEB 92 as wid_surf.for                FEB 92 al * 
C              Converted to C                                OCT 92 al *
C              Modified                                      NOV 93 lo *
C              Modified                                      NOV 11 al *
C **********************************************************************
C *  AUTHOR: ArDean Leith                                                  *
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
C
C **********************************************************************
*/

#include "common.h"
#include "routines.h"

 // Static internal variables
 static int            nsr, vcx, vcy, vcz;
 static float          xl, xh, yl, yh, zl, zh, rm11, rm21;
 static float          rm31, rm12, rm22, rm32, rm13, rm23, rm33;

 /************************* surfrot ****************************/

 float surfrot(int * newang, int * newsize, float thetat,
              float phit, float psit, unsigned char * vol, 
              unsigned char lthlev, int iy, int iz)

 {
 float          theta, phi, psi;
 float          y, z, qr1, qr2, qr3, qout;
 float          u, v, w, bv, fv, xx, yy, zz;
 float          a1, a2, a3, a4, a5, a6, a7, a8;
 int            ix, ixgo, ixend, iox, ioy, ioz;
 unsigned char  *p1, *p2, *p3, *p4, *p5; 
 unsigned char  *cptr, *p6, *p7, *p8;


 if (*newang) 
    {
    theta = thetat / 360.0 * 8.0 * atan(1.0);
    phi   = phit   / 360.0 * 8.0 * atan(1.0);
    psi   = psit   / 360.0 * 8.0 * atan(1.0); 

    rm11  =  cos(theta)*cos(phi)*cos(psi)-sin(phi)*sin(psi);
    rm21  = -cos(theta)*cos(phi)*sin(psi)-sin(phi)*cos(psi);
    rm31  =  sin(theta)*cos(phi);
    rm12  =  cos(theta)*sin(phi)*cos(psi)+cos(phi)*sin(psi);
    rm22  = -cos(theta)*sin(phi)*sin(psi)+cos(phi)*cos(psi);
    rm32  =  sin(theta)*sin(phi);
    rm13  = -sin(theta)*cos(psi);
    rm23  =  sin(theta)*sin(psi);
    rm33  =  cos(theta);

    rm11  =  cos(theta)*cos(phi)*cos(psi)-sin(phi)*sin(psi);
    rm21  = -cos(theta)*cos(phi)*sin(psi)-sin(phi)*cos(psi);
    rm31  =  sin(theta)*cos(phi);
    rm12  =  cos(theta)*sin(phi)*cos(psi)+cos(phi)*sin(psi);
    rm22  = -cos(theta)*sin(phi)*sin(psi)+cos(phi)*cos(psi);
    rm32  =  sin(theta)*sin(phi);
    rm13  = -sin(theta)*cos(psi);
    rm23  =  sin(theta)*sin(psi);
    rm33  =  cos(theta);


     * newang  = FALSE;
    }

 if (*newsize) 
      {   
      vcx  = nslice / 2;
      vcy  = nsam   / 2;
      vcz  = nrow   / 2;

      xl   = 1.0    - vcx;
      xh   = nslice - vcx; 
      yl   = 1.0    - vcy;
      yh   = nsam   - vcy; 
      zl   = 1.0    - vcz;
      zh   = nrow   - vcz;
      nsr  = nslice * nsam;

      * newsize  = FALSE;
      }

    ixgo  = nslice1 - vcx;
    ixend = nslice2 - vcx;
    y     = iy - vcy;
    z     = iz - vcz;
        
    qr1  = rm11 * ixgo + rm21 * y + rm31 * z;   
    qr2  = rm12 * ixgo + rm22 * y + rm32 * z;   
    qr3  = rm13 * ixgo + rm23 * y + rm33 * z;   

    for (ix = ixgo; ix <= ixend; ix++)
       {
       if (qr1 > xl && qr1 < xh && qr2 > yl && 
           qr2 < yh && qr3 > zl && qr3 < zh)  
             {
             xx  = vcx + qr1;
             yy  = vcy + qr2;
             zz  = vcz + qr3;

             iox = xx;       ioy = yy;       ioz = zz;
             u   = xx - iox;   v = yy - ioy;   w = zz - ioz;

             cptr = vol + (ioz-1)*nsr + (ioy-1)*nslice + iox-1; 

             p1 = cptr;            p5 = p1 + 1;
             p2 = p1 + nslice;     p6 = p2 + 1;
             p3 = p2 + nsr;        p7 = p3 + 1;
             p4 = p1 + nsr;        p8 = p4 + 1;

             a1 = *p1; a2 = *p2; a3 = *p3; a4 = *p4;
             a5 = *p5; a6 = *p6; a7 = *p7; a8 = *p8;

             bv = a1 + w*(a4 - a1) 
                + v*(a2 - a1 + w*(a3 + a1 - a2 - a4));
             fv = a5 + w*(a8 - a5) 
                + v*(a6 - a5 + w*(a7 + a5 - a6 - a8));

             qout = u*(fv - bv) + bv;
             if ( qout >= (float) lthlev)
                 {return( 1.0 );}
             }
       qr1 += rm11;
       qr2 += rm12;
       qr3 += rm13;
     }

    return( 0.0 );
}


/*************************************************************************/
/* ORIGINAL                                                                    */
/* volval computes the value within the volume at position x,y,z (origin */
/* shifted to middle of volume) using trilinear interpolation.           */
/*                                                                       */
/*************************************************************************/


float volval(int ipos, int jpos, int kpos, unsigned char * vol)

  {
  int              iox, ioy, ioz;
  float            u, bv, fv, qr1, qr2, qr3;
  float            v, w, x, y, z, xx, yy, zz;
  float            a1, a2, a3, a4, a5, a6, a7, a8;
  unsigned char   *cptr, *p1, *p2, *p3;
  unsigned char   *p4, *p5, *p6, *p7, *p8; 


       x = ipos - vcx;
       y = jpos - vcy;
       z = kpos - vcz;

       qr1  = rm11 * x + rm21 * y + rm31 * z;   
       qr2  = rm12 * x + rm22 * y + rm32 * z;   
       qr3  = rm13 * x + rm23 * y + rm33 * z;   

       if (qr1 > xl && qr1 < xh && qr2 > yl && 
           qr2 < yh  && qr3 > zl && qr3 < zh)  
          {
             xx = vcx + qr1;
             yy = vcy + qr2;
             zz = vcz + qr3;

             iox = xx;       ioy = yy;       ioz = zz;
             u = xx - iox;   v = yy - ioy;   w = zz - ioz;

             cptr = vol + (ioz-1)*nsr + (ioy-1)*nsam + iox-1; 

             p1 = cptr;            p5 = p1 + 1;
             p2 = p1 + nsam;       p6 = p2 + 1;
             p3 = p2 + nsr;        p7 = p3 + 1;
             p4 = p1 + nsr;        p8 = p4 + 1;

             a1 = *p1; a2 = *p2; a3 = *p3; a4 = *p4;
             a5 = *p5; a6 = *p6; a7 = *p7; a8 = *p8;

             bv = a1 + w*(a4 - a1) 
                + v*(a2 - a1 + w*(a3 + a1 - a2 - a4));
             fv = a5 + w*(a8 - a5) 
                + v*(a6 - a5 + w*(a7 + a5 - a6 - a8));

             return( u*(fv - bv) + bv );
          }
       else
          /* Have to return original spot value.... erroneous but satisfactory! */
          return( *(vol + (kpos-1)*nsr + (jpos-1)*nsam + ipos-1) );   

  }

#ifdef NEVER
// NSAM   --> NSLICE
// NROW   --> NSAM
// NSLICE --> NROW
/*************************************************************************/
/*                                                                       */
/* volval computes the value within the volume at position x,y,z (origin */
/* shifted to middle of volume) using trilinear interpolation.           */
/*                                                                       */
/*************************************************************************/


float volval(int ipos, int jpos, int kpos, unsigned char * vol)

  {
  int              iox, ioy, ioz;
  float            u, bv, fv, qr1, qr2, qr3;
  float            v, w, x, y, z, xx, yy, zz;
  float            a1, a2, a3, a4, a5, a6, a7, a8;
  unsigned char   *cptr, *p1, *p2, *p3;
  unsigned char   *p4, *p5, *p6, *p7, *p8; 


       x = ipos - vcx;
       y = jpos - vcy;
       z = kpos - vcz;

       qr1  = rm11 * x + rm21 * y + rm31 * z;   
       qr2  = rm12 * x + rm22 * y + rm32 * z;   
       qr3  = rm13 * x + rm23 * y + rm33 * z;   

       if (qr1 > xl && qr1 < xh && qr2 > yl && 
           qr2 < yh  && qr3 > zl && qr3 < zh)  
          {
             xx = vcx + qr1;
             yy = vcy + qr2;
             zz = vcz + qr3;

             iox = xx;       ioy = yy;       ioz = zz;
             u = xx - iox;   v = yy - ioy;   w = zz - ioz;

             cptr = vol + (ioz-1)*nsr + (ioy-1)*nslice + iox-1; 

             p1 = cptr;            p5 = p1 + 1;
             p2 = p1 + nslice;       p6 = p2 + 1;
             p3 = p2 + nsr;        p7 = p3 + 1;
             p4 = p1 + nsr;        p8 = p4 + 1;

             a1 = *p1; a2 = *p2; a3 = *p3; a4 = *p4;
             a5 = *p5; a6 = *p6; a7 = *p7; a8 = *p8;

             bv = a1 + w*(a4 - a1) 
                + v*(a2 - a1 + w*(a3 + a1 - a2 - a4));
             fv = a5 + w*(a8 - a5) 
                + v*(a6 - a5 + w*(a7 + a5 - a6 - a8));

             return( u*(fv - bv) + bv );
          }
       else
          /* Have to return original spot value.... erroneous but satisfactory! */
          return( *(vol + (kpos-1)*nsr + (jpos-1)*nslice + ipos-1) );   

  }
#endif
