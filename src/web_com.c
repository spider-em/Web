
/***********************************************************************
 *                                                                     *
 * web_com.c                                                           *
 *              _BGFG foreground & background    Oct 2005 ArDean Leith *
 *              _BGFG in submenus                Oct 2007 ArDean Leith *
 *              Removed blackout menu item       Mar 2011 ArDean Leith *
 *              Removed lu sub menu items        Mar 2011 ArDean Leith *
 *              Pixel rewrite                    Jun 2011 ArDean Leith *
 *              Truecolor                        Sep 2011 ArDean Leith *
 *              Montage quick button             Nov 2012 ArDean Leith *
 *              Surface quick button             Mar 2013 ArDean Leith *
 *              Removed Lut button               Mar 2015 ArDean Leith *
 *                                                                     *
 ***********************************************************************
 C=* AUTHOR: ArDean Leith                                              *
 C=* FROM: WEB - VISUALIZER FOR SPIDER MODULAR IMAGE PROCESSING SYSTEM *
 C=* Copyright 1985-2015  Health Research Inc.,                        *
 C=* Riverview Center, 150 Broadway, Suite 560, Menands, NY 12204.     *
 C=* Email:  spider@wadsworth.org                                      *
 C=*                                                                   *
 C=* This program is free software; you can redistribute it and/or     *
 C=* modify it under the terms of the GNU General Public License as    *
 C=* published by the Free Software Foundation; either version 2 of    *
 C=* the License, or (at your option) any later version.               *
 C=*                                                                   *
 C=* This program is distributed in the hope that it will be useful,   *
 C=* but without any warranty; without even the implied warranty of    *
 c=* merchantability or fitness for a particular purpose.  See the GNU *
 C=* General Public License for more details.                          *
 C=* You should have received a copy of the GNU General Public License *
 C=* along with this program. If not, see <http://www.gnu.org/licenses>*
 C=*                                                                   *
 C**********************************************************************
 * 
 * web_com(iw_menub)
 *
 * PURPOSE:     Contains commands for menu bar
 *
 * PARAMETERS:  iw_menub     menu-bar widget for menu item    
 *
 ***********************************************************************
*/

#include "routines.h"
#include "common.h"

#define MAX_ARGS     20
Arg args[MAX_ARGS];   /* Arg list */

// Function prototypes
extern Widget wid_seper(Widget, Widget, char *,  int, int);

/*******************************  web_com  ***************************/

 void web_com(Widget iw_menub)
 { 
 Widget iw_menup1c, iw_menup1d;
 Widget iw_menup1,  iw_menup2,  iw_menup4, iw_menup5;
 Widget iw_menup2a, iw_menup2b, iw_menup2c, iw_menup2d,iw_menup2e;
 Widget iw_menup2f, iw_menup2g;

 int  n;

 n = 0;

 // Create OPTIONS entry on menubar -------------------------------
 iw_menup1 = wid_pulldown(iw_menub,"OPTIONS  ",'O');

 // Create menu items for OPTIONS  
 wid_pushg(iw_menup1,(Widget)0,"Image",         imoptmen,     "0",0,0);
 wid_pushg(iw_menup1,(Widget)0,"Contrast",      contrastmen,  "0",0,0);
 wid_pushg(iw_menup1,(Widget)0,"Cursor",        curmen,       "0",0,0);
 wid_pushg(iw_menup1,(Widget)0,"Display",       dispmen,      "0",0,0);
 wid_pushg(iw_menup1,(Widget)0,"Font",          fontmen,      "0",0,0);
 wid_pushg(iw_menup1,(Widget)0,"Magnify",       setmag,       "0",0,0);
//#ifndef WEB_TRUE
// wid_pushg(iw_menup1,(Widget)0,"Lut",           lutmen,       "0",0,0);
//#endif
 wid_pushg(iw_menup1,(Widget)0,"Colors",        colmen,       "0",0,0);
 
 // Create a line style menu item in OPTIONS 
 iw_menup1c = wid_pulldown(iw_menup1,"Line Style",'M');

 wid_pushg(iw_menup1c,(Widget)0,"Solid",        setline,   "0",0,0);
 wid_pushg(iw_menup1c,(Widget)0,"OnOffDash",    setline,   "1",0,0);
 wid_pushg(iw_menup1c,(Widget)0,"DoubleDash",   setline,   "2",0,0);

 // Create a line width menu item in OPTIONS 
 iw_menup1d = wid_pulldown(iw_menup1,"Line Width",'M');

 wid_pushg(iw_menup1d,(Widget)0,"0",            setline,   "3",0,0);
 wid_pushg(iw_menup1d,(Widget)0,"1",            setline,   "4",0,0);
 wid_pushg(iw_menup1d,(Widget)0,"2",            setline,   "5",0,0);
 wid_pushg(iw_menup1d,(Widget)0,"3",            setline,   "6",0,0);
 wid_pushg(iw_menup1d,(Widget)0,"4",            setline,   "7",0,0);
 wid_pushg(iw_menup1d,(Widget)0,"5",            setline,   "8",0,0);



// Create COMMANDS entry on menubar ---------------------------------
 iw_menup2 = wid_pulldown(iw_menub,"COMMANDS ",'C');

 /* Create menu items for COMMANDS */ 
 wid_pushg(iw_menup2,(Widget)0,"Clear",                webclear,  "0",0,0);
 wid_pushg(iw_menup2,(Widget)0,"Image",                imagemen,  "1",0,0);
 wid_pushg(iw_menup2,(Widget)0,"Montage",              imagemen,  "2",0,0);
 wid_pushg(iw_menup2,(Widget)0,"Surface",              imagemen,  "3",0,0);
 wid_pushg(iw_menup2,(Widget)0,"Shift",                imagemen,  "i",0,0);
 wid_pushg(iw_menup2,(Widget)0,"Annotate",             imagemen,  "9",0,0);
#ifndef WEB_TRUE
 wid_pushg(iw_menup2,(Widget)0,"Colored Surface",      imagemen,  "h",0,0);
#endif
 wid_pushg(iw_menup2,(Widget)0,"Convolve",             imagemen,  "a",0,0);
 wid_pushg(iw_menup2,(Widget)0,"Corr-Map",             comapmen,  "0",0,0);
 wid_pushg(iw_menup2,(Widget)0,"Dendrogram",           imagemen,  "e",0,0);
 wid_pushg(iw_menup2,(Widget)0,"Distance",             distmen,   "0",0,0);
 wid_pushg(iw_menup2,(Widget)0,"Histogram",            imagemen,  "4",0,0);
 wid_pushg(iw_menup2,(Widget)0,"Magnify",              mag     ,  "0",0,0);
 wid_pushg(iw_menup2,(Widget)0,"Markers",              imagemen,  "5",0,0);
 wid_pushg(iw_menup2,(Widget)0,"Mask",                 imagemen,  "6",0,0);
 wid_pushg(iw_menup2,(Widget)0,"Montage from doc file",imagemen,  "k",0,0);

 /* Create a  Movie menu item in COMMANDS */
 iw_menup2g = wid_pulldown(iw_menup2,"Movie",'G');

 wid_pushg(iw_menup2g,(Widget)0,"Large images",     imagemen,"o",0,0);
 wid_pushg(iw_menup2g,(Widget)0,"Small images",     imagemen,"g",0,0);

 wid_pushg(iw_menup2,(Widget)0,"Tilted Particles",  imagemen,"8",0,0);
 wid_pushg(iw_menup2,(Widget)0,"CTF from doc file", imagemen,"m",0,0);

/* Create a  Categorize menu item in COMMANDS */
 iw_menup2f = wid_pulldown(iw_menup2,"Categorize",'N');

 wid_pushg(iw_menup2f,(Widget)0,"Sequential montage",imagemen,"b",0,0);
 wid_pushg(iw_menup2f,(Widget)0,"Doc file montage",  imagemen,"n",0,0);

 /* Create a  Power Spectrum menu item in COMMANDS */
 iw_menup2e = wid_pulldown(iw_menup2,"Power Spectrum",'M');

 wid_pushg(iw_menup2e,(Widget)0,"Interactive",      winsiz,   "81",0,0);
 wid_pushg(iw_menup2e,(Widget)0,"Fixed size",       winsiz,   "82",0,0);
 wid_pushg(iw_menup2e,(Widget)0,"Specified coord.", winsiz,   "83",0,0);

 wid_pushg(iw_menup2,(Widget)0,"Pixel on screen",   pixelmen_sc_nod, "0",0,0); 
 wid_pushg(iw_menup2,(Widget)0,"Pixel within image",imagemen,    "p",0,0); 
 wid_pushg(iw_menup2,(Widget)0,"Profile",           imagemen,    "7",0,0);
 wid_pushg(iw_menup2,(Widget)0,"Reflections",       imagemen,    "c",0,0);
 wid_pushg(iw_menup2,(Widget)0,"General filter",    imagemen,    "l",0,0);
 wid_pushg(iw_menup2,(Widget)0,"SIRDS",             imagemen,    "f",0,0);


/* Create a RECORD menu item in COMMANDS */
// iw_menup2b = wid_pulldown(iw_menup2,"Record screen",'W');

// wid_pushg(iw_menup2b,(Widget)0,"Interactive",      winsiz,   "61",0,0);
// wid_pushg(iw_menup2b,(Widget)0,"Fixed size",       winsiz,   "62",0,0);
// wid_pushg(iw_menup2b,(Widget)0,"Specified coord.", winsiz,   "63",0,0);

// wid_pushg(iw_menup2,(Widget)0,"Show Recording",    imagemen, "d",0,0);

 /* Create a EDIT entry on menubar -----------------------------------*/
 iw_menup5 = wid_pulldown(iw_menub,"EDIT ",'O');

 wid_pushg(iw_menup5,(Widget)0,"Bars",              bar,        "0",0,0);
 wid_pushg(iw_menup5,(Widget)0,"Drill-n-fill",      imagemen,   "j",0,0);
 wid_pushg(iw_menup5,(Widget)0,"Lines",             rubberline, "0",0,0);
 wid_pushg(iw_menup5,(Widget)0,"Text",              itex,       "0",0,0);

/* Create a boxes menu item in EDIT */
 iw_menup2c = wid_pulldown(iw_menup5,"Draw Boxes",'W');

 wid_pushg(iw_menup2c,(Widget)0,"Interactive",      winsiz,     "51",0,0);
 wid_pushg(iw_menup2c,(Widget)0,"Fixed size",       winsiz,     "52",0,0);
 wid_pushg(iw_menup2c,(Widget)0,"Specified coord.", winsiz,     "53",0,0);

 /* Create a move area menu item in EDIT */
 iw_menup2a = wid_pulldown(iw_menup5,"Move area",'M');

 wid_pushg(iw_menup2a,(Widget)0,"Interactive",      winsiz,     "71",0,0);
 wid_pushg(iw_menup2a,(Widget)0,"Fixed size",       winsiz,     "72",0,0);
 wid_pushg(iw_menup2a,(Widget)0,"Specified coord.", winsiz,     "73",0,0);

 /* Create a window file  menu item in EDIT */
 iw_menup2d = wid_pulldown(iw_menup5,"Window Last File",'M');

 wid_pushg(iw_menup2d,(Widget)0,"Interactive",      winsiz,     "41",0,0);
 wid_pushg(iw_menup2d,(Widget)0,"Fixed size",       winsiz,     "42",0,0);
 wid_pushg(iw_menup2d,(Widget)0,"Specified coord.", winsiz,     "43",0,0);
 
 /* Create a SYSTEM  entry on menubar ----------------------------------*/
 iw_menup4 = wid_pulldown(iw_menub,"SYSTEM",'O');

 /* Create menu items in SYSTEM */ 
 wid_pushg(iw_menup4,(Widget)0,"File extension",    newext   ,  "0",0,0);
 wid_pushg(iw_menup4,(Widget)0,"Current directory", changedir,  "0",0,0);
 wid_pushg(iw_menup4,(Widget)0,"File pattern ",     changepat,  "0",0,0);
 wid_pushg(iw_menup4,(Widget)0,"X-info",            web_info,   "0",0,0);
 wid_pushg(iw_menup4,(Widget)0,"EXIT",              exitexit,   "0",0,0);

 }

 
/*******************************  web_com2  ***************************/

#include <Xm/PushBG.h>

 void web_com2(Widget iw_quickmen)
 { 
 Widget         iw_quicki, iw_quickm, iw_clear, iw_exit;
 Widget         iw_space, iw_skip;
 Widget         iw_exit2;
 Widget         iw_contrast;
 Widget         iw_seper1 = (Widget) 0;
 Widget         iw_seper2 = (Widget) 0;
 Widget         iw_seper3 = (Widget) 0;
 Widget         iw_quicks = (Widget) 0;

 int            n;

 // Create a separator -----------------------------------------
 iw_seper1 = wid_seper(iw_quickmen, iw_seper1, "|", 0, 0);
 
 // Create a push button  for QUICK IMAGE ----------------------

 n = 0;
#ifdef WEB_BGFG
 XtSetArg(args[n], XmNbackground,      icolorxmb); n++;      
#endif
 XtSetArg(args[n], XmNshadowThickness, 0);                 n++;
 XtSetArg(args[n], XmNmarginHeight,    0);                 n++;
 XtSetArg(args[n], XmNtopAttachment,   XmATTACH_FORM);     n++;

 n = 4;
 XtSetArg(args[n], XmNleftAttachment,  XmATTACH_FORM);     n++;

 iw_quicki = XmCreatePushButtonGadget(iw_quickmen," IMAGE",args,n);
 if (iw_quicki == (Widget) NULL)
    { XtWarning("*** Can not create Quick image widget!"); exit (-1);}

 XtAddCallback(iw_quicki, XmNactivateCallback, imagemen, "1");
 XtManageChild(iw_quicki); 


 // Create a push button  for QUICK Montage ----------------------

 n = 4;
 XtSetArg(args[n], XmNleftAttachment,  XmATTACH_WIDGET);   n++;
 XtSetArg(args[n], XmNleftWidget,      iw_quicki);         n++;

 iw_quickm = XmCreatePushButtonGadget(iw_quickmen," MONTAGE",args,n);
 if (iw_quickm == (Widget) NULL)
    { XtWarning("*** Can not create Quick MONTAGE widget!"); exit (-1);}

 XtAddCallback(iw_quickm, XmNactivateCallback, imagemen, "2");
 XtManageChild(iw_quickm); 

 // Create a push button  for QUICK Surface ----------------------

 n = 4;
 XtSetArg(args[n], XmNleftAttachment,  XmATTACH_WIDGET);   n++;
 XtSetArg(args[n], XmNleftWidget,      iw_quickm);         n++;

 iw_quicks = XmCreatePushButtonGadget(iw_quickmen," SURFACE",args,n);
 if (iw_quicks == (Widget) NULL)
    { XtWarning("*** Can not create Quick MONTAGE widget!"); exit (-1);}

 XtAddCallback(iw_quicks, XmNactivateCallback, imagemen, "3");
 XtManageChild(iw_quicks); 



 // Create a push button  for CLEAR ---------------------------

 n = 4;
 XtSetArg(args[n], XmNleftAttachment,  XmATTACH_WIDGET);   n++;
 XtSetArg(args[n], XmNleftWidget,      iw_quickm);         n++;

 iw_clear = XmCreatePushButtonGadget(iw_quickmen," CLEAR ",args,n);
 if (iw_clear == (Widget) NULL)
    { XtWarning("*** Can not create CLEAR widget!"); exit (-1);}

 XtAddCallback(iw_clear, XmNactivateCallback, (XtCallbackProc) webclear, "0");
 XtManageChild(iw_clear); 

 // Create a push button  for EXIT ----------------------------

 n = 4;
 XtSetArg(args[n], XmNleftAttachment,  XmATTACH_WIDGET);   n++;
 XtSetArg(args[n], XmNleftWidget,      iw_clear);         n++;
 XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM);     n++;

 iw_exit = XmCreatePushButtonGadget(iw_quickmen,"EXIT ",args,n);
 if (iw_exit == (Widget) NULL)
    { XtWarning("*** Can not create EXIT widget!"); exit (-1);}

 XtAddCallback(iw_exit, XmNactivateCallback, (XtCallbackProc) exitexit, "0");
 XtManageChild(iw_exit); 


 // Create a separator -----------------------------------------
iw_seper2 = wid_seper(iw_quickmen, iw_seper2, "|", 0, 0);


 // Create a widget  for Contrast ---------------------------------
 iw_contrast = wid_contrast(iw_quickmen," I:",  0,0);

 // Create a widget  for SKIP ---------------------------------
 iw_skip = wid_skip(iw_quickmen," X:",  0,0);

 // Create a separator -----------------------------------------
iw_seper3 = wid_seper(iw_quickmen, iw_seper3, "|", 0, 0);

 // Create a push button  for EXIT ----------------------------

 n = 4;
 XtSetArg(args[n], XmNleftAttachment,  XmATTACH_WIDGET);   n++;
 XtSetArg(args[n], XmNleftWidget,      iw_clear);         n++;
 XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM);     n++;

 iw_exit2 = XmCreatePushButtonGadget(iw_quickmen,"EXIT ",args,n);
 if (iw_exit2 == (Widget) NULL)
    { XtWarning("*** Can not create EXIT widget!"); exit (-1);}

 XtAddCallback(iw_exit2, XmNactivateCallback, (XtCallbackProc) exitexit, "0");
 XtManageChild(iw_exit2); 
 }
