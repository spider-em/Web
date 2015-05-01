/*$Header: /usr8/web/src/RCS/web_main.c,v 1.47 2011/09/29 14:26:32 leith Exp $*/

/*
 ***********************************************************************
 *                                                                     *
 * web_main.c                                                          *
 *               moved: realize all widgets now  Dec 2004 ArDean Leith *
 *               _BGFG foreground & background   Oct 2005 ArDean Leith *
 *               EXIT                            Feb 2011 ArDean Leith *
 *               WEB_TRUE                        Sep 2011 ArDean Leith *
 *                                                                     *
 ***********************************************************************
 C=* FROM: WEB - VISUALIZER FOR SPIDER MODULAR IMAGE PROCESSING SYSTEM *
 C=* Copyright (C) 1992-2011  Health Research Inc.                     *
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
 * web_main(parent)
 * 
 * PURPOSE:     Initialize X Windows
 *
 * RETURNS:     iw_main
 * 
 * PARAMETERS:  parent       Parent widget   
 *
 ***********************************************************************
*/

#include "routines.h"
#include "common.h"

/*  Define web icon */
#include "web_icon.xbm"  

#include <Xm/Text.h>
#include <Xm/Form.h>
#include <Xm/ScrollBar.h>
#include <Xm/ScrolledW.h>
#include <Xm/DrawingA.h>
#include <Xm/RowColumn.h>
#include <Xm/PushBG.h>
#include <X11/Xatom.h>

#define MAX_ARGS     20

 // Internal functions defined here
 void query_widget(char * label, Widget iw_widget);

 // File scope variables
 char           outstr[80];

#ifdef WEB_TRUE
 
// **************************  web_main  ******************************

 Widget web_main(Widget parent)          // parent = iw_top
 { 
 Arg                   args[MAX_ARGS];   // Arg list 
 int                   n;                // Arg count 

 Widget                iw_exit, iw_quickb;
 Widget                iw_left;
 Pixmap                icon_pixmap;
 XWMHints              wm_hints;
 Colormap              mapdef;
 unsigned long         icon_fore, icon_back;
 XTextProperty         windowName, iconName;
 XColor                col_def, rgb_def;
 XSetWindowAttributes  xswa;
 Window                win_list[2];
 char                * icon_name   = "WEB";
 char                * window_name = 
  "Web -- A SPIDER image viewer and analyzer       COPYRIGHT (c) 1992-2011 Health Research Inc., Menands, NY";

 int                   icolorxtb,icolorxtf;

 /* Set main window position and size -------------------------*/
 ihighx = HeightOfScreen(iscreen) - 100;
 iwidex = WidthOfScreen(iscreen)  - 100;
 printf(" Screen size:        %d X %d\n",iwidex,ihighx);

 n = 0;
 XtSetArg(args[n], XmNx, 0);                n++;
 XtSetArg(args[n], XmNy, 0);                n++;
 XtSetArg(args[n], XmNwidth,  iwidex);      n++;
 XtSetArg(args[n], XmNheight, ihighx);      n++;

 if ((iw_main = XmCreateForm(parent, "main", args,n)) == 0)
   { XtWarning("*** Web can not open main window!"); exit (-1); }
 XtManageChild(iw_main);

 XtSetArg(args[0], XmNbackground,  &icolorxtb);                       
 XtSetArg(args[1], XmNforeground,  &icolorxtf);
 XtGetValues(iw_main,args,2); 
 printf(" GUI Background:     %#8x \n",icolorxtb);
 printf(" GUI Foreground:     %#8x \n",icolorxtf);

 icolorxmb = WhitePixelOfScreen(iscreen);
 icolorxmf = BlackPixelOfScreen(iscreen);
 printf(" White Pixel:        %#8x \n",icolorxmb);      
 printf(" Black Pixel:        %#8x \n",icolorxmf);     
 icolorxmb = 0xEEEEEE;      // jul 2011   

 // Create a menu bar for top line commands ---------------------

 n = 0;           
 Setbgfg(args, MAX_ARGS, &n);  // Handles BGFG Args
 XtSetArg(args[n], XmNtopAttachment,   XmATTACH_FORM); n++;
 XtSetArg(args[n], XmNleftAttachment,  XmATTACH_FORM); n++;
 XtSetArg(args[n], XmNorientation,     XmHORIZONTAL);  n++;
 iw_left = XmCreateRowColumn(iw_main, "left", args, n);
 if (iw_left == NULL)
    { XtWarning("*** Can not create left menu widget!"); exit (-1);}
 XtManageChild(iw_left);

 n = 1;   // keep: XmNbackground =  icolorxmb
 XtSetArg(args[n], XmNtopAttachment,  XmATTACH_FORM);  n++;
 XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM);  n++;
 XtSetArg(args[n], XmNshadowThickness, 0);             n++;
 XtSetArg(args[n], XmNmarginHeight,    7);             n++;
 iw_menub = XmCreateMenuBar(iw_left, "menub", args, n);
 if (iw_menub == NULL)
    { XtWarning("*** Can not create menu bar widget!"); exit (-1);}
 XtManageChild(iw_menub);

 // Create menu items under top menu bar ----------------------- 
 web_com(iw_menub);

 // Create a menu bar for QUICK PICKS selection -------------- QUICK
 n = 1;            // Keeps: XmNbackground = icolorxmb
 XtSetArg(args[n], XmNtopAttachment,   XmATTACH_FORM);   n++;
 XtSetArg(args[n], XmNleftAttachment,  XmATTACH_WIDGET); n++;
 XtSetArg(args[n], XmNleftWidget,      iw_menub);        n++;
 XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM);   n++;
 XtSetArg(args[n], XmNorientation,     XmHORIZONTAL);    n++;

 iw_quickb = XmCreateRowColumn(iw_main, "quickb", args, n);
 if (iw_quickb == NULL)
    { XtWarning("*** Can not create quick menu widget!"); exit (-1);}
 XtManageChild(iw_quickb);

 // Create menu items under QUICK PICKS 
 web_com2(iw_quickb);




 // Create scrolled label window for error messages etc  ----- MESSAGES
 n = 0;
 Setbgfg(args, MAX_ARGS, &n);  // Handles BGFG Args
 XtSetArg(args[n], XmNscrollingPolicy,    XmAUTOMATIC);       n++;
 XtSetArg(args[n], XmNscrollBarDisplayPolicy, XmSTATIC);      n++;
 XtSetArg(args[n], XmNscrollBarPlacement, XmBOTTOM_RIGHT);    n++;
 XtSetArg(args[n], XmNvisualPolicy,       XmCONSTANT);        n++;
 XtSetArg(args[n], XmNeditMode,           XmMULTI_LINE_EDIT); n++;
 XtSetArg(args[n], XmNeditable,           False);             n++;
 XtSetArg(args[n], XmNleftAttachment,     XmATTACH_FORM);     n++;
 XtSetArg(args[n], XmNrightAttachment,    XmATTACH_FORM);     n++; 
 XtSetArg(args[n], XmNbottomAttachment,   XmATTACH_FORM);     n++;
 XtSetArg(args[n], XmNheight,             90);                n++;

 iw_scroll = XmCreateScrolledText(iw_main, "comscroll", args, n);
 XtManageChild(iw_scroll);

 /* Find parent for the scrolling text window */
 iw_scrollpar = XtParent(iw_scroll);


 // Create spider image widget ------------------------------ IMAGES 

 n = 0;
 Setbgfg(args, MAX_ARGS, &n);  // Handles BGFG Args
 XtSetArg(args[n], XmNtopWidget,        iw_menub);          n++;
 XtSetArg(args[n], XmNtopAttachment,    XmATTACH_WIDGET);   n++;
 XtSetArg(args[n], XmNleftAttachment,   XmATTACH_FORM);     n++;
 XtSetArg(args[n], XmNrightAttachment,  XmATTACH_FORM);     n++;
 
 XtSetArg(args[n], XmNbottomWidget,     iw_scrollpar);      n++;
 XtSetArg(args[n], XmNbottomAttachment, XmATTACH_WIDGET);   n++;
 XtSetArg(args[n], XtNcolormap,         map);               n++;

 iw_win = XmCreateDrawingArea(iw_main, "win", args, n);
 XtManageChild(iw_win);


 /* Use expos_cb for redrawing the image widget contents */
 XtAddCallback(iw_win,XmNexposeCallback, (XtCallbackProc) expos_cb,NULL);

 // Find root window for this screen 
 iroot = RootWindowOfScreen(iscreen);
 //printf("*** RootWindowOfScreen: %d\n",iroot); 

 /* Create an offscreen backing store for image & graphics window */
 imagsav = XCreatePixmap(idispl,iroot,iwidex,ihighx,idepth);
 if (imagsav <= 0)  
    { XtWarning("*** Can not create image store!"); exit (-1); } 

 // Realize all widgets now 
 XtRealizeWidget(iw_top);

 // Print opening output lines in iw_scroll 
 if (trace_web) spoutfile(TRUE);
 spout("This is Web;  An interactive display module for SPIDER");
 spout("Output messages appear here");

 sprintf(outstr,"Image ramp: %d...%d  Colors: %d...%d",
         mapgo,mapend,colorgo,colorend);
 spout(outstr);

 // After realizing find window descriptor for the image widget
 iwin  = XtWindow(iw_win);
 //printf("*** iw_win: %d --> iwin: %d \n",iw_win,iwin); 

 // Find window descriptor for the top widget */
 iwtop  = XtWindow(iw_top);
 //printf("*** iw_top: %d --> iwtop: %d \n",iw_top,itop); 

 // Set default background drawing color. Sets both icolorb and icolorxb 
 wicolor(icontx, -icolorb-1000000);   // Background (YES THIS IS OK)
 printf(" Background color:   %#8x \n", ispicol[icolorb]);

 // Set default foreground drawing color. Sets both icolor and icolorx 
 wicolor(icontx,icolor+colorgo);

 /* Set cursor foreground & background color */
 setacursor(icursor,icolorcf,icolorcb); 

 /* Set default initial image position */
 nrit = iwidex;
 nbot = ihighx;

 // Create an icon for web 
 mapdef      = DefaultColormapOfScreen(iscreen);
 XAllocNamedColor(idispl, mapdef, "Gold", &col_def, &rgb_def);
 icon_fore   = col_def.pixel;
 XAllocNamedColor(idispl, mapdef, "Blue", &col_def, &rgb_def);
 icon_back   = col_def.pixel;
 icon_pixmap = XCreatePixmapFromBitmapData(idispl, iwtop, web_icon_bits, 
                 web_icon_width, web_icon_height, icon_fore, icon_back, idepth);

 XStringListToTextProperty(&icon_name,   1, &iconName);
 XStringListToTextProperty(&window_name, 1, &windowName);

 wm_hints.initial_state = NormalState;
 wm_hints.icon_pixmap   = icon_pixmap;
 wm_hints.flags         = StateHint | IconPixmapHint;

 XSetWMProperties(idispl, iwtop, &windowName, &iconName, 
                   NULL, 0, NULL, &wm_hints, NULL);


 // Clear Display window to black
 webclear(NULL,NULL,NULL);

 //query_widget("main  widget",iw_main); 
 //query_widget("menub widget",iw_menub); 
 //query_widget("image widget",iw_win); 


 return iw_main;
}


#ifdef NEVER
  // Have been unable to apply new colormap to existing image window
  // after realization even if I unmanage/manage again. 
  // Have succeeded in altering menu widgets only!! al mar 2011
  // Have exhustively tried following everywhere I could think of
  //       in order to get colormap on OSX and Motif (nothing worked)

  //setamap(map);    NOT USED ANYMORE ???

  xswa.colormap = map;
  XChangeWindowAttributes (idispl, iwtop, CWColormap, &xswa);
  XChangeWindowAttributes (idispl, iwin,  CWColormap, &xswa);

  /* Associate colormap with window */
  XSetWindowColormap(idispl, iwin, map);

  win_list[0] = iwin;
  win_list[1] = iwtop;
  XSetWMColormapWindows(idispl,iwtop,win_list,2);

  XtSetArg(args[0], XmNcolormap, map);      
  XtSetValues(parent, args, 1 );
  printf(" Set colormap:       %#x \n",map); 

  XmChangeColor(iw_menub, icolorxmb) ;

 /* Install colormaps  */
 XInstallColormap(idispl, map);

#endif



#else
// NOT WEB_TRUE

// **************************  web_main  ******************************

 Widget web_main(Widget parent)          // parent = iw_top
 { 
 Arg                   args[MAX_ARGS];   // Arg list 
 int                   n;                // Arg count 

 Widget                iw_exit, iw_quickb;
 Pixmap                icon_pixmap;
 XWMHints              wm_hints;
 Colormap              mapdef;
 unsigned long         icon_fore, icon_back;
 XTextProperty         windowName, iconName;
 XColor                col_def, rgb_def;
 XSetWindowAttributes  xswa;
 Window                win_list[2];
 char                * icon_name   = "WEB";
 char                * window_name = 
  "Web -- A SPIDER image viewer and analyzer       COPYRIGHT (c) 1992-2011 Health Research Inc., Menands, NY";

 int                   icolorxtb,icolorxtf;

 //int screen_num = XDefaultScreen(idispl);
 //printf(" DefaultScreen(idispl):          %d  \n",DefaultScreen(idispl) );
 //printf(" XDefaultScreen(idispl):         %d  \n",XDefaultScreen(idispl) );
 //printf(" DefaultScreenOfDisplay(idispl): %d  \n",DefaultScreenOfDisplay(idispl) );

 // Apply private colormap -------------------------------- 
 //XtSetArg(args[0], XmNcolormap, map);     
 //XtSetValues(parent, args, 1 );
 //XtSetValues(iw_top, args, 1 );
 //XtSetValues(iw_win, args, 1 );
 //printf(" Set colormap:       %#x \n",map); 

 /* Set main window position and size -------------------------*/
 ihighx = HeightOfScreen(iscreen) - 100;
 iwidex = WidthOfScreen(iscreen)  - 100;
 printf(" Screen size:        %d X %d\n",iwidex,ihighx);

 n = 0;
 XtSetArg(args[n], XmNx, 0);                n++;
 XtSetArg(args[n], XmNy, 0);                n++;
 XtSetArg(args[n], XmNwidth,  iwidex);      n++;
 XtSetArg(args[n], XmNheight, ihighx);      n++;

 if ((iw_main = XmCreateForm(parent, "main", args,n)) == 0)
   { XtWarning("*** Web can not open main window!"); exit (-1); }
 XtManageChild(iw_main);

 XtSetArg(args[0], XmNbackground,  &icolorxtb);                       
 XtSetArg(args[1], XmNforeground,  &icolorxtf);
 XtGetValues(iw_main,args,2); 
 printf(" Background:         %d \n",icolorxtb);
 printf(" Foreground:         %d \n",icolorxtf);

 icolorxmb = WhitePixelOfScreen(iscreen);
 icolorxmf = BlackPixelOfScreen(iscreen);
 printf(" White Pixel:        %#8x \n",icolorxmb);      
 printf(" Black Pixel:        %#8x \n",icolorxmf);     
 icolorxmb = 0xEEEEEE;      // jul 2011   

 // Create a menu bar for top line commands ---------------------

 n = 0;           
 Setbgfg(args,     MAX_ARGS, &n);      // Handles BGFG Args
 XtSetArg(args[n], XmNtopAttachment,   XmATTACH_FORM); n++;
 XtSetArg(args[n], XmNleftAttachment,  XmATTACH_FORM); n++;
 XtSetArg(args[n], XmNorientation,     XmHORIZONTAL);  n++;
 Widget iw_left;
 iw_left = XmCreateRowColumn(iw_main, "left", args, n);
 if (iw_left == NULL)
    { XtWarning("*** Can not create left menu widget!"); exit (-1);}
 XtManageChild(iw_left);

 n = 1;   // keep: XmNbackground =  icolorxmb
 XtSetArg(args[n], XmNtopAttachment,  XmATTACH_FORM);  n++;
 XtSetArg(args[n], XmNleftAttachment, XmATTACH_FORM);  n++;
 XtSetArg(args[n], XmNshadowThickness, 0);             n++;
 XtSetArg(args[n], XmNmarginHeight,    7);             n++;
 iw_menub = XmCreateMenuBar(iw_left, "menub", args, n);
 if (iw_menub == NULL)
    { XtWarning("*** Can not create menu bar widget!"); exit (-1);}
 XtManageChild(iw_menub);

 // Create menu items under top menu bar ----------------------- 
 web_com(iw_menub);

 // Create a menu bar for QUICK PICKS selection -------------- QUICK
 n = 1;            // Keeps: XmNbackground = icolorxmb
 XtSetArg(args[n], XmNtopAttachment,   XmATTACH_FORM);   n++;
 XtSetArg(args[n], XmNleftAttachment,  XmATTACH_WIDGET); n++;
 XtSetArg(args[n], XmNleftWidget,      iw_menub);        n++;
 XtSetArg(args[n], XmNrightAttachment, XmATTACH_FORM);   n++;
 XtSetArg(args[n], XmNorientation,     XmHORIZONTAL);    n++;

 iw_quickb = XmCreateRowColumn(iw_main, "quickb", args, n);
 if (iw_quickb == NULL)
    { XtWarning("*** Can not create quick menu widget!"); exit (-1);}
 XtManageChild(iw_quickb);

 // Create menu items under QUICK PICKS 
 web_com2(iw_quickb);




 // Create scrolled label window for error messages etc  ----- MESSAGES
 n = 0;
#ifdef WEB_BGFGO
 XtSetArg(args[n], XmNbackground,         icolorxmb);         n++;      
#endif
 XtSetArg(args[n], XmNscrollingPolicy,    XmAUTOMATIC);       n++;
 XtSetArg(args[n], XmNscrollBarDisplayPolicy, XmSTATIC);      n++;
 XtSetArg(args[n], XmNscrollBarPlacement, XmBOTTOM_RIGHT);    n++;
 XtSetArg(args[n], XmNvisualPolicy,       XmCONSTANT);        n++;
 XtSetArg(args[n], XmNeditMode,           XmMULTI_LINE_EDIT); n++;
 XtSetArg(args[n], XmNeditable,           False);             n++;
 XtSetArg(args[n], XmNleftAttachment,     XmATTACH_FORM);     n++;
 XtSetArg(args[n], XmNrightAttachment,    XmATTACH_FORM);     n++; 
 XtSetArg(args[n], XmNbottomAttachment,   XmATTACH_FORM);     n++;
 XtSetArg(args[n], XmNheight,             90);                n++;

 iw_scroll = XmCreateScrolledText(iw_main, "comscroll", args, n);
 XtManageChild(iw_scroll);

 /* Find parent for the scrolling text window */
 iw_scrollpar = XtParent(iw_scroll);


 // Create spider image widget ------------------------------ IMAGES 

 n = 0;
 XtSetArg(args[n], XmNtopWidget,        iw_menub);          n++;
 XtSetArg(args[n], XmNtopAttachment,    XmATTACH_WIDGET);   n++;
 XtSetArg(args[n], XmNleftAttachment,   XmATTACH_FORM);     n++;
 XtSetArg(args[n], XmNrightAttachment,  XmATTACH_FORM);     n++;
 
 XtSetArg(args[n], XmNbottomWidget,     iw_scrollpar);      n++;
 XtSetArg(args[n], XmNbottomAttachment, XmATTACH_WIDGET);   n++;
 XtSetArg(args[n], XtNcolormap,         map);               n++;

 iw_win = XmCreateDrawingArea(iw_main, "win", args, n);
 XtManageChild(iw_win);


 /* Use expos_cb for redrawing the image widget contents */
 XtAddCallback(iw_win,XmNexposeCallback, (XtCallbackProc) expos_cb,NULL);

 // Find root window for this screen 
 iroot = RootWindowOfScreen(iscreen);
 //printf("*** RootWindowOfScreen: %d\n",iroot); 

 /* Create an offscreen backing store for image & graphics window */
 imagsav = XCreatePixmap(idispl,iroot,iwidex,ihighx,idepth);
 if (imagsav <= 0)  
    { XtWarning("*** Can not create image store!"); exit (-1); } 

 // Realize all widgets now 
 XtRealizeWidget(iw_top);

 // Print opening output lines in iw_scroll 
 if (trace_web) spoutfile(TRUE);
 spout("This is Web;  An interactive display module for SPIDER");
 spout("Output messages appear here");

 sprintf(outstr,"Image ramp: %d...%d  Colors: %d...%d",
         mapgo,mapend,colorgo,colorend);
 spout(outstr);

 // After realizing find window descriptor for the image widget
 iwin  = XtWindow(iw_win);
 //printf("*** iw_win: %d --> iwin: %d \n",iw_win,iwin); 


 // Find window descriptor for the top widget */
 iwtop  = XtWindow(iw_top);
 //printf("*** iw_top: %d --> iwtop: %d \n",iw_top,itop); 

 // Set default drawing color 
 //printf(" colorgo:: %d , icolor: %d\n",colorgo,icolor);
 wicolor(icontx,icolor+colorgo);
 //printf(" colorgo:: %d , icolor: %d\n",colorgo,icolor);

  
 // Set default background color 
 icolorxb = ispicol[icolorb];

 /* Set default cursor and its colors */
 setacursor(icursor,colorgo+icolorcf,colorgo+icolorcb);

 /* Set default initial image position */
 nrit = iwidex;
 nbot = ihighx;

 // Create an icon for web 
 mapdef      = DefaultColormapOfScreen(iscreen);
 XAllocNamedColor(idispl, mapdef, "Gold", &col_def, &rgb_def);
 icon_fore   = col_def.pixel;
 XAllocNamedColor(idispl, mapdef, "Blue", &col_def, &rgb_def);
 icon_back   = col_def.pixel;
 icon_pixmap = XCreatePixmapFromBitmapData(idispl, iwtop, web_icon_bits, 
                 web_icon_width, web_icon_height, icon_fore, icon_back, idepth);

 XStringListToTextProperty(&icon_name,   1, &iconName);
 XStringListToTextProperty(&window_name, 1, &windowName);

 wm_hints.initial_state = NormalState;
 wm_hints.icon_pixmap   = icon_pixmap;
 wm_hints.flags         = StateHint | IconPixmapHint;

 XSetWMProperties(idispl, iwtop, &windowName, &iconName, 
                   NULL, 0, NULL, &wm_hints, NULL);


 // Clear Display window to black
 webclear(NULL,NULL,NULL);

 //query_widget("main  widget",iw_main); 
 //query_widget("menub widget",iw_menub); 
 //query_widget("image widget",iw_win); 


 return iw_main;
}

#ifdef NEVER
  // Have been unable to apply new colormap to existing image window
  // after realization even if I unmanage/manage again. 
  // Have succeeded in altering menu widgets only!! al mar 2011
  // Have exhustively tried following everywhere I could think of
  //       in order to get colormap on OSX and Motif (nothing worked)

  //setamap(map);    NOT USED ANYMORE ???

  xswa.colormap = map;
  XChangeWindowAttributes (idispl, iwtop, CWColormap, &xswa);
  XChangeWindowAttributes (idispl, iwin,  CWColormap, &xswa);

  /* Associate colormap with window */
  XSetWindowColormap(idispl, iwin, map);

  win_list[0] = iwin;
  win_list[1] = iwtop;
  XSetWMColormapWindows(idispl,iwtop,win_list,2);

  XtSetArg(args[0], XmNcolormap, map);      
  XtSetValues(parent, args, 1 );
  printf(" Set colormap:       %#x \n",map); 

  XmChangeColor(iw_menub, icolorxmb) ;

 /* Install colormaps  */
 XInstallColormap(idispl, map);

#endif

#endif



/***********************  query_widget  ************************/

 void query_widget(char * label, Widget iw_widget)
 { 
 Arg           args[3];               /* Arg list */
 Colormap      mapt;
 unsigned long icolorxtb,icolorxtf;

 XtSetArg(args[0], XmNcolormap,    &mapt);                       
 XtSetArg(args[1], XmNbackground,  &icolorxtb);                       
 XtSetArg(args[2], XmNforeground,  &icolorxtf);
                       
 XtGetValues(iw_widget,args,3); 

 printf(" %s  -- Colormap:%#x,  Background: %d  Foreground: %d\n",
         label, mapt,icolorxtb,icolorxtf);
 }
