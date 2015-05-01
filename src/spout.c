
/*$Header: /usr8/web/src/RCS/spout.c,v 1.8 2005/10/18 17:00:05 leith Exp $*/

/*
 ***********************************************************************
 *
 * spout.c
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
 * spout(message)
 * 
 * PURPOSE:   prints out a message in message area of web
 *
 * VARIATIONS:
 *            spout :   next message goes on new line
 *            spouts:   next message does not start new line 
 *            spouto:   next message overwrites this line 
 *            spouti:   put out integer, next message does not 
 *                      start new line 
 *
 * RETURNS:   void 
 * 
 * PARAMETERS:     
 *
 ***********************************************************************
 */

#include "std.h"
#include "x.h"
#include "routines.h"
#include <Xm/Text.h>

 /* externally defined global variables */
 extern FILE      * resfp;           /* web_results file */
 extern Widget    iw_scroll;         /* scroll window widget */
 extern int       trace_web;

 /* internal static variables */
 static XmTextPosition igo  = 0; 
 static XmTextPosition iend = 0;

 /**************************** spout **********************************/

 void spout (char *message)
   {
   /* add the text after the last position */

   if (strcspn(message,"$")  == strlen(message) -1)
      {   /*  $ in last position is flag for overwrite with next line */

      /* put line in scrolling window */
      /* to insert text the values for from and to should be equal */
      XmTextReplace(iw_scroll, igo, iend, message);

      /* remember position of end of this line */
      iend = XmTextGetLastPosition(iw_scroll);

      /* remove the $ */
      XmTextReplace(iw_scroll, iend-1, iend, " ");

      XmTextSetInsertionPosition(iw_scroll, igo);
      /* next line start position same as start position of this one */
      }

   else
      {
      /* put line in scrolling window */

      XmTextReplace(iw_scroll, igo, iend, message);

      /* add a newline to the string */
      XmTextInsert(iw_scroll, XmTextGetLastPosition(iw_scroll), "\n");

      /* move cursor so XmNmotionVerifyCallback gets called */
      XmTextSetInsertionPosition(iw_scroll, XmTextGetLastPosition
                                        (iw_scroll)-1);

      /* update next line starting position */ 
      igo = XmTextGetLastPosition(iw_scroll);

      /* remember position of end of this line */
      iend = igo;

      if (trace_web) 
         {
         /* print message in file */
         fprintf(resfp,"%s\n",message);
         fflush(resfp);
         }
      }
  }

 /**************************** spouts *********************************/

 void spouts (char *message)
   {
   /* put line in scrolling window, do not add newline!!! */
   XmTextReplace(iw_scroll, igo, iend, message);

   /* move cursor so XmNmotionVerifyCallback gets called */
   XmTextSetInsertionPosition(iw_scroll, XmTextGetLastPosition
                                        (iw_scroll)-1);

   /* update next line starting position */ 
   igo = XmTextGetLastPosition(iw_scroll);

   /* remember position of end of this line */
   iend = igo;

   if (trace_web) 
      {
      /* print message in file */
      fprintf(resfp,"%s",message);
      /* no flush as line is not finished yet */
      }
  }
 /**************************** spouti *********************************/

 void spouti (int nval)
   {
   char * cval;

   /* put integer in scrolling window */

   /* convert nval to char string */
   if (!(cval = itoa(nval))) return;

   XmTextReplace(iw_scroll, igo, iend, cval);

   /* move cursor so XmNmotionVerifyCallback gets called */
   XmTextSetInsertionPosition(iw_scroll, XmTextGetLastPosition
                                        (iw_scroll)-1);

   /* update starting position */ 
   igo = XmTextGetLastPosition(iw_scroll);

   /* remember position of end of this line */
   iend = igo;

   if (trace_web) 
      {
      /* print message in file */
      fprintf(resfp,"%s",cval);
      /* no flush as line is not finished yet */
      }

   if (cval) free(cval);
   }

 /**************************** spouto *********************************/

 void spouto (char *message)
   {
   /* overwrite this text line with next incoming text line*/

   /* put line in scrolling window */
   /* to insert text the values for from and to should be equal */
   XmTextReplace(iw_scroll, igo, iend, message);

   /* remember position of end of this line */
   iend = XmTextGetLastPosition(iw_scroll);

   /* next line start position same as start position of this line */
   XmTextSetInsertionPosition(iw_scroll, igo);
   }


 
