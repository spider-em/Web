/*
*****************************************************************************
*
*  reflmen.c         3/15/94
*
*  wid_refl.for (in VAX) ported to UNIX
*
C **********************************************************************
* *  AUTHOR:  Mahieddine Ladjadj                                           *
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
*
*    reflmen()
*
*    PURPOSE:	pick reflections from power spectrum and place locations
*               in a doc file.
*
*****************************************************************************
*/

#include "common.h"
#include "routines.h"
#include <Xm/Text.h>

 /* internal subroutine prototypes */
 void  reflmen_buts (Widget, caddr_t, caddr_t);
 void  reflmen_buta (Widget, caddr_t, caddr_t);
 void  refl0        (void);
 void  refl_pop     (Widget, XEvent *, String *, Cardinal *);
 void  refl1        (void);
 void  getvals      (char *, char *, char *);
 void  getvals_buta (Widget, caddr_t , caddr_t);
                            
 /* internal file variables */
 static Widget   iw_reflmen = (Widget) 0;
 static Widget   iw_fdoc, iw_key, iw_rad2, iw_leavit;
 static Widget   iw_getvals  = (Widget) 0, iw_ival1, iw_ival2;
 static int      first = TRUE;
 static int      fkx, fky, ikeyt, iarad, leavitt = TRUE;
 static int      itype2, hind, kind;
 static FILE *   fpt; 
 static char     docnamt[81];
 static int      openitref; 
 
 /***************************  reflmen ********************************/

 void reflmen(void)
 {
 Widget iw_rowcol, iw_pushs, iw_pushc, iw_pusha, iw_dum = (Widget) 0;
 char cval[5];


 if (iw_reflmen <= (Widget) 0)
     {   /* pixel menu widget does not exist. create it */
     iw_reflmen = wid_dialog(iw_win,0,"refl options", -1,-1);

     /* profile menu widget */
     iw_rowcol = wid_rowcol(iw_reflmen,'v',-1,-1);

     /* set up rows and columns in the dialog widget */
     iarad      = 5;
     ikeyt      = 1;

     iw_fdoc    = (Widget) 0;
     iw_key     = (Widget) 0;
     iw_rad2    = (Widget) 0;
     iw_leavit  = (Widget) 0;
     iw_dum     = (Widget) 0;
   
     /* create a text box for doc file name input */
     iw_fdoc = wid_textboxb(iw_rowcol,iw_fdoc,"    Doc. File:", docnamt,20);

     /* create a box for  key # input */
     sprintf(cval,"%4d",ikeyt);
     iw_key = wid_textboxb(iw_rowcol,iw_key,  "        Key #:",cval,4);

     /* create a box for marker radius input */
     sprintf(cval,"%4d",iarad);
     iw_rad2 = wid_textboxb(iw_rowcol,iw_rad2,"Marker radius:",cval,4);
                                              
     /* create a toggle box for LEAVE OT */              
     iw_leavit = wid_toggleg(iw_rowcol,iw_leavit,"  Leave marker",
                        leavitt,toggle2_cb,(char *) &leavitt,-1,-1);

     /* create a push button for stop, cancel & apply */
     iw_dum = wid_stdbut(iw_rowcol,iw_reflmen,
                         &iw_pushs,&iw_pushc,&iw_pusha,"SCA",
                         reflmen_buts, fin_cb, reflmen_buta,NULL);
     }
  
 openitref = TRUE;  
 XtManageChild(iw_reflmen);
 return;
 }

/*********************** reflmen_buts   ******************************/

void reflmen_buts(Widget iw_temp, caddr_t data, caddr_t call_data)
  {
  /* remove popup button menu */
  showbutx("","","",TRUE);

  /* cancel menu and stop this routine */
  XtUnmanageChild(iw_reflmen);

  /* cancel menu, stop masking mask button assignments */
  XtUninstallTranslations(iw_win);

  /* close document file */
  if (fpt != (FILE *) NULL) fclose(fpt);

  /* We want to force the user to display a new image before he
       can do another refl command.  */
  
  first = TRUE;
  }
                                
 /*******************************   reflmen_buta  *********************/
                                                               
 void reflmen_buta(Widget iw_t, caddr_t itag, caddr_t callpar)
 {
 char * string;

 /* get docnamt from text widget */
 string = XmTextGetString(iw_fdoc);
 if (strlen(string) == 0)
        {
        if (string) XtFree(string);
        spout("*** No doc file name entered");
        return;
        }
 else
        {    
        /* probably got a valid doc file name */
        strcpy(docnamt,string);
        if (string) XtFree(string);
        }

 /* get next key number */
 string = XmTextGetString(iw_key);
 sscanf(string,"%d",&ikeyt);       
 if (string) XtFree(string);

 /* get radius for marker */
 string = XmTextGetString(iw_rad2);
 sscanf(string,"%d",&iarad);
 if (string) XtFree(string);
    
 XtUnmanageChild(iw_reflmen);

 if (first)
     {
     /* must start refl program the first time thru the menu */
     refl0();
     first = FALSE;
     }
               
 return;    
 }
                                  
 /********************************   refl0   *************************/

 void refl0(void)
    {
    showbutx("Record peak location",
            "Display menu.",
            "Stop this routine.", FALSE);

    /* set the actions for right, left, and center buttons */
    actions(iw_win, "refl_pop",refl_pop,"123");
    }

 /*****************************   refl_pop   *************************/

  void refl_pop(Widget iw_t, XEvent *event, String *params,
                   Cardinal *num_params)
  {
   int    ixc, iyc, ixi, iyi;
   char   outstr[80];                     

    if (!(strcmp(*params,"1")))                /* button 1 pushed */
        {
        /* get mouse position ixc,iyc  */
        getloc(event,'M',&ixc,&iyc);

        /*  find location relative to image */
        ixi = ixc - ixul + 1;
        iyi = iyc - iyul + 1;

        if (ixi < 1 || ixi > nsam || iyi < 1 || iyi > nrow)
            {
            /*  outside of image. can not select this location */
            sprintf(outstr,"Outside row: %d col: %d", ixi,iyi);
            spout(outstr);
            }
        else
            {  /* inside of image. */
            if (leavitt)
                {
                /* draw a permanent circle at this position */
                xorc(iwin,    icontx, TRUE, ixc, iyc, iarad);
                xorc(imagsav, icontx, TRUE, ixc, iyc, iarad);
                }

            fkx = ixi;
            fky = iyi;

            /* display the getvals widget for hind & kind */
            getvals("h","H:","K:");
            }
        }

    else if (!(strcmp(*params,"2")))            /* button 2 pushed */
        {
        /* middle button show menu */
        reflmen();
        }

    else if (!(strcmp(*params,"3")))            /* button 3 pushed */
        {
        /* right button stops the routine */
        reflmen_buts(NULL,NULL,NULL);
        }
  }

/*************************** refl1  **********************************/

  void refl1(void)
  {                       
    int ix, iy;
    char          outstr[80];
    float         dlist[6];
 
    dlist[0] = ikeyt;
    dlist[1] = ikeyt;
    dlist[2] = hind;
    dlist[3] = kind;
    dlist[4] = fkx;
    dlist[5] = fky;
       
    fpt = savdn1(docnamt,datexc,&fpt,dlist,6,&openitref,TRUE,TRUE);
    ix = fkx;
    iy = fky;

    sprintf(outstr,"Key #: %d (%d,%d) index: %d %d",
               ikeyt,ix,iy,hind,kind);
    spout(outstr);
  
    ikeyt = ikeyt + 1;
  }


 /***************************  getvals  *******************************/

 void getvals(char *ctype, char *label1, char *label2)
 {
  Widget iw_pushc, iw_pusha, iw_pushs, iw_rowcol, iw_dum = (Widget) 0;
  char cval[5];

  if (iw_getvals <= (Widget) 0)
       {
       /* create a dialog box */
       iw_getvals = wid_dialog(iw_win,(Widget) 0,"getvals options",-1,-1);
 
       /* reflection menu widget */
       iw_rowcol = wid_rowcol(iw_getvals,*ctype,-1,-1);             

       iw_dum     = (Widget) 0;
       iw_ival1   = (Widget) 0;
       iw_ival2   = (Widget) 0;
       
       /* create a box for  hind input */
       sprintf(cval,"%4d",hind);
       iw_ival1 = wid_textboxb(iw_rowcol,iw_ival1,label1,cval,4);

       /* create a box for kind input */
       if( *label2 != (char) NULL)
           {
           itype2 = 1;           
           sprintf(cval,"%4d",kind);
           iw_ival2 = wid_textboxb(iw_rowcol,iw_ival2,label2,cval,4);
           }
       else
           itype2 = 0;
                                                       
       /* create a push button for stop, cancel & apply */
       iw_dum = wid_stdbut(iw_rowcol,iw_getvals,
                           &iw_pushs,&iw_pushc,&iw_pusha,"CA",
                           fin_cb, fin_cb, getvals_buta,
                           NULL);
       }

 XtManageChild(iw_getvals);
                
 }

 /**************************  getvals_buta   *************************/

 void getvals_buta(Widget iw_temp, caddr_t client_data, caddr_t call_data)
 {
 char * string;
 int  errors;

 errors = FALSE;
                                  
 /* get hind  from text widget */
 string = XmTextGetString(iw_ival1);
 if (strlen(string) == 0)
     {
     XtFree(string);
     spout("*** WARNING: no  hind  integer value ");
     errors = TRUE;
     }
 else
     {
     sscanf(string,"%d",&hind);
     XtFree(string);
     }
                                  
 if (itype2)
     {
     /* get kind  from text widget */
     string = XmTextGetString(iw_ival2);
     if (strlen(string) == 0)
         {
         XtFree(string);
         spout("*** WARNING: no  kind integer value ");
         errors = TRUE;
         }
     else
         {                    
         sscanf(string,"%d",&kind);
         XtFree(string);
         }
     }                             
      
 if(!errors) 
     {   /* cancel menu and stop this routine */
     if (iw_getvals != (Widget) NULL)
             XtUnmanageChild(iw_getvals);
        
     /* put relections in the doc file */                        
     refl1();
     }  
  }

