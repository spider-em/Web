
/*$Header: /usr8/web/src/RCS/colmen.c,v 1.22 2011/09/14 15:47:04 leith Exp $*/
/*
C++*********************************************************************
C
C WID_COLMEN.FOR  -- CREATED MARCH 91
C COLMEN.C           CONVERTED TO C -- NOV 92 al  
C                                               
C **********************************************************************
C *  AUTHOR: A. LEITH                                                  *
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
C    COLMEN
C
C    PURPOSE:      Displays color selection super-widget
C
C    CALLED BY:    web_com
C
C--********************************************************************
*/


#include <Xm/Text.h>
#include <Xm/ToggleBG.h>
#include <Xm/PushB.h>

#include "common.h"
#include "routines.h"

#define MAX_ARGS   12

 /* Internal function prototypes */
 void            colmen_but(Widget, XtPointer, XtPointer);
 void            colset    (Widget, XtPointer, XtPointer);
 
 /* Internal common variables */
 static Widget   iw_colmen = (Widget)0;
 static Widget   iw_colorf, iw_colorb, iw_colorov, iw_colorcf, iw_colorcb;
 static Widget   iw_curf, iw_curb;

 static int      itemp = 0;         // Current selected color number
 unsigned long   itempx;            // Current selected x color

 /*************************   colmen   *******************************/

 void colmen(Widget iw_caller, XtPointer data, XtPointer call_data)

 { 
 Arg             args[MAX_ARGS];  
 Widget          iw_temp;
 int             iwid, ixd, iyd, ixulb, iyulb, ici, icolort;
 unsigned long   icolorxt;
 int             n;
 XmString        str_label;       

/* printf(" Colmen itemp: %d \n",itemp); */

 if (iw_colmen <= (Widget)0)
    {   
    /* Create color choice menu widget first */

    iw_colmen  = wid_dialog(iw_win, 0, "Color options", -1, -1);

    /* Create label box for color palette ----------------- Pallette */
    wid_labelg(iw_colmen, 0, "COLOR PALETTE (Click on color first) ", 60, 25);

    n = 0;
    XtSetArg(args[n], XmNautoUnmanage, FALSE); n++; 
    XtSetValues(iw_colmen, args, 1);

    /* Set size for overall pallete */
    iwid    = 688;

    /* Set size for individual color boxes */
    ixd     = 32;
    iyd     = 32;
    ixulb   = iwid;
    iyulb   = 30;

    n = 0;
#ifdef WEB_BGFGO
    XtSetArg(args[n], XmNbackground,  icolorxmb); n++;
#endif

    /* XtSetArg(args[n], XmNshowAsDefault,   0); n++; ?????? */
    XtSetArg(args[n], XmNmarginWidth,     0); n++; 
    XtSetArg(args[n], XmNmarginHeight,    0); n++; 
    XtSetArg(args[n], XmNshadowThickness, 0); n++; 
    XtSetArg(args[n], XmNwidth,         ixd); n++; 
    XtSetArg(args[n], XmNheight,        iyd); n++;
    XtSetArg(args[n], XmNcolormap,      map); n++;

    str_label = XmStringCreateLtoR("",XmSTRING_DEFAULT_CHARSET);
    XtSetArg(args[n], XmNlabelString, str_label);  n++;

    for (ici = 0; ici < MAXCOLSPI; ici++)
       {
       ixulb = ixulb + ixd;
       if (ixulb > (iwid-ixd) || ici == 16 || ici == 32 || ici ==48 )
          {
          /* Start new line of color boxes */
          ixulb = 20;
          iyulb = iyulb + iyd;
          }

       /* Set x color for this box */
       icolorxt  = ispicol[ici+colorgo];
 
       XtSetArg(args[n],   XmNx, ixulb);
       XtSetArg(args[n+1], XmNy, iyulb); 
       XtSetArg(args[n+2], XmNbackground, icolorxt);

       iw_temp = XmCreatePushButton(iw_colmen, "iw_pb", args, n+3);

       XtAddCallback(iw_temp,         XmNactivateCallback,
                    (XtCallbackProc)  colmen_but,
                    (XtPointer)       &ispicoln[colorgo + ici]);
       XtManageChild(iw_temp);
 
       } // End of for: ici 

    // Free the String (not doing this may result in a memory leak) 
    XmStringFree(str_label);

    // Y position for line of current colors
    iyulb = iyulb + 80;

    // Create boxes for current colors 
    iw_colorf  = wid_label(iw_colmen, 0, "  ", map,   20,  iyulb );
    iw_colorb  = wid_label(iw_colmen, 0, "  ", map,  120,  iyulb );
    iw_colorov = wid_label(iw_colmen, 0, "  ", map,  220,  iyulb );
    iw_colorcf = wid_label(iw_colmen, 0, "  ", map,  320,  iyulb );
    iw_colorcb = wid_label(iw_colmen, 0, "  ", map,  420,  iyulb );

    // Put visible border around current color boxes 
    XtSetArg(args[0], XmNborderWidth, 2);

    /* Place current x color in  current color boxes */
    XtSetArg(args[1], XmNbackground, icolorx);
    XtSetValues(iw_colorf, args, 2);
    XtSetArg(args[1], XmNbackground, icolorxb);
    XtSetValues(iw_colorb, args, 2);
    XtSetArg(args[1], XmNbackground, icolorxov);
    XtSetValues(iw_colorov, args, 2);
    XtSetArg(args[1], XmNbackground, icolorxcf);
    XtSetValues(iw_colorcf, args, 2);
    XtSetArg(args[1], XmNbackground, icolorxcb);
    XtSetValues(iw_colorcb, args, 2);

    /*
    printf(" icolorx:   %#8x \n", icolorx);
    printf(" icolorxb:  %#8x \n", icolorxb);
    printf(" icolorxov: %#8x \n", icolorxov);
    printf(" icolorxcf: %#8x \n", icolorxcf);
    printf(" icolorxcb: %#8x \n", icolorxcb);
    */

    // Y position for line of current colors push buttons
    iyulb = iyulb + 30;

    // Create color setting buttons 
    wid_pushg(iw_colmen, 0,"   FOREGROUND  ",     colset, "fore",   20, iyulb );
    wid_pushg(iw_colmen, 0,"   BACKGROUND  ",     colset, "back",  120, iyulb );
    wid_pushg(iw_colmen, 0,"   OVERLAY     ",     colset, "over",  220, iyulb );
    wid_pushg(iw_colmen, 0,"   CURSOR      ",     colset, "curf",  320, iyulb );
    wid_pushg(iw_colmen, 0,"   CURSOR OUTLINE  ", colset, "curb",  420, iyulb );

    // Y position for finished button
    iyulb = iyulb + 90;

    // Create finished button 
    wid_pushg(iw_colmen, 0,"   FINISHED  ", fin_cb,
              (char *) iw_colmen,  300,iyulb);
    }

 XtManageChild(iw_colmen);
 }


 /*********** Color assignment button callback **************************/

 void colset(Widget iw_temp, XtPointer data, XtPointer call_data)
    {
    Arg           args[1];

    //printf(" colset; icolor: %d,  itempx:  %#8x \n",itemp,itempx);

    if (!strcmp(data, "fore"))
       { // Set current color for draw operations 

       /* Update color in foreground color box */
       XtSetArg(args[0], XmNbackground, itempx);
       XtSetValues(iw_colorf, args, 1);

       // wicolor sets both icolor and icolorx 
       wicolor(icontx, itemp);
       }

     else if (!strcmp(data, "back"))
       {  /* Set window background (used in clearing) */
       icolorb  = itemp;
       icolorxb = itempx;

       /* Update color in backgound color box */
       XtSetArg(args[0], XmNbackground, itempx);
       XtSetValues(iw_colorb, args, 1);

       XtSetArg(args[0], XmNbackground, itempx);
       XtSetValues(iw_win, args, 1);
       }

     else if (!strcmp(data, "over"))
       {   /* Set color used in mask overlay */
       icolorov  = itemp;
       icolorxov = itempx;

       /* Update color in overlay color box */
       XtSetArg(args[0], XmNbackground, itempx);
       XtSetValues(iw_colorov, args, 1);
       }

    else if (!strcmp(data, "curf"))
       {  /* Set cursor foreground  */
       icolorcf   = itemp;
       icolorxcf  = itempx;

       /* Update color in cursor foregound color box */
       XtSetArg(args[0], XmNbackground, itempx);
       XtSetValues(iw_colorcf, args, 1);

       /* Set cursor foreground color */
       setacursor(icursor,icolorcf,-1); 
       }

    else if (!strcmp(data, "curb"))
       {  /* Set cursor background  */
       icolorcb   = itemp;
       icolorxcb  = itempx;

       /* Update color in cursor background color box */
       XtSetArg(args[0], XmNbackground, itempx);
       XtSetValues(iw_colorcb, args, 1);

       /* Set cursor background color */
       setacursor(icursor,-1,icolorcb); 
       }
    }


/*********** Select color button callback **************************/

 void colmen_but(Widget iw_temp, XtPointer data, XtPointer call_data)
   {
   itemp    = * (int * ) data; 
   itempx   = ispicol[itemp];

   //printf(" setting color: %d \n",itemp,itempx);
   //printf(" itemp: %d,  itempx:  %#8x \n",itemp,itempx);
   }


                              


                              
