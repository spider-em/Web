
/*$Header: /usr16/software/web/src/RCS/curmen.c,v 1.13 2018/12/07 17:03:31 leith Exp $*/

/*
 C**********************************************************************
 C
 C wid_curmen.for     Created                              Mar   91 al
 C curmen.c           Converted to C                       Nov   92 al 
 C                    True color bug                       Jul 2015 al 
 C                                                                   
 C**********************************************************************
 C * AUTHOR: A. LEITH                                                  *
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
 C    CURMEN()
 C
 C    PARAMETERS:   iw_caller     Parent widget
 C
 C    PURPOSE:      Sets up a cursor selection super-widget
 C
 C    CALLED BY: 
 C
 C**********************************************************************
*/

#include <Xm/Text.h>
#include <Xm/ToggleBG.h>
#include <Xm/PushB.h>

#include "common.h"
#include "routines.h"

#define MAX_ARGS 10
#define MAXCURS 77

 /* Internal function prototypes */
 static void    curmen_but(Widget iw_temp, XtPointer, XtPointer);
 
 /* Externally used common variables */

 /* Internal common variables */
 static Widget   iw_curmen = (Widget)0;
 static Window   iwcurpal;
 static int      ixd, iyd, nperrow;


 /* USE A 16X16 GRID FOR CURSORS.  USE A ENDIAN NUMBERING SYSTEM
    THAT STARTS WITH ONE AT LEFT END OF EACH BYTE!  */


 /********************   curmen   ****************************/

 void curmen(Widget iw_caller, XtPointer data, XtPointer call_data)

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
 char            cit[1];

 if (iw_curmen <= (Widget)0)
    {   /* Create cursor choice menu widget first */

    iw_curmen  = wid_dialog(iw_win, 0, "Cursor options", -1, -1);

    /* Create label box for cursor palette  */
    iw_lab  = wid_labelg(iw_curmen, 0, "CURSORS", 66, 4);

    XtSetArg(args[0], XmNautoUnmanage, FALSE); 
    XtSetValues(iw_curmen, args, 1);

    /* Set size for overall pallate */
    iwid    = 338;
    ihi     = 282;

    /* Set size for individual color boxes */
    ixd     = 32;
    iyd     = 32;
    nperrow = iwid / ixd;

    /* Need to create a new graphics context for use here */
    icontcc = XCreateGC(idispl,iwin,0,&gcval);
    XCopyGC(idispl,icontx,0,icontcc);

    /*  Load X cursor font in server for char cursors */
    ifontcur = XLoadFont(idispl,"cursor");

    /* Associate X cursor font with graphics context */
    XSetFont(idispl,icontcc,ifontcur);

    icursav = XCreatePixmap(idispl,iroot, iwid,ihi,
                          DefaultDepthOfScreen(iscreen));
    if (icursav <= (Pixmap)0)  
      {  spout("*** Can not create cursor pixmap!"); return; } 

    /* Set color in icontcc to black, clear the cursor pixmap */
    wicolor(icontcc,colorgo);
    XFillRectangle(idispl,icursav,icontcc,0,0,iwid,ihi);

    /*  Set color to white in icontcc */
#ifdef WEB_TRUE
    wicolor(icontcc,colorgo+7);
#else
    wicolor(icontcc,colorgo+10);
#endif
    
    ixulb   = iwid;
    iyulb   = -14;
    icur    = 0;

    for (icur = 0; icur < MAXCURS-1; icur++)
       {
       ixulb = ixulb + ixd;
       if (ixulb > (iwid-ixd))
          {   /*  Start new line of cursor  boxes */
          ixulb = 4;
          iyulb = iyulb + iyd;
          }

       /*   Set cursor for this box */
       it     = icur * 2;
       cit[0] = it;

       /*  Write cursor text to screen */
       /* witext(icontcc, cit, ixulb+12, iyulb+15, 1, 0, -1, 2, FALSE);*/
       XDrawString(idispl,icursav,icontcc,ixulb+12,iyulb+15, cit,1);

       }

    /*  Special code for Radermacher's crosshair */
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


     /*  Create push button for finished */
     iw_pushf = wid_pushg(iw_curmen, 0,"   FINISHED  ", fin_cb,
                        (char *) iw_curmen,  180,iyulb + iyd + 50);

     icursor = 0;

     XtManageChild(iw_curmen);

     iwcurpal = XtWindow(iw_curpal);
     }
 else
     XtManageChild(iw_curmen);
 }


 /*********** Cursor changed button callback *************************/

 void curmen_but(Widget iw_temp, XtPointer data, XtPointer call_data)
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
     /*  Pointer is in the desired display and window */

     /* Derive cursor number from position in the cursor pallete */
     icursort = (((iy-12) / iyd)  * nperrow) + (ix / ixd);

     /***** debug ***************
     printf("ix,iy,nperrow: %d  %d %d icursort: %d \n",
             ix,iy,nperrow,icursort); ***********/

     if (icursort >= MAXCURS-1) 
        {   /*  special code for Radermacher's x-hair */
        icursort = MAXCURS-1;
        }

     /* Set cursor in iwin and color it */ 
     setacursor(icursort, icolorcf, icolorcb);

     /* Set cursor in cursor pallette area */
     XDefineCursor(idispl,iwcurpal,icursorx);
     }
 }


