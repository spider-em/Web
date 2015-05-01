/* 
C++************************************************************************
C
C  bldr.c	Converted from Fortran program in Nov 95
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
C  Purpose:	get the rotation matrix from the given eular angles 
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

/**************************** bldr *******************************/
 
void 	bldr(float psi, float theta, float phi, float *matrix)
{
    float	cpsi, ctheta, cphi;		/* cos values */
    float	spsi, stheta, sphi;		/* sin values */
    float	r[9];				/* return matrix */
    int		i;

    cphi   = cos(phi);
    sphi   = sin(phi);

    ctheta = cos(theta);
    stheta = sin(theta);

    cpsi   = cos(psi);
    spsi   = sin(psi);

    r[0] = cphi * ctheta * cpsi - sphi * spsi;
    r[3] = -cphi * ctheta * spsi - sphi * cpsi;
    r[6] = cphi * stheta;
    r[1] = sphi * ctheta *cpsi + cphi * spsi;
    r[4] = -sphi * ctheta *spsi + cphi * cpsi;
    r[7] = sphi * stheta;
    r[2] = -stheta * cpsi;
    r[5] = stheta * spsi;
    r[8] = ctheta;

for (i=0; i<9; i++) {
    *matrix = r[i];
    matrix++;
    }
 
    return;
}

