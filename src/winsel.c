
/*$Header: /usr16/software/web/src/RCS/winsel.c,v 1.10 2018/12/07 17:06:44 leith Exp $*/
/*

 C++********************************************************************
 C
 C  WID_WINSEL.FOR -- CREATED MAY 91 al
 C                    CONVERTED TO C -- SEPT 92 al 
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
 C winsel
 C
 C PURPOSE:       Selects windowed operations 
 C
 C PARAMETERS:    Void
 C
 C                     / winsiz_pop --> winsiz_pop2 --> winsel --> movearea
 C winsiz --> winsiz1 -- wid_posiz  --> winmov      /           
 C                     \ wid_posiz  --> winsiz3    /
 C
 C--*********************************************************************
*/

#include "common.h"
#include "routines.h"
#include "extras.h"

 /* Global variables */


 /****************************  winsel  *******************************/

 void winsel(void)
 {
 if (ioper > 9) ioper = ioper / 10;


 if       (ioper == 4) 
   {     /* Window from a source file */
   winfile();
   }

 else if  (ioper == 5) 
   {     /* Draw box(s) */
   winbox(iwintyp);
   }

 else if (ioper == 6) 
   {     /* Record area command */
   winread();
   }

 else if (ioper == 7) 
   {     /* Move area command */
   movearea();
   }

 else if (ioper == 8) 
   {     /* Power spectrum command */
   powermen();
   }
 }
