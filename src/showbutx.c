/*$Header: /usr8/web/src/RCS/showbutx.c,v 1.17 2015/09/10 13:17:48 leith Exp $*/

/*
 ***********************************************************************
 *                                                                     *
 * showbutx.c                                                          *
 *              Added showbuts                   Jun 2011 ArDean Leith *
 *              Added showbuts_str               Sep 2015 ArDean Leith *
 *                                                                     *
 ***********************************************************************
 C=* FROM: WEB - VISUALIZER FOR SPIDER MODULAR IMAGE PROCESSING SYSTEM *
 C=* Copyright (C) 1992-2015  Health Research Inc.                     *
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
 * PURPOSE:	Opens a message box displaying the mapping of the 
 *              mouse buttons
 *
 * RETURNS:     void
 * 
 * PARAMETERS:	char *  but1	       Text description of button 1
 *		char *  but2	       Text description of button 2
 *		char *  but3	       Text description of button 3
 *		int	unmanage       Unmanage the window if TRUE
 *
 ***********************************************************************
*/

#include <Xm/LabelG.h>
#include <Xm/BulletinB.h>
#include "common.h"
#include "routines.h"
#include <strings.h>

#define MAX_MESSAGE_LENGTH 240

/* External global variables */
extern Widget     iw_win;    /* Image window widget  */
extern Display *  idispl;

/* Internal file variables */
static Widget     iw_mess   = (Widget) 0 ;  /* Dialog widget   */
static Widget     iw_labelg = (Widget) 0 ;  /* Label widget    */

// Internal function prototypes
void              ForceUpdate (Widget w);   // unused
void              showbuts_str(Widget * iw_buts_ptr, Widget * iw_labelg_str_ptr, 
                       char *but1, char *but2, char *but3, int manage);

/********************** showbuts_str ***************************/
// Can redefine strings within a widget

void showbuts_str(Widget * iw_buts_ptr, Widget * iw_labelg_str_ptr, 
                  char *but1, char *but2, char *but3, int manage)
 {   
 unsigned long   ilen;
 char            message[MAX_MESSAGE_LENGTH];   
 Arg             args[2];      /* Arg list */
 XmString        str_label;    /* String label */
   
 if (* iw_buts_ptr == 0 ) 
    {
    * iw_buts_ptr = XmCreateBulletinBoardDialog(iw_win,"iw_buts_ptr",args,0);

    XtSetArg(args[0], XmNalignment, XmALIGNMENT_BEGINNING);
    * iw_labelg_str_ptr = XmCreateLabelGadget(*iw_buts_ptr,"iw_labelg_str_ptr",args,1);

  //  XtManageChild(* iw_labelg_str_ptr);

  //printf(" iw_buts_ptr: %d  iw_labelg_str_ptr: %d \n",*iw_buts_ptr, *iw_labelg_str_ptr);
    }

 /* Error trap kludge for alpha bug on particle picking  buggy */
 if (but1 != NULL) ilen = strlen(but1);
 if (but2 != NULL) ilen += strlen(but2);
 if (but3 != NULL) ilen += strlen(but3);

 if ((ilen + 57) >=  MAX_MESSAGE_LENGTH)
    {
    message[MAX_MESSAGE_LENGTH - 1] = '\0';
    printf("*** Showbuts got an overlength string: %d \n",ilen);
    printf("*** String:%s \n",message); 
    XBell(idispl,50);
    }

 /* Set the message string */
 strcpy(message, "BUTTON   left:    ");
 strcat(message, but1);
 strcat(message, "\n");

 if (but2 != NULL) 
    {
    strcat(message, "         center:  ");
    strcat(message, but2);
    strcat(message, "\n");
    }

 if (but3 != NULL)
    {
    strcat(message, "         right:   ");
    strcat(message, but3);
    }

 //  Note: XmStringCreateLtoR handles newline char oK) if this used
 //        XmStringCreate you woould have to solve newline problem!

 str_label = XmStringCreateLtoR(message,XmSTRING_DEFAULT_CHARSET);
 XtSetArg(args[0], XmNlabelString, str_label); 
 XtSetValues(* iw_labelg_str_ptr,args,1);

 /* Free the String (not doing this may result in a memory leak) */
 XmStringFree(str_label);
 XtManageChild(* iw_labelg_str_ptr);

 if (manage) 
    {XtManageChild  (* iw_buts_ptr); }
 else
    {XtUnmanageChild(* iw_buts_ptr);}

 //XFlush(idispl);

 return;
 }



/********************** showbutx ***************************/

void showbutx(char *but1, char *but2, char *but3, int unmanage)
 {   
 unsigned long   ilen;
 char            message[MAX_MESSAGE_LENGTH];   
 Arg             args[3];     /* Arg list */
 XmString        str_label;   /* String label */
 int             n;
 
 if (unmanage && iw_mess)
    { XtUnmanageChild(iw_mess); }

 else 
    {
    /* Error trap kludge for alpha bug on particle picking  buggy */
    if (but1 != NULL) ilen = strlen(but1);
    if (but2 != NULL) ilen += strlen(but2);
    if (but3 != NULL) ilen += strlen(but3);

    if ((ilen + 57) >=  MAX_MESSAGE_LENGTH)
       {
       message[MAX_MESSAGE_LENGTH - 1] = '\0';
       printf("*** Showbutx got an overlength string: %d \n",ilen);
       printf("*** String:%s \n",message); XBell(idispl,50);
       XBell(idispl,50);
       }

    /* Set the message string */
    strcpy(message, "BUTTON   left:    ");
    strcat(message, but1);
    strcat(message, "\n");

    if (but2 != NULL) 
       {
       strcat(message, "         center:  ");
       strcat(message, but2);
       strcat(message, "\n");
       }

    if (but3 != NULL)
       {
       strcat(message, "         right:   ");
       strcat(message, but3);
       }
    //printf(" draw 0 \n");

    if (iw_mess == (Widget) 0) 
       {  /* Must create widget the first time it is used */
       //printf(" creating iw_mess 11 \n");

       n = 0;
       Setbgfg(args, 3, &n);  // Handles BGFG Args
       XtSetArg(args[n], XmNdefaultPosition, False);    n++;    
       iw_mess   = XmCreateBulletinBoardDialog(iw_win,"iw_mess",args,n);

       XtSetArg(args[n-1], XmNalignment, XmALIGNMENT_BEGINNING); 
       iw_labelg = XmCreateLabelGadget(iw_mess,"iw_labelg",args,n);
       XtManageChild(iw_labelg);
       }

    /* Note: XmStringCreateLtoR handles newline char oK) if this
       changed to XmStringCreate you have to solve newline problem  */

    str_label = XmStringCreateLtoR(message,XmSTRING_DEFAULT_CHARSET);
    XtSetArg(args[0], XmNlabelString, str_label); 
    XtSetValues(iw_labelg,args,1);

    /* Free the String (not doing this may result in a memory leak) */
    XmStringFree(str_label);

    XtManageChild(iw_mess);
    // ForceUpdate(iw_mess);
    }

  XFlush(idispl);
  }


/********************** showbuts ***************************/

Widget showbuts(Widget iw_buts, 
        char *but1, char *but2, char *but3, int manage)
 {   
 unsigned long   ilen;
 char            message[MAX_MESSAGE_LENGTH];   
 Arg             args[2];      /* Arg list */
 XmString        str_label;    /* String label */
 Widget          iw_labelgt;   /* Label widget    */
   
 if (iw_buts == 0 ) 
    {
    /* Error trap kludge for alpha bug on particle picking  buggy */
    if (but1 != NULL) ilen = strlen(but1);
    if (but2 != NULL) ilen += strlen(but2);
    if (but3 != NULL) ilen += strlen(but3);

    if ((ilen + 57) >=  MAX_MESSAGE_LENGTH)
       {
       message[MAX_MESSAGE_LENGTH - 1] = '\0';
       printf("*** Showbuts got an overlength string: %d \n",ilen);
       printf("*** String:%s \n",message); XBell(idispl,50);
       XBell(idispl,50);
       }

    /* Set the message string */
    strcpy(message, "BUTTON   left:    ");
    strcat(message, but1);
    strcat(message, "\n");

    if (but2 != NULL) 
       {
       strcat(message, "         center:  ");
       strcat(message, but2);
       strcat(message, "\n");
       }

    if (but3 != NULL)
       {
       strcat(message, "         right:   ");
       strcat(message, but3);
       }

    iw_buts = XmCreateBulletinBoardDialog(iw_win,"iw_buts",args,0);

    XtSetArg(args[0], XmNalignment, XmALIGNMENT_BEGINNING); 
    iw_labelgt = XmCreateLabelGadget(iw_buts,"iw_labelgt",args,1);
    XtManageChild(iw_labelgt);

    /* Note: XmStringCreateLtoR handles newline char oK) if this
       changed to XmStringCreate you will have to solve newline problem  */

    str_label = XmStringCreateLtoR(message,XmSTRING_DEFAULT_CHARSET);
    XtSetArg(args[0], XmNlabelString, str_label); 
    XtSetValues(iw_labelgt,args,1);

    /* Free the String (not doing this may result in a memory leak) */
    XmStringFree(str_label);
    }

 if (manage) XtManageChild(iw_buts);
 
 XFlush(idispl);

 return iw_buts;
 }


/********************** ForceUpdate ******** unused***********/

    /* ForceUpdate() -- a superset of XmUpdateDisplay() that ensures
    ** that a window's contents are visible before returning. 
    ** The monitoring of window states is necessary because an attempt to
    ** map a window is subject to the whim of the window manager, which can
    ** introduce a significant delay before the window is actually mapped
    ** and exposed. This function is intended to be called after XtPopup(), 
    ** XtManageChild() or XMapRaised(). Don't use it in other situations
    ** as it may sit and process other unrelated events until the widget
    ** becomes visible.
    */

    /* The parameter widget must be visible before the function returns */
    void ForceUpdate (Widget w)
        {
        Widget             diashell, topshell;
        Window             diawindow, topwindow;
        XtAppContext       cxt = XtWidgetToApplicationContext (w);
        Display            *dpy;
        XWindowAttributes  xwa;
        XEvent             event;
        /* Locate the shell we are interested in */
        for (diashell = w; !XtIsShell (diashell);
            diashell = XtParent (diashell));
        /* Locate its primary window's shell (which may be the same) */
        for (topshell = diashell; !XtIsTopLevelShell (topshell);
            topshell = XtParent (topshell));
        /* If the dialog shell (or its primary shell window) is
        ** not realized, don't bother... nothing can possibly happen.
        */
        if (XtIsRealized (diashell) && XtIsRealized (topshell)) {
            dpy = XtDisplay (topshell);
            diawindow = XtWindow (diashell);
            topwindow = XtWindow (topshell);

            /* Wait for the dialog to be mapped.
            ** It's guaranteed to become so
            */
            while (XGetWindowAttributes (dpy, diawindow, &xwa) &&
                                        xwa.map_state != IsViewable) {
                /*...if the primary is (or becomes) unviewable or
                ** unmapped, it's probably iconic, and nothing will happen.
                */
                if (XGetWindowAttributes (dpy, topwindow, &xwa) &&
                                        xwa.map_state != IsViewable)
                    break;
                /* we are guaranteed there will be an event of some kind. */
                XtAppNextEvent (cxt, &event);
                XtDispatchEvent (&event);
            }
        }
        /* The next XSync() will get an expose event. */
        XmUpdateDisplay (topshell);
        }

