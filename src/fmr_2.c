/*************************************************************************
* changed to fmr_2.c       mahieddine ladjadj   5/25/94
* $$ FMR_2.FOR
*
C **********************************************************************
* *  SPIDER - MODULAR IMAGE PROCESSING SYSTEM.  AUTHOR: J.FRANK            *
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
*
*
*/

#include "routines.h"

/*****************************  fmr_2   *******************************/
int fmr_2(float *x, int nsam, int nrow, int * inv)
  {  
  /* x[nSAM][nROW]  note that this is FORTRAN way of ordering array */
  register int i,j;
  float *p, *work, *wtemp, q;
  int ins, msiz, irtflg;

  /* run time allocation */
  msiz = MYMAX(nsam, nrow);
  if ( (work = (float *) malloc(msiz * sizeof(float)) ) 
              == (float *) NULL)                                   {
              spout(" *** WARNING: return from fmr_2.c ");
              spout(" MALLOC() CAN'T ALLOCATE HEAP MEMORY SPACE");
              return 1;                                            }
   if ( (wtemp = (float *) malloc(msiz * sizeof(float)) ) 
              == (float *) NULL)                          {
              printf(" *** WARNING: return from fmr_2.c ");
              printf(" MALLOC() CAN'T ALLOCATE HEAP MEMORY SPACE");
            /*  free(work); */
              return 1;                                   }

  if ((*inv) > 0)
      {
      for (i = 1; i <= nrow; i++)
         {
         irtflg = fmr_1(&x[(i-1)*nsam+1-1],nsam,work,inv);
         if (!(*inv))  { free(work); free(wtemp);  return 0; }
         }

      if (nsam%2 > 0)
          {
          /* copy one row to work array */
          for (j = 1; j <= nrow; j++)
              work[j-1] = x[(j-1)*nsam+1-1];

          irtflg = fmr_1(work, nrow, wtemp, inv);
          if (!(*inv))   { free(work); free(wtemp);  return 0; }
    
          for (j = 1; j <= nrow; j++)
              x[(j-1)*nsam+1-1] = work[j-1];
          ins = nsam;
          for (i = 3; i <= (nsam-2); i+=2)
              {
              irtflg = fftmcf(&x[(1-1)*nsam+i-1], &x[(1-1)*nsam+i+1-1],
                      nrow, nrow, nrow, &ins);
              }
          ins = nsam;
          irtflg = fftmcf(&x[(1-1)*nsam+nsam-1], &x[(1-1)*nsam+2-1],
                   nrow, nrow, nrow, &ins);
          free(work); free(wtemp);
          return 0;
          }
      else
          {
          for (i = 1; i <= 2; i++)
              { 
              /* copy a row  */                         
              for (j = 1; j <= nrow; j++)
                  work[j-1] = x[(j-1)*nsam+i-1];

              irtflg = fmr_1(work,nrow,wtemp, inv);
              if (!(*inv))   { free(work); free(wtemp);  return 0; }
 
              for (j = 1; j <= nrow; j++)
                  x[(j-1)*nsam+i-1] = work[j-1];
              }
 
          ins = nsam;
          for (i = 3; i <= nsam; i+=2)
              {
              irtflg = fftmcf(&x[(1-1)*nsam+i-1], &x[(1-1)*nsam+i+1-1],
                  nrow, nrow, nrow, &ins);
              }
          free(work); free(wtemp);
          return 0;
          }
      }    /*************  if (inv)  ***/  

  q = 1.0 / (float) nrow;
  for (j = 1; j <= nrow; j++)
       for (i = 3; i <= nsam; i++)
            x[(j-1)*nsam + i-1] *= q;
  if(nsam%2 > 0)
      {   
      for (j = 1; j <= nrow; j++)
            x[(j-1)*nsam+2-1] *= q;
      for (j = 1; j <= nrow; j++)
            work[j-1] = x[(j-1)*nsam+1-1];

      irtflg = fmr_1(work, nrow, wtemp, inv);
      if (!(*inv))  { free(work); free(wtemp);  return 0; }

      for (j = 1; j <= nrow; j++)
             x[(j-1)*nsam+1-1] = work[j-1]; 
      ins = - nsam;
      for (i = 3; i <= nsam-2; i+=2)
          {
          irtflg = fftmcf(&x[(1-1)*nsam+i-1], &x[(1-1)*nsam+i+1-1],
               nrow, nrow, nrow, &ins);
          }
      irtflg = fftmcf(&x[(1-1)*nsam+nsam-1], &x[(1-1)*nsam+2-1],
             nrow, nrow, nrow, &ins);
      }
  else
      {
      for (i = 1; i <= 2; i++)
          {
          for (j = 1; j <= nrow; j++)
               work[j-1] = x[(j-1)*nsam+i-1];

          irtflg = fmr_1(work, nrow, wtemp, inv);
          if (!(*inv))   { free(work); free(wtemp);  return 0; }

          for (j = 1; j <= nrow; j++)
               x[(j-1)*nsam+i-1] = work[j-1];
          }
      ins = - nsam;
      for (i = 3; i <= nsam; i+=2)
          {
          irtflg = fftmcf(&x[(1-1)*nsam+i-1], &x[(1-1)*nsam+i+1-1],
                 nrow, nrow, nrow, &ins);
          }
      }
  for (i = 1; i <= nrow; i++)
      {
      irtflg = fmr_1(&x[(i-1)*nsam+1-1], nsam, wtemp, inv);
      if (!(*inv))  { free(work); free(wtemp);  return 0; }
      }

  free(work); free(wtemp);
  return 0;
  }     /***************   end of fmr_2  *********/

