/*
*****************************************************************************
** comapmen3.c		8/93
**
** Purpose: Display menu for saving image numbers in doc file.
**
**
** WID_COMAPMEN3.FOR transported to UNIX
**
C **********************************************************************
** *   AUTHOR:  Mahieddine Ladjadj                                          *
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
*****************************************************************************
*/

#include <Xm/Text.h>
#include "common.h"
#include "std.h"
#include "routines.h"
#include "comap.h"

  char   docnammo[81] = "docimg001";
  int    itxrego, ityrego, labrego;

  static void   comapmen3_buta (Widget, void *, void *);

  /* internal variables */
  static Widget iw_comapmen3= (Widget) 0, iw_doc, iw_xreg, 
                iw_yreg, iw_labreg;

/********************** comapmen3 ***********************************/

void comapmen3(Widget iw_t, void * itag, void * callpar)
{
  Widget iw_rowcol, iw_dum, iw_buts,iw_buta, iw_butc;
  char cval[5];
 
  if (iw_comapmen3 == (Widget) 0)
      {
      /* create a dialog box */
      iw_comapmen3 = wid_dialog(iw_win,0,"Saving Image options",20,20);
      iw_rowcol = wid_rowcol(iw_comapmen3,'v',-1,-1);

      /* set up variables in the dialog widget */
      itxrego = 1;
      ityrego = 2;
      labrego  = 3;

      /* create a text box for doc file name input */
      iw_doc =  wid_textboxb(iw_rowcol,(Widget) 0,
                             "New Doc. File:", docnammo,20);
      /* create a box for ixrego input */
      sprintf(cval,"%4d",itxrego);
      iw_xreg = wid_textboxb(iw_rowcol,(Widget) 0,"        X Reg:",cval,4);

      /* create a box for iyrego input */
      sprintf(cval,"%4d",ityrego);
      iw_yreg = wid_textboxb(iw_rowcol,(Widget) 0,"        Y Reg:",cval,4);

      /* create a box for labrego input */
      sprintf(cval,"%4d",labrego);
      iw_labreg = wid_textboxb(iw_rowcol,(Widget) 0,"    Label Reg:",cval,4);

      /* create a push button for stop, cancel & apply */
      iw_dum = wid_stdbut(iw_rowcol,iw_comapmen3,
                          &iw_buts,&iw_butc,&iw_buta,"CA",
                          fin_cb,fin_cb,comapmen3_buta, NULL);
      }
  else
      { /* doc file name is incremented between calls. redraw it */
      iw_doc = wid_textboxb(iw_rowcol,iw_doc, "New Doc. File:", 
                             docnammo,20);
      }

  XtManageChild(iw_comapmen3);

  } /* end of comapmen3 function */

/*******************  comapmen3_buta ******************************/

void comapmen3_buta(Widget iw_t, void * itag, void * callpar)
  {
  char  outstr[80];
  char *string;

  /* get docnammo from text widget */
  string = XmTextGetString(iw_doc);
  if (strlen(string) == 0)
    {
      XtFree(string);
      spout(" *** WARNING: no doc file name: docnammo ");
      return;
    }
  else
    {
      strcpy(docnammo,string);
      XtFree(string);
    }

  /* get x register */
  string = XmTextGetString(iw_xreg);
  sscanf(string,"%d",&itxrego);
  XtFree(string);

  /* get y register */
  string = XmTextGetString(iw_yreg);
  sscanf(string,"%d",&ityrego);
  XtFree(string);

  /* get label register */
  string = XmTextGetString(iw_labreg);
  sscanf(string,"%d",&labrego);
  XtFree(string);

  if (itxrego == ityrego || itxrego == labrego || ityrego == labrego)
     {sprintf(outstr," WARNING: USING SAME X & Y or LABEL REGISTERS %d %d %d", 
              itxrego, ityrego, labrego); spout(outstr);}

  /* remove menu */
  XtUnmanageChild(iw_comapmen3);

  /* start comap doc saving routine */
  comapsav(1);

} /* end of comapmen3_buta function */
