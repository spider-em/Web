
/*$Header: /usr8/web/src/RCS/setprm.c,v 1.9 2012/11/27 19:12:42 leith Exp $*/
/*
 C++********************************************************************
 C
 C SETPRM   VAX FORTRAN original                1/1/81
 C          MRC support, remuved type set       Nov 2012  ArDean Leith  
 C
 C *********************************************************************
 C=* FROM: WEB - VISUALIZER FOR SPIDER MODULAR IMAGE PROCESSING SYSTEM *
 C=* Copyright 1985-2012  Health Research Inc.,                        *
 C=* Riverview Center, 150 Broadway, Suite 560, Menands, NY 12204.     *
 C=* Email:  spider@wadsworth.org                                      *
 C=*                                                                   *
 C=* This program is free software; you can redistribute it and/or     *
 C=* modify it under the terms of the GNU General Public License as    *
 C=* published by the Free Software Foundation; either version 2 of    *
 C=* the License, or (at your option) any later version.               *
 C=*                                                                   *
 C=* This program is distributed in the hope that it will be useful,   *
 C=* but without any warranty; without even the implied warranty of    *
 c=* merchantability or fitness for a particular purpose.  See the GNU *
 C=* General Public License for more details.                          *
 C=* You should have received a copy of the GNU General Public License *
 C=* along with this program. If not, see <http://www.gnu.org/licenses>*
 C *********************************************************************
 C
 C  setprm(FILEDATA *fileptr, char *unused)
 C
 C  PURPOSE:  Set label parameters identifying 
 C            normalization status of file, and to write label into file.
 C 
 C          First record of SPIDER format file contains following info: 
 C          element # 5  flag indicating data type (=iform)
 c                    6  imami = flag indicating if the image has 
 c                       been searched for max and min. 
 c                    7  fmax_spid = image maximum
 c                    8  fmin_spid = image minimum
 c                    9  avd       = image average
 c                   10  sig       = std dev. (sq. root of var)
 c
 C--*******************************************************************
*/

#include "files.h"

 /* External subroutines */
 extern int    redlin (FILEDATA *, float *, int, int);
 extern int    wrtlin (FILEDATA *, float *, int, int);
 extern void   spout  (char *);

 /********************** setprm  **********************************/

 int setprm(FILEDATA *fileptr, char *unused)

 {
 float     buf[256];

 // Retrieve header record
 // Can not use filedata buffer as size may be too small! 

 if (redlin(fileptr,buf,256,0) != TRUE)
    {spout(" *** Unable to read header in setprm.");  return FALSE; }

 if ((fileptr -> iform) > 99)
    {  // MRC file 
    buf[19] = fileptr -> fmin_spi;
    buf[20] = fileptr -> fmax_spi;
    buf[21] = fileptr -> av;
    }

 else
    { // SPIDER file 
    //   Set flag indicating whether fmax_spid & fmin_spid available or not
    buf[5] = 1.0;
    if (fileptr -> fmin_spi == fileptr -> fmax_spi) 
       buf[5] = 0.0;

    buf[6] = fileptr -> fmax_spi;
    buf[7] = fileptr -> fmin_spi;
    buf[8] = fileptr -> av;
    buf[9] = fileptr -> sig;
    }

 // Write updated header into file
 if (wrtlin(fileptr,buf,256,0) != TRUE)
    { spout(" *** Unable to update header in setprm."); return FALSE;  }

 return TRUE;
 } 

