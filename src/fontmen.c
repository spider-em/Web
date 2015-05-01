
/*
C++************************************************************************
C
C
C **********************************************************************
C *  AUTHOR:  Lekan Odesanya                                               *
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
C    FONTMEN
C
C    PURPOSE:         SPECIFIES FONT SELECTION
C
C
C***********************************************************************
*/

#include "common.h"
#include <Xm/List.h>

#define MAX_ITEMS 12


 /* external function prototypes */

 extern Widget wid_rowcol   (Widget, char , int, int);
 extern Widget wid_labelg   (Widget, Widget, char *, int, int);
 extern Widget wid_dialog   (Widget, Widget, char *, int, int);
 extern Widget wid_stdbut   (Widget, Widget, Widget*, Widget*, Widget*, 
                             char *, void (*)(), void (*)(), void (*)(),
                             void (*)());
 extern void   spout        (char *);
 extern void   fin_cb       (Widget, caddr_t, caddr_t);


 /* internal function prototypes */
 void          fontmen_buta(Widget, caddr_t, caddr_t);
 void          fontmen_sel(Widget, int, XmListCallbackStruct *);
 

 /* internal common variables */
 static Widget   iw_labels[3], iw_lab[3], iw_fontmen = (Widget)0;
 static int      tf, wt, ot, sz;

 static XFontStruct         * fontptr, * prevptr = NULL;
 static XmFontList            list, prevlist = NULL;

 static char     * preview[] = { "1234567890","abcdefghijklmnopqrstuvwxyz",
                                 "ABCDEFGHIJKLMNOPQRSTUVWXYZ" };
 static char     * family[] = { "courier", "helvetica", "times",
                                "new century", "avant garde", "palatino", 
                                "charter", "lucida", "utopia", "haeberli",
                                "rock", "symbol" };
 static char     * weight[] = { "medium", "bold", "book", "demi" };
 static char     * orientation[] = { "r", "o", "i"};
 static char     * size[] = { "80", "100", "120", "140", "180", "240", "360"};
/*
                              "480", "720", "960" };
*/
 /********************   fontmen   ****************************/

void fontmen(Widget iw_caller, caddr_t data, caddr_t call_data)
{ 
 Widget iw_rcvt, iw_rcht, iw_rch, iw_rcv;
 Widget iw_pushc, iw_pusha,iw_dums, iw_list;
 XmString * cmpdstr;
 int    n;
 Arg    args[5];


 if (iw_fontmen <= (Widget)0)
  {   /* create font menu widget first */

   iw_fontmen  = wid_dialog(iw_win, 0, "Font options", -1, -1);

    iw_rcvt     = wid_rowcol(iw_fontmen, 'v', -1, -1);
      iw_rcht     = wid_rowcol(iw_rcvt, 'h', -1, -1);

      cmpdstr = (XmString *) XtMalloc(sizeof(XmString) * MAX_ITEMS);

       /* create vertical rowcol for typeFace column  ----------------  */
       iw_rcv  = wid_rowcol(iw_rcht, 'v', -1, -1);

         /* create horizontal rowcol for typeFace label and its echo ----  */
         iw_rch  = wid_rowcol(iw_rcv, 'h', -1, -1);

           /* create label typeface --------------------- */
           (void) wid_labelg(iw_rch, 0, "TypeFace:", -1, -1);

           /* create label typeface echo --------------------- */
           iw_labels[0]  = wid_labelg(iw_rch, 0,"Courier", -1, -1);

         /* create list widget for typeface  --------------------------  */
          cmpdstr[0] = XmStringCreate("Courier", XmSTRING_DEFAULT_CHARSET);
          cmpdstr[1] = XmStringCreate("Helvetica", XmSTRING_DEFAULT_CHARSET);
          cmpdstr[2] = XmStringCreate("Times", XmSTRING_DEFAULT_CHARSET);
          cmpdstr[3] = XmStringCreate("New Century", 
                                XmSTRING_DEFAULT_CHARSET);
          cmpdstr[4] = XmStringCreate("Avant Garde", XmSTRING_DEFAULT_CHARSET);
          cmpdstr[5] = XmStringCreate("Palatino", XmSTRING_DEFAULT_CHARSET);
          cmpdstr[6] = XmStringCreate("Charter", XmSTRING_DEFAULT_CHARSET);
          cmpdstr[7] = XmStringCreate("Lucida", XmSTRING_DEFAULT_CHARSET);
          cmpdstr[8] = XmStringCreate("Utopia", XmSTRING_DEFAULT_CHARSET);
          cmpdstr[9] = XmStringCreate("Haeberli", XmSTRING_DEFAULT_CHARSET);
          cmpdstr[10] = XmStringCreate("Rock", XmSTRING_DEFAULT_CHARSET);
          cmpdstr[11] = XmStringCreate("Symbol", XmSTRING_DEFAULT_CHARSET);

          n = 0;
#ifdef WEB_BGFGO
          XtSetArg( args[n], XmNbackground,  icolorxmb); n++;
#endif
          XtSetArg( args[n], XmNitems, cmpdstr);         n++;
          XtSetArg( args[n], XmNitemCount, 12);          n++;
          XtSetArg( args[n], XmNvisibleItemCount, 4);    n++;
          iw_list = XmCreateScrolledList(iw_rcv, "typefacelist",args,n);
          XtManageChild(iw_list);
          XtAddCallback(iw_list, XmNbrowseSelectionCallback, 
                  (XtCallbackProc) fontmen_sel, (XtPointer) 1);
          tf = 0;
          XmListSelectItem(iw_list, cmpdstr[0], False); 

          /* create vertical rowcol for style column  ----------------  */
          iw_rcv  = wid_rowcol(iw_rcht, 'v', -1, -1);

          /* Create horizontal rowcol for style label and its echo ----  */
          iw_rch  = wid_rowcol(iw_rcv, 'h', -1, -1);

          /* Create label style --------------------- */
          (void) wid_labelg(iw_rch, 0, "Style:", -1, -1);

          /* Create label style echo --------------------- */
          iw_labels[1]  = wid_labelg(iw_rch, 0,"Plain Upright", -1, -1);

          /* Create list widget for style  --------------------------  */
          cmpdstr[0] = XmStringCreate("Plain Upright", 
                                           XmSTRING_DEFAULT_CHARSET);
          cmpdstr[1] = XmStringCreate("Bold Upright", 
                                           XmSTRING_DEFAULT_CHARSET);
          cmpdstr[2] = XmStringCreate("Plain Italic", 
                                           XmSTRING_DEFAULT_CHARSET);
          cmpdstr[3] = XmStringCreate("Bold Italic", 
                                           XmSTRING_DEFAULT_CHARSET);
          n = 0;
#ifdef WEB_BGFGO
          XtSetArg( args[n], XmNbackground,  icolorxmb); n++;
#endif
          XtSetArg( args[n], XmNitems, cmpdstr); n++;
          XtSetArg( args[n], XmNitemCount, 4); n++;
          XtSetArg( args[n], XmNvisibleItemCount, 4); n++;
          iw_list = XmCreateScrolledList(iw_rcv, "stylelist", args, n);
          XtManageChild(iw_list);

          XtAddCallback(iw_list, XmNbrowseSelectionCallback, 
                     (XtCallbackProc) fontmen_sel, (XtPointer) 2);
          wt = ot = 0;
          XmListSelectItem(iw_list, cmpdstr[0], False); 
 

          /* create vertical rowcol for size column  ----------------  */
          iw_rcv  = wid_rowcol(iw_rcht, 'v', -1, -1);

          /* create horizontal rowcol for size label and its echo ----  */
          iw_rch  = wid_rowcol(iw_rcv, 'h', -1, -1);

          /* create label for size --------------------- */
          (void) wid_labelg(iw_rch, 0, "Size:", -1, -1);

          /* create label for size echo --------------------- */
          iw_labels[2]  = wid_labelg(iw_rch, 0,"12pt", -1, -1);

          /* create list widget for size  --------------------------  */
          cmpdstr[0] = XmStringCreate(" 8pt", XmSTRING_DEFAULT_CHARSET);
          cmpdstr[1] = XmStringCreate("10pt", XmSTRING_DEFAULT_CHARSET);
          cmpdstr[2] = XmStringCreate("12pt", XmSTRING_DEFAULT_CHARSET);
          cmpdstr[3] = XmStringCreate("14pt", XmSTRING_DEFAULT_CHARSET);
          cmpdstr[4] = XmStringCreate("18pt", XmSTRING_DEFAULT_CHARSET);
          cmpdstr[5] = XmStringCreate("24pt", XmSTRING_DEFAULT_CHARSET);
          cmpdstr[6] = XmStringCreate("36pt", XmSTRING_DEFAULT_CHARSET);
/*
          cmpdstr[7] = XmStringCreate("48pt", XmSTRING_DEFAULT_CHARSET);
          cmpdstr[8] = XmStringCreate("72pt", XmSTRING_DEFAULT_CHARSET);
          cmpdstr[9] = XmStringCreate("96pt", XmSTRING_DEFAULT_CHARSET);
*/
          n = 0;
#ifdef WEB_BGFGO
          XtSetArg( args[n], XmNbackground,  icolorxmb); n++;
#endif
          XtSetArg( args[n], XmNitems, cmpdstr);      n++;
          XtSetArg( args[n], XmNitemCount, 7);        n++;
          XtSetArg( args[n], XmNvisibleItemCount, 4); n++;
          iw_list = XmCreateScrolledList(iw_rcv, "sizelist", args, n);
          XtManageChild(iw_list);

          XtAddCallback(iw_list, XmNbrowseSelectionCallback,  
                  (XtCallbackProc) fontmen_sel, (XtPointer) 3);
          sz = 2;
          XmListSelectItem(iw_list, cmpdstr[2], False); 
 
          XtFree((char *) cmpdstr);

     /* create label for numbers --------------------- */
     iw_lab[0] =  wid_labelg(iw_rcvt, 0, preview[0], -1, -1);

     /* create label for lower case letters --------------------- */
     iw_lab[1] = wid_labelg(iw_rcvt, 0, preview[1], -1, -1);

     /* create label for upper case letters --------------------- */
     iw_lab[2] = wid_labelg(iw_rcvt, 0, preview[2], -1, -1);

     fontmen_sel(NULL, 0, NULL);

     /* Create box for apply ----------------------------------- apply */
     iw_dums = wid_stdbut(iw_rcvt, iw_fontmen, 
                        &iw_dums, &iw_pushc, &iw_pusha,  "CA",
                        fin_cb, fin_cb, fontmen_buta, NULL);
  }

  XtManageChild(iw_fontmen);

 }

void fontmen_sel(Widget iw_temp, int data, XmListCallbackStruct * call_data)
 {
   char            fontname[120], output[80];
   int             n, wtt, pos;
   Arg             args[2];

 if(data)
  {
   /* Echo the selection in the label widget above list  */
    n = 0;
    XtSetArg( args[n], XmNlabelString, call_data->item ); n++;
    XtSetValues( iw_labels[data-1], args, n );

    pos = call_data->item_position;

    switch( data )
     {
       case 1:   /*  TypeFace list  */
         tf = pos - 1;
         break;

       case 2:   /*  Style list     */
          wt = 0;    /*  medium  */
          if(pos != 1 && pos != 3) wt = 1;   /* bold  */
          ot = 0;  /*  upright */
          if(pos != 1 && pos != 2) ot = 1; /* slanted or italic */
         break;

       case 3:   /*  Size list      */
         sz = pos - 1;
         break;
          
       default:  
         break;
     }   
  }

   wtt = wt;
   if(tf == 4)  wtt += 2;  /*  for Avant Garde only  */

/* build the fontname from the selections  */
   sprintf(fontname,"*-*%s*-%s-%s-*%s*",family[tf],weight[wtt],
              orientation[ot], size[sz] );

   if( (fontptr = XLoadQueryFont(idispl, fontname)) == NULL )
     if( ot == 1) /* try using "i" instead of "o" in the fontname  */
      {
         sprintf(fontname,"*-*%s*-%s-%s-*%s*",family[tf],weight[wtt],
              orientation[2], size[sz] );
         if( (fontptr = XLoadQueryFont(idispl, fontname)) == NULL )
          {
            sprintf(output,"Cannot load font: %s\n",fontname);
            spout(output);
            fontptr = prevptr;
            return;
          }
      }

   if( (list = XmFontListCreate(fontptr, XmSTRING_DEFAULT_CHARSET)) == NULL)
    {
      sprintf(output,"Cannot load font: %s\n",fontname);
      spout(output); 
      fontptr = prevptr;
      return;
    }

   n = 0;
   XtSetArg( args[n], XmNfontList, list);  n++;
   XtSetValues(iw_lab[0], args, n);

   n = 0;
   XtSetArg( args[n], XmNfontList, list);  n++;
   XtSetValues(iw_lab[1], args, n);
 
   n = 0;
   XtSetArg( args[n], XmNfontList, list);  n++;
   XtSetValues(iw_lab[2], args, n);

   if( prevptr ){ XFreeFont(idispl, prevptr); prevptr = NULL; }
   if( prevlist ) { XmFontListFree( prevlist ); prevlist = NULL; }

   prevptr = fontptr;
   prevlist = list;

 }

void fontmen_buta(Widget iw_temp, caddr_t data, caddr_t call_data)
   {
   XtUnmanageChild(iw_fontmen);
   XSetFont( idispl, icontx, fontptr->fid );
   }

