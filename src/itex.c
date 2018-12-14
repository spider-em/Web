
/*$Header: /usr16/software/web/src/RCS/itex.c,v 1.5 2018/12/07 17:03:32 leith Exp $*/

/*
 * itex.c
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
 * 
 * 
 * PURPOSE:  Callback routine for Text selection in Command pulldown
 *           menu     
 * 
 * PARAMETERS:	Widget 		iw_temp		pushbutton (not used)
 *		void *		data		label text string
 *		void *		call_data	not used 
 *
 ***********************************************************************
*/

#include "x.h"
#include "std.h"
#include "common.h"
#include "routines.h"
#include "posiz.h"



 /* internal functions */
 static void     itex_cb   (Widget, void *, void *);
 static void     itex_pop  (Widget, XEvent *, String *, Cardinal *);
 static void     itex3     (void);

 /* common variables */



 /* file variables */
 char            label_string[] = "Enter text";  /* label for text widget  */
 Widget          iw_text, iw_posiz;
 XmFontList      font_list;
 int             ixtext;           /* x coord for text       */
 int             iytext;           /* y coord for text       */
  

/************************  itex  ************************************/

 void itex(Widget iw_temp, void * data, void * call_data)
 {

 /* open a message window with the following strings */
 showbutx("Set text location with mouse.",
          "Enter text coordinates.",
          "Stop this routine.", FALSE);

 /* set the actions for right, left, and center buttons */
 actions(iw_win, "itex_pop", itex_pop, "123");
 }



 /********************* itex_pop  ************************/

 void itex_pop(Widget iw_t, XEvent *event, String *params,
                  Cardinal *num_params)
 {
 int ixt, iyt, idum; 
 
 if (!(strcmp(*params,"1")))                  /* button 1 pushed */
    {
    /* get mouse position */
    getloc(event,'B',&ixtext,&iytext);

   /* create text input box */
    iw_text = wid_text(iw_win, iw_text, label_string, (char *) NULL,
                       *itex_cb, NULL, ixtext, iytext);
    }


 else if (!(strcmp(*params,"2")))            /* button 2 pushed */
    {
    /* get mouse position */
    getloc(event,'B',&ixt,&iyt);

    /* query position */
    iw_posiz = wid_posiz(iw_win, iw_posiz, ixt, iyt,  idum, idum, 
                         'P', itex3);
    }


 else if (!(strcmp(*params,"3")))            /* button 3 pushed */
    {
    /* uninstall translations */
    XtUninstallTranslations(iw_t);

    /* unmanage information box */
    showbutx("","","",TRUE);
    }

 }


 /**********************  itex_cb  *********************************/

 void itex_cb(Widget iw_temp, void * data, void * call_data)
 {
 XmString    txt_string;
 char        *text;

 XmSelectionBoxCallbackStruct *cbs =
      (XmSelectionBoxCallbackStruct *) call_data;
 
 txt_string = cbs -> value;
 text = extractstr(cbs -> value);

 /* write text in current color */
 witext(icontx, text, ixtext, iytext, 0, 0, -9, 2, FALSE);
 }


 /****************  itex3 (runs after wid_posiz) *****************/

 void itex3(void)
 {

 ixtext = ix_posiz;
 iytext = iy_posiz;

 /* create text input box now */
 iw_text = wid_text(iw_win, iw_text, label_string, (char *) NULL,
                    itex_cb, NULL, ixtext, iytext);
 
 }
