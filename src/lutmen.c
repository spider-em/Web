
/*$Header: /usr8/web/new/RCS/lutmen.c,v 1.13 2005/10/18 17:00:01 leith Exp $*/
/*
C++*************************************************************************
C
C lutmen.c   Created from imagemen.c                    Nov  92
C                                                                    
C **********************************************************************
C *  AUTHOR: A. LEITH                                                      *
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
C    LUTMEN
C
C    PARAMETERS:   iw_temp, data, call_data (Usual callback data)
C
C    PURPOSE:      Makes a file widget, interrogates file widget and
c                  calls lut display routine
C
C    CALLED BY:    web_com
C
C    Note: need to add *.RGB to filter box!!!!
C
C--********************************************************************
*/

#include <Xm/FileSB.h>
#include "common.h"
#include "routines.h"

 /* Internal function  prototypes */
 void            lutmen_cb (Widget, XtPointer, XtPointer);
 void            lutmen1   (char *);

 /* Internal file variables */
 static char   * fillutp;
 static char     fillut[81]  = "\0";  // Sets selected file
 static Widget   iw_lutfiles = 0;
 static char     coper[1];
 static char     lutdirstr[81];

/********************************  lutmen  ***************************/

 void lutmen(Widget iw_temp, XtPointer data, XtPointer call_data)
 { 
 char * lutdirptr;

 /* Create a lut file selector widget */
 strncpy(coper, data,1);

if (iw_lutfiles == (Widget) 0)
   {
   if( (lutdirptr = getenv("SPRGB_DIR") ) == NULL)
       {
       spout("*** The SPRGB_DIR environment variable is blank!");
       lutdirstr[0] = '\0';
       }
   else
       { strcpy(lutdirstr,lutdirptr); }
    }

 /* Create a file selector box */
 iw_lutfiles =  wid_file(iw_win, iw_lutfiles , "Look-up-table",
               &fillut[0], lutdirstr, *lutmen_cb, -1, -1);

 /* Kludge to keep filename in selection box */
 wid_fileset(iw_lutfiles,fillut);
 }


/************************** lutmen_cb ****************************/

 void lutmen_cb(Widget iw_temp, XtPointer data, XtPointer call_data)
 { 
 XmFileSelectionBoxCallbackStruct *cbs;
 Colormap                          mapt;
 unsigned long                     cells[256];
 
 /* Get file name from file selector widget */
 cbs = (XmFileSelectionBoxCallbackStruct *) call_data;

 fillutp = extractstr(cbs -> value);

 /* Get rid of file selector */
 XtUnmanageChild(iw_lutfiles);
 
 /* Apply the lut */
 if (!strcmp(coper, "0"))
    lut(&fillutp[0], map, mapgo, mapend);
 else
    {
    mapt = XCopyColormapAndFree(idispl, map);
    XAllocColorCells(idispl, map, TRUE, NULL, 0, cells, 256);
    lut(&fillutp[0], map, 0, 255);
    XFreeColormap(idispl, mapt);
    }

 /* Start lutmen again */
 strcpy(fillut,extractstr(cbs -> value));

 lutmen(NULL,coper,NULL);

 return;
 }

