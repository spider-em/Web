/*********************
 RDPRM(CS,        'CS(MM)')
 RDPRM2(DZ,LAMBDA,'DEFOCUS(ANGSTROEMS), LAMBDA(ANGSTROEMS)')

 RDPRM(KM,        'MAXIMUM SPATIAL FREQUENCY[A-1]')
 RDPRM2(Q,DS,     'SOURCE SIZE[A-1], DEFOCUS SPREAD[A]')
 RDPRM2(DZA,AZZ,  'ASTIGMATISM[A], AZIMUTH[DEG]')
 RDPRM2(WGH,ENV,  'AMPL CONTRAST RATIO [0-1], GAUSSIAN ENV HALFW [FOU UNITS]')
*************************/

   /* returns b only */


#include "std.h"
  /* internal function prottype */
  float tfd(float cs, float dz,  float lambda, float q, float ds,
             double ak, float wgh, float env);
 
  void trafc(float cs, float dz, float lambda, float km, float q, 
               float ds, float dza, float azz, float wgh, float env, 
               int nsam, float * tf)
  {
  double sc, ak, az, azr, dzz;
  int    i,j,ix;
  float  fixt;

  double pi = 3.1415926;
  
  env = 1.0 / (env * env);
  sc  = km / (float)(nsam / 2);

  j = 0;
  for (i = 0; i < nsam ; i++)
     {
     fixt = (float)i / 2.0;    /* ix = i / 2; */

     ak = sqrt(fixt * fixt) * sc; /* ak = sqrt((float)(ix * ix)) * sc;*/
     if (ak == 0.0)
        {az = pi / 2.0;}
      else
        {az = 0.0;}
    
     azr     = azz * (pi / 180.0);
     dzz     = dz + dza / 2 * sin(2 * (az - azr));
     tf[j++] = i + 1;
     tf[j++] = tfd(cs,dzz,lambda,q,ds,ak,wgh,env);
     }
  }
 


  float tfd(float cs, float dz,  float lambda, float q, float ds,
             double ak, float wgh, float env)
  {
  float  b;
  double cs1, f1, f2, q1, env1, akk, f, ds1,kappa, dz1, p, ch, qqt;

  double pi = 3.14159;

  cs1   = cs * 1.e7;
  f1    = 1. / sqrt(cs1 * lambda);
  f2    = sqrt(sqrt(cs1 * pow(lambda,3)));
  q1    = (q * f2) * (q * f2);
  env1  = env / (f2 * f2);
  akk   = ak * f2;
  f     = -pow(pi,2);
  ds1   = ds  * f1;
  kappa = ds1 * f / (16.0 * log(2.0));
  dz1   = dz * f1;
  p     = pow(akk,3) - dz1 * akk;
  ch    = exp( pow(akk,4) * kappa);
  b     = (exp(f * q1 * pow(p,2)) * ch) * exp(-env1 * pow(akk,2));
  qqt   = 2.0 * pi * (0.25 * pow(akk,4) - 0.5 * dz1 * pow(akk,2));
  b     = b * ((1.0 - wgh) * sin(qqt) - wgh * cos(qqt));

  return b;
  }

