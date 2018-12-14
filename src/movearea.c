 
/*$Header: /usr16/software/web/src/RCS/movearea.c,v 1.12 2018/12/07 17:03:33 leith Exp $*/

/*
C++*********************************************************************
C                                                                      *
C MOVEAREA -- New                                MAR   91 ArDean Leith *
C             Converted to C                     SEP   92 ArDean Leith *                                               
C             copyarea used                      SEP 2011 ArDean Leith *                                               
C                                                                      *
C **********************************************************************
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
C    movearea()
C
C    PURPOSE:       Moves an area of the screen to another location
C
C    PARAMETERS:    
C
C    CALLED BY:     winsel
C
C--********************************************************************
*/

#include "common.h"
#include "routines.h"
#include "extras.h"

 /* Internal function prototypes */
 static void     movearea_pop (Widget, XEvent *, String *, Cardinal *);

 /* Internal common variables */
 static GC       icontxx;

 /* External common variables */

 /**************************************************************/

 void movearea()

  {
#ifdef WEB_TRUE
  icontxx = icontx;
#else
  /* Make a xor graphics context */
  icontxx = setxor(icontx);
#endif

  // Use overlay color in icontxx
  wicolor(icontxx,colorgo+icolorov);

  /* Set up action procedure to move area */
  actions(iw_win, "movearea_pop", movearea_pop, "M123");

  showbutx("Copies window to this location.",
           "Moves window to this location.",
           "Stops this routine.", FALSE);

  XFlush(idispl);
  } 


 /**************** Set window position ***************/


 void movearea_pop(Widget iw_t, XEvent *event, String *params,
                   Cardinal *num_params)
 {
 int   ixul1,iyul1,ixult,iyult;
 int   icolorsav;
 char  outstr[60];

 if (!(strcmp(*params,"M")))            /* Cursor moved */
    {
    /* Get mouse position ix,iy  */
    getloc(event,'M',&ixult,&iyult);

    /* Mouse moved so erase old box and draw new box */
    xorboxt(iwin,icontxx,FALSE,TRUE,TRUE,ixult,iyult,nsamw,nroww);

    ixul1 = ixult;
    iyul1 = iyult;
    } 
  
 else if (!(strcmp(*params,"1")))            /* Button 1 pushed */
    {
    getloc(event,'M',&ixult,&iyult);

#ifndef WEB_TRUE
    /* Erase old box */
    xorbox(iwin,icontxx,TRUE,ixult,iyult,nsamw,nroww);
#endif

    /* Copy source window in backing store to destination */
    XCopyArea(idispl,imagsav,imagsav,icontx,
              ixulw,iyulw,nsamw,nroww,ixult,iyult);

    /* Copy source window in backing store to on-screen destination */
    copyarea(idispl,imagsav,iwin,icontx,
              ixulw,iyulw,nsamw,nroww,ixult,iyult);

    /*******************************/
    sprintf(outstr," (%d,%d) %d X %d --> (%d,%d)", 
    ixulw,iyulw,nsamw,nroww,ixult,iyult); 
    spout(outstr); 
    /*******************************/
 
    /* Since original windowed may be altered,
       set next move to this destination window */ 
    ixulw      = ixult;
    iyulw      = iyult;
    neednewmap = TRUE;
    }

 else if (!(strcmp(*params,"2")))            /* Button 2 pushed */
    {
    /* Get mouse position 
    ixult = event -> xkey.x_root;
    iyult = event -> xkey.y_root;   */

    getloc(event,'M',&ixult,&iyult);

#ifndef WEB_TRUE
    /* Erase old box and move original  window to this location */
    xorbox(iwin,icontxx,TRUE,ixult,iyult,nsamw,nroww); // sets neednewmap
#endif

    /* Set current color to background in icontx GC */
    icolorsav = icolor;
    wicolor(icontx,icolorb);

    XFillRectangle(idispl,iwin,icontx,ixulw,iyulw,nsamw,nroww);

    /* Copy source window in backing store to on-screen destination */
    XCopyArea(idispl,imagsav,iwin,icontx,
              ixulw,iyulw,nsamw,nroww,ixult,iyult);

    /* Copy source window in backing store to destination in backing store */
    XCopyArea(idispl,imagsav,imagsav,icontx,
              ixulw,iyulw,nsamw,nroww,ixult,iyult);

    /* Clear backing store source window location */
    XFillRectangle(idispl,imagsav,icontx,ixulw,iyulw,nsamw,nroww);

    /* Copy on-screen destination window in backing store destination */ 
    XCopyArea(idispl,iwin,imagsav,icontx,
              ixult,iyult,nsamw,nroww,ixult,iyult);  

    /* Since original windowed altered set next move to destination */ 
    ixulw = ixult;
    iyulw = iyult;

    /* Restore current foreground color */
    wicolor(icontx,icolorsav);

    neednewmap = TRUE;
    }

 else if (!(strcmp(*params,"3")))               
    {                                       /* Button 3 pushed */

   /* Erase old box */
    xorbox(iwin,icontxx,TRUE,ixul1,iyul1,nsamw,nroww);

#ifndef WEB_TRUE
    XFreeGC(idispl,icontxx);
#endif

    /* Uninstall translations */
    XtUninstallTranslations(iw_win);

    // printf(" movearea 1        \n");
    /* Unmanage information box */
    //showbutx("","","",TRUE);  !! CAUSES DELAY
    // printf(" movearea 2  calling winsiz1      \n");

    if (iwintyp == 1)
       {  /* Interactive windowing (Set size again) */
       winsiz1();
       }
    else if (iwintyp == 2)
       {  /* Fixed size windowing (Use same size) */
       winmov();
       }
    }

 }

