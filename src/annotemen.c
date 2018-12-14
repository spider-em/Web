
/*$Header: /usr16/software/web/src/RCS/annotemen.c,v 1.6 2018/12/07 17:03:30 leith Exp $*/

/***********************************************************************
 * annotemen.c                      7/93
 *
 * ported WID_ANNOTEMEN.FOR (VAX) to unix.
 *
 ***********************************************************************
 * author : Mahieddine Ladjadj
 C=* FROM: WEB - VISUALIZER FOR SPIDER MODULAR IMAGE PROCESSING SYSTEM *
 C=* Copyright (C) 1992-2018  Health Research Inc.                     *
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
 * annotemen()
 *
 * PURPOSE:    displays wid_annote menu.
 *
 ***********************************************************************
*/

#include <Xm/Text.h>
#include "common.h"
#include "routines.h"
#include "annote.h"

  /* internal function prototypes */
  void   annotemen_buta       (Widget, XtPointer, XtPointer);
   
  /* internal variables */
  static Widget   iw_annotemen = (Widget) 0;
  static Widget   iw_fdoc, iw_xregt, iw_yregt, iw_labreg, iw_tkey1,
           iw_tkey2, iw_rad;
  int      ix_reg, iy_reg, lab_reg, tkey1, tkey2, iarad ;

/*********************  annotemen ************************************/

void annotemen(void)
  {
  Widget   iw_buta, iw_butc, iw_buts;
  Widget   iw_rowcol;
  char     cval[5];


  if (iw_annotemen == (Widget) 0)
     /* annote menu widget does not exist, create it */
     {
     /* create a dialog box */
     iw_annotemen = wid_dialog(iw_win,0,"Annote options",-1,-1);
     iw_rowcol    = wid_rowcol(iw_annotemen,'v',-1,-1);
   
     /* set up rows and columns in the dialog widget */
     ix_reg     = 1;
     iy_reg     = 2;
     lab_reg    = 0;
     iarad      = 5;
     tkey1      = 1;
     tkey2      = 2000;

    /* create a text box for doc file name input */
    iw_fdoc = wid_textboxb(iw_rowcol,0, "Doc. File:   ", docnam,20);

    /* create a box for first key # input */
    sprintf(cval,"%4d",tkey1);
    iw_tkey1 = wid_textboxb(iw_rowcol,0,"Starting Key:",cval,4);

    /* create a box for last key # input */
    sprintf(cval,"%4d",tkey2);
    iw_tkey2 = wid_textboxb(iw_rowcol,0,"Ending Key:  ",cval,4);

    /* create a box for ix_reg input */
    sprintf(cval,"%4d",ix_reg);
    iw_xregt = wid_textboxb(iw_rowcol,0,"X reg:       ",cval,4);

    /* create a box for iy_reg input */
    sprintf(cval,"%4d",iy_reg);
    iw_yregt = wid_textboxb(iw_rowcol,0,"Y reg:       ",cval,4);

    /* create a box for lab_reg input */
    sprintf(cval,"%4d",lab_reg);
    iw_labreg = wid_textboxb(iw_rowcol,0,"Label Reg:   ",cval,4);

    /* create a box for marker radius input */
    sprintf(cval,"%4d",iarad );
    iw_rad =   wid_textboxb(iw_rowcol,0,"Marker Radius",cval,4);

    /* create a push button for stop, cancel & apply */
    wid_stdbut(iw_rowcol,iw_annotemen,
               &iw_buts, &iw_butc, &iw_buta,"SCA",
               fin_cb,fin_cb,annotemen_buta,NULL);
    }
  XtManageChild(iw_annotemen);
  return;
  }

 /*******************  annotemen_buta ******************************/

 void annotemen_buta(Widget iw_t, XtPointer itag, XtPointer callpar)
  {
  char  outstr[80];
  char * string;

  /* get docnam from text widget */
  string = XmTextGetString(iw_fdoc);
  if (strlen(string) == 0)
     {
     if (string) XtFree(string);
     spout("*** No doc. file name entered");
     return;
     }
  else
     {    /* probably got a valid doc file name */
     strcpy(docnam,string);
     if (string) XtFree(string);
     }

  /* get first key number */
  string = XmTextGetString(iw_tkey1);
  tkey1  = atoi(string);
  if (string) XtFree(string);

  /* get last key number */
  string = XmTextGetString(iw_tkey2);
  tkey2  = atoi(string);
  if (string) XtFree(string);

  /* get x register */
  string = XmTextGetString(iw_xregt);
  ix_reg = atoi(string);
  if (string) XtFree(string);

  /* get y register */
  string = XmTextGetString(iw_yregt);
  iy_reg = atoi(string);
  if (string) XtFree(string);

  /* get label register */
  string = XmTextGetString(iw_labreg);
  lab_reg = atoi(string);
  if (string) XtFree(string);

  /* get radius */
  string = XmTextGetString(iw_rad);
  iarad  = atoi(string);
  if (string) XtFree(string);

  if( tkey2 < tkey1)
     {
     sprintf(outstr,"*** Key 2= %d < key1= %d \n", ix_reg, iy_reg);
     spout(outstr);

     /* redo it */
     XtUnmanageChild(iw_annotemen);
     annotemen();
     return;
     }
  if (ix_reg == iy_reg || ix_reg == lab_reg || iy_reg == lab_reg)
     {
     sprintf(outstr,"WARNING: Using same registers: %d %d %d\n", 
              ix_reg, iy_reg, lab_reg);
     spout(outstr);
     }

  XtUnmanageChild(iw_annotemen);

  /* start annote program */
  annote(docnam);           
  return;
  }
