/***************************************************************************
*
* POWER.FOR 
*
C **********************************************************************
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
*  POWER(X,WORK,NSAM,NROW)
*
*  PURPOSE:      POWER SPECTRUM CALCULATION
*
*  PARAMETERS:   
*
*  CALLED BY:    WID_POWER
*
*  note: in the fortran version, a ( horizontal) line on the screen
*        is stored in a column, i.e; line 5 was x(i,5) in array x(i,j)
*	 since variable i varies the fastest ( storage in fortran is done
*	 by column first).
*
*	 in c, we had to reverse that since in x(i,j) j varies fastest.
*
*	 so x(ntrow, ntsam).
*/

#include "common.h"
#include "routines.h"

/********************************* power() ************************************/
int power(float *x, int ntsam, int ntrow)
  {
  register int i, j;
  int irtflg, ifns, ifnr, ns2, nr2, k, ins, i1, j1;
  float  scl, t;
                  
  ifns = (ntsam%2 == 0) ? 1 : 0;
  ifnr = (ntrow%2 == 0) ? 1 : 0;
  ns2 = (int) ntsam/2;
  nr2 = (int) ntrow/2;
  scl = 2.0 / ((float) ntsam  * (float) ntrow);

  ins = 1;
  irtflg = fmr_2(x,ntsam,ntrow,&ins);
  if (!ins)
      { spout(" *** error in routine fmr_2 "); return 1;}

  /* for lower half */
  for (i=1; i <=ntrow; i++)
      {
      k = ntsam;
      for (j=ntsam; j >=3 ; j-=2,  k--)
          {
          x[(i-1)*ntsam + k-1] = scl * 
                  sqrt(x[(i-1)*ntsam +j-1] * x[(i-1)*ntsam +j-1] +
                       x[(i-1)*ntsam +j-1-1]*x[(i-1)*ntsam +j-1-1]);
          }
      }
  if (ifns)
      {
      if (ifnr)
          {
          /* for two unlucky lines */
          x[(1-1)*ntsam+ns2+1-1] = scl * 
	      (float) fabs((double)(x[(2-1)*ntsam+1-1]));
          k = ntrow-1;      
          for (j=2; j <= nr2; j++, k -= 2)
              {
              x[(j-1)*ntsam+ns2+1-1] = scl * 
                        sqrt(x[(k-1)*ntsam+1-1] * x[(k-1)*ntsam+1-1] 
                      + x[(k+1-1)*ntsam+1-1] * x[(k+1-1)*ntsam+1-1]);
              x[(ntrow-j+2-1)*ntsam + ns2+1-1] = x[(j-1)*ntsam + ns2+1-1];
              }
          x[(1-1)*ntsam+1-1] = scl *
	        (float) fabs((double)(x[(2-1)*ntsam +2-1]));
          x[(nr2+1-1)*ntsam+1-1] = scl *
	        (float) fabs((double)(x[(1-1)*ntsam+2-1]));
          k = ntrow-1;
          for (j=2; j <= nr2; j++, k -= 2)
              {
              x[(j-1)*ntsam+1-1] =  scl * 
                         sqrt(x[(k-1)*ntsam + 2-1]*x[(k-1)*ntsam + 2-1] + 
                          x[(k+1-1)*ntsam + 2-1]*x[(k+1-1)*ntsam + 2-1]);
              x[(ntrow -j+2-1)*ntsam + 1-1] = x[(j-1)*ntsam+1-1];
              }
          /* rewrite symmetric parts */
          /* left side */
          i1 = nr2+1;
          for (i=1; i <= (nr2+1); i++, i1--)
              {
              j1 = ntsam;
              for (j=2; j <= ns2; j++, j1--)
                  {
                  x[(i-1)*ntsam + j-1] = x[(i1-1)*ntsam + j1-1];
                  }        
              }       
          /* right side */
          i1 = ntrow;
          for (i=(nr2+2); i <= ntrow; i++, i1--)
              {
              j1 = ntsam;
              for (j=2; j <= ns2; j++, j1--)
                  {
                  x[(i-1)*ntsam + j-1] = x[(i1-1)*ntsam + j1-1];
                  }
              }

          /* sort lower half */
          i1 = nr2+1;
          for (i=1; i <= nr2; i++, i1++)
              {
              for (j=(ns2+2); j <= ntsam; j++)
                  {
                  t = x[(i-1)*ntsam + j-1];
                  x[(i-1)*ntsam + j-1] = x[(i1-1)*ntsam + j-1];
                  x[(i1-1)*ntsam + j-1] = t;
                  }
              }

          }   /* if (ifnr) */
      else      
          {
          /* for two unlucky lines */
          k = ntrow-1;
          for (j=1; j <=nr2; j++, k -= 2)
              {
              x[(j-1)*ntsam + ns2+1-1] =  scl * 
                        sqrt(x[(k-1)*ntsam+1-1] * x[(k-1)*ntsam+1-1]
                        + x[(k+1-1)*ntsam+1-1] * x[(k+1-1)*ntsam+1-1]); 
              x[(ntrow -j+1-1)*ntsam + ns2+1-1] = x[(j-1)*ntsam + ns2+1-1];
              }
          x[(nr2+1-1)*ntsam+1-1] = scl*
	   (float) fabs((double)(x[(1-1)*ntsam+2-1]));
          k = ntrow-1;
          for (j=1; j <=nr2; j++, k -= 2)
              {
              x[(j-1)*ntsam+1-1] =  scl * 
                     sqrt(x[(k-1)*ntsam+ 2-1] * x[(k-1)*ntsam+ 2-1] + 
                               x[(k+1-1)*ntsam+ 2-1] * x[(k+1-1)*ntsam+ 2-1]);
              x[(ntrow -j + 1-1)*ntsam+1-1] = x[(j-1)*ntsam+1-1];
              }

          /* rewrite symmetric parts */
          /* left side */
          i1 = nr2+1;
          for (i=1; i <= nr2+1; i++, i1--)
              {
              j1 = ntsam;
              for (j=2; j <= ns2; j++, j1--)
                  {
                  x[(i-1)*ntsam + j-1] = x[(i1-1)*ntsam + j1-1];
                  }
              }

          /* right side */
          i1 = ntrow;
          for (i=nr2+2; i <= ntrow; i++, i1--)
              {
              j1 = ntsam;
              for (j=2; j <= ns2; j++, j1--)
                  {
                  x[(i-1)*ntsam + j-1] = x[(i1-1)*ntsam + j1-1];
                  }
              }
                          
          /* copy and transpose upper part onto lower half */
          i1 = ntrow;
          for (i=1; i <= ntrow; i++, i1--)
              {
              j1 = ntsam;
              for (j=2; j <= ns2; j++, j1--)
                  {
                  x[(i1-1)*ntsam + j1-1] = x[(i-1)*ntsam + j-1];
                  }
              }
          }     /* else if (ifnr) */
      }        /* if (ifns) */
  else     
      {
      if (ifnr)
          {
          /* for one unlucky lines */
          x[(1-1)*ntsam+ns2+1-1] = scl *
	    (float) fabs((double)(x[(2-1)*ntsam+1-1]));
          k = ntrow-1;
          for (j = 2; j <= nr2; j++, k -= 2)
              {
              x[(j-1)*ntsam +ns2+1-1] =  scl * 
                    sqrt(x[(k-1)*ntsam+1-1] * x[(k-1)*ntsam+1-1]
                     + x[(k+1-1)*ntsam+1-1] * x[(k+1-1)*ntsam+1-1]);
              x[ (ntrow -j + 2-1)*ntsam +ns2+1-1] = x[(j-1)*ntsam + ns2+1-1];
              }

          /* rewrite symmetric parts */
          /* left side */                                   
          i1 = nr2+1;
          for (i=1; i <= (nr2+1); i++, i1--)
              {
              j1 = ntsam;
              for (j=1; j <= ns2; j++, j1--)
                  {
                  x[(i-1)*ntsam + j-1] = x[(i1-1)*ntsam + j1-1];
                  }
              }

          /* right side */
          i1 = ntrow;
          for (i=nr2+2; i <= ntrow; i++, i1--)
              {
              j1 = ntsam ;
              for (j=1; j <= ns2; j++, j1--)
                  {
                  x[(i-1)*ntsam + j-1] = x[(i1-1)*ntsam + j1-1];
                  }
              }

          /* sort lower half */
          i1 = nr2+1;
          for (i=1; i <= nr2; i++, i1++)
              {
              for (j=ns2+2; j <= ntsam; j++)
                  {
                  t = x[(i-1)*ntsam + j-1];
                  x[(i-1)*ntsam + j-1] = x[(i1-1)*ntsam + j-1];
                  x[(i1-1)*ntsam + j-1] = t;
                  }
              }
          }
      else      /* if (ifnr) */
          {
          /* for ONE unlucky lines */
          k = ntrow -1;
          for (j=1; j <= nr2; j++, k -= 2)
              {
              x[(j-1)*ntsam + ns2+1-1] =  scl *
                 sqrt(x[(k-1)*ntsam+1-1] * x[(k-1)*ntsam+1-1] 
                 +x[(k+1-1)*ntsam+1-1] * x[(k+1-1)*ntsam+1-1]);
              x[(ntrow -j+1-1)*ntsam +ns2+1 -1] = x[(j-1)*ntsam + ns2+1-1];
              }

          /* rewrite symmetric parts */
          /* left side */
          i1 = nr2+1;
          for (i=1; i <= nr2+1; i++, i1--)
              {
              j1 = ntsam;
              for (j=1; j <= ns2; j++, j1--)
                  {
                  x[(i-1)*ntsam + j-1] = x[(i1-1)*ntsam + j1-1];
                  }
              }

          /* right side */
          i1 = ntrow;
          for (i=nr2+2; i <= ntrow; i++, i1--)
              {
              j1 = ntsam ;
              for (j=1; j <= ns2; j++, j1--)
                  {
                  x[(i-1)*ntsam + j-1] = x[(i1-1)*ntsam + j1-1];
                  }
              }
                   
          /* copy and transpose upper part onto lower half */
          i1 = ntrow;
          for (i=1; i <= ntrow; i++, i1--)
              {
              j1 = ntsam;
              for (j=1; j <= ns2; j++, j1--)
                  {
                  x[(i1-1)*ntsam + j1-1] = x[(i-1)*ntsam + j-1];
                  }
              }       
          }     /* else if (ifnr) */
      }    /*  else if (ifns) */

  /* set the (0,0) to the neighboring value */
  x[(nr2+1-1)*ntsam + ns2+1-1] = x[(nr2-1)*ntsam+ns2-1];

/*
  if(mode == 'l') al10(x,ntsam*ntrow);
*/

  return 0;
  }

/*****************  end of power() ************************************/
