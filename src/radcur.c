
/*$Header: /usr8/web/src/RCS/radcur.c,v 1.2 2005/10/18 16:59:45 leith Exp $*/
/*
C++*************************************************************************
C
C RADCUR.C  -- CREATED NOV 93 from curmen.c -- al
C                                                                   
C **********************************************************************
C *  AUTHOR: A. LEITH                                                      *
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
C    radcur()
C
C    PARAMETERS:   IW_PARENT   PARENT WIDGET
C                  IXUL,IYUL   CORNERS OF WIDGET
C
C    PURPOSE:      SETS UP A CURSOR SELECTION SUPER-WIDGET
C
C    CALLED BY: 
C
C--*********************************************************************
*/

#include "common.h"

 /* external function prototypes */
 extern void   spout        (char *);
 extern int    wimakecur    (Cursor *, char * , char *,
                             XColor *, XColor *, int, int);
 extern void   setacursor   (int, int, int);
 
 /* externally defined common variables */
 extern Cursor   icurrad;

 /* internal common variables */


 /*  TO CALCULATE CURSOR APPEARANCE USE A 16X16 GRID.  USE A 
      ENDIAN NUMBERING SYSTEM THAT STARTS WITH ONE AT LEFT END
      OF EACH BYTE NOT AT THE USUAL RIGHT END!  */


 static char lfore[] = {
      0X01, 0X80,   0X02, 0X40, 
      0X04, 0X20,   0X08, 0X10, 
      0X10, 0X08,   0X00, 0X00, 
      0X00, 0X00,   0X00, 0X00, 
      0X00, 0X00,   0X00, 0X00, 
      0X00, 0X00,   0X10, 0X08, 
      0X08, 0X10,   0X04, 0X20, 
      0X02, 0X40,   0X01, 0X80 };

 static char lback[] = {
      0X03, 0XC0,   0X07, 0XE0, 
      0X0E, 0X70,   0X1C, 0X38, 
      0X18, 0X18,   0X00, 0X00, 
      0X00, 0X00,   0X00, 0X00, 
      0X00, 0X00,   0X00, 0X00, 
      0X00, 0X00,   0X18, 0X18, 
      0X1C, 0X38,   0X0E, 0X70, 
      0X07, 0XE0,   0X03, 0XC0 };


 /***************************** radcur *******************************/

 void radcur(int idum)
 {


 if (icurrad == 0)
    {
    /* must make cursor before it can be used */

    wimakecur(&icurrad,lfore,lback,
                      &colorcursf,&colorcursb, 8,8);
    }
 }


