
/*$Header: /usr16/software/web/src/RCS/web_info.c,v 1.15 2018/12/07 17:03:35 leith Exp $*/
/*
 C++********************************************************************
 C
 C  web_info.c
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
 C **********************************************************************
 C
 C  web_info
 C
 C  PURPOSE:  GIVES MISC. INFO ON X-WINDOWS SYSTEM USEFUL FOR DEBUGGING
 C
 C--********************************************************************
*/

#include "common.h"
#include "routines.h"

 /**********************  web_info  **************************/

 void  web_info(Widget iw_temp, void * data, void * call_data)
 { 

 Arg           args[1];
 int           idepth, iwide, ihigh, black, white;
 int           nummap, minmap, maxmap;
 Colormap      *maps, defmap, imap;
 char          *enviro, mandir[81];
 int           i;
 char          *oper;

 oper = data;
 if(*oper == '1')  // Inactivated the call jul 2011
  {
    if( (enviro = getenv("WEBMAN_DIR") ) == NULL )
     {
      spout("Could not get the WEBMAN_DIR environment");
      return;
     }
     strcpy(mandir,enviro);
     strcat(mandir,"web.man");
     web_man(NULL, mandir, NULL);
     return;
  }

 printf(" ------- X Window & Motif information --------------- \n");
 idepth = DefaultDepthOfScreen(iscreen);
 ihigh  = HeightOfScreen(iscreen);
 iwide  = WidthOfScreen(iscreen);

 printf(" Screen width & height:   %6d, %6d \n",iwide,ihigh);
 printf(" Screen depth:          %6d \n",idepth);

 white  = WhitePixelOfScreen(iscreen);
 black  = BlackPixelOfScreen(iscreen);
 printf(" Black & white pixels:      %#x , %#x \n",black,white);

 minmap = XMinCmapsOfScreen(iscreen);
 maxmap = XMaxCmapsOfScreen(iscreen);

 defmap = XDefaultColormapOfScreen(iscreen);
 printf(" Default colormap:          %p \n",(void *)defmap);
 printf(" Web colormap:              %p \n",(void *)map);

 XtSetArg(args[0], XmNcolormap, &imap); 
 XtGetValues(iw_top,args,1);
 printf(" iw_top colormap:           %p \n",(void *)imap);
 XtGetValues(iw_main,args,1);
 printf(" iw_main colormap:          %p \n",(void *)imap);
 XtGetValues(iw_win,args,1);
 printf(" iw_win colormap:           %p \n",(void *)imap);
 XtGetValues(iw_menub,args,1);
 printf(" iw_menub colormap:         %p \n",(void *)imap);

 maps = XListInstalledColormaps(idispl,iwtop,&nummap);
 printf(" # of Installed maps:  %6d \n",nummap);
 for (i=0; i<nummap; i++)
 printf(" Map %d:                     %p \n",i+1,(void *)maps[i]);

 printf(" Mapgo...mapend:         %6d , %6d\n",mapgo,mapend);
 printf(" Colorgo...colorend:     %6d , %6d\n",colorgo,colorend);

 /* list current visual information */
 int deptht,ncolorst;
 visual_info(idispl, visual, &deptht, &ncolorst);

  /* list available visuals on this workstation */
 printf(" ---------- Available visuals ----------\n");
 visuals(idispl,XDefaultScreen(idispl));

 /* list values in SPIDER colormap */
 printf(" ---------- Spider colormap -----%p -----\n",(void *)map);
 lut_info("Spider",map,  idepth, 0,256); 

 planeforcolor(129);
 planeforcolor(130);
 planeforcolor(135);

 }



#ifdef NEVER
       INTEGER         ILOC(10)
       INTEGER         MASK
       INTEGER         IPMASK
       LOGICAL         LPMASK
       EQUIVALENCE     (IPMASK,LPMASK)

       CALL WID_OUTPUT(0,' THE INFO IS PLACED IN FOR00?.DAT',0)

       WRITE(NDAT,*) ' '
       WRITE(NDAT,*) ' X-WINDOWS INFORMATION ----------'

C      GET VISUAL (CAN QUERY WITH DEBUGGER)
       CALL X$DEFAULT_VISUAL_OF_SCREEN(ISCREEN,VISUAL)

C      LIST DISPLAY AND SCREEN POINTERS
       WRITE(NDAT,*) ' IPARENT & IROOT:      ' ,IPARENT,IROOT
       WRITE(NDAT,*) ' IDISPL & ISCREEN:     ' ,IDISPL,ISCREEN
       WRITE(NDAT,*) ' IWIN & ICONTX         ' ,IWIN,ICONTX
       WRITE(NDAT,*) ' ISPIDER,IMAGSAV:      ' ,ISPIDER,IMAGSAV

       IWMAIN   = XT$WINDOW(IW_MAIN)
       IWMENUB  = XT$WINDOW(IW_MENUB)
       IWMENUP1 = XT$WINDOW(IW_MENUP1)

       WRITE(NDAT,*) ' IWCOM,IWMAIN:         ' ,IWCOM,IWMAIN
       WRITE(NDAT,*) ' IWMENUB,IWMENUP1      ' ,IWMENUB,IWMENUP1


       CALL X$QUERY_BEST_CURSOR(IDISPL,IWIN,16,16,IW2,IH2)
       WRITE(NDAT,*) ' CURSOR SIZE <= 16X16: ',IW2,IH2

       NORDR   = X$IMAGE_BYTE_ORDER(IDISPL)
       IF (NORDR .EQ. X$C_LSB_FIRST) THEN
          WRITE(NDAT,*) ' LEAST SIG. BIT FIRST'
       ELSEIF (NORDR .EQ. X$C_MSB_FIRST) THEN
          WRITE(NDAT,*) ' MOST SIG. BIT FIRST'
       ELSE
          WRITE(NDAT,*) ' UNKNOWN BYTE FIRST:', NORDR
       ENDIF
       ISIZE   = X$BITMAP_UNIT(IDISPL)
       IPAD    = X$BITMAP_PAD(IDISPL)
       WRITE(NDAT,*) ' BITMAP UNIT, PAD:     ', ISIZE,IPAD

       NSCREEN = X$SCREEN_COUNT(IDISPL)
       ISNUM   = X$DEFAULT_SCREEN(IDISPL)
       WRITE(NDAT,*) ' # SCREENS, SCREEN #:  ', NSCREEN,ISNUM

       IDEPTH  = X$DEFAULT_DEPTH_OF_SCREEN(ISCREEN)
       IPLAN   = X$PLANES_OF_SCREEN(ISCREEN)
       WRITE(NDAT,*) ' SCREEN DEPTH & PLANES:', IDEPTH,IPLAN

       IP1     = X$DISPLAY_PLANES(IDISPL,ISNUM)
       IC1     = X$DISPLAY_CELLS(IDISPL,ISNUM)
       WRITE(NDAT,*) ' DISPLAY PLANES,CELLS: ', IP1,IC1

       IPMASK = X$ALL_PLANES()
       WRITE(NDAT,*) ' ALL PLANES MASK:      ', IPMASK

C      FIND HOSTS
       CALL X$LIST_HOSTS(IDISPL,NUMHOSTS,STATE,,10,ILOC)
       WRITE(NDAT,*) ' # HOSTS:              ', NUMHOSTS,ILOC(1)

C      FIND OUT PRESENT FOCUS
       CALL X$GET_INPUT_FOCUS(IDISPL,IFOCUS)
       WRITE(NDAT,*) ' CURRENT INPUT FOCUS   ', IFOCUS


       CALL DWT$VMS_SET_ARG(%LOC(IX),ARG_LIST,0,DWT$C_NX)
       CALL DWT$VMS_SET_ARG(%LOC(IY),ARG_LIST,1,DWT$C_NY)
       CALL DWT$VMS_SET_ARG(%LOC(IW),ARG_LIST,2,DWT$C_NWIDTH)
       CALL DWT$VMS_SET_ARG(%LOC(IH),ARG_LIST,3,DWT$C_NHEIGHT)
       CALL XT$GET_VALUES(IW_OUTPUT,ARG_LIST,4)
       WRITE(NDAT,*) ' OUTPUT WINDOW POS     ', IX,IY
       WRITE(NDAT,*) ' OUTPUT WINDOW SIZE    ', IW,IH

       ITEMP = MESMAX
       WRITE(NDAT,*) ' SCROLLING MESSAGES    ', ITEMP
       CALL XT$GET_VALUES(IW_VERS,ARG_LIST,4)
       WRITE(NDAT,*) ' SCROLLING WINDOW POS  ', IX,IY
       WRITE(NDAT,*) ' SCROLLING WINDOW SIZE ', IW,IH


       MINMAP  = X$MIN_CMAPS_OF_SCREEN(ISCREEN)
       MAXMAP  = X$MAX_CMAPS_OF_SCREEN(ISCREEN)
       WRITE(NDAT,*) ' MIN & MAX # MAPS:     ', MINMAP,MAXMAP

       IBLACK1 = X$BLACK_PIXEL_OF_SCREEN(ISCREEN)
       IBLACK2 = X$BLACK_PIXEL(IDISPL,ISNUM)
       WRITE(NDAT,*) ' BLACK INDICES:        ', IBLACK1,IBLACK2
       IWHITE1 = X$WHITE_PIXEL_OF_SCREEN(ISCREEN)
       IWHITE2 = X$WHITE_PIXEL(IDISPL,ISNUM)
       WRITE(NDAT,*) ' WHITE  INDICES:       ', IWHITE1,IWHITE2
       WRITE(NDAT,*) ' MAPGO..MAPEND:        ', MAPGO,MAPEND

C      LIST MAP POINTER
       WRITE(NDAT,*) ' SHARED MAP POINTER    ', MAPSHAR
       WRITE(NDAT,*) ' PRIVATE MAP POINTER   ', MAPPRIV
       WRITE(NDAT,*) ' CURRENT MAP POINTER   ', MAP
       IER = X$LIST_INSTALLED_COLORMAPS(IDISPL,IWIN,NUMMAP,,10,ILOC)
       WRITE(NDAT,*) ' NO. OF INSTALLED MAPS:', NUMMAP,ILOC(1)
       WRITE(NDAT,*) ' '
       WRITE(NDAT,*) ' IPIXELS ARRAY:'
       WRITE(NDAT,900) (IPIXELS(I),I=1,256)
900    FORMAT(10(10I4,/))
c      LIST COLOR-MAP CONTENTS
       IF (MAPSHAR .GT. 0) THEN
           WRITE(NDAT,*) ' SHARED COLORMAP --------------------'
           CALL WID_LUTINFO(MAPSHAR,IRTFLG)
       ENDIF
       IF (MAPPRIV .GT. 0) THEN
           WRITE(NDAT,*) ' PRIVATE COLORMAP -------------------'
           CALL WID_LUTINFO(MAPPRIV,IRTFLG)
       ENDIF
C*************   
C      GET WINDOW TREE FOR THE ROOT WINDOW
C         DO NOT ACTIVATE IN SEANDAWAY UNTIL RDPRIS IS REMOVED!!!!!!
C**       CALL WID_TREE(IDISPL,IROOT)
C********************
#endif

