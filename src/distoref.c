
/*$Header: /usr8/web/src/RCS/distoref.c,v 1.3 2005/10/18 16:59:46 leith Exp $*/

/*
C**************************************************************************
C
C   DISTOREF.C  -- 
C             
C **********************************************************************
C *  AUTHOR: ArDean Leith                                *
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
C DISTOREF()
C
C  PURPOSE:     CONVERTS  2-D DISTANCE ARRAY TO 2-D REFLECTION ARRAY
C
C  PARAMETERS:  
C
C  RETURNS:     VOID
C
C  CALLED BY:   DENT
C
C--*********************************************************************
*/

#include "common.h"
#include <assert.h>
  
 /**************************   distoref   ****************************/

 void distoref(int idepth, int nsams, int nrows, int * distimg, 
          unsigned char * refmap, float fct, float csfact, 
          float fd1, float fd2,
          int nsam1t, int nsam2t, int nrow1t, int nrow2t,
          int mapgot)        

 {
 unsigned char  *cptr;
 int            *distemp;
 int            npixels;
 int            iloc, idist, irow, isam;
 int            nsam1, nsam2, nrow1, nrow2;     /* not global ! */
 int            iv, ih, igrad, ival, icon;
 float          fctd4, divbyzero;

 npixels = nsams * nrows;

 /* take care of first line */
 nrow1 = nrow1t;
 if (nrow1 < 2) 
    {
    for (cptr = refmap; cptr < refmap + nsams; cptr++)
       *cptr = mapgot;
    nrow1 = 2;
    }
 
 /* take care of final line */
 nrow2 = nrow2t;
 if (nrow2 >= nrows) 
    {
    for (cptr = refmap+npixels-nsams; cptr < refmap + npixels; cptr++)
       *cptr = mapgot;
    nrow2 = nrows-1;
    }

 /* take care of first col. */
 nsam1 = nsam1t;
 if (nsam1 < 2)
    {
    for (cptr = refmap; cptr < refmap + npixels; cptr+=nsams)
       *cptr = mapgot;
    nsam1 = 2;
    }


 /* take care of last col. */
 nsam2 = nsam2t;
 if (nsam2 >= nsams) 
    {
    for (cptr = refmap + nsams-1; cptr < refmap + npixels; cptr+=nsams)
       *cptr = mapgot;
    nsam2 = nsams-1;
    }

 fctd4 = fct / 4.0;
 divbyzero = -0.99 / fctd4;

 /* set starting location for this row in reflection image */
 for (irow = nrow1; irow <= nrow2; irow++)
    {
    icon = irow * nsams - nsams - 1;
    for (isam = nsam1; isam <= nsam2; isam++)
       {
       /* find location pointer inside map */
       /* iloc = (irow - 1) * nsams + isam - 1; original */
       iloc    = icon + isam;
       distemp = distimg + iloc;
       idist   = *distemp;

       if (idist < 1 || idist > idepth) 
          {  /* do not have a positive distance in distmap */
          *(refmap + iloc) = mapgot;
          }
       else  
          {    
          /* find reflection component of intensity and mix with the
          desired amount of distance shading and normalize to 0...127 */

          ih    = *(distemp+1)     - *(distemp-1);
          iv    = *(distemp+nsams) - *(distemp-nsams);
          igrad = ih * ih + iv * iv;

          /* avoid division by zero */
          if (igrad < divbyzero)
             {
             ival = fd1 * idist + fd2;
             }
          else
             {
             ival = fd1 * idist + fd2 
                   + csfact / sqrt((fctd4 * igrad) + 1.0);
             }

          /***************** debug */
          assert (ival >= mapgot);          
          assert (ival < 255);          
          assert (iloc < nsams * nrows);          
          /***************************/

          *(refmap+iloc) = ival;
          }
       }
    }
 }
