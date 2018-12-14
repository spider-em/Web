
/*$Header: /usr16/software/web/src/RCS/gongon.c,v 1.4 2018/12/07 17:03:32 leith Exp $*/
/*
c    gongon.for
c    called by:  gold1.c

c...gongon - modifed from jgoneult.for on may 20 1987 by a. leith at
c...          nysdoh, albany to calculate angles one by one instead
c...          of taking input from a file.
c...goneul - convert goniometer angles theta and phi to eulerian angles
c...         alpha, beta and gamma.
c...mark rould  29dec84
c...theta is angle of tilt (around tilt axis.) (y-axis)
c...phi is angle around microscope axis (counterclockwise, as usual.) (z-axis)
 */

#include "routines.h"

#define tol_error  0.001
#include "std.h"

      /* external functions */

      /* internal functions */
      static double gongon6( double, double, double, double,
                      double, double, double[3][3]);
      static double  gongon5(double, double);
      static void    gongon3(double, double, double,
                      double[3][3],  double, double, double);
      static void    gongon4(double[3][3], double[3][3], double[3][3]);
      static void    gongon2(double, double, double, double[3][3]);

 /***************************  gongon ***************************/

      int gongon(float thetad, float phid, float * alphad,
                 float *  betad,  float * gammad)
      {
      double rot[3][3], erot[3][3];
      double theta, phi, tau,degtorad, sinb, cosb, cosa, sina, cosc;
      double sinc, alpha, beta, gammat;

      degtorad = 3.141592653589793 / 180.0;

      theta = thetad * degtorad;
      phi   = phid   * degtorad;
      tau   = 0;
      gongon2(theta, phi, tau, rot);

      sinb = -rot[2][0];
      cosb = sqrt(1 - sinb * sinb);
      if ( cosb == 0.0 ) 
         {
         spout("*** error in gongon: cannot complete. cosb = 0.");
         return FALSE;
         }
      cosa = rot[0][0] / cosb;
      sina = rot[1][0] / cosb;
      cosc = rot[2][2] / cosb;
      sinc = rot[2][1] / cosb;

      if ( gongon6(cosa, sina, cosb, sinb,cosc, sinc, rot) > tol_error) 
         {
         spout("*** in gongon,  tolerable error exceeded.");
         return FALSE;
         }

      alpha = gongon5(sina, cosa);
      beta  = gongon5(sinb, cosb);
      gammat = gongon5(sinc, cosc);

      gongon3(alpha, beta, gammat, erot, 1.0 ,1.0, 1.0);

      *alphad = alpha  / degtorad;
      *betad  = beta   / degtorad;
      *gammad = gammat / degtorad;

      if (*alphad > 180.) *alphad = *alphad - 360.0;
      if (*betad  > 180.) *betad  = *betad  - 360.0;
      if (*gammad > 180.) *gammad = *gammad - 360.0;

      return TRUE;
      }
 
 /*---------------------------------------------------------- */

      double gongon6( double ca, double sa, double cb, double sb,
                      double cc, double sc, double r[3][3])
      {
      double dif;

      dif = 0.0;
      dif = dif + fabs( r[0][1] - (-sa*cc+ca*sb*sc));
      dif = dif + fabs( r[0][2] - ( sa*sc+ca*sb*cc));
      dif = dif + fabs( r[1][1] - ( ca*cc+sa*sb*sc));
      dif = dif + fabs( r[1][2] - (-ca*sc+sa*sb*cc));

      return dif;
      }

 /* ---------------------------------------------------------------*/

      double  gongon5(double  y,  double  x)

      {
      double  pi, gongon5t;

      pi = 3.141592653589793;
      if (x != 0.0 ) 
         gongon5t = atan2(y, x);

      else
         {
         if (y == 0.0 ) 
           gongon5t = 0.0;

         else
            {
            if (y < 0.0 ) 
              gongon5t = -pi / 2.0;
            else
              gongon5t =  pi / 2.0;
            }
         }    
      if (gongon5t < 0.0) gongon5t = gongon5t + 2.0 * pi;
      return gongon5t;
      }

 /* -------------------------------------------------------------- */

      void gongon3(double  alpha, double  beta, double  gammat,
                   double  rot[3][3], 
                   double  diag1, double  diag2, double  diag3)

      {

      /*  converts rotation angles to euler matrix
      c...alpha is rotation about z, beta about y, and gammat about x. 
      */

      double  a[3][3], b[3][3], g[3][3], temp[3][3];
      int   i,j;
      double sa,sb,sg,ca,cb,cg;

      for   (i = 0; i < 3; i++)
        for (j = 0; j < 3; j++)
          {
          a[i][j] = 0.0;
          b[i][j] = 0.0;
          g[i][j] = 0.0;
          }

      sa = sin(alpha);
      sb = sin(beta);
      sg = sin(gammat);
      ca = cos(alpha);
      cb = cos(beta);
      cg = cos(gammat);

/* alpha is rotation about z-axis (positive angle from x-axis to y-axis) */
      a[0][0] =  ca;
      a[0][1] = -sa;
      a[1][0] =  sa;
      a[1][1] =  ca;
      a[2][2] =  diag1;
 
/* beta is rotation about y-axis (positive angle from z-axis to x-axis) */
      b[0][0] =  cb;
      b[0][2] =  sb;
      b[2][0] = -sb;
      b[2][2] =  cb;
      b[1][1] =  diag2;

/* gammat is rotation about x-axis (positive angle from y-axis to z-axis) */
      g[0][0] =  diag3;
      g[1][1] =  cg;
      g[1][2] = -sg;
      g[2][1] =  sg;
      g[2][2] =  cg;
  
      gongon4(a, b, temp);
      gongon4(temp, g, rot);
      }

/*-----------------------------------------------------------------*/

      void gongon4(double  a[3][3], double  b[3][3], double  ab[3][3])
      {
      int i,j,k;

      /* multiply matrix a by matrix b to give matrix ab  */

      for   (i = 0; i < 3; i++)
        for (j = 0; j < 3; j++)
          {
          ab[i][j] = 0.0;

          for ( k = 0; k < 3; k++)
            ab[i][j] = ab[i][j] + a[i][k] * b[k][j];
          }
      }

 /* ---------------------------------------------------------------*/

      void gongon2( double  theta, double  phi, double  tau, 
                    double  rot[3][3])

      {
      int      i,j;
      double   t[3][3], p[3][3], ta[3][3], temp[3][3];
      double   ct, st, cp, sp, ctau, stau;

      for (i = 0; i < 3; i++)
        for (j = 0; j < 3; j++)
          {
          t[i][j]  = 0.0;
          p[i][j]  = 0.0;
          ta[i][j] = 0.0;
          }

      ct   = cos(theta);
      st   = sin(theta);
      cp   = cos(phi);
      sp   = sin(phi);
      ctau = cos(tau);
      stau = sin(tau);

      /* theta (in rads) is rotation about x-axis (right-hand rule.) */
      t[0][0] = 1.0;
      t[1][1] =  ct;
      t[2][2] =  ct;
      t[2][1] =  st;
      t[1][2] = -st;

      /* phi (in rads) is rotation about z-axis. */
      p[2][2] = 1.0;
      p[0][0] =  cp;
      p[1][1] =  cp;
      p[1][0] =  sp;
      p[0][1] = -sp;

      /* tau (in rads) is rotation about z-axis. */
      ta[2][2] =  1.0;
      ta[0][0] =  ctau;
      ta[1][1] =  ctau;
      ta[1][0] =  stau;
      ta[0][1] = -stau;


/* 
c...note that rotation of phi about the z-axis is applied first,
c...followed by rotation of theta about the y-axis, followed by rotation
c...of tau about the z-axis;  however, in
c...forming the composite matrix representing the complete operation,
c...the constituent matrices are multiplied in reverse order, ie,
c...rot = tau * theta * phi, in accordance with the rules of composition of
c...functions.    *******/

      gongon4(ta, t, temp);
      gongon4(temp, p, rot);
      }
