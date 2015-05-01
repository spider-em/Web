
/*$Header: /usr8/web/src/RCS/surfmovie.c,v 1.15 2007/11/01 19:33:45 leith Exp $*/
/*
C**************************************************************************
C
C   surfmovie.c    
C             
C **********************************************************************
C *  AUTHOR: Lekan Odesanya                                                *
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
C SURFMOVIE(CANROTATE)
C
C  PURPOSE:     CREATES MOVIE SEQUENCES OF 2-D 
C               REFLECTION IMAGES OF DIFFERENT ORIENTATIONS
C
C  RETURNS:     VOID
C
C  CALLED BY:   SURFMEN
C
C  NOTES:       FOR EACH PIXEL, THE PREVIOUS PIXEL ON THE ROW AND THE
C               PIXEL ONE COL. DOWN ARE USED TO FORM A TRIANGULAR TILE
C               THE TILE IS SHADED ACCORDING TO ITS ORIENTATION ACROSS
C               THE X AXIS AND ITS DISTANCE ALONG THE X AXIS FROM THE
C               LEFT END OF THE FILE.
C
C--*********************************************************************
*/

#include "common.h"
#include "routines.h"

#include <Xm/Text.h>
#include <Xm/ToggleBG.h>


 extern void   surf_pop     (Widget, XEvent *, String *, Cardinal *);

 /* externally defined variables used here */
 extern float            phi, theta, psi, thlev;
 extern unsigned char  * refmap;

 /* internal function prototypes */
 void          surfmovie_buta(Widget iw_temp, caddr_t, caddr_t);
 void          surfmovie_buts(Widget iw_temp, caddr_t, caddr_t);
 void          surfmovie_next(Widget iw_temp, caddr_t, caddr_t);


 /* internal common variables */
 static float    jslope, kslope;
        enum     SEQTYPE {Thresh, Phi, Theta, Psi};
 static int      seqcount = 1, read_ok, rotate, nsams, nrows;
 static Widget   iw_thresh, iw_phi, iw_theta, iw_psi;
 static Widget   iw_sval, iw_eval, iw_nfrm, iw_seq, iw_rowcolv;
 static Widget   iw_pushc, iw_pusha, iw_doc, iw_rch2;
 static Widget   iw_surfmovie = (Widget) 0;


 /**************************   surfmovie   ****************************/

 void surfmovie(int canrotate)
 {
   Widget iw_dums, iw_radio, iw_rch, iw_rcv;
   Arg    args[2];


    rotate = canrotate; 

 /* find reflection image size */
    jslope = (1.0 - nrow*scaleval)/(1.0 - nrow);
    kslope = (1.0 - nslice*scaleval)/(1.0 - nslice);
    nsams  = (nrow2 - nrow1)*jslope + 1;
    nrows  = (nslice2 - nslice1)*kslope + 1;

    if (iw_surfmovie <= (Widget)0)
     { /* create surface movie menu widget first */
      iw_surfmovie  = wid_dialog(iw_win, 0, "Surface Movie Options", -1, -1);
      iw_rowcolv  = wid_rowcol(iw_surfmovie, 'v', -1, -1);

      iw_doc = wid_textboxb(iw_rowcolv,0,"First Output File:",docnam, 12);

    /* create toggle boxes for sequence type */
      iw_rcv  = wid_rowcol(iw_rowcolv, 'v', -1, -1);

      (void) wid_labelg(iw_rcv, 0, "Build sequences on:   ", -1, -1);

      iw_radio  = wid_rowcol(iw_rcv,  'h', -1, -1);
      XtSetArg(args[0], XmNradioBehavior, TRUE);
      XtSetValues(iw_radio, args, 1);

      iw_thresh = wid_togmen(iw_radio,0,"Thresh",True,-1,-1);
      iw_phi = wid_togmen(iw_radio,0,"Phi",False,-1,-1);
      iw_theta = wid_togmen(iw_radio,0,"Theta",False,-1,-1);
      iw_psi = wid_togmen(iw_radio,0,"Psi",False,-1,-1);

    /* create text box for start and end values input ------------- values */
      iw_rch  = wid_rowcol(iw_rowcolv, 'h', -1, -1);
      iw_sval = wid_textboxb(iw_rch, 0, "Start Value:", "", 6);
      iw_eval = wid_textboxb(iw_rch, 0, "End Value:", "", 6);
      iw_nfrm = wid_textboxb(iw_rowcolv, 0, "No. of Frames:", "1", 6);

    /* create label box for Sequences ---------------------------- Sequence  */
      iw_rch2  = wid_rowcol(iw_rowcolv,  'h', -1, -1);
      iw_seq  = wid_labelg(iw_rch2, 0, "Sequence: 1", -1, -1);
      (void) wid_labelg(iw_rch2, 0, "         ", -1, -1);
      (void) wid_pushg(iw_rch2, 0,"Next Sequence",surfmovie_next,"0",0,0);

    /* create box for apply ----------------------------------- apply */
      iw_dums = wid_stdbut(iw_rowcolv, iw_surfmovie, 
                        &iw_dums, &iw_pushc, &iw_pusha,  "SA",
                        fin_cb, surfmovie_buts, surfmovie_buta, NULL);
     }
   else
     {
       if(seqcount == 1)
        { 
          iw_doc = wid_textboxb(iw_rowcolv,iw_doc,"First File:",docnam, 12);
          XtManageChild( XtParent(iw_doc) );
          iw_seq  = wid_labelg(iw_rch2, iw_seq, "Sequence: 1", -1, -1);
        }
     }
 
   XtManageChild(iw_surfmovie);
     
 }

 /************ next button callback *********************************/

 void surfmovie_next(Widget iw_temp, caddr_t data, caddr_t call_data)
 {
   int          nframes;
   Arg          args[2];
   char         label[100], *string;
   float        val1, val2, ndiv, val, *buft;
   int          i, j, k, kk, nlet, numfil, irec;
   FILEDATA     *filedatat;
   enum         SEQTYPE type;
   XmString     str;

   read_ok = 0;

   if (XtIsManaged(XtParent(iw_doc))) 
    {
     strcpy(docnam,XmTextGetString(iw_doc));
     if (strlen(docnam) == 0) 
       {
       spout("*** Must specify name for first file.");
       return;
       }

 /* add datexc to docnam if not already there */
      if (strstr(docnam,datexc) == 0)
       {
        strcat(docnam,".");
        strcat(docnam,datexc);
       }                      
         
     XtUnmanageChild(XtParent(iw_doc));
    }

   if(XmToggleButtonGadgetGetState(iw_thresh)) type = Thresh;
   else
    if(XmToggleButtonGadgetGetState(iw_phi)) type = Phi;
    else
     if(XmToggleButtonGadgetGetState(iw_theta)) type = Theta;
     else
      if(XmToggleButtonGadgetGetState(iw_psi)) type = Psi;

 /* get minimum value */
    string = XmTextGetString(iw_sval);
    if (sscanf(string,"%f",&val1) <= 0 )
     {   /* can not interpret val1 */
      spout("*** Can not interpret Start value entry in surfmovie! ");
      if (string) free(string);
      return;
     }

 /* get maximum value */
    string = XmTextGetString(iw_eval);
    if (sscanf(string,"%f",&val2) <= 0 )
     {   /* can not interpret val2 */
      spout("*** Can not interpret End value entry in surfmovie! ");
      if (string) free(string);
      return;
     }

 /* get number of frames */
    string = XmTextGetString(iw_nfrm);
    if (sscanf(string,"%d",&nframes) <= 0 )
     {   /* can not interpret nframes */
      spout("*** Can not interpret No. of frames entry in surfmovie! ");
      if (string) free(string);
      return;
     }

 if (string) free(string);

   if(type == Thresh)
    {
      if (val1 > filedata -> fmax_spi || val1 < filedata -> fmin_spi) 
       {
        sprintf(label,"*** Start value must be between %f and %f in surfmovie",
               filedata -> fmin_spi,filedata -> fmax_spi);
        spout(label);
        return;
       }
      if (val2 > filedata -> fmax_spi || val2 < filedata -> fmin_spi) 
       {
        sprintf(label,"*** End value must be between %f and %f in surfmovie",
               filedata -> fmin_spi,filedata -> fmax_spi);
        spout(label);
        return;
       }
    }
   else
    {
      if (val1 > 360.0 || val1 < 0.0) 
       {
        spout("*** Start value must be between 0 and 360!");
        return;
       }
      if (val2 > 360.0 || val2 < 0.0) 
       {
        spout("*** End value must be between 0 and 360!");
        return;
       }
    }

   if(val2 <= val1)
   {
    spout("*** End value must be greater than Start value in surfmovie!");
    return;
   }

   read_ok = 1;

   if((buft = (float *) malloc(sizeof(float)*nsams)) == NULL)
    {
      spout("*** Allocation failed for writing to file in surfmovie!");
      return;
    }

   XtUnmanageChild(iw_surfmovie);

   val  = val1;
   ndiv = (val2 - val1)/(nframes-1);

   for(j = 0; j < nframes; j++)
    {
         switch(type)
          {
            case Thresh: thlev = val; break;
            case Phi:    phi   = val; break;
            case Theta:  theta = val; break;
            case Psi:    psi   = val; break;
          }

         val += ndiv;
         surf(rotate, FALSE, FALSE, ioper == 17);

 /* open new output file */
      filedatat = opennew(docnam, nsams, nrows, 1, 1, "u");  

 /* put newimage in output file */
        kk = 0;
        for (irec = 1; irec <= nrows; irec++)
         {
           for(k = 0; k < nsams; k++) buft[k] = (float) refmap[kk++];

           if (!wrtlin(filedatat, buft, nsams, irec)) 
               { spout("*** Error writing to file in surfmovie!");
                 closefile(filedatat); free(buft); return;
               }
         }

         closefile(filedatat);     
 /* increment file name */
         filinc(docnam,filnext,&nlet,&numfil);
         strcpy(docnam,filnext);   
    }
   
   free(buft);

   seqcount++;
   sprintf(label,"Sequence: %d",seqcount);
   str = XmStringCreateLtoR(label,XmSTRING_DEFAULT_CHARSET);
   XtSetArg(args[0], XmNlabelString, str); 
   XtSetValues( iw_seq, args, 1);
   XmStringFree(str);

   XtManageChild(iw_surfmovie);
 } 

/************ accept button callback *********************************/

 void surfmovie_buta(Widget iw_temp, caddr_t data, caddr_t call_data)
 {

   surfmovie_next( NULL, NULL, NULL );
   if(!read_ok) return;
  
   XtUnmanageChild(iw_surfmovie);

   seqcount = 1;

   /* display menu */
   showbutx("Recalls Menu.", 
          "Recalls Menu.",
          "Stop surfacing.",FALSE); 

   /* define buttons for menu retrieval or stop */
    actions(iw_win, "surf_pop", surf_pop, "123");

 }

 /************ cancel button callback *********************************/

 void surfmovie_buts(Widget iw_temp, caddr_t data, caddr_t call_data)
 {
  XtUnmanageChild(iw_surfmovie);
  seqcount = 1;
 }
