
/*$Header: /usr16/software/web/src/RCS/annote.c,v 1.7 2018/12/07 17:03:30 leith Exp $*/

/***********************************************************************
 * annote.c
 *
 * Ported WID_ANNOTE.FOR (VAX) to Unix.
 *
 ***********************************************************************
 * author : Mahieddine Ladjadj
 C=* FROM: WEB - VISUALIZER FOR SPIDER MODULAR IMAGE PROCESSING SYSTEM *
 C=* Copyright (C) 1992-2005  Health Research Inc.                     *
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
 ***********************************************************************
 *
 * annote(* filnam)
 * 
 * PURPOSE:    places labels from a doc file on top of an image display.
 *
 * RETURNS:    true or false.
 *
 * PARAMETERS: filnam      filename of doc file         :input
 *
 ***********************************************************************
*/

#include "common.h"
#include "routines.h"
#include "annote.h"


/* external common variables */
  

/***********************  annote *************************************/

int annote(char *filnam)
{
  register int k;
  int      icall, first, firstl, ix, iy, nlist, maxkey, pkey, maxreg;
  int      numcor, numtot, nchar, hikey;
  float    *dbuf, plist[8], x, y;
  int      labno;
  float    *ptr;
  float    scale;   /* dgm adds scale and the following lines */
  char outstr[81];
  
  if ( iskip < 1 ) {
     scale = -iskip ;
   } else if ( iskip > 1 ) {
     scale = 1. / (float) iskip;
   } else {
     scale = 1;
   }


  /*    recover all of doc file contents and store in dbuf.
  **    key "ikey" is stored in dbuf[ikey][] row.
  **    dbuf[ikey][0] = number of registers for that key
  **    following columns are registers values for each reg.
  */

  icall  = TRUE;
  nlist  = MYMAX (MYMAX(ix_reg, iy_reg), lab_reg);
  maxkey = MYMAX (tkey1,tkey2);             
  maxreg = MYMAX (MYMAX(ix_reg, iy_reg), lab_reg) + 1;
  
  /* plist not needed here, assigned plist[1] */
  pkey = 0;
  if (unsdal(filnam,&icall,pkey,plist,nlist,&dbuf,&hikey,maxkey,maxreg) > 0) 
       return FALSE;

  first  = TRUE;
  firstl = FALSE;
  numcor = numtot = 0;

  ptr    = dbuf;
  
  for (k = tkey1; k <= hikey; k++)
      {
      if      (*ptr < ix_reg)
          {
          /* no register for ix*/
          sprintf(outstr,"*** Key: %d lacks X reg:%d", k,ix_reg);
          spout(outstr);
          }

      else if (*ptr < iy_reg)
          {
          /* no register for iy */
          sprintf(outstr,"*** Key: %d lacks Y reg:%d", k,iy_reg);
          spout(outstr);
          }
      else if (*ptr < lab_reg)
          {
          /* no register for label */
          sprintf(outstr,"*** Key: %d lacks label reg: %d", k,lab_reg);
          spout(outstr);
          }
      else
          {
          x = *(ptr + ix_reg);
          y = *(ptr + iy_reg);

          /*  is location within an image */
          if (x > 0 && x <= nsam  &&  y > 0 && y <= nrow)
              {
              /* find screen position (dgm adds scale factor) */
              ix = ixul + x * scale - 1;
              iy = iyul + y * scale - 1;

              /* draw a permanent circle at this position */
              xorc(iwin,    icontx, TRUE, ix, iy, iarad);
              xorc(imagsav, icontx, TRUE, ix, iy, iarad);

              /* find label number */
              if (lab_reg > 0)
                 labno = *(ptr+lab_reg);
              else
                 labno = k;

              nchar = numdig(labno, 1);
              if (nchar <= 4) 
                 {
                 /* write marker at this position */
                 sprintf(outstr,"%d",labno);
                 witext(icontx,outstr,ix,iy,0,0,-9,2,FALSE);
                 }
              else if (firstl)
                 {
                 /* write a warning one time */
                 spout("*** Label number > 4 digits");
                 firstl = FALSE;
	         }
              numcor++;
              }
          else if (first)
              {
              /* write a warning one time */
              spout("*** Labels(s) outside of image!");
              first =  FALSE;
              }
          numtot++;
          }     /* else ends */
      ptr  += maxreg; 

      }  /* end of for loop */

  sprintf(outstr,"Labels: %d  -- Labels within image extent: %d",
                   numtot,numcor);
  spout(outstr);
  return TRUE;
}
