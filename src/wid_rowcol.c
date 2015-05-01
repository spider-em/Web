
/*$Header: /usr8/web/new/RCS/wid_rowcol.c,v 1.5 2011/05/10 13:34:37 leith Exp $*/
/*
C **********************************************************************
C
C WID_ROWCOL  -- JULY 92 al
C **********************************************************************
C *  AUTHOR:  ArDean Leith                                 
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
C  WID_ROWCOL
C
C  PURPOSE:        Creates a row col widget
C
C  PARAMETERS      IW_PARENT       Parent widget
C                  IW_IT           Dialog widget
C                  TYPE            Row or column
C                  IX,IY           Position (in pixels)
C
C  CALLED BY:      imoptmen   montmen3   others
C
C--*********************************************************************
*/

#include<Xm/RowColumn.h>
#include "common.h"

#define MAX_ARGS 6

 extern void  spout(char *);

/************************************************************/

 Widget wid_rowcol(Widget iw_parent, char type, int ix, int iy) 

 { 
 Arg           args[MAX_ARGS];   /* arg list */
 int           n;                /* arg count */

 Position      ixp,iyp;          /* parent widget's position */
 Widget        iw_rowcol;

 n = 0;
 if (ix >= 0 || iy >= 0)
    {
    /* need to find location relative to present position of top window */
    XtSetArg(args[0], XmNx, &ixp);
    XtSetArg(args[1], XmNy, &iyp);
    XtGetValues(iw_top,args,2);

    /*( printf ("getvalues on iw_top gave, ixp,iyp: %d %d \n",ixp,iyp);*/
    XtSetArg(args[n], XmNx, (Position) ix+ixp);   n++;
    XtSetArg(args[n], XmNy, (Position) iy+iyp);   n++;
    XtSetArg(args[n], XmNdefaultPosition, False); n++;
    }

 if (type == 'h')
    { XtSetArg(args[n], XmNorientation, XmHORIZONTAL);  n++; }
 else
    { XtSetArg(args[n], XmNspacing, 6);           n++; }

#ifdef WEB_BGFG
  XtSetArg(args[n], XmNbackground,  icolorxmb);   n++;      
#endif

 /* create a RowColumn widget as a parent for text input box  */
 iw_rowcol = XmCreateRowColumn(iw_parent,"iw_rowcol", args, n);

 if (iw_rowcol == (Widget)0)
    { spout("*** wid_rowcol cannot create widget."); return iw_rowcol; }

 XtManageChild(iw_rowcol);

 return iw_rowcol;
 }
 
