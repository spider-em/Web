
/*$Header: /usr16/software/web/src/RCS/cantrot.c,v 1.4 2018/12/07 17:03:30 leith Exp $*/

/*
C**************************************************************************
C
C   CANTROT.C  -- CREATED DEC 93 by lo 
C                 CONVERTED FORM SURF.C written by al
C             
C **********************************************************************
C *  AUTHOR: ArDean Leith / Lekan Odesanya                                 *
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
C CANTROT(NSAM1,NSAM2,NROW1,NROW2,NSLICE1,NSLICE2,
C      THLEV,REFL,FCT,PHI,THETA,PSI,CANROTATE)
C
C  PURPOSE:     READS SPIDER 3-D PICTURE FILE, CREATES 2-D 
C               REFLECTION IMAGE OF LEFT SIDE VIEW
C
C  PARAMETERS:  NSAM1,NSAM2     SAMPLE RANGE
C               NROW1,NROW2     ROW RANGE
C               NSLICE1,NSLICE2 SLICE RANGE
C               THLEV           THRESHOLD DESIRED
C               REFL            REFLECTION %
C               FCT             ATTENUATION %
C               PHI,THETA,PSI   VIEWING ANGLES
C               FASTSURF        LOGICAL FLAG NON-INTERPOLATE ROTS
C               CANROTATE       LOGICAL FLAG FOR ROTATE IN MEMORY
C
C  RETURNS:     VOID
C
C  CALLED BY:   SURF
C
C  NOTES:       FOR EACH PIXEL, THE PREVIOUS PIXEL ON THE ROW AND THE
C               PIXEL ONE COL. DOWN ARE USED TO FORM A TRIANGULAR TILE
C               THE TILE IS SHADED ACCORDING TO ITS ORIENTATION ACROSS
C               THE X AXIS AND ITS DISTANCE ALONG THE X AXIS FROM THE
C               LEFT END OF THE FILE.
C
C--*********************************************************************
*/

#include "common.h"
#include "routines.h"
#include "surf.h"

 /* external function prototype */


 
 /**************************   cantrot   ****************************/

 void cantrot(int nsams, int nrows, float *idistl, 
          unsigned char * refmap, float csfact, float fd1, float fd2)
              

 {
 int            i, iloc, irown, itemp;
 int            islicet, irow, isam, isamn, irect;
 float          * buf, ft, iv2, lastdl, ivall;


 /* find buf for redlin */
 buf = filedata -> buf;

    /* NOTE: Distance shading is done according to original nsam     */
    /*       not the windowed nsam1 dimension                        */


 /*  set starting location in reflection image */
 iloc   = -1;

 for (islicet = nslice1; islicet <= nslice2; islicet++)
    {
    irown = islicet - nslice1 + 1;
    itemp = islicet * nrow - nrow;

    /* set starting location for this row in reflection image */
    for (irow = nrow1; irow <= nrow2; irow++)
       {
       isamn = irow - nrow1; 

          /*  find file record for this distance ray  */
          /*  irect = (islicet - 1) * nrow + irow     */
          /*  irect = islicet * nrow - nrow + irow    */

          irect = itemp + irow;   
          redlin(filedata,buf,nsam,irect);
          ivall = 0;
          for (isam = nsam1-1; isam < nsam2; isam++)
             {
             if (buf[isam] >= thlev)
                {
                /*  voxel is at or above threshold, stop hunting */
                ivall = isam + 1.0;
                break;
                }
             }


       /* surface located, now find value for the reflection map */

       if (irown > 2 && irown <= nrows) 
          {    
          /* non-zero row in refmap, (filling preceeding row) */
         
          if (isamn > 0 && isamn < nsams-1) 
             {    /* non-zero column in refmap */

             iv2  = idistl[isamn];
          
             if (lastdl > 0.0 && iv2 > 0.0 && ivall > 0.0)
                {  
                /* all 3 corner pixels of tile have positive distance
                   find reflection component of intensity and mix with the
                   desired amount of distance shading and normalize to 0...127
                **/

                   ft   = (iv2 - lastdl) * (iv2 - lastdl) + 
                       (iv2 - ivall)  * (iv2 - ivall);

                   refmap[++iloc] = fd1*ivall + fd2 
                                  + csfact / sqrt((fct * ft) + 1.0);
                }
             else
                {   /* >= one corner pixel does not have + distance */
                   refmap[++iloc] = imagego;
                }

             /* save the current idistl for use by next pixel to right */
               lastdl = iv2;
             }
          else 
             {
             /*  first & last col of reflection image are always zero */
             lastdl = idistl[isamn];
             refmap[++iloc] = imagego;
            }
          }
       else if (irown == 2)
          {
          /*  first row of reflection image is always zeroed */
          refmap[++iloc]  = imagego;
          }

       /* record present distance for use by next row */
       idistl[isamn] = ivall;
       }
    }

 /* zero last row(s) of reflection image */
 for (i = nsams*(nslice2-1); i < nsams*nrows; i++)
      refmap[i] = imagego;

 if (nslice1 > 1)
    {   /* fill top of image */
      for (iloc=0; iloc < nslice1-1 * nrow; iloc++)
       refmap[iloc] = imagego;
    }
 }
