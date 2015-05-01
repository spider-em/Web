
/*$Header: /usr8/web/src/RCS/changepat.c,v 1.5 2005/10/18 16:59:50 leith Exp $*/

/***********************************************************************
 *
 * changepat.c
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
 * PURPOSE:  Change current filename search pattern     
 * 
 * PARAMETERS:	Widget 		iw_temp		pushbutton (not used)
 *		caddr_t		data		label text string
 *		caddr_t		call_data	not used 
 *
 ***********************************************************************
*/

#include "common.h"
#include <Xm/FileSB.h>
#include "routines.h"


 /* internal functions */
 void            changepat_cb   (Widget, caddr_t, caddr_t);


 /* file variables */
 static char       label_string[] = "Enter new file search string";  
 static Widget     iw_text = (Widget)0;
 static int        ixtext = 0;      /* x coord for widget    */
 static int        iytext = 0;      /* y coord for widget    */
 

/************************  changepat  ***********************************/

 void changepat(Widget iw_temp, caddr_t data, caddr_t call_data)
 {

 /* create new directory input box */
 iw_text = wid_text(iw_win, iw_text, label_string, filwant,
                       *changepat_cb, NULL, ixtext, iytext);
 }


 /**********************  changepat_cb  *********************************/

 void changepat_cb(Widget iw_temp, caddr_t data, caddr_t call_data)
 {
 XmString      txt_string;
 Arg           args[1];   /* arg list */
 XmString      str_dir;

 XmSelectionBoxCallbackStruct *cbs =
      (XmSelectionBoxCallbackStruct *) call_data;
 
 strncpy(filwant,extractstr(cbs -> value),80);


 }

