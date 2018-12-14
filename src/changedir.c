
/*$Header: /usr16/software/web/src/RCS/changedir.c,v 1.12 2018/12/07 17:03:30 leith Exp $*/

/***********************************************************************
 *
 * changedir.c
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
 * PURPOSE:  Change current directory     
 * 
 * PARAMETERS:	Widget 		iw_temp		pushbutton (not used)
 *		XtPointer		data		label text string
 *		XtPointer		call_data	not used 
 *
 ***********************************************************************
*/

#include <unistd.h>
#include <Xm/FileSB.h>
#include "common.h"
#include "routines.h"

 /* Internal functions */
 static void     changedir_cb(Widget, XtPointer, XtPointer);

 /* File scope variables */
 static Widget     iw_text = (Widget)0;

/************************  changedir  ***********************************/

 void changedir(Widget iw_temp, XtPointer data, XtPointer call_data)
 
 {
 /* Create new directory input box */
 iw_text = wid_text(iw_win, iw_text, "Enter new directory", dirnow,
                       *changedir_cb, NULL, 0, 0);
 }


 /**********************  changedir_cb  *********************************/

 void changedir_cb(Widget iw_temp, XtPointer data, XtPointer call_data)
 {
 Arg         args[1];   
 XmString    str_dir;
 char        *cptr;

 XmSelectionBoxCallbackStruct *cbs =
      (XmSelectionBoxCallbackStruct *) call_data;
 
 cptr = extractstr(cbs -> value);

 if (strlen(cptr) > (size_t) 80)
    {
    spout("*** Directory name truncated ");
    XBell(idispl,50);
    }
 /* Should have better handling of too long directories!!! */

 strncpy(dirnow,cptr,80);
 free(cptr);
 chdir(dirnow);
 getcwd(dirnow,80);
 spouts(" Current directory: "); spout(dirnow);

 if (iw_files != (Widget)0)
    {   /* Update file widget current directory */

    str_dir = XmStringCreate(dirnow, XmSTRING_DEFAULT_CHARSET);
    XtSetArg(args[0], XmNdirectory, str_dir);   
    XtSetValues(iw_files, args,1);
    if (str_dir)  XmStringFree(str_dir);
    }
 }

