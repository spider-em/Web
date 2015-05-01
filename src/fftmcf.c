/**************************************************************************
*  fftmcf()
*	translated from fftmcf.f by mahieddine ladjadj.     6/6/94
*
*
*
***************************************************************************/
/*
* 	NOTE: instead of worrying about the indexes of the arrays,
*             substract one (-1) from each index of the arrays.
*	      later, change the index of the do loop instead.
*/
/*******************  fftmcf *****************************/

#include "common.h"
#include "routines.h"
/*
#include <sys/types.h>
#include <malloc.h>
*/

int fftmcf(float *a, float *b, int ntot, int n, int nspan, int *isn)
  {
  register int i,j;
  int tdim = 23, maxf =23, maxp = 209;
  int * nfac, *np;
  float *at, *ck, *bt, *sk;
  float rad, radf, s72, s120, c72;
  int nt, ks, kspan, nn, jc, jf, inc, k, m, jj, ii;
  int  kt, kk, k1, k2, k3, k4, kspnn;
  float sd, ak, bk, akm, akp, aj, bj, aa, bb, ajm, bjm, ajp, bjp;
  float bkm, bkp, c1, c2, c3, s1, s2, s3, cd;

  if(n < 2) return 0;

   /* run time allocation of memory space */
   if ( (nfac = (int *) malloc(maxf*sizeof(int)) ) 
              == (int *) NULL)                                     {
              spout(" *** WARNING: return from fftmcf.c ");
              spout(" MALLOC() CAN'T ALLOCATE HEAP MEMORY SPACE");
              return 1;                                            }
   if ( (np = (int *) malloc(maxp*sizeof(int)) ) 
              == (int *) NULL)                                     {
              spout(" *** WARNING: return from fftmcf.c ");
              spout(" MALLOC() CAN'T ALLOCATE HEAP MEMORY SPACE");
              free(nfac);      
              return 1;                                            }
   if ( (at = (float *) malloc(maxf*sizeof(int)) ) 
              == (float *) NULL)                                   {
              spout(" *** WARNING: return from fftmcf.c ");
              spout(" MALLOC() CAN'T ALLOCATE HEAP MEMORY SPACE");
              free(nfac);
              free(np);
              return 1;                                            }
   if ( (bt = (float *) malloc(maxf*sizeof(int)) ) 
              == (float *) NULL)                                   {
              spout(" *** WARNING: return from fftmcf.c ");
              spout(" MALLOC() CAN'T ALLOCATE HEAP MEMORY SPACE");
              free(nfac);
              free(np);
              free(at);
              return 1;                                            }
   if ( (ck = (float *) malloc(maxf*sizeof(int)) ) 
              == (float *) NULL)                                   {
              spout(" *** WARNING: return from fftmcf.c ");
              spout(" MALLOC() CAN'T ALLOCATE HEAP MEMORY SPACE");
              free(nfac);
              free(np);
              free(at);
              free(bt);
              return 1;                                            }
   if ( (sk = (float *) malloc(maxf*sizeof(int)) ) 
              == (float *) NULL)                                   {
              spout(" *** WARNING: return from fftmcf.c ");
              spout(" MALLOC() CAN'T ALLOCATE HEAP MEMORY SPACE");
              free(nfac);
              free(np);
              free(at);
              free(bt);
              free(ck);
              return 1;                                            }

  inc = *isn;
  c72 = cos((8.0 * atan(1.0))/5.0);
  rad = (inc >= 0) ? 8.0 * atan(1.0) : -8.0 * atan(1.0);
  s72 = (inc >= 0) ? sin((8.0 * atan(1.0))/5.0) :
                    - sin((8.0 * atan(1.0))/5.0);
  s120= (inc >= 0) ? sqrt(0.75) : - sqrt(0.75);
  inc = (inc >= 0) ? inc : (-inc);
  nt = inc*ntot;
  ks = kspan = inc*nspan;
  nn = nt - inc;
  jc = ks / n;
  radf = rad * ((float) jc)*0.5;

/*-------DETERMINE THE FACTORS OF N--------------------------------------*/

  i = jf = 0;
  m = 0;
  k = n;
  while ((k -(k/16)*16) == 0) 
      {    
      m++;     
      nfac[m -1] = 4;
      k = k/16;
      }

  j = 3;
  jj = 9;
  do   {    /*  while (jj <= k)  */
       while(k%jj == 0)
           {    
           m++;  
           nfac[m -1] = j;
           k = k/jj;
           }
           j +=2;
           jj = j * j;
       }  while (jj <= k);

   if(k <= 4 )
       {
       kt = m;
       nfac[m+1 -1] = k;
       if (k != 1) m++;
       }
   else
       {
       if((k-(k/4)*4) == 0)
           {
           m++;
           nfac[m -1] = 2;
           k  = k/4;
           }
       kt = m;
       j = 2;
       do  {                 /*  while (j <= k) */
           if(k%j == 0)
               {
               m++;
               nfac[m -1] = j;
               k = k/j;
               }
           j = ((j+1)/2)*2 + 1;
           }   while (j <= k);
       }            /* else if(k <= 4 ) */ 

  if(kt != 0)
      {
      j = kt;
      do  {
          m++;
          nfac[m -1] = nfac[j -1];
          j--;
          }   while (j != 0);
      }

/*-------  COMPUTE FOURIER TRANSFORM   ------------------------------*/

m100:
  sd = radf/((float) kspan);
/*  cd = 2.0 * sin(sd)**2;  */
  cd = 2.0 * sin(sd)*sin(sd);
  sd = sin(sd+sd);
  kk = 1;
  i++;
  if (nfac[i -1] != 2) goto m400;

/*-------TRANSFORM FOR FACTOR OF 2 (INCLUDING ROTATION FACTOR)----------*/

  kspan /= 2;
  k1 = kspan +2;
  do  {      /* while (kk <= jc) */
      do  {        /* while (kk <= nn)  */
          k2 = kk + kspan;
          ak = a[k2 -1];
          bk = b[k2 -1];
          a[k2 -1] = a[kk -1] - ak;
          b[k2 -1] = b[kk -1] - bk;
          a[kk -1] += ak;
          b[kk -1] += bk;
          kk = k2 + kspan;
          }  while (kk <= nn);
      kk -= nn;
      }  while (kk <= jc);
   
  if (kk > kspan) goto m800;

  do  {              /* while (kk <= (jc+jc)) */
      c1 = 1.0 - cd;
      s1 = sd;
      do  {               /* while (kk < k2) */
          do  {                /* while (kk > k2) */
              do  {                  /* while (kk < nt) */
                  k2 = kk + kspan;
                  ak = a[kk -1] - a[k2 -1];
                  bk = b[kk -1] - b[k2 -1];
                  a[kk -1] += a[k2 -1];
                  b[kk -1] += b[k2 -1];
                  a[k2 -1] = c1 * ak - s1 * bk;
                  b[k2 -1] = s1 * ak + c1 * bk;
                  kk = k2 + kspan;
                  }   while (kk < nt);
              k2 = kk - nt;
              c1 = -c1;
              kk = k1 - k2;
              }   while (kk > k2);
          ak = c1 - (cd * c1 + sd * s1);
          s1 = (sd * c1 - cd * s1) + s1;
          /*
          * THE FOLLOWING THREE STATEMENTS COMPENSATE FOR TRUNCATION
          * ERROR. IF ROUNDED ARITHMETIC IS USED, SUBSTITUTE
          * C1 = AK              */
          c1 = 0.5 / (ak*ak + s1*s1) + 0.5;
          s1 *= c1;
          c1 *= ak;
          kk += jc;
          }   while (kk < k2);
      k1 += (inc + inc);
      kk = (k1-kspan)/2 + jc;
      }   while (kk <= (jc+jc));
  goto m100;

/*-------TRANSFORM FOR FACTOR OF 3 (OPTIONAL CODE)--------------------*/

m320:
  do  {             /*  while (kk <= kspan) */
      do  {               /*  while (kk < nn) */
          k1 = kk + kspan;
          k2 = k1 + kspan;                      
          ak = a[kk -1];
          bk = b[kk -1];
          aj = a[k1 -1] + a[k2 -1];
          bj = b[k1 -1] + b[k2 -1];
          a[kk -1] = ak + aj;
          b[kk -1] = bk + bj;
          ak += (-0.5 * aj);
          bk += (-0.5 * bj);
          aj = (a[k1 -1] - a[k2 -1]) * s120;
          bj = (b[k1 -1] - b[k2 -1]) * s120;
          a[k1 -1] = ak - bj;
          b[k1 -1] = bk + aj;
          a[k2 -1] = ak + bj;
          b[k2 -1] = bk - aj;
          kk = k2 + kspan;
          }   while (kk < nn);
      kk -= nn;
      }    while (kk <= kspan);
  goto m700;

/*-------TRANSFORM FOR FACTOR OF 4-------------------------------*/

m400:
  if(nfac[i -1] != 4) goto m600;
  kspnn = kspan;
  kspan /= 4;
  do   {          /*  while (kk <= jc)  */
       c1 = 1.0;
       s1 = 0;
       do   {          /*  while (kk <= kspan) */
            do   {       /*  while (kk <= nt)  */ 
                 k1 = kk + kspan;
                 k2 = k1 + kspan;
                 k3 = k2 + kspan;
                 akp = a[kk -1] + a[k2 -1];
                 akm = a[kk -1] - a[k2 -1];
                 ajp = a[k1 -1] + a[k3 -1];
                 ajm = a[k1 -1] - a[k3 -1];
                 a[kk -1] = akp + ajp;
                 ajp = akp - ajp; 
                 bkp = b[kk -1] + b[k2 -1];
                 bkm = b[kk -1] - b[k2 -1];
                 bjp = b[k1 -1] + b[k3 -1];
                 bjm = b[k1 -1] - b[k3 -1];
                 b[kk -1] = bkp + bjp;
                 bjp = bkp - bjp;
                 if ((*isn) < 0) 
                     {
                     akp  = akm + bjm;
                     akm -= bjm;
                     bkp  = bkm - ajm;
                     bkm += ajm;
                     }
                 else
                     {
                     akp  = akm - bjm;
                     akm += bjm;
                     bkp  = bkm + ajm;
                     bkm -= ajm;
                     }
                 if (s1 == 0)
                     {
                     a[k1 -1] = akp;
                     b[k1 -1] = bkp;
                     a[k2 -1] = ajp;
                     b[k2 -1] = bjp;
                     a[k3 -1] = akm;
                     b[k3 -1] = bkm;
                     }
                 else
                     { 
                     a[k1 -1] = akp * c1 - bkp * s1;
                     b[k1 -1] = akp * s1 + bkp * c1;
                     a[k2 -1] = ajp * c2 - bjp * s2;
                     b[k2 -1] = ajp * s2 + bjp * c2;
                     a[k3 -1] = akm * c3 - bkm * s3;
                     b[k3 -1] = akm * s3 + bkm * c3;
                     }
                 kk = k3 + kspan;    
                 }  while (kk <= nt);
            c2 = c1 -(cd * c1 + sd * s1);
            s1 = (sd * c1 - cd * s1) + s1;
            /* THE FOLLOWING THREE STATEMENTS COMPENSATE FOR TRUNCATION
            *  ERROR. IF ROUNDED ARITHMETIC IS USED, SUBSTITUTE
            * C1 = C2                        */
            c1 = 0.5 / ((c2*c2)+(s1*s1)) + 0.5;
            s1 *= c1;
            c1 *=c2;
            c2 = (c1*c1) - (s1*s1);
            s2 = 2.0 * c1 * s1;
            c3 = c2 * c1 - s2 * s1;
            s3 = c2 * s1 + s2 * c1;
            kk -= (nt - jc);
            }   while (kk <= kspan);
       kk -= (kspan-inc);
       }  while (kk <= jc) ;
  if(kspan == jc) goto m800;
  goto m100;
                                                                       
/* -------TRANSFORM FOR FACTOR OF 5 (OPTIONAL CODE)-----------------  */
m510:
  c2 = (c72*c72) - (s72*s72);
  s2 = 2.0 * c72 * s72;
  do  {                            /*  while (kk <= kspan)   */
      do  {                               /* while (kk < nn) */
          k1 = kk + kspan;
          k2 = k1 + kspan;
          k3 = k2 + kspan;
          k4 = k3 + kspan;    
          akp = a[k1 -1] + a[k4 -1];
          akm = a[k1 -1] - a[k4 -1];
          bkp = b[k1 -1] + b[k4 -1];
          bkm = b[k1 -1] - b[k4 -1];
          ajp = a[k2 -1] + a[k3 -1];
          ajm = a[k2 -1] - a[k3 -1];
          bjp = b[k2 -1] + b[k3 -1];
          bjm = b[k2 -1] - b[k3 -1];
          aa = a[kk -1];
          bb = b[kk -1];
          a[kk -1] = aa + akp + ajp;
          b[kk -1] = bb + bkp + bjp;        
          ak = akp * c72 + ajp * c2 + aa;
          bk = bkp * c72 + bjp * c2 + bb;
          aj = akm * s72 + ajm * s2;
          bj = bkm * s72 + bjm * s2;
          a[k1 -1] = ak - bj;
          a[k4 -1] = ak + bj;
          b[k1 -1] = bk + aj;
          b[k4 -1] = bk - aj;
          ak = akp * c2 + ajp * c72 + aa;
          bk = bkp * c2 + bjp * c72 + bb;
          aj = akm * s2 - ajm * s72;
          bj = bkm * s2 - bjm * s72;
          a[k2 -1] = ak - bj;
          a[k3 -1] = ak + bj;
          b[k2 -1] = bk + aj;
          b[k3 -1] = bk - aj;
          kk = k4 + kspan;
          }   while (kk < nn);
      kk -= nn;
      }  while (kk <= kspan) ;
  goto m700;

/* -------  TRANSFORM FOR ODD FACTORS  --------------------------------- */

m600:
  k = nfac[i -1];
  kspnn = kspan;
  kspan /= k;
  if (k == 3) goto m320;
  if (k == 5) goto m510;
  if (k != jf) 
      {
      jf = k;
      s1 = rad / ((float) k);
      c1 = cos(s1);
      s1 = sin(s1);
      if (jf > maxf) 
          {  
          *isn = 0;       
          if (nfac) free(nfac);
          if (np) free(np);
          if (at) free(at);
          if (bt) free(bt);
          if (ck) free(ck);
          if (sk) free(sk);
          return 0;
          }
      ck[jf -1] = 1.0;
      sk[jf -1] = 0.0;
      j = 1;
      do   {                  /* while (j < k)    */
           ck[j -1] = ck[k -1] * c1 + sk[k -1] * s1;
           sk[j -1] = sk[k -1] * s1 - sk[k -1] * c1;
           k--;
           ck[k -1] = ck[j -1];
           sk[k -1] = -sk[j -1];
           j++;
           }  while (j < k) ;
       }                       /* if (k != jf) */

  do   {                 /*  while (kk <= kspan) */
       do   {                 /*  while (kk <= nn)  */
            k1 = kk;
            k2 = kk + kspnn;
            aa = ak = a[kk -1];
            bb = bk = b[kk -1];
            j  = 1;
            k1 = k1 + kspan;       
            do   {                 /* while (k1 < k2)  */ 
                 k2 -= kspan;
                 j++;
                 at[j -1] = a[k1 -1] + a[k2 -1];
                 ak    += at[j -1];
                 bt[j -1] = b[k1 -1] + b[k2 -1];
                 bk    += bt[j -1];
                 j++;
                 at[j -1] = a[k1 -1] - a[k2 -1];
                 bt[j -1] = b[k1 -1] - b[k2 -1];
                 k1 += kspan;
                 }  while (k1 < k2);
            a[kk -1] = ak;
            b[kk -1] = bk;
            k1 = kk;
            k2 = kk + kspnn;
            j = 1;
            do   {     /* while (j < k) */
                 k1 += kspan;
                 k2 -= kspan;
                 jj = j;
                 ak = aa;
                 bk = bb;
                 aj = bj = 0.0;
                 k = 1;
                 do   {      /*  while (k < jf) */
                      k++;                 
                      ak += at[k -1] * ck[jj -1];
                      bk += bt[k -1] * ck[jj -1];
                      k++;                 
                      aj += at[k -1] * sk[jj -1];
                      bj += bt[k -1] * sk[jj -1];
                      jj += j;
                      if (jj > jf) jj -= jf;
                      }    while (k < jf);
                 k = jf -j;
                 a[k1 -1] = ak - bj;
                 b[k1 -1] = bk + aj;
                 a[k2 -1] = ak + bj;
                 b[k2 -1] = bk - aj;
                 j++; 
                 }    while (j < k);
            kk += kspnn;
            }   while (kk <= nn);
       kk -= nn;
       }  while (kk <= kspan);
                           
/* ------- MULTIPLY BY ROTATION FACTOR (EXCEPT FOR FACTORS OF 2 AND 4)---- */

m700:
  if (i != m) 
      {
      kk = jc + 1;
      do   {                 /*  while (kk <= (jc+jc)) */
           c2 = 1.0 - cd;
           s1 = sd;
           do   {                /*  while (kk < kspan) */
                c1 = c2;
                s2 = s1;
                kk += kspan;
                do   {               /*  while (kk <= kspnn) */
                     do   {                 /*  while (kk <= nt) */
                          ak = a[kk -1];
                          a[kk -1] = c2 * ak - s2 * b[kk -1];
                          b[kk -1] = s2 * ak + c2 * b[kk -1];
                          kk += kspnn;
                          }  while (kk <= nt);
                     ak = s1 * s2;
                     s2 = s1 * c2 + c1 * s2;
                     c2 = c1 * c2 - ak;
                     kk -= (nt - kspan);
                     }  while (kk <= kspnn) ;
                c2 = c1 - (cd * c1 + sd * s1);
                s1 = s1 + (sd * c1 - cd * s1);
                /* THE FOLLOWING THREE STATEMENTS COMPENSATE FOR 
                 * TRUNCATION ERROR. IF ROUNDED ARITHMETIC IS USED,
                 * THEY MAY BE DELETED.        */
                c1 = 0.5 / (c2*c2 + s1*s1) + 0.5;
                s1 *= c1;
                c2 *= c1;
                kk -= (kspnn - jc); 
                }  while (kk <= kspan);
           kk -= (kspan - jc - inc);
           }  while (kk <= (jc+jc));
      goto m100;
      }                              /* if (i != m) */

/*-------PERMUTE THE RESULTS TO NORMAL ORDER---DONE IN TWO STAGES--------
 *       PERMUTATION FOR SQUARE FACTORS OF N      */

m800:
  np[1 -1] = ks;
  if (kt == 0) goto m890;
  k = kt + kt + 1;
  if (m < k) k--;
  j = 1;
  np[k+1 -1] = jc;
  do   {
       np[j+1 -1] = np[j -1] / nfac[j -1];
       np[k -1] = np[k+1 -1] * nfac[j -1];
       j++;
       k--;
       }  while (j < k);
  k3 = np[k+1 -1];
  kspan = np[2 -1];
  kk = jc+1;
  j = 1;
  k2 = kspan+1;
  if (n == ntot) 
       {   /* PERMUTATION FOR SINGLE-VARIATE TRANSFORM (OPTIONAL CODE) */

m820:
       do   {            /*  while (k2 < ks) */
            ak = a[kk -1];
            a[kk -1] = a[k2 -1];
            a[k2 -1] = ak;
            bk = b[kk -1];
            b[kk -1] = b[k2 -1];
            b[k2 -1] = bk;
            kk += inc;
            k2 += kspan;
            }   while (k2 < ks);

       do   {            /* while (kk < ks) */
            do   {           /*  while (k2 > np[j -1]) */
                 k2 -= np[j -1];
                 j++;
                 k2 += np[j+1 -1];
                 }   while (k2 > np[j -1]);
            j = 1;

            do   {       /*    while (k2 < ks) */
                 if ( kk < k2) goto m820;
                 kk += inc;
                 k2 += kspan;
                 }  while (k2 < ks);
           }  while (kk < ks);
       }                      /*******    if (n == ntot)  */
  else
       {   /* PERMUTATION FOR MULTIVARIATE TRANSFORM */

m850:
       do  {      /*   while (k2 < ks) */
           do   {       /* while (kk < nt) */
                k = kk + jc;
                do   {    /*  while (kk < k) */
                     ak       = a[kk -1];
                     a[kk -1] = a[k2 -1];
                     a[k2 -1] = ak;
                     bk       = b[kk -1];
                     b[kk -1] = b[k2 -1];
                     b[k2 -1] = bk;
                     kk += inc;
                     k2 += inc;
                     }  while (kk < k);
                kk += (ks - jc);
                k2 += (ks - jc);
                }  while (kk < nt);
           k2 += (-nt + kspan);
           kk += (-nt + jc);
           }  while (k2 < ks);
       do  {                           /*   while (kk < ks) */
           do   {
                k2 -=  np[j -1];
                j++;
                k2 +=  np[j+1 -1];
                }   while (k2 > np[j -1]) ;
           j = 1;
           do   {                      /*  while (k2 < ks)  */
                if (kk < k2 ) goto m850;
                kk += jc;
                k2 += kspan;        
                }   while (k2 < ks);
           }  while (kk < ks);
       }
  jc = k3;

m890:
  if (2*kt+1 >= m) 
     {
     if (nfac) free(nfac);
     if (np) free(np);
     if (at) free(at);
     if (bt) free(bt);
     if (ck) free(ck);
     if (sk) free(sk);
     return 0;
     }
  kspnn = np[kt+1-1];

/*-------PERMUTATION FOR SQUARE-FREE FACTORS OF N------------------------*/

  j = m - kt;
  nfac[j+1 -1] = 1;

m900:
  do  {
      nfac[j -1] *= nfac[j+1 -1];
      j--;
      }  while (j != kt);
  kt ++;
  nn = nfac[kt -1] -1;
  if (nn > maxp) 
      {
      *isn = 0; 
      if (nfac) free(nfac);
      if (np) free(np);
      if (at) free(at);
      if (bt) free(bt);
      if (ck) free(ck);
      if (sk) free(sk);
      return 0; 
      }      
  jj = j = 0;

  k2 = nfac[kt -1];
  k = kt + 1;
  kk = nfac[k -1];
  j++;     
  while (j <= nn) 
       {
       jj += kk;
       while (jj >= k2)
            {
            jj -= k2;
            k2 = kk;
            k++;
            kk = nfac[k -1];
            jj += kk;
            }     /*   while (jj >= k2)  */
       np[j -1] = jj;
       k2 = nfac[kt -1];
       k = kt + 1;
       kk = nfac[k -1];
       j++;     
       }       /* while (j <= nn)   */
   
/*-------DETERMINE THE PERMUTATION CYCLES OF LENGTH GREATER THAN 1-------*/

  j = 0;
  do   {                   /*   while (j != nn)   */
       do   {
            j++;
            kk = np[j -1];
            }   while (kk < 0) ;
       while (kk != j) 
            {
            do   {
                 k = kk;
                 kk = np[k -1];
                 np[k -1] = -kk;         
                 }  while (kk != j) ;
            k3 = kk;
            do   {             
                 j++;          
                 kk = np[j -1];
                 }   while (kk < 0) ;
            }   /* while (kk != j) */
       np[j -1] = -j;
       }    while (j != nn) ;

  maxf *= inc;

/*-------REORDER A AND B, FOLLOWING THE PERMUTATION CYCLES------------*/

  j = k3 + 1;
  nt -= kspnn;
  ii = nt - inc + 1;
  if (nt < 0) 
     {
     if (nfac) free(nfac);
     if (np) free(np);
     if (at) free(at);
     if (bt) free(bt);
     if (ck) free(ck);
     if (sk) free(sk);
     return 0;
     }
   
m924: 
  do   {                                /*    while (nt >= 0)  */
       do   {                                /* while (j != 1)  */
            do   {              
                 j--;
                 }  while (np[j -1] < 0) ;
            jj = jc;
            do   {                               /* while (jj != 0)  */
                 kspan = jj;
                 if (jj > maxf) kspan = maxf;
                 jj -= kspan;
                 k = np[j -1];
                 kk = jc * k + ii + jj;
                 k1 = kk + kspan;
                 k2 = 0;
                 do   {    /*   while (k1 != kk)  */
                      k2++;
                      at[k2 -1] = a[k1 -1];
                      bt[k2 -1] = b[k1 -1];
                      k1 -=inc;
                      }  while (k1 != kk);
                 
                 do   {                    /*   while (k != j)  */
                      k1 = kk + kspan;
                      k2 = k1 - jc * (k + np[k -1]);
                      k = -np[k -1];
                      do   {                  /*  while (k1 != kk)  */
                           a[k1 -1] = a[k2 -1];
                           b[k1 -1] = b[k2 -1];
                           k1 -= inc;
                           k2 -= inc;
                           }    while (k1 != kk) ;
                      kk = k2;
                      }     while (k != j);
                 k1 = kk + kspan;
                 k2 = 0;
                 do   {                     /*   while (k1 != kk)  */
     	              k2++;
                      a[k1 -1] = at[k2 -1];
                      b[k1 -1] = bt[k2 -1];
                      k1 -= inc;
                      }  while (k1 != kk) ;
                 }   while (jj != 0);
            }   while (j != 1);
            j = k3 + 1;
            nt -= kspnn;
            ii = nt - inc + 1;
       } while (nt >= 0);

  if (nfac) free(nfac);
  if (np) free(np);
  if (at) free(at);
  if (bt) free(bt);
  if (ck) free(ck);
  if (sk) free(sk);
  return 0;              

  }   /*******************  end if fftmcf *****************************/
