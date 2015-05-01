/* 
C++************************************************************************
C
C cald.c	Converted from Fortran program in Nov 95
C
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
C
C PURPOSE: Combine two sets of euler angles and return resulting angles
C
C23456789012345678901234567890123456789012345678901234567890123456789012
C--************************************************************************
C
*/
#include "common.h"
#include "routines.h"
#include "x.h"
#include "std.h"
#include <math.h>

/* Constants */
#define	DEPS	1.0e-7

/******************************** cald ***********************/
float* cald(float *a1, float *a2)
{
    float	r1[9], r2[9], r3[9];		/* Rotation matrix */
    float	psi, theta, phi;		/* Eular angles    */
    int		i, j, k;                        /* Loop variables  */
    float	*atmp1, *atmp2;

    /* Change angle to euler defination */
    atmp1 = a1;
    atmp2 = a2;
    for (i=0; i<3; i++) {
	*atmp1 = antiang(*atmp1);
	*atmp2 = antiang(*atmp2);
	atmp1++;  atmp2++;
	}

    /* Clear the buffers */
    for (i=0; i<9; i++) 
        { r1[i] = 0.0;  r2[i] = 0.0; r3[i] = 0.0; }

    /* Build up the matrix from a1 and a2 */
    bldr(a2[0], a2[1], a2[2], &r2[0]);
    bldr(a1[0], a1[1], a1[2], &r1[0]);
  
    /* Get the new matrix */
    for (i=0; i<3; i++) {
	for (j=0; j<3; j++) {
	    for (k=0; k<3; k++) 
	    	r3[i*3+j] = r3[i*3+j] + r2[i*3+k] * r1[k*3+j];
    	    }
	}

    /* Formalize the numbers */
    for (i=0; i<9; i++) {
	if (fabs(r3[i]) < DEPS) 	r3[i] = 0.0;
	if (r3[i]-1.0  > -DEPS) r3[i] = 1.0;
	if (r3[i]+1.0  < DEPS)  r3[i] = -1.0;
	}

    /* Calculate the angles */
    if (r3[8] == 1.0) {
	theta = 0.0;
	psi   = 0.0;

	if (r3[0] == 0.0)	phi = angle(asin(r3[1]));
	else { 
	  if (r3[0]>=0.0 && r3[1]>=0.0)	
	    phi = angle(atan(r3[1]/r3[0]));
	  else if (r3[0]<0.0)
	    phi = 180.0 + angle(atan(r3[1]/r3[0]));
	  else		
	    phi = 360.0 + angle(atan(r3[1]/r3[0]));
	     }
 	}

    else if (r3[8] == -1.0) {
	theta = 180.0;
	psi   = 0.0;

	if (r3[0] == 0.0)	phi = angle(asin(-r3[1]));
	else {
 	  if (r3[0]<=0.0 && r3[1]<=0.0)	
	    phi = angle(atan(r3[1]/r3[0]));
	  else if (r3[0]>0.0)
	    phi = 180.0 + angle(atan(r3[1]/r3[0]));
	  else		
	    phi = 360.0 + angle(atan(r3[1]/r3[0]));
	     }
	}

    else {
	    theta = angle(acos(r3[8]));

	if (r3[6] == 0.0)
            {
	    if (theta*r3[7] <= 0.0)  
               psi = 270.0;
	    else		
               psi = 90.0;
	    }
	else 
          {
	  if (r3[6]>=0.0 && r3[7]>=0.0)
	    psi = angle(atan(r3[7]/r3[6]));
	  else if (r3[6]<0.0)
	    psi = 180.0 + angle(atan(r3[7]/r3[6]));
	  else  
	    psi = 360.0 + angle(atan(r3[7]/r3[6]));
	    }

	if (r3[2] == 0.0) {
	    if (theta*r3[5] <= 0.0)	phi = 270.0;
	    else			phi = 90.0;
	    }
	else
          {
	  if (-r3[2]>=0.0 && r3[5]>=0.0) 
	    phi = angle(atan(r3[5]/-r3[2]));
	  else if (-r3[2]<0.0)
	    phi = 180 + angle(atan(r3[5]/-r3[2]));
	  else 
	    phi = 360.0 + angle(atan(r3[5]/-r3[2]));
	  }
	}

    a1[0] = roundang(phi);
    a1[1] = roundang(theta);
    a1[2] = roundang(psi);

    for (i=0; i<3; i++)
        {
	if ( a1[i] == 360.0 )
	    {a1[i] = 0.0;}
	}

    return a1;
}


	
