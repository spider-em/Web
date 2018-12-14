 
/*$Header: /usr16/software/web/src/RCS/setacursor.c,v 1.11 2018/12/07 17:03:34 leith Exp $*/
/*

C++*********************************************************************
C
C SETCURSOR.FOR  -- CREATED APRIL 92
C                   CONVERTED TO C -- SEPT 92 al
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
C    SETCURSOR(
C
C    PARAMETERS:   ICURSOR            SPIDER CURSOR NUMBER  (READ)
C                  ICOLORF            SPIDER COLOR NUMBER   (READ)
C                  ICOLORB            SPIDER COLOR NUMBER   (READ)
C
C    PURPOSE:      SETS UP A CURSOR FOR WEB USE
C
C    CALLED BY:    web_main   curmen_but 
C
C--*********************************************************************
*/

#include "common.h"
#include "routines.h"

#define MAXCURSOR 75

 /* External function prototypes */

 /* Internal static variables */
 static int first = TRUE;

 /********************  setacursor  *****************************/

 void setacursor(int icursort, int forecolor, int backcolor)

 {

 if (icursort < 0)
    {   /* Icursort is X-window cursor pointer not spider cursor number */
     
    icursor  = 0;
    icursorx = -icursort;
    XDefineCursor(idispl,iwin,icursorx);
    }

 else if (first || (icursor != icursort && icursort <= MAXCURSOR))
    {    
    /* Want a new spider cursor by number */
    first   = FALSE;
    icursor = icursort;

    /* Use default cursor if icursort is zero */
    if (icursor == 0) icursor = 34;
     
    icursorx = XCreateFontCursor(idispl,icursor*2);  
    XDefineCursor(idispl,iwin,icursorx);
    }

 else if  (icursor  != icursort    && 
           icursort >= MAXCURSOR+1 &&
           icursor  <= MAXCURSOR +4)
    {  
    /* Special code for Rademachers cross-hair cursor and other */ 
    /* locally defined cursors */
 
    first    = FALSE;

    /* Define the cursor shape (if necessary) */
    icursorx = newcursor(icursort, MAXCURSOR);
    icursor  = icursort;

    XDefineCursor(idispl,iwin,icursorx);
    }

 if (forecolor >= 0)
    {
    /* Query  color definition for this color number */
    colorcursf.pixel = (unsigned long) ispicol[forecolor];
    colorcursf.flags = DoRed|DoGreen|DoBlue; 
    XQueryColor(idispl,map,&colorcursf);
    }

 if (backcolor >= 0)
    {
    /* Query  color definition for this color number */
    colorcursb.pixel = (unsigned long) ispicol[backcolor];
    colorcursb.flags = DoRed|DoGreen|DoBlue; 
    XQueryColor(idispl,map,&colorcursb);
    }


 XFlush(idispl);

 // If (forecolor or backcolor is <0 then color is not altered
 /* Set cursor colors */
 XRecolorCursor(idispl,icursorx,&colorcursf,&colorcursb);
 XFlush(idispl);

 return;
 }


/*******************************************************************
C      #(1)  /'cw                  '/#(2)  /'cursor_shape        '/
C      #(3)  /'num_glyphs          '/#(4)  /'X                   '/
C      #(5)  /'arrow               '/#(6)  /'based_arrow_down    '/
C      #(7)  /'based_arrow_up      '/#(8)  /'boat                '/
C      #(9)  /'bogosity            '/#(10) /'bottom_left_corner  '/
C      #(11) /'bottom_right_corner '/#(12) /'bottom_side         '/
C      #(13) /'bottom_tee          '/#(14) /'box_sprial          '/
C      #(15) /'center_ptr          '/#(16) /'circle              '/
C      #(17) /'clock               '/#(18) /'coffee_mug          '/
C      #(19) /'cross               '/#(20) /'cross_reverse       '/
C      #(21) /'cross_hair          '/#(22) /'diamond_cross       '/
C      #(23) /'dot                 '/#(24) /'dot_box             '/
C      #(25) /'double_arrow        '/#(26) /'draft_large         '/
C      #(27) /'draft_small         '/#(28) /'draped_box          '/
C      #(29) /'exchange            '/#(30) /'fleur               '/
C      #(31) /'gobbler             '/#(32) /'gumby               '/
C      #(33) /'hand                '/#(34) /'hand1               '/
C      #(35) /'heart               '/#(36) /'icon                '/
C      #(37) /'iron_cross          '/#(38) /'left_ptr            '/
C      #(39) /'left_side           '/#(40) /'left_tee            '/
C      #(41) /'left_button         '/#(42) /'ll_angle            '/
C      #(43) /'lr_angle            '/#(44) /'man                 '/
C      #(45) /'middle_button       '/#(46) /'mouse               '/
C      #(47) /'pencil              '/#(48) /'pirate              '/
C      #(49) /'plus                '/#(50) /'question_arrow      '/
C      #(51) /'right_ptr           '/#(52) /'right_side          '/
C      #(53) /'right_tee           '/#(54) /'right_button        '/
C      #(55) /'rtl_logo            '/#(56) /'sailboat            '/
C      #(57) /'sb_down_arrow       '/#(58) /'sb_h_double_arrow   '/
C      #(59) /'sb_left_arrow       '/#(60) /'sb_right_arrow      '/
C      #(61) /'sb_up_arrow         '/#(62) /'sb_v_double_arrow   '/
C      #(63) /'shuttle             '/#(64) /'sizing              '/
C      #(65) /'spider              '/#(66) /'spraycan            '/
C      #(67) /'star                '/#(68) /'target              '/
C      #(69) /'t_cross             '/#(70) /'top_left_arrow      '/
C      #(71) /'top_left_corner     '/#(72) /'top_right_corner    '/
C      #(73) /'top_side            '/#(74) /'top_tee             '/
C      #(75) /'trek                '/#(76) /'ul_angle            '/
C      #(77) /'umbrella            '/#(78) /'ur_angle            '/
C      #(79) /'watch               '/#(80) /'xterm               '/
C      #(81) /'select              '/#(82) /'leftselect          '/
C      #(83) /'help_select         '/#(84) /'wait                '/
C      #(85) /'inactive            '/#(86) /'resize              '/
C      #(87) /'vpane               '/#(88) /'hpane               '/
C      #(89) /'text_insertion      '/#(90) /'text_insertion_bl   '/
C      #(91) /'cross_hair          '/#(92) /'draw                '/
C      #(93) /'pencil              '/#(94) /'rpencil             '/
C      #(95) /'center              '/#(96) /'rightselect         '/
C      #(97) /'wselect             '/#(98) /'eselect             '/
C      #(99) /'x                   '/#(100) /'circle             '/
C      #(101) /'mouse_c            '/#(102) /'lpencil            '/
C      #(103) /'leftgrab           '/#(104) /'grabhand           '/
C      #(105) /'rightgrab          '/#(106) /'leftpointing       '/
C      #(107) /'uppointing         '/#(108) /'rightpointing      '/
C      #(109) /'check               '/
*******************************************************************/

