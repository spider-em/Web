
/*$Header: /usr16/software/web/src/RCS/windata.c,v 1.4 2018/12/07 17:06:44 leith Exp $*/
/*
 C++********************************************************************
 C
 C WID_WINDAT.FOR -- CREATED MAY 91 al
 C                   RENAMED WINDATA, CONVERTED TO C -- SEPT 92 al 
 C     
 C *********************************************************************
 C=* FROM: WEB - VISUALIZER FOR SPIDER MODULAR IMAGE PROCESSING SYSTEM *
 C=* Copyright (C) 1992-2018  Health Research Inc.                     *
 C=* Riverview Center, 150 Broadway, Suite 560, Menands, NY 12204.     *
 C=* Email:  spider@health.ny.gov                                      *
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
 C *********************************************************************
 C
 C WINDATA
 C
 C PURPOSE:       DESCRIBES SELECTED WINDOW
 C
 C PARAMETERS:    NONE
 C
 C CALLS:         X-WINDOWS ROUTINES
 C
 C--*********************************************************************
*/

#include "std.h"
#include "x.h"
#include "common.h"
#include "routines.h"

 /* external functions */

 /************************* windata ********************************/
 

 void windata(int ixi, int iyi, int nsamd, int nrowd)
 {
 char outstr[60];

 if (leavit)
    {    /* leave box around window at this location */
    xorbox(iwin,    icontx, TRUE, ixulw, iyulw, nsamw, nroww);
    xorbox(imagsav, icontx, TRUE, ixulw, iyulw, nsamw, nroww);
    }

 if (sayit)
    {    /* list window coordinates */
    sprintf(outstr," Screen window: (%d,%d) %d X %d",
                     ixulw,iyulw,nsamw,nroww);
    spout(outstr);
    if (ixi > 0)
       {    /* list image coordinates */
       sprintf(outstr," Image  window: (%d,%d) %d X %d",
                        ixi,iyi,nsamd,nrowd);
       spout(outstr);
       }
    }
 }


#ifdef NEVER
 ********************** FUTURE *******************************
 if (leavit)
          RECORD WINDOW COORDINATES IN DOC FILE
          IDOC = IDOC + 1
          DLIST(1) = IDOC
          DLIST(2) = IXULW
          DLIST(3) = IYULW
          DLIST(4) = NSAMW
          DLIST(5) = NROWW
          CALL SAVDN1(LUNDOCW,DOCNAM,DLIST,5,NRUN,0)
       ENDIF
       IF (SAYIT .AND. DOCIT) THEN
C         LIST WINDOW COORDINATES WITH DOC FILE KEY

          WRITE(OUTSTR,8000) IDOC,IXI,IYI,NSAMD,NROWD
8000      FORMAT('IMAGE WINDOW: ',I4,' (',I4,',',I4,')...',I4,' X',I4)
          CALL WID_OUTPUT(0,OUTSTR,0)

#endif 
