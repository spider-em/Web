

/*
C++************************************************************************
C
C CTFMEN  -- CREATED JUNE 99 al
C            ICOLORSAV                            SEP 2011 ARDEAN LEITH
C **********************************************************************
C *  AUTHOR:  ArDean Leith                                                 *
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
C    CTFMEN
C
C    PURPOSE:         DISPLAY CTF menu
C
C    CALLED BY:       IMAGEMEN
C
C    imagemen --> wid_files --> imagemen_cb --> ctfmen0 --> ctfmen
C                                                             v
C                          <-- ctfmen1set  <-- ctfmen1 <-- ctfmen_buta
C                                         <-- ctfmen1 <-- ctfmenf_buta
C                                   
C***********************************************************************
*/

#include <Xm/Scale.h>
#include <Xm/Text.h>

#include "common.h"
#include "routines.h"

#define MAX_ARGS 3


 /* external function prototypes */

 /* internal function prototypes */
 static int   	ctfnormal (void);
 static int   	ctfnormal1(void);
 static void   	ctfmen (void);
 static void   	ctfmen1(void);
 static void   	ctfmen_buts (Widget iw_temp, XtPointer, XtPointer);
 static void   	ctfmen_buta (Widget iw_temp, XtPointer, XtPointer);
 static void   	ctfmen1_buta(Widget iw_temp, XtPointer, XtPointer);
 static void   	ctfmen1set(void);
 static void   	ctfmen1f_buta(Widget iw_temp, XtPointer, XtPointer);
 static void   	ctfplot(float xrange, float yrange, float valmin);
 static int     plotit(float * linearray, int nval, 
                       float xoffset, float yoffset, 
                       float xscale,  float yscale, 
                       float xmin, float ymin);
 static int     plotvert(float * linearray, int nval, 
                       int iygo, int iyend, 
                       float xoffset, float xscale, float xmin);
  
 /* external common variables */

 /* internal common variables */
 static Widget   iw_ctfmen   = (Widget) 0;
 static Widget   iw_ctfmen1  = (Widget) 0;
 static Widget   iw_defocusc = (Widget) 0;
 static Widget   iw_defocusf = (Widget) 0;
 static Widget   iw_cs, iw_kev, iw_srcsiz, iw_km;
 static Widget   iw_ds, iw_dza, iw_azz,    iw_wgh, iw_env, iw_cutoff;

 static float    cs, dz, km, ds, dza, azz, wgh, env, lambda;
 static float    pixsiz, recip_srcsiz, valmax, valmin;
 static int      idefocus    = 20000;
 static int      idefocusc   = 20000;
 static int      idefocusf   = 0;
 static int      lastdefocus = -100.0;
 static int      cutoff      = 100;
 static int      lastcutoff  = -100;
 static int      kev;
 static float*   dbuf = NULL;
 static float*   tbuf = NULL;
 static float*   tf   = NULL;
 static int      maxkeyfound;

 /***************************   ctfmen0  ****************************/

 void ctfmen0(char * ctfdocfile)

 { 
 int          maxreg = 2;
 int          maxkeys = 17008;
         
 if (getdoc((FILE *) NULL, ctfdocfile, NULL, maxkeys, maxreg, 
             &dbuf, &maxkeyfound) > 0) 
    {   /* problem retrieving doc file data */
    if (dbuf) free(dbuf); dbuf = NULL;
    return;
    }

  /* run time allocation of memory space for tbuf */
  if ((tbuf = (float *) calloc(maxkeyfound*2, sizeof(float)))
           == (float *) NULL)
     {
     spout("*** Memory allocation failed in ctfmen0!"); 
     if (dbuf) free(dbuf); dbuf = NULL;
     return;
     }

  /* run time allocation of memory space for tf */
  if ((tf = (float *) calloc(maxkeyfound*2, sizeof(float)))
           == (float *) NULL)
     {
     spout("*** Memory allocation failed in ctfmen0!"); 
     if (dbuf) free(dbuf); dbuf = NULL;
     if (tbuf) free(tbuf); tbuf = NULL;
     return;
     }

 /* normalize the doc file ctf values over range 0...1 */
 ctfnormal();

 /* display menu for input of all parameters */
 ctfmen();
 } 

 /************************ ctfnormal *********************************/

 int ctfnormal(void)
 {
 int     i,j, k;
 float   val;
 float   ymin   = 0.0;
 float   yrange = 1.0;

 /* find min and max value in doc file */
 valmin = dbuf[1];
 valmax = valmin;

 for ( i = 1; i < maxkeyfound*2; i=i+2)
    {
    val    = dbuf[i];
    valmin = MYMIN(valmin,val);
    valmax = MYMAX(valmax,val);
    }
 if (valmax <= valmin) 
    { 
    spout("*** Document file min. same as max!"); 
    return FALSE;
    }

 /* normalize doc file contents over range ymin...ymin+yrange */
 /* keep x values (just 1...nsam) in tbuf */
 j = 0; k = 1;
 for (i = 1; i < maxkeyfound*2; i=i+2)
    {
    val       = dbuf[i];
    tbuf[j++] = k++;
    dbuf[j++] = (((val - valmin) / (valmax - valmin)) * yrange) + ymin;
    }
 return TRUE;
 }

 /***************************   ctfmen   ****************************/

 void ctfmen(void)

 { 
 Widget iw_dums,     iw_pushc,  iw_pusha;
 Widget iw_rowcolh1, iw_rowcolh2, iw_rowcolh3, iw_rowcolv;
 char   cval[50];
 Arg    args[2];
 
 if (iw_ctfmen <= (Widget) 0)
    {   /* create ctf menu widget first */

    iw_ctfmen  = wid_dialog(iw_win, 0, "CFT options", -1, -1);
    iw_rowcolv = wid_rowcol(iw_ctfmen, 'v', -1, -1);

    /* create text box for km ----------------------------------  km */
    pixsiz = 3.33333;
    sprintf(cval,"%f",pixsiz);
    iw_km = wid_textboxb(iw_rowcolv,0,"Pixel size (A):",cval,10);

    /* create horizontal rowcol for abberation and kev ------------  */
    iw_rowcolh1  = wid_rowcol(iw_rowcolv, 'h', -1, -1);

    /* create text box for abberation ------------------  abberation */
    cs = 2.0;
    sprintf(cval,"%f",cs);
    iw_cs = wid_textboxb(iw_rowcolh1,0,
              "Spherical abberation constant (mm):",cval,10);

    /* create label box for spacing */
    wid_labelg(iw_rowcolh1, 0,"  ", -1, -1);

    /* create text box for kev ---------------------------------  kev */
    kev = 100.0;
    sprintf(cval,"%d",kev);
    iw_kev = wid_textboxb(iw_rowcolh1,0,
             "Electron energy (kev):",cval,10);

    /* create horizontal rowcol for source size  and spread -------- */
    iw_rowcolh2 = wid_rowcol(iw_rowcolv, 'h', -1, -1);

    /* create text box for source size ----------------- source size */
    recip_srcsiz = 0.005;
    sprintf(cval,"%f",recip_srcsiz);
    iw_srcsiz = wid_textboxb(iw_rowcolh2,0,"Source size (A^-1):",
                             cval,10);

    /* create label box for spacing  */
    wid_labelg(iw_rowcolh2, 0,"   ", -1, -1);

    /* create text box for spread --------------------------- spread */
    ds = 0.0;
    sprintf(cval,"%f",ds);
    iw_ds = wid_textboxb(iw_rowcolh2,0,"Defocus spread (A):",
                         cval,10);

    /* create horizontal rowcol for astigmatism. & azimuth --------- */
    iw_rowcolh3 = wid_rowcol(iw_rowcolv, 'h', -1, -1);

    /* create text box for astigmatism ----------------- astigmatism */
    dza = 0.0;
    sprintf(cval,"%f",dza);
    iw_dza = wid_textboxb(iw_rowcolh3,0,"Astigmatism (A):",
                           cval,10);

    /* create label box for spacing  */
    wid_labelg(iw_rowcolh3, 0,"   ", -1, -1);

    /* create text box for azimuth ------------------------- azimuth */
    azz = 0.0;
    sprintf(cval,"%f",azz);
    iw_azz = wid_textboxb(iw_rowcolh3,0,"Azimuth (Deg.):",
                          cval,10);

    /* create text box for wgh --------------------------------- wgh */
    wgh = 0.1;
    sprintf(cval,"%f",wgh);
    iw_wgh = wid_textboxb(iw_rowcolv,0,
              "Amplitude contrast ratio (0..1):",cval,10);

    /* create text box for envelope ----------------------- envelope */
    env = 10000.0;
    sprintf(cval,"%f",env);
    iw_env = wid_textboxb(iw_rowcolv,0,
                          "Gaussian envelope halfwidth (A^1):",
                          cval,10);

    /* create box for apply ---------------------------------- apply */
    iw_dums = wid_stdbut(iw_rowcolv, iw_ctfmen, 
                        &iw_dums, &iw_pushc, &iw_pusha,  "CA",
                        fin_cb, ctfmen_buts, ctfmen_buta, NULL);
    }   

 XtManageChild(iw_ctfmen);
 }


 /*********** stop button callback *********************************/

 void ctfmen_buts(Widget iw_temp, XtPointer data, XtPointer call_data)
   {
   /* remove parameter menu */
   XtUnmanageChild(iw_ctfmen);

   /* remove dofocus menu */
   if (iw_ctfmen1) XtUnmanageChild(iw_ctfmen1);

   /* free allocated memory */
   if (dbuf) free(dbuf); dbuf = NULL;
   if (tf)   free(tf);   tf   = NULL;
   if (tbuf) free(tbuf); tbuf = NULL;

   }

 /*********** parameter menu accept button callback *****************/

 void ctfmen_buta(Widget iw_temp, XtPointer data, XtPointer call_data)
   {
 char *string;

 /* find cs */
 string = XmTextGetString(iw_cs);
 sscanf(string,"%f",&cs);
 if (string) free(string);

 /* find km */
 string = XmTextGetString(iw_km);
 sscanf(string,"%f",&pixsiz);
 if (string) free(string);
 km = 1.0 / ( 2 * pixsiz);

 /* find kev */
 string = XmTextGetString(iw_kev);
 sscanf(string,"%d",&kev);
 if (kev < 0)
   { spout("*** Illegal kev value!"); return;}

 lambda = 12.398 / (sqrt (kev * (1022.0 + kev)));
 if (string) free(string);

 /* find recip_srcsiz (q) */
 string = XmTextGetString(iw_srcsiz);
 sscanf(string,"%f",&recip_srcsiz);
 if (string) free(string);

 /* find ds */
 string = XmTextGetString(iw_ds);
 sscanf(string,"%f",&ds);
 if (string) free(string);

 /* find dza */
 string = XmTextGetString(iw_dza);
 sscanf(string,"%f",&dza);
 if (string) free(string);

 /* find azz */
 string = XmTextGetString(iw_azz);
 sscanf(string,"%f",&azz);
 if (string) free(string);

 /* find wgh */
 string = XmTextGetString(iw_wgh);
 sscanf(string,"%f",&wgh);
 if (string) free(string);

 /* find env */
 string = XmTextGetString(iw_env);
 sscanf(string,"%f",&env);
 if (string) free(string);

 /* removed all parameter menu window */
 XtUnmanageChild(iw_ctfmen);

 /* find defocus & cutoff & plot values */
 ctfmen1_buta((Widget) NULL, (XtPointer) NULL, (XtPointer) NULL);

 }



 /*********** defocus menu accept button callback *****************/

 void ctfmen1_buta(Widget iw_temp, XtPointer data, XtPointer call_data)
 {
 if (iw_defocusc) 
    {
    /* get Defocus, set other slider */
    XmScaleGetValue(iw_defocusc ,&idefocusc);

    /* get cutoff */
    XmScaleGetValue(iw_cutoff,&cutoff);
    }
 idefocus = idefocusc + idefocusf;
 ctfmen1set();
 
 }
 /*********** defocusf menu accept button callback *****************/

 void ctfmen1f_buta(Widget iw_temp, XtPointer data, XtPointer call_data)
 {

 if (iw_defocusf) 
    {
    /* get Defocus, set other slider */
    XmScaleGetValue(iw_defocusf,&idefocusf);

    /* get cutoff */
    XmScaleGetValue(iw_cutoff,&cutoff);
    }
 idefocus = idefocusc + idefocusf;
 ctfmen1set();
 
 }

 /*********** ctfmen1set -- defocus setter *****************/

 void ctfmen1set(void)
 {
 float defocus;

 /* clear screen */
 webclear(NULL,NULL,NULL);
 expos_cb(NULL,NULL,NULL);

 /* calculate doc file plot with new cutoff */
 if (cutoff != lastcutoff)
    ctfnormal1();

 /* calculate curve plot */
 if (idefocus != lastdefocus)
    {
    defocus = (float) idefocus;
    trafc(cs, defocus, lambda, km, recip_srcsiz, ds, dza, azz, wgh, env, 
               maxkeyfound, tf);
    }

 /* display doc file plot & curve plot */
 ctfplot(maxkeyfound, 2.0,valmin);

 /* defocus menu remains on screen */
 }


 /************************ ctfnormal1 *********************************/

 int ctfnormal1(void)
 {   /* applies cuttof to doc file plot */
 int     i,j, k;
 float   val;
 float   invcutoff;

 /* normalize tbuf over range 0.0...1.0  with values above */
 /* cutoff (%)  set to 1.0 */

 invcutoff = 1.0 / ((float) cutoff / 100.0);
 
 j = 1; 
 for (i = 1; i < maxkeyfound*2; i=i+2)
    {
    val = dbuf[i] * invcutoff;
    if (val > 1.0) val = 1.0; 
    tbuf[j] = val;
    j+=2;
    }
 return TRUE;
 }

 /************************ ctfplot *********************************/

 void ctfplot(float xrange, float yrange, float valmin)
 {    /* plots both curves */

 float    xscale, yscale, lefmargin, botmargin,ritmargin, topmargin;
 int      ixgo, ixend, iygo, iyend, botgo, ici;
 char     cval[5];
 char     cvala[10];

 float    fval,recipa;
 int      ixinc;
 int      icolorsav;

 icolorsav = icolor;
 /* set white color */
 ici = colorgo + 7;
 wicolor(icontx,ici);

 /* set empty margins for x (left)  & y (bottem).....*/ 
 lefmargin = 75.0; botmargin = 75.0;
 ritmargin = 75.0; topmargin = 75.0;

 /* find y-scaling factor for height */
 yscale  = (float) (ihighx - botmargin - topmargin ) / yrange;

 /* find x-scaling factor for width */
 xscale = (float) (iwidex - lefmargin - ritmargin) / xrange;

 /* make horizontal x axis at y=0.0 (in middle of lower plot) */
 ixgo  = lefmargin;
 ixend = ixgo + xrange * xscale;

 iygo  = ihighx - (1.0 * yscale + botmargin);
 xorline(iwin,   icontx,TRUE, ixgo,iygo, ixend,iygo);
 xorline(imagsav,icontx,TRUE, ixgo,iygo, ixend,iygo);

 /* make vertical y axis along left at x = lefmargin  */
 ixgo  = lefmargin;
 iygo  = ihighx - botmargin;

 iyend = ihighx - (botmargin + yscale * yrange);
 xorline(iwin,   icontx,TRUE, ixgo,iygo, ixgo,iyend);
 xorline(imagsav,icontx,TRUE, ixgo,iygo, ixgo,iyend);

 /* plot vertical zero crossing lines */
 plotvert(tf, maxkeyfound, iygo, iyend, lefmargin, xscale, 0.0);

 /* set green color for tbuf */
icolorsav = icolor;
 ici = colorgo + 2;
 wicolor(icontx,ici);

 /* plot tbuf across top (with y =  0.00... 1.0) */
 botgo   = 1.0 * yscale + botmargin;
 if (valmin < 0.0) 
    {  /* (plot with y =  -1.00... 1.0) */
    plotit(tbuf, maxkeyfound, lefmargin, botmargin, 
        xscale,2*yscale, 0.0,0.0);
    }
 else
    {   /* (plot with y =  0.00... 1.0) */
    plotit(tbuf, maxkeyfound, lefmargin, botgo, 
        xscale,yscale, 0.0,0.0);
    }

 /* set red color for tf */
 ici = colorgo + 4;
 wicolor(icontx,ici);

 /* plot tf across bottem (with y =  -1.00... 1.0 */
 plotit(tf, maxkeyfound, lefmargin,botmargin,  
        xscale,yscale, 0.0,-1.0);

 /* set color to white */
 wicolor(icontx,ici);

 /* set labels for x axis */

 ixgo  = lefmargin;
 iygo  = ihighx - botmargin + 15;
 ixinc = (xrange * xscale) / 5.0;

 for (fval=1/(2*xrange); fval<=(0.5 +.05); fval+=0.1)
    {
    sprintf(cval,"%3.2f",fval);
    witext(icontx, cval, ixgo, iygo+25, 0, 0, -9, 2, FALSE);
    /* tick mark */
    xorline(iwin,   icontx,TRUE, ixgo,iygo, ixgo,iygo+10);
    xorline(imagsav,icontx,TRUE, ixgo,iygo, ixgo,iygo+10);

    /* recip Ang. label */
    recipa = pixsiz / fval;

    sprintf(cvala,"1/%4.1f",recipa);
    witext(icontx, cvala, ixgo, iygo+45, 0, 0, -9, 2, FALSE);

    ixgo = ixgo + ixinc;
    }

 /* set color back  */
 wicolor(icontx,icolorsav);

 /* display defocus menu */
 ctfmen1();
 }

/******************* plotit ******************************************/

int plotit(float * linearray, int nval, float xoffset, float yoffset, 
           float xscale, float yscale, float xmin, float ymin)
 {
 int i,ix1,iy1, ix2,iy2;

 /* plot curve */
 for (i=0; i < nval*2; i+=2)
    {
    if (i > 0)
       {ix1 = ix2; iy1 = iy2;}

    ix2  = (int) (((linearray[i]   - xmin) * xscale) + xoffset);
    iy2  = ihighx - (int) 
                 (((linearray[i+1] - ymin) * yscale) + yoffset);

    if (i > 0)
       {
       xorline(iwin,   icontx,TRUE, ix1,iy1, ix2,iy2);
       xorline(imagsav,icontx,TRUE, ix1,iy1, ix2,iy2);
       }
    }

 return TRUE;
 }

/******************* plotvert ******************************************/

int plotvert(float * linearray, int nval, int iybot, int iytop, 
             float xoffset, float xscale, float xmin)
 {
 int   i, ix1, ix2;
 int   ixt;
 float lasttf, thistf;

 /* plot vertical line */
 for (i=0; i < nval*2; i+=2)
    {
    if (i > 0)
       {
       ix1    = ix2; 
       lasttf = thistf;
       }

    thistf = linearray[i+1];

    ix2  = (int) (((linearray[i]   - xmin) * xscale) + xoffset);

    if (i > 0 && 
       ((lasttf > 0.0 && thistf <= 0.0) || 
        (lasttf < 0.0 && thistf >= 0.0)))
       {
       ixt = ((ix2 - ix1) / 2.0) + ix1;

       xorline(iwin,   icontx,TRUE, ixt,iybot, ixt,iytop);
       xorline(imagsav,icontx,TRUE, ixt,iybot, ixt,iytop);
       }
    }

 return TRUE;
 }

 /***************************   ctfmen1  ****************************/

 void ctfmen1(void)

 { 
 Widget iw_dums1, iw_rowcolv1, iw_pushc1, iw_pusha1;
 
 if (iw_ctfmen1 <= (Widget) 0)
    {   /* create defocus menu widget first */

    iw_ctfmen1  = wid_dialog(iw_win, 0, "CFT modifcation", -1, -1);
    iw_rowcolv1 = wid_rowcol(iw_ctfmen1, 'v', -1, -1);

    /* create scale widget for defocus --------------------- defocusc */
    iw_defocusc = wid_scale(iw_rowcolv1, 0, "Defocus 0...70,000 (A): ",
                           0,70000, idefocusc,500,50,-1,-1); 

    /* create scale widget for fine defocus ---------------- defocusf */
    iw_defocusf = wid_scale(iw_rowcolv1, 0, "Defocus 0...1,000 (A): ",
                           0,1000,    idefocusf,500,50,-1,-1); 

    /* create scale widget for cutoff ----------------------- cutoff */
    iw_cutoff = wid_scale(iw_rowcolv1, 0, "Plot cutoff (%): ",0,100,
                           cutoff,300,50,-1,-1); 

    /* create box for apply ---------------------------------- apply */
    iw_dums1 = wid_stdbut(iw_rowcolv1, iw_ctfmen1, 
                        &iw_dums1, &iw_pushc1, &iw_pusha1, "SCA",
                        ctfmen_buts, fin_cb, ctfmen1_buta, NULL);

    XtAddCallback(iw_defocusc,XmNvalueChangedCallback,
                           (XtCallbackProc) ctfmen1_buta,"0");

    XtAddCallback(iw_defocusf,XmNvalueChangedCallback,
                           (XtCallbackProc) ctfmen1f_buta,"0");

    XtAddCallback(iw_cutoff,XmNvalueChangedCallback,
                           (XtCallbackProc) ctfmen1_buta,"0");
    }   

 XtManageChild(iw_ctfmen1);
 }


#ifdef NEVER
 
 /* set start label for axis */
 strcpy(cval,"0%");
 ixt = ixgo;
 witext(icontx,cval, ixt,iyt, 1, 0, 9, 2, FALSE);
 xorline(iwin,   icontx,TRUE, ixt,iygo, ixt,iyb);
 xorline(imagsav,icontx,TRUE, ixt,iygo, ixt,iyb);

 for (i=1; i < 10; i++)
    {
    sprintf(cval,"%2d",i*10);
    ixt = (int) (ixgo + xinc * i);
    witext(icontx,cval, ixt,iyt, 1, 0, 9, 2, FALSE);

    xorline(iwin,   icontx,TRUE, ixt,iygo, ixt,iyb);
    xorline(imagsav,icontx,TRUE, ixt,iygo, ixt,iyb);
    }

 /* set end label for axis */
 ixt = ixgo + (xinc * 10.0);
 strcpy(cval,"100%");
 witext(icontx,cval, ixt,iyt, 1, 0, 9, 2, FALSE);
 xorline(iwin,   icontx,TRUE, ixt,iygo, ixt,iyb);
 xorline(imagsav,icontx,TRUE, ixt,iygo, ixt,iyb);

#endif
