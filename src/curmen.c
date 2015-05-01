
/*$Header: /usr8/web/src/RCS/curmen.c,v 1.11 2005/10/18 17:00:01 leith Exp $*/
/*
C++*************************************************************************
C
C WID_CURMEN.FOR  -- CREATED MAR 91
C CURMEN.C           CONVERTED TO C NOV 92 al 
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
C    CURMEN()
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

#include <Xm/Text.h>
#include <Xm/ToggleBG.h>
#include <Xm/PushB.h>

#include "common.h"
#include "routines.h"

#define MAX_ARGS 10
#define MAXCURS 77

 /* internal function prototypes */
 void          curmen_but(Widget iw_temp, caddr_t, caddr_t);
 
 /* externally used common variables */

 /* internal common variables */
 static Widget   iw_curmen = (Widget)0;
 static Window   iwcurpal;
 static int      ixd, iyd, nperrow;


 /*  TO CALCULATE CURSOR APPEARANCE USE A 16X16 GRID.  USE A 
      ENDIAN NUMBERING SYSTEM THAT STARTS WITH ONE AT LEFT END
      OF EACH BYTE NOT AT THE USUAL RIGHT END!  */



 /********************   curmen   ****************************/

 void curmen(Widget iw_caller, caddr_t data, caddr_t call_data)

 { 
 Arg        args[MAX_ARGS];   /* arg list */

 static Widget   iw_lab, iw_curpal;
 GC              icontcc;
 Pixmap          icursav;
 XGCValues       gcval;
 Font            ifontcur;

 Widget          iw_pushf;
 unsigned int    iwid, ihi;
 int             ixulb, iyulb;
 int             icur, ixt, idi, ioff;
 char            it;
 char            cit[2];

 if (iw_curmen <= (Widget)0)
    {   /* create cursor choice menu widget first */

    iw_curmen  = wid_dialog(iw_win, 0, "Cursor options", -1, -1);

    /* create label box for cursor palette  */
    iw_lab  = wid_labelg(iw_curmen, 0, "CURSORS", 66, 4);

    XtSetArg(args[0], XmNautoUnmanage, FALSE); 
    XtSetValues(iw_curmen, args, 1);

    /* set size for overall pallete */
    iwid    = 338;
    ihi     = 282;

    /* set size for individual color boxes */
    ixd     = 32;
    iyd     = 32;
    nperrow = iwid / ixd;

    /* need to create a new graphics context for use here */
    icontcc = XCreateGC(idispl,iwin,0,&gcval);
    XCopyGC(idispl,icontx,0,icontcc);

    /*  load x cursor font in server for char cursors */
    ifontcur = XLoadFont(idispl,"cursor");

    /* associate x cursor font with graphics context */
    XSetFont(idispl,icontcc,ifontcur);

    icursav = XCreatePixmap(idispl,iroot, iwid,ihi,
                          DefaultDepthOfScreen(iscreen));
    if (icursav <= (Pixmap)0)  
      {  spout("*** Can not create cursor pixmap!"); return; } 

    /* set color in icontcc to black, clear the cursor pixmap */
    wicolor(icontcc,colorgo);
    XFillRectangle(idispl,icursav,icontcc,0,0,iwid,ihi);

    /*  set color to white in icontcc */
    wicolor(icontcc,colorgo+10);
    
    ixulb   = iwid;
    iyulb   = -14;
    icur    = 0;

    for (icur = 0; icur < MAXCURS-1; icur++)
       {
       ixulb = ixulb + ixd;
       if (ixulb > (iwid-ixd))
          {   /*  start new line of cursor  boxes */
          ixulb = 4;
          iyulb = iyulb + iyd;
          }

       /*   set cursor for box */
       it     = icur * 2;
       cit[0] = it;

       /*            write text to screen */
       XDrawString(idispl,icursav,icontcc,ixulb+12,iyulb+15,
                   cit,1);
       }

    /*  special code for radermacher's crosshair */
    ixt   = 7;
    ixulb = ixulb + ixd + ixt;
    iyulb = iyulb + ixt;
    idi   = 4;
    ioff  = 16;
    XDrawLine(idispl,icursav,icontcc,
              ixulb,iyulb,ixulb+idi,iyulb+idi);
    XDrawLine(idispl,icursav,icontcc,
               ixulb+ioff,iyulb,ixulb+ioff-idi,iyulb+idi);
    XDrawLine(idispl,icursav,icontcc,
               ixulb,iyulb+ioff,ixulb+idi,iyulb+ioff-idi);
    XDrawLine(idispl,icursav,icontcc,
              ixulb+ioff,iyulb+ioff,ixulb+ioff-idi,iyulb+ioff-idi);


    iw_curpal = wid_push(iw_curmen, 0,"", curmen_but,
                        (char *) iw_curmen,  map, 5,32);
    iwcurpal  = XtWindow(iw_curpal);

     XtSetArg(args[0],XmNshadow,       FALSE);
     XtSetArg(args[1],XmNmarginWidth,  0);
     XtSetArg(args[2],XmNmarginHeight, 0);
     XtSetArg(args[3],XmNborderWidth,  0);
     XtSetArg(args[4],XmNwidth,        iwid);
     XtSetArg(args[5],XmNheight,       ihi);
     XtSetArg(args[6],XmNlabelType,    XmPIXMAP);
     XtSetArg(args[7],XmNlabelPixmap,  icursav);
     XtSetValues(iw_curpal,args,8);


     /*  create push button for finished */
     iw_pushf = wid_pushg(iw_curmen, 0,"   FINISHED  ", fin_cb,
                        (char *) iw_curmen,  180,iyulb + iyd + 50);

     icursor = 0;

     XtManageChild(iw_curmen);
     iwcurpal  = XtWindow(iw_curpal);
     }
 else
     XtManageChild(iw_curmen);

 }


 /*********** cursor changed button callback *************************/

 void curmen_but(Widget iw_temp, caddr_t data, caddr_t call_data)
 {
 int          ix,iy, ixr,iyr, icursort;
 Bool         onscreen;
 Window       irootwin, iwinnow;
 unsigned int idum;

 /* find cursor location */
 onscreen = XQueryPointer(idispl,iwcurpal, &irootwin, &iwinnow, 
            &ixr, &iyr, &ix, &iy, &idum);

 if (onscreen) 
     {   
     /*  pointer is in the desired display and window */

     /* derive cursor number from position in the cursor pallete */
     icursort = (((iy-12) / iyd)  * nperrow) + (ix / ixd);

     /***** debug ***************
     printf("ix,iy,nperrow: %d  %d %d icursort: %d \n",
             ix,iy,nperrow,icursort); ***********/

     if (icursort >= MAXCURS-1) 
        {   /*  special code for Radermacher's x-hair */
        icursort = MAXCURS-1;
        }

     /* set cursor in iwin and color it */ 
     setacursor(icursort, icolorcf, icolorcb);

     /* set cursor in cursor pallete area */
     XDefineCursor(idispl,iwcurpal,icursorx);
     }
 }


