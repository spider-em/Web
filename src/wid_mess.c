/*$Header: /usr8/web/src/RCS/wid_mess.c,v 1.7 2011/07/19 19:43:04 leith Exp $*/

/*
 ***********************************************************************
 *
 * wid_mess.c
 *
 ***********************************************************************
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
 ***********************************************************************
 *
 * wid_mess(iw_parent, iw_mess, message, ix, iy)
 *
 * 
 * PURPOSE:   	Creates a message dialog widget  
 *
 * RETURNS:   	The message widget's number
 * 
 * PARAMETERS:	Widget	iw_parent	parent widget
 *		Widget	iw_mess		message widget
 *		char	*message	message to be put in widget
 *		int	ix		x coord of ulh corner
 *		int     iy		y coord of ulh corner
 *
 ***********************************************************************
*/

#include "common.h"
#include <Xm/BulletinB.h>

#define  MAX_ARGS  4

static  Widget iw_messlabel;

/* Function prototypes */
Widget wid_labelg (Widget, Widget, char *, int, int);


/**********************************************************************/
Widget wid_mess(Widget iw_parent, Widget iw_mess, char *message,
                   int ix, int iy)
{
   Arg          args[MAX_ARGS];     /* Arg list  */
   int          n;                  /* Arg count */
   Position    ixp,iyp;             /* Parent widget's position */


   if (iw_mess == (Widget)0) 
      {
      /* Need to find location relative to present position of top window */
      XtSetArg(args[0], XmNx, &ixp);
      XtSetArg(args[1], XmNy, &iyp);
      XtGetValues(iw_top,args,2);

      if (ix < 0 || iy < 0)
         {    /*  Use default placement for message box */
         ix = 40;
         iy = 40;
         }

      n = 0;
#ifdef WEB_BGFGO
      XtSetArg(args[n], XmNbackground,  icolorxmb);    n++;
#endif
      XtSetArg(args[n], XmNx, (Position) ix+ixp);      n++;
      XtSetArg(args[n], XmNy, (Position) iy+iyp);      n++;
      XtSetArg(args[n], XmNdefaultPosition, False);    n++;

      iw_mess      = XmCreateBulletinBoardDialog(iw_parent,"iw_mess",args,n);
      iw_messlabel = (Widget) 0;
      }

   iw_messlabel = wid_labelg(iw_mess, iw_messlabel,message,-1,-1);

   XtManageChild(iw_mess);

   return (iw_mess);
}
