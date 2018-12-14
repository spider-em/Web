/*
************************************************************************
**  comapmen.c
**
**  WID_COMAPMEN.FOR (VAX) ported to UNIX   7/28/93 Mahieddine Ladjadj
**  Put itxreg & iytreg back in             Oct 99  ArDean Leith
**  Altered defaults for testing            Nov 07  ArDean Leith
**
************************************************************************
**    AUTHOR:  Mahieddine Ladjadj                                      *
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
************************************************************************
**
**    PURPOSE:      PUTS UP MENU FOR COMAP PARAMETERS
**
************************************************************************
*
*                  comapmen
*                      |
*                     \|/
*                      |
*                    comap
*                      |
*                     \|/
*                      |
*                  corr_mask
*                      |
*                     \|/
*                      |
*   |------->>---- comapmen2
*   |                   |
*   |                  \|/
*   |                   |
*   |     -----<<---------------------->>------------------->>---------
*   |     |                   |                   |                   |
*   |    \|/                 \|/                 \|/                 \|/
*   |     |                   |                   |                   |
*   | comapmen2_buta    comapmen2_buts        comapmen5           comapmen3
*   |     |                                       |                   |
*   |    \|/                                     \|/                 \|/
*   |     |                                       |                   |
*   |     |                                  comapmen5_buta     comapmen3_buta
*   |     |                                       |                   |
*   |    \|/                                     \|/                 \|/
*   |     |                                       |                   |
*   |     |                                       |                   |
*   -- comapsav  ----------------<<-------------------------<<---------                      
*
*
*/

#include <Xm/Text.h>
#include <Xm/ToggleBG.h>
#include "common.h"
#include "routines.h"
#include "comap.h"

  /* function prototypes */
  static void  comapmen_buta (Widget, void *, void *);

  /* externally used variables */
  int    itxreg, ityreg, labreg, itrad, sayno, key1, key2;
  float  stndrt, rangel, rangeu;
  char   filnamm[81],docxnam[81];

  /* internal variables */
  static Widget iw_comapmen= (Widget) 0, iw_doc, iw_xreg, iw_yreg,
         iw_stndrt, iw_rangel, iw_rangeu, iw_key1, iw_key2, iw_imn,
         iw_rad, iw_sayno, iw_itxreg, iw_ityreg;

/**************************  comapmen  *********************************/

void comapmen(Widget w, XtPointer c, XtPointer u)
  {
  Widget iw_buta, iw_butc, iw_buts, iw_rowcol;
  char   filnamm[] = "sma***", docxnam[]  = "doc_sdc";
  char   cval[15];
 
  /* comap menu Widget does not exist, create it */
  if (iw_comapmen == (Widget) 0)
      {
      /* create a dialog box */
      iw_comapmen = wid_dialog(iw_win,(Widget) 0,"comap options",-1,-1);
      iw_rowcol = wid_rowcol(iw_comapmen,'v',-1,-1);
 
      /* set up the variables in the dialog widget */
      itxreg = 1;
      ityreg = 2;
      itrad  = 5;
      key1   = 1;
      key2   = 80;
      sayno  = TRUE;
      stndrt = 2.3;
      rangel = rangeu = 3.0;

      /* create a text box for doc file name input */
      iw_doc = wid_textboxb(iw_rowcol,(Widget) 0,
                            "          Doc. File:",docxnam,20);

      /* create a box for filename  input */
      iw_imn =  wid_textboxb(iw_rowcol,(Widget) 0,
                             "         Image File:",filnamm,20);

      /* create a box for first image key  input */
      sprintf(cval,"%12d",key1);
      iw_key1 = wid_textboxb(iw_rowcol,(Widget) 0,"    First Image Key:",cval,12);

      /* create a box for last image key  input */
      sprintf(cval,"%12d",key2);
      iw_key2 = wid_textboxb(iw_rowcol,(Widget) 0,"     Last Image Key:",cval,12);

      /* create a box for itxreg  input */
      sprintf(cval,"%12d",itxreg);
      iw_itxreg = wid_textboxb(iw_rowcol,(Widget) 0,
                       "X axis factor register:",cval,12);

      /* create a box for ityreg  input */
      sprintf(cval,"%12d",ityreg);
      iw_ityreg = wid_textboxb(iw_rowcol,(Widget) 0,
                       "Y axis factor register:",cval,12);

      /* create a box for stndrt */
      sprintf(cval,"%f",stndrt);
      iw_stndrt = wid_textboxb(iw_rowcol,(Widget) 0," Extent (std. dev.):",
                               cval,12);

      /* create a box for rangel input */
      sprintf(cval,"%f",rangel);
      iw_rangel = wid_textboxb(iw_rowcol,(Widget) 0,"Lower Image Thresh.:",
                               cval,12);

      /* create a box for rangeu input */
      sprintf(cval,"%f",rangeu);
      iw_rangeu = wid_textboxb(iw_rowcol,(Widget) 0,"Upper Image Thresh.:",
                               cval,12);

      /* create a box for marker radius input */
      sprintf(cval,"%12d",itrad);
      iw_rad = wid_textboxb(iw_rowcol,(Widget) 0,"      Marker Radius:",cval,12);

      /* create a toggle box for sayno input */
      iw_sayno = wid_toggleg(iw_rowcol,(Widget) 0," Show Image Numbers",
                             sayno,toggle2_cb, &sayno ,-1,-1);
 
      /* create a push button for stop, cancel & apply */
      wid_stdbut(iw_rowcol,iw_comapmen,
                 &iw_buts,&iw_butc,&iw_buta,"SCA",
                 fin_cb,fin_cb,comapmen_buta,NULL);
      }

  XtManageChild(iw_comapmen);
  }

/*******************  comapmen_buta ******************************/

void comapmen_buta(Widget iw_t, void * itag, void * callpar)
  {
  char  outstr[80];
  char *string;

  /* get docxnam from text widget */
  string = XmTextGetString(iw_doc);
  if (strlen(string) == 0)
    {
    XtFree(string);
    spout(" *** ERROR: No doc file name entered ");
    return;
    }
  else
    {
    strcpy(docxnam,string);
    XtFree(string);
    }

  /* get filnamm from text widget */
  string = XmTextGetString(iw_imn);
  if (strlen(string) == 0)
    {
    XtFree(string);
    spout(" *** ERROR: No file name entered");
    return;
    }
  else
    {
    strcpy(filnamm,string);
    XtFree(string);
    }

  /* get standard deviation from text widget */
  string = XmTextGetString(iw_stndrt);
  sscanf(string,"%f",&stndrt);
  XtFree(string);

  /* get first key number */
  string = XmTextGetString(iw_key1);
  sscanf(string,"%d",&key1);        
  XtFree(string);

  /* get last key number */
  string = XmTextGetString(iw_key2);
  sscanf(string,"%d",&key2);
  XtFree(string);

  /* get range low threshold from text widget */
  string = XmTextGetString(iw_rangel);
  sscanf(string,"%f",&rangel);        
  XtFree(string);

  /* get upper threshold from text widget */
  string = XmTextGetString(iw_rangeu);
  sscanf(string,"%f",&rangeu);
  XtFree(string);

  /* get radius */
  string = XmTextGetString(iw_rad);
  sscanf(string,"%d",&itrad);       
  XtFree(string);

  /* get itxreg */
  string = XmTextGetString(iw_itxreg);
  sscanf(string,"%d",&itxreg);       
  XtFree(string);

  /* get ityreg */
  string = XmTextGetString(iw_ityreg);
  sscanf(string,"%d",&ityreg);       
  XtFree(string);

  if (itxreg < 0 || itxreg > 9 || ityreg < 0 || ityreg > 9)
     {
     spout(" *** ERROR: Factor register range is 1...9");
     return;
     }

  /* get sayno toggle image numbers */
  sayno = XmToggleButtonGadgetGetState(iw_sayno);

  if (key1 > key2)
     {
     sprintf(outstr," *** ERROR: key1 %d > key2 %d ", key1,key2);
     spout(outstr);
     return;
     }

  XtUnmanageChild(iw_comapmen);

  /* start comap program */
  comap();
  }
