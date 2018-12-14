
/*$Header: /usr16/software/web/src/RCS/winmov.c,v 1.11 2018/12/07 17:06:44 leith Exp $*/

/*
 C++********************************************************************
 C
 C WID_WINMOV.FOR -- CREATED JAN 90 al
 C                   WIDGETIZED APRIL 91 al
 C WINMOV.C          CONVERTED TO C -- SEPT 92 al
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
 C winmov
 C
 C PURPOSE:       Positions a determined size of window on screen
 C
 C CALLED BY:     winsiz
 C
 C                     / winsiz_pop --> winsiz_pop2 --> winsel --> movearea
 C winsiz --> winsiz1 -- wid_posiz  --> winmov      /           
 C                     \ wid_posiz  --> winsiz3    /
 C
 C--*********************************************************************
*/

#include "common.h"
#include "routines.h"

 static void    winmov_pop (Widget, XEvent *, String *, Cardinal *);

 static GC      icontxx;

 /**************************************************************/

 void winmov(void)
 {

 /* Make a xor graphics context */
 icontxx = setxor(icontx);
 wicolor(icontxx,colorgo+icolorov);

 /* Set up action procedure to set window location */
 actions(iw_win, "winmov_pop", winmov_pop, "M123");

 showbutx("Sets window location.",
          "Changes window size.",
          "Stops this routine.", FALSE);
 } 


 /**************** Set window position ***************/


 void winmov_pop(Widget iw_t, XEvent *event, String *params,
                   Cardinal *num_params)
 {

 if (!(strcmp(*params,"M")))                 /* Cursor moved */
    {
    /* Get mouse position ix,iy  */
    getloc(event,'M',&ixulw,&iyulw);

    /* Mouse moved so erase old box and draw new box */
    xorbox(iwin,icontxx,FALSE,ixulw,iyulw,nsamw,nroww);
    } 

 else if (!(strcmp(*params,"1")))            /* Button 1 pushed */
    {
    /* Erase old box */
    xorbox(iwin,icontxx,TRUE,ixulw,iyulw,nsamw,nroww);

    /* If 'record the last file', keep the window */
    if (ioper == 4)	
    	xorbox(imagsav,icontx,TRUE,ixulw,iyulw,nsamw,nroww);

    XFreeGC(idispl,icontxx);

    /* Uninstall translations */
    XtUninstallTranslations(iw_t);
 
    /* Unmanage information box */
    showbutx("","","",TRUE);

    winsel();
    return;
    }

 else if (!(strcmp(*params,"2")))            /* Button 2 pushed */
    {
    /* Erase old box and use winsiz to change box size */
    xorbox(iwin,icontxx,TRUE,ixulw,iyulw,nsamw,nroww);

    /* Uninstall translations */
    XtUninstallTranslations(iw_t);
 
    /* Unmanage information box */
    showbutx("","","",TRUE);

    winsiz(NULL, NULL,NULL);
    return;
    }

 else if (!(strcmp(*params,"3")))          /* Button 3 pushed */        
    {                                       
    /* Erase old box and cancel this routine */
    xorbox(iwin,icontxx,TRUE,ixulw,iyulw,nsamw,nroww);

    XFreeGC(idispl,icontxx);

    /* Uninstall translations */
    XtUninstallTranslations(iw_t);
 
    /* Unmanage information box */
    showbutx("","","",TRUE);

    return;
    }
 }

