
/*$Header: /usr8/web/src/RCS/wid_file.c,v 1.15 2011/07/20 16:36:51 leith Exp $*/

/*
C++*************************************************************************
C
C WID_FILE -- CREATED JUNE 91
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
C    WID_FILE
C
C    PARAMETERS:   IW_PARENT   PARENT WIDGET            READ
C                  IW_FILE     WIDGET NUMBER            READ/WRITE
C                  LABEL       WIDGET LABEL             READ
C                  FILNAM      FILE NAME                READ
C                  FILPATH     DIRECTORY PATH           READ
C                  CB          CALLBACK ON ACCEPT       READ
C                  IX,IY       UPPER LEFT CORNER        READ
C
C    PURPOSE:      MAKES A FILE WIDGET
C
C    CALLED BY:    WID_IMAGEMEN
C
C--********************************************************************
*/

#include "common.h"
#include "routines.h"
#include <Xm/FileSB.h>
#include <Xm/SelectioB.h>

 /* External common variables */
 extern Widget         iw_top;            /* Top widget   */

 /* File variables */

 /********************  wid_file  ****************************/

 Widget wid_file(Widget iw_parent, Widget iw_file, 
               char *label, char *filnam, char *filpath,
               void (*cb) (), int ix, int iy)

 {
 Arg           args[8];          /* Arg list */
 register      int  n;           /* Arg count */
 Position      ixp,iyp;          /* Parent widget's position */
 XmString      str_filnam = 0,  str_label = 0,  str_filpath = 0;
 XmString      str_prompt = 0;
 Widget	       iw_temp;
 Widget        widlist;

 /* Need to find location relative to present position of top window */
 XtSetArg(args[0], XmNx, &ixp);
 XtSetArg(args[1], XmNy, &iyp);
 XtGetValues(iw_top,args,2);

 n = 0;
#ifdef WEB_BGFGO
 XtSetArg(args[n], XmNbackground,  icolorxmb); n++;      
#endif

 /* Set label for filename list in the widget */
 str_label = XmStringCreate(label, XmSTRING_DEFAULT_CHARSET);
 XtSetArg(args[n], XmNfileListLabelString, str_label); n++;

 if (*filpath != '\0')
    {
    /* Set up dir search pattern */ 
    str_filpath = XmStringCreate(filpath, XmSTRING_DEFAULT_CHARSET);
    XtSetArg(args[n], XmNdirMask, str_filpath);    n++;
    }

 if (*filnam != '\0')
    {
    /* Set up file name listed in the selection box */
    str_filnam = XmStringCreate(filnam, XmSTRING_DEFAULT_CHARSET); 
    XtSetArg(args[n], XmNtextString, str_filnam);  n++;
    }

 if (iw_file == (Widget)0)
    { /* Must create and manage new file widget */

    XtSetArg(args[n], XmNdefaultPosition, False);  n++;

    if (ix < 0 || iy < 0)
       {    /*  Use default placement for dialog box */
       ix = 20;
       iy = 20;
       }

    XtSetArg(args[n], XmNx, (Position) ix+ixp);    n++;
    XtSetArg(args[n], XmNy, (Position) iy+iyp);    n++;

    iw_file = XmCreateFileSelectionDialog(iw_parent,"iw_file",args,n);
	
    XtAddCallback(iw_file,XmNokCallback,cb,NULL);
    XtAddCallback(iw_file,XmNcancelCallback, (XtCallbackProc) fin_cb,
                         (XtPointer)iw_file);

    XtManageChild(iw_file);

    /* Get rid of help button in widget */
    XtUnmanageChild(XmFileSelectionBoxGetChild(
                    iw_file,XmDIALOG_HELP_BUTTON));

#ifdef WEB_BGFGO

    XtSetArg(args[n], XmNbackground,  icolorxmb); n++;      
    widlist = XtNameToWidget (iw_file, "DirListSW.VertScrollBar");
    //printf("*VertScrollBar: %d \n",widlist);
    XtSetValues(widlist,args,1);
    widlist = XtNameToWidget (iw_file, "DirListSW.HorScrollBar");
    XtSetValues(widlist,args,1);

    XtSetArg(args[n], XmNbackground,  icolorxmb); n++;      
    widlist = XtNameToWidget (iw_file, "ItemsListSW.VertScrollBar");
    XtSetValues(widlist,args,1);
    widlist = XtNameToWidget (iw_file, "ItemsListSW.HorScrollBar");
    XtSetValues(widlist,args,1);

    XtSetValues(XmFileSelectionBoxGetChild(
                iw_file,XmDIALOG_FILTER_TEXT),args,1);
    XtSetValues(XmFileSelectionBoxGetChild(
                iw_file,XmDIALOG_DIR_LIST),args,1);
    XtSetValues(XmFileSelectionBoxGetChild(
                iw_file,XmDIALOG_LIST),args,1);
    XtSetValues(XmFileSelectionBoxGetChild(
                iw_file,XmDIALOG_TEXT),args,1);
#endif

    }

 else 
    { /* Can use existing widget */
	
    if ( ix > 0 || iy > 0)
       {
       XtSetArg(args[n], XmNx, (Position) ix+ixp); n++;
       XtSetArg(args[n], XmNy, (Position) iy+iyp); n++;
       }
    XtSetValues(iw_file,args,n);

    XtRemoveAllCallbacks(iw_file,XmNokCallback); 
    XtAddCallback(iw_file,XmNokCallback,cb,NULL);
    XtAddCallback(iw_file,XmNcancelCallback,(XtCallbackProc) fin_cb,
                                   (XtPointer) iw_file);

    XtManageChild(iw_file);

    /* Remove the Help button */
    if ((iw_temp = XmFileSelectionBoxGetChild(
		   iw_file,XmDIALOG_HELP_BUTTON))!=NULL)
         XtUnmanageChild(iw_temp);
    }

 if (str_label)   XmStringFree(str_label);
 if (str_filpath) XmStringFree(str_filpath);
 if (str_filnam)  XmStringFree(str_filnam);

 return iw_file;

 }
 /********************  wid_fileset  ****************************/

 void wid_fileset(Widget iw_file, char *filnam)

 {
 Arg           args[1];   
 XmString      str_filnam;


 if (*filnam != '\0')
    {
    /* Set up file name listed in the selection box */
    str_filnam = XmStringCreate(filnam, XmSTRING_DEFAULT_CHARSET); 

    XtSetArg(args[0], XmNtextString, str_filnam);    

    XtSetValues(iw_file,args,1);
    XmStringFree(str_filnam);
    }
 }

