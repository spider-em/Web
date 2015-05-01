
/*$Header: /usr8/web/new/RCS/wrtvol.c,v 1.3 2005/10/18 16:59:50 leith Exp $*/

/*
C**************************************************************************
C
C   WRTVOL.C  -- CREATED NOV 13 1993
C             
C **********************************************************************
C *  AUTHOR: ArDean Leith                                                  *
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
C  WRTVOL(filedata,  float * data)
C           
C
C  PURPOSE:     WRITES SPIDER IMAGE INTO 8 / OR 32 BIT ARRAY .
C
C  PARAMETERS:  filedata           image file structure pointer
C               data               floating point data
C
C  RETURNS:     TRUE OR FALSE 
C
C--*********************************************************************
*/

#include "files.h"

 /* external function prototypes */
 extern int   wrtlin    (FILEDATA *, float *, int, int);


 /*************************   wrtvol   ****************************/

 int wrtvol(FILEDATA *filedata,  float * fptr)

 {
 int       nsam, iloc, irec;
 
 nsam  = filedata -> nsam;

 iloc  = 0;

 for (irec = 1; irec <= (filedata -> nslice) * (filedata -> nrow);irec++)
    {
    if (!wrtlin(filedata, &fptr[iloc], nsam, irec)) return FALSE;
    iloc += nsam;
    }

 return TRUE;
 }




