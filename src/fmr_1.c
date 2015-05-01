/*************************************************************************
* changed to fmr_1.c       mahieddine ladjadj   5/25/94
* $$ FMR_1.FOR
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
*
*************************************************************************
*
* 
*/

#include "routines.h"

/********************  fmr_1   ********************************************/

int fmr_1(float * x, int n, float * work, int * inv)
  { 
  /*  inv can be +1 (forward FFT) or -1 (inverse FFT) */
  register int i;
  int irtflg ;

  if ((*inv) > 0)
      {
      for (i = 0; i < n; i++)
          work[i] = 0.0;
      irtflg = fftmcf(x, work, n, n, n, inv);
      if(n%2 > 0) 
          {   /* positive result */
          x[n-1] = x[n/2+1-1];
          for (i = n-2; i >= 3; i -= 2)
               {
               x[i-1]   = x[i/2+1-1];
               x[i+1-1] = work[i/2+1-1];
               }
          x[2-1] = work[n/2+1-1];
          return 0;
          }

      /* n%2 <= 0 */
      work[1-1] = x[n/2+1-1];
      for (i = n-1; i >= 3; i -=2)
          {
          x[i-1]   = x[(i+1)/2-1];
          x[i+1-1] = work[(i+1)/2-1];
          }
      x[2-1] = work[1-1];
      return 0;
      }  /************** if (*inv)  *****************/

  /* (*inv) less or equal to 0 */
  if (n%2 > 0)
      {  /* positive result */
      work[n/2+1-1] = x[2-1]/n;
      work[n/2 + 2-1] = -x[2-1]/n;
      for (i = 2; i <= n/2; i++)
          {
          work[i-1]   = x[2*i-1]/n;
          work[n-i+2-1] = -work[i-1];
          }
      x[1-1] = x[1-1]/n;
      for (i = 2; i <= (n/2+1); i++)
          x[i-1] = x[2*i-1-1]/n;
      for (i = n; i >= n/2+2; i--)
          x[i-1] = x[n-i+2-1];
      }
  else
      {
      work[n/2+1-1] = 0.0;
      for (i = 2; i <= n/2; i++)
          {
          work[i-1]   = x[2*i-1]/n;
          work[n-i+2-1] = -work[i-1];
          }
      work[1-1] = x[2-1]/n;
      x[1-1] = x[1-1]/n;
      for (i = 2; i <= n/2; i++)
          x[i-1] = x[2*i-1-1]/n;
      for (i = n/2+2; i <= n; i++)
          x[i-1] = x[n-i+2-1];   
      x[n/2+1-1] = work[1-1];
      }

  work[1-1] = 0.0;
  irtflg = fftmcf(x, work, n, n, n, inv);
  return 0;

  }   /*****************  end of fmr_1   ***********/
