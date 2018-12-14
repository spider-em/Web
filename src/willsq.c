
/*$Header: /usr16/software/web/src/RCS/willsq.c,v 1.13 2018/12/07 17:06:44 leith Exp $*/
/*
C   Transfered from willsq.for by                     JING SU 8/31/93
C 
C++**********************************************************************
C   PROGRAM TO CALCULATE A LINEAR LEAST SQUARE FIT TO DETERMINE
C   THE DIRECTION OF THE TILT-AXIS, THE TILT ANGLE AND THE RELATIVE
C   POSITION OF A TILT PAIR (0 DEG VERSUS TILTED)
C 
C   PARAMETERS:
C   X         ARRAY  X-COORD. OF PARTICLES IN 0 DEG IMAGE     (SENT)
C   Y         ARRAY  Y-COORD. OF PARTICLES IN 0 DEG IMAGE     (SENT)
C   XS        ARRAY  X-COORD. OF PARTICLES IN TILTED IMAGE    (SENT)
C   YS        ARRAY  Y-COORD. OF PARTICLES IN TILTED IMAGE    (SENT)
C   XU0T,YU0T   LOCATION OF ORIGIN IN UNTILTED IMAGE          (SENT)   
C   XS0T,YS0T LOCATION OF ORIGIN IN TILTED IMAGE       (SENT & RETURNED)
C   PHI DIRECTION OF TILT AXIS IN TILTED IMAGE (RELATIVE TO Y)(RETURNED)
C   GAM DIRECTION OF TILT AXIS IN UNTILTED IMAGE        (RETURNED)
C   THE TILTANGLE THETA                                 (SENT)
C   N   NUMBER OF MESUREMENTS                           (SENT)
C   AVAL, BVAL   ARE ALLOCATED BUT NOT USED ELSEWHERE   (ADDRESS SENT)
C   
C   RETURNS:    0   OK
C               1   LEAST SQUARE FIT IMPOSSIBLE OR FAILS
C                   IF (FAILS GAMMA AND PHI NOT ALTERED)
C
C
C   VARIABLES:
C   EPS   ARRAY  ERROR E(1)->XS0, E(2)->YS0, E(3)->PHI, E(4)->THE
C
C   THE NORMAL EQUATIONS SOLVED ARE:
C
C      ( A   * A   + B  * B  ) D    =   A  (XS -FX ) +  B  (YS - FY )
C         KI    IL    KI   IL   L        KI   I   I      KI   I    I
C  
C   WITH D = delta XS0, D = delta YS0, D =delta PHI, D =delta THE
c         1              2              3             4
c
c   Fx := (X cos(gam)-Y sin(gam))cos(THE)cos(PHI) 
c     i     i          i            
c
c         +(X sin(gam)+Y cos(gam))sin(phi) + XS0
c            i          i
c
c   Fy := -(X cos(gam) - Y sin(gam))cos(the)sin(phi) 
c            i            i
c
c        +(X sin(gam)+Y cos(gam))cos(phi) + YS0
c           i          i 
c
c   A  = dFx /dXS0, A  = dFx /dYS0, A  = dFx /dPHI, A  = dFx /dGAM
c    i1     i        i2     i        i3     i        i4     i
c
c   B  = dFy /dXS0, B  = dFy /dYS0, B  = dFy /dPHI, B  = dFy /dGAM
c    i1     i        i2     i        i3     i        i4     i
c
c-*****************************************************************************
*/

#include "std.h"
#include "common.h"
#include "routines.h"
#include "fit.h"
#include "extras.h"

/****************************** willsq ******************************/

int willsq(float *x, float *y, float *xs, float *ys,
	    int n, float thetaw, float * gammaw, float * phiw)
 {
 float            sqa[4][5], sqb[4][4], r[4];
 float            rthe,rphi,rgam,cthe,cphi,cgam,sphi,sgam;
 float            eps, check, qxsum,qysum, fx, fy;
 int              i,it,k,l;
 float            phi, gamma, theta;
 float            * aval;
 float            * xsmf, *ysmf;

 const float pid = (3.1415927 / 180.0);
 const float pud = (180.0 / 3.1415927);

 if (n < 3)
    {
    spout(" *** Unable to fit angles: 3 or more points needed");
    return 1;
    }

 /* allocate space for  arrays */
 if (((aval= (float *) malloc(n * 4 * sizeof(float))) == (float *) NULL) || 
     ((xsmf =(float *) malloc(n *     sizeof(float))) == (float *) NULL) ||
     ((ysmf =(float *) malloc(n *     sizeof(float))) == (float *) NULL))
    { spout("*** Unable to reallocate memory in willsq."); return 2; }


 phi   = *phiw;
 gamma = *gammaw;
 theta = thetaw;

 rthe = theta * pid;
 rphi = phi   * pid;
 rgam = gamma * pid;

 cthe = cos(rthe);
 cphi = cos(rphi);
 cgam = cos(rgam);
 sphi = sin(rphi);
 sgam = sin(rgam);
 it   = 0;

 do {
 it++;

 if (it > 100)
    {
    spout("***Determination of fit angles failed at 100 iterations!");
    return 1;
    }

 qxsum = 0;   qysum = 0;

 /* Build system of normal equations build matrice A, calculate XSMF */

 /* Build system of normal equations build matrice A, calculate XSMF */

 /* Ai1:  */
 for (i = 0; i < n; i++)
     {
     fx = ((x[i] - xu0t) * cgam - (y[i] - yu0t) * sgam) * cthe * cphi
        + ((x[i] - xu0t) * sgam + (y[i] - yu0t) * cgam) * sphi + xs0t;

     xsmf[i]    = xs[i] - fx;
     qxsum      = qxsum + xsmf[i] * xsmf[i];

     aval[i*4] = 1.0;

     /* Ai2: */
     aval[i*4+1] = 0.0;

     /* Ai3: */
     aval[i*4+2] = 
        - ((x[i] - xu0t) * cgam - (y[i] - yu0t) * sgam) * sphi * cthe
        + ((x[i] - xu0t) * sgam + (y[i] - yu0t) * cgam) * cphi;

     /* Ai4: */
     aval[i*4+3] =
          (-(x[i] - xu0t) * sgam - (y[i] - yu0t) * cgam) * cthe * cphi
         + ((x[i] - xu0t) * cgam - (y[i] - yu0t) * sgam) * sphi;
     }

 /*  Calculate square matrice Aki * Ail	 */
 for (l = 0; l < 4; l++)
     {
     for (k = 0; k < 4; k++)
         {
         sqa[k][l] = 0.0;
	 for (i = 0; i < n; i++)
             {
	     sqa[k][l] =  sqa[k][l] + 
                             aval[i*4+k] * aval[i*4+l];
             }
	 }
     }

 /* Calculate first part of left side of normal equation */
 for (k = 0; k < 4; k++)
     {
     r[k] = 0.0;
     for (i = 0; i < n; i++)
	 r[k] = r[k] + aval[i*4+k] * xsmf[i];
     }

 /*  Build matrice B, calculate YSMF ------------------------------ */

 /* Bil:   */

 for (i = 0; i < n; i++)
     {
     fy = -((x[i] - xu0t) * cgam - (y[i] - yu0t) * sgam) * cthe * sphi
          +((x[i] - xu0t) * sgam + (y[i] - yu0t) * cgam) * cphi + ys0t;

     ysmf[i]      = ys[i] - fy;
     qysum          = qysum + ysmf[i] * ysmf[i];
     aval[i*4+0] = 0.0;

     /*  Bi2   */
     aval[i*4+1] = 1.0;

     /*  Bi3   */
     aval[i*4+2] = 
         -((x[i] - xu0t) * cgam - (y[i] - yu0t) * sgam) * cphi * cthe
         -((x[i] - xu0t) * sgam + (y[i] - yu0t) * cgam) * sphi;

     /* Bi4:				*/
     aval[i*4+3] = 
        -(-(x[i] - xu0t) * sgam - (y[i] - yu0t) * cgam) * cthe * sphi
        + ((x[i] - xu0t) * cgam - (y[i] - yu0t) * sgam) * cphi;
     }

 /* Calculate square matrice Bki * Bil: */

 for (l = 0; l < 4; l++)
     {
     for (k = 0; k < 4; k++)
	{
	sqb[k][l] = 0.0;
	for (i = 0; i < n; i++)
          sqb[k][l] = sqb[k][l] + aval[i*4+k] * aval[i*4+l];
        }
     }

 /*  Calculate second part of left side of normal equation:  */

 for (k = 0; k < 4; k++)
     {
     for (i = 0; i < n; i++)
	 r[k] = r[k] + aval[i*4+k] * ysmf[i];
     }

 /*  Add SQA and SQB   */
 for (k = 0; k < 4; k++)
     {
     for (l = 0; l < 4; l++)
	 sqa[k][l] = sqa[k][l] + sqb[k][l];
     }

 eps = 0.0;
 for (i = 0; i < 4; i++)
     sqa[i][4] = r[i];

 if (mircol(4,1,5, sqa, eps, r) != 0 )
    {
    spout("*** Least Square Fit failed!");
    spout("*** Give more coordinates or better start values.");
    XBell(idispl,50);  return 1;
    }

 xs0t    = xs0t + r[0];
 ys0t    = ys0t + r[1];
 rphi    = rphi + r[2];
 rgam    = rgam + r[3];

 phi    = rphi * pud;
 gamma  = rgam * pud;

 /* Determine accuracy of solution */
 cphi   = cos(rphi);
 sphi   = sin(rphi);
 cgam   = cos(rgam);
 sgam   = sin(rgam);

 
 for (i = 0; i < n; i++)
     {
     fx = ((x[i] - xu0t) * cgam - (y[i] - yu0t) * sgam) * cthe * cphi
        + ((x[i] - xu0t) * sgam + (y[i] - yu0t) * cgam) * sphi + xs0t;

     xsmf[i] = xs[i] - fx;
     qxsum   = qxsum + xsmf[i] * xsmf[i];

     fy = -((x[i] - xu0t) * cgam - (y[i] - yu0t) * sgam) * cthe * sphi
          +((x[i] - xu0t) * sgam + (y[i] - yu0t) * cgam) * cphi + ys0t;

     ysmf[i] = ys[i] - fy;
     qysum   = qysum + ysmf[i] * ysmf[i];
     }

   if (resfp) fprintf(resfp,
        "Itera: %4d  Phi: %7.2f, Gam: %7.2f, Orig:(%7.2f,%7.2f)",
        it,phi,gamma, xs0t,ys0t);
   if (resfp) fprintf(resfp, "Qxsum: %f  Qysum: %f\n",qxsum,qysum);

  check = fabs(r[0]) + fabs(r[1]) + fabs(r[2]) + fabs(r[3]);

 } while (check > 0.00005);

 *phiw   = phi;
 *gammaw = gamma;

 if (aval) {free(aval); aval = (float *) NULL;}
 if (xsmf) {free(xsmf); xsmf = (float *) NULL;}
 if (ysmf) {free(ysmf); ysmf = (float *) NULL;}

 return 0;
 } 
