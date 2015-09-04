
/*$Header: /usr8/web/src/RCS/newcursor.c,v 1.3 2015/09/01 18:59:18 leith Exp $*/

/*
 C**********************************************************************
 C
 C newcursor.c    Created from radcur.c                   Jan 31 94 al
 C                Cosmetic                                Jul    15 al
 C                                                                   
 C**********************************************************************
 C   AUTHOR:  ArDean Leith
 C=* FROM: WEB - VISUALIZER FOR SPIDER MODULAR IMAGE PROCESSING SYSTEM *
 C=* Copyright (C) 1992-2015  Health Research Inc.                     *
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
 C**********************************************************************
 C
 C  newcursor()
 C
 C  PARAMETERS:   
 C
 C  PURPOSE:  Creates some cursors    
 C
 C  CALLED BY: 
 C
 C**********************************************************************
*/

#include "common.h"

 /* External function prototypes */
 extern int    wimakecur    (Cursor *, char * , char *,
                             XColor *, XColor *, int, int);
 
 /* Externally defined common variables */

 /* Internal common variables */
 static Cursor icurrad   = 0;
 static Cursor icurone   = 0;
 static Cursor icurtwo   = 0;
 static Cursor icurthree = 0;

 /* USE A 16X16 GRID FOR CURSORS.  USE A ENDIAN NUMBERING SYSTEM
    THAT STARTS WITH ONE AT LEFT END OF EACH BYTE!  */

 static char radfront[] = {
      0X01, 0X80,   0X02, 0X40, 
      0X04, 0X20,   0X08, 0X10, 
      0X10, 0X08,   0X00, 0X00, 
      0X00, 0X00,   0X00, 0X00, 
      0X00, 0X00,   0X00, 0X00, 
      0X00, 0X00,   0X10, 0X08, 
      0X08, 0X10,   0X04, 0X20, 
      0X02, 0X40,   0X01, 0X80 };

 static char radback[] = {
      0X03, 0XC0,   0X07, 0XE0, 
      0X0E, 0X70,   0X1C, 0X38, 
      0X18, 0X18,   0X00, 0X00, 
      0X00, 0X00,   0X00, 0X00, 
      0X00, 0X00,   0X00, 0X00, 
      0X00, 0X00,   0X18, 0X18, 
      0X1C, 0X38,   0X0E, 0X70, 
      0X07, 0XE0,   0X03, 0XC0 };

static char oneback[] = {
   0x00, 0x03, 0x80, 0x02, 0x40, 0x02, 0xa0, 0x02, 0xe0, 0x02, 0x80, 0x02,
   0x80, 0x02, 0x80, 0x02, 0x80, 0x02, 0x80, 0x02, 0x80, 0x02, 0x80, 0x02,
   0xf0, 0x1e, 0x10, 0x10, 0xf0, 0x1f, 0x00, 0x00};

static char onefront[] = {
   0x00, 0x00, 0x00, 0x01, 0x80, 0x01, 0x40, 0x01, 0x00, 0x01, 0x00, 0x01,
   0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01,
   0x00, 0x01, 0xe0, 0x0f, 0x00, 0x00, 0x00, 0x00};

static char twoback[] = {
   0xc0, 0x03, 0x20, 0x04, 0xd0, 0x0b, 0x28, 0x14, 0x34, 0x14, 0x1c, 0x14,
   0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x16, 0xe0, 0x0b, 0x10, 0x04,
   0xe8, 0x3f, 0x04, 0x20, 0xfc, 0x3f, 0x00, 0x00};

static char twofront[] = {
   0x00, 0x00, 0xc0, 0x03, 0x20, 0x04, 0x10, 0x08, 0x08, 0x08, 0x00, 0x08,
   0x00, 0x08, 0x00, 0x08, 0x00, 0x08, 0x00, 0x08, 0x00, 0x04, 0xe0, 0x03,
   0x10, 0x00, 0xf8, 0x1f, 0x00, 0x00, 0x00, 0x00};

static char threeback[] = {
   0x00, 0x00, 0xf0, 0x07, 0x18, 0x18, 0xe8, 0x17, 0x38, 0x14, 0x00, 0x14,
   0xf0, 0x1b, 0x10, 0x0c, 0x10, 0x0c, 0xf0, 0x1b, 0x00, 0x14, 0x38, 0x14,
   0xe8, 0x17, 0x10, 0x08, 0xe0, 0x07, 0x00, 0x00};

static char threefront[] = {
   0x00, 0x00, 0xe0, 0x07, 0x10, 0x08, 0x00, 0x08, 0x00, 0x08, 0x00, 0x08,
   0x00, 0x08, 0xe0, 0x07, 0xe0, 0x07, 0x00, 0x08, 0x00, 0x08, 0x00, 0x08,
   0x10, 0x08, 0xe0, 0x07, 0x00, 0x00, 0x00, 0x00};

 /***************************** radcur *******************************/

 Cursor newcursor(Cursor icursort, int maxcursor)
 {

 if (icursort == maxcursor + 1) 
    {
    /* Want + cursor */
    if (icurrad == 0) 
       {
       /* Must make cursor before it can be used */
       wimakecur(&icurrad,radfront,radback,
                      &colorcursf,&colorcursb, 8,8);
       }
    return icurrad;
    }

 else if (icursort == maxcursor + 2) 
    {
    /* Want + cursor */
    if (icurone == 0) 
       {
       /* Must make cursor before it can be used */
       wimakecur(&icurone,onefront,oneback,
                      &colorcursf,&colorcursb, 8,8);
       }
    return icurone;
    }

 else if (icursort == maxcursor + 3) 
    {
    /* Want + cursor */
    if (icurtwo == 0) 
       {
       /* Must make cursor before it can be used */
       wimakecur(&icurtwo,twofront,twoback,
                      &colorcursf,&colorcursb, 8,8);
       }
    return icurtwo;
    }

 else if (icursort == maxcursor + 4) 
    {
    /* Want + cursor */
    if (icurthree == 0) 
       {
       /* Must make cursor before it can be used */
       wimakecur(&icurthree,threefront,threeback,
                      &colorcursf,&colorcursb, 8,8);
       }
    }
    return icurthree;
 }


