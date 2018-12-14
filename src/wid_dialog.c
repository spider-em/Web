
/*$Header: /usr16/software/web/src/RCS/wid_dialog.c,v 1.9 2018/12/07 17:03:35 leith Exp $*/
/*
 C *********************************************************************
 C
 C WID_DIALOG                        
 C               CONVERTED TO C -- JULY 92 al
 C *********************************************************************
 C *  AUTHOR:  ArDean Leith                                 
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
C  WID_DIALOG
C
C  PURPOSE:        CREATES A DIALOG WIDGET
C
C  PARAMETERS      IW_PARENT       PARENT WIDGET
C                  IW_IT           DIALOG WIDGET
C                  TITLE           WIDGET TITLE
C                  IX,IY           POSITION (IN PIXELS)
C
C  CALLED BY:      imoptmen   montmen3   others
C
C--*********************************************************************
*/

#include <Xm/BulletinB.h>
#include "common.h"
#include "routines.h"

#define MAX_ARGS 5


/************************************************************/

 Widget wid_dialog(Widget iw_parent, Widget iw_dialog, char * title, 
                   int ix, int iy)

 { 
 Arg           args[MAX_ARGS];   /* Arg list */
 register      int  n;           /* Arg count */
 Position      ixp,iyp;          /* Parent widget's position */

 /* Need to find location relative to present position of top window */
 XtSetArg(args[0], XmNx, &ixp);
 XtSetArg(args[1], XmNy, &iyp);
 XtGetValues(iw_top,args,2);
 /*( printf ("getvalues on iw_top gave, ixp,iyp: %d %d \n",ixp,iyp);*/

 if (ix < 0 || iy < 0)
    {    /*  Use default placement for dialog box */
    ix = 30;
    iy = 30;
    }

 n = 0;
#ifdef WEB_BGFGO
 XtSetArg(args[n], XmNbackground,  icolorxmb); n++;
#endif

 XtSetArg(args[n], XmNx, (Position) ix+ixp); n++;
 XtSetArg(args[n], XmNy, (Position) iy+iyp); n++;
 XtSetArg(args[n], XmNdefaultPosition, False); n++;

 if (iw_dialog == (Widget)0)
    {   /* Create widget first */
    iw_dialog = XmCreateBulletinBoardDialog(iw_parent,title,
                                            args,n);
    if (iw_dialog == (Widget)0)
       {
       spout("*** WID_DIALOG Cannot open dialog widget.");
       return iw_dialog;
       }
    }
 else
    { 
    XtSetValues(iw_dialog,args,n);
    }
 return iw_dialog;
 }
 
