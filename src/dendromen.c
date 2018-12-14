
/*
C**********************************************************************
C
C dendromen.c -- Adapted from DENDRO2.FOR 27 JUN  94 Lekan Odesanya
C                scale bar                   MAR  13 ArDean Leith
C
C**********************************************************************
C FORMS A TRUNCATED SCREEN PLOT FOR DENDROGRAM
C
C BASED ON ARBRE.FOR,  A DENDROGRAM PGM BY - JEAN-PIERRE BRETAUDIERE 
C UNIVERSITY OF TEXAS HEALTH SCIENCE CENTER AT HOUSTON MEDICAL SCHOOL - 
C DEPARTMENT OF PATHOLOGY AND LABORATORY MEDICINE              
C P.O. BOX 20708, HOUSTON, TX 77225.                                            
C*--------------------------------------------------------------------*         
*/      

#include <Xm/Scale.h>
#include <Xm/Text.h>
#include <Xm/ToggleBG.h>
/* Common variables */
#include "common.h"
#include "routines.h"
#include "fit.h"

 /* External global variables */

 /* Internal function prototypes */
 static void      dendromen_buta(Widget iw_temp, XtPointer, XtPointer);
 static void      dendro_pop    (Widget, XEvent *, String *, Cardinal *);
 static void      dendromen_d   (Widget, XtPointer, XtPointer);

 /* Internal common variables  */
 int            icut;
 int          * NUM,  newdoc, wantshow = FALSE;
 float        * V, * YT, *buf, vmin, vmax;
 char         * fname = NULL, * cbuf, filpat[81];
 static Widget  iw_dendromen = (Widget)0, 
                iw_patt      = (Widget)0, 
                iw_rc        = (Widget)0,
		iw_rowcolv   = (Widget)0, 
                iw_cut       = (Widget)0,
                iw_show      = (Widget)0;


void dendromen( char * dfil)
{
 char     cval[50], * cdum;
 int      maxreg, maxkeys;
 int      k, doc_read;
 float  * dbuf = NULL, *ptr;
 float    plist[8], val;
 Widget   iw_dums, iw_pusha, iw_pushc;
 int      icut;

 /* Set size for array pointed to by dbuf */
 maxreg    = 3+1; 
 maxkeys   = 9999;

 /* Tell it to open the doc files each time */
 doc_read = TRUE;

 if (unsdal(dfil, &doc_read, 0, plist, 2, &dbuf, &maxpart, maxkeys, maxreg) > 0) 
    {  /* Problem retrieving doc file, assume it does not exist */
    if (dbuf) free(dbuf); dbuf = NULL; return;
    }

 /* Allocate array space */
 if (((V =   (float *) realloc((void *) V,   maxpart * sizeof(float))) == 
             (float *) NULL) ||
     ((NUM = (int *)   realloc((void *) NUM, maxpart * sizeof(int))) == 
             (int *)   NULL) ||
     ((YT =  (float *) realloc((void *) YT,  maxpart * sizeof(float))) == 
             (float *) NULL))
   { spout("*** Unable to reallocate memory in dendromen."); return; }   

 newdoc = TRUE;
 ptr    = dbuf;
 
 // Initialize min/max values 
 NUM[0] = (int) *(ptr+3);                // Image id 
 V[0]   = vmin = vmax = val = *(ptr+2);  // Leaf height 
 ptr   += maxreg;

 
 for (k=1; k<maxpart; k++)
    {
    NUM[k]  = (int) *(ptr+3);   // Image id
    V[k]    = val = *(ptr+2);   // Leaf height
      
    if ( val < vmin ) vmin = val;
    if ( val > vmax ) vmax = val;
     
    ptr  += maxreg; 
    }

/* Strip pathname from filename  */
#ifdef SP_NT
     fname = strrchr(dfil,'\\');
#else
     fname = strrchr(dfil,'/');
#endif

 /* Free dbuf */
 if (dbuf) free(dbuf); dbuf = NULL;

 if (iw_dendromen <= (Widget)0)
    {  /* Create dendrogram menu widget first */

    iw_dendromen = wid_dialog(iw_win, 0, "Dendrogram Cutoff", -1, -1);
    iw_rowcolv   = wid_rowcol(iw_dendromen, 'v', -1, -1);

    /* Create horizontal rowcol for cutoff -----------------------------  */
    iw_rc  = wid_rowcol(iw_rowcolv, 'h', -1, -1);

    /* Create scale widget for cutoff  -------------------------- cutoff */
    icut = 10;
    iw_cut  = wid_scale(iw_rc, 0, "Cutoff(%)", 
                        0,100,  icut, 180,50, -1,-1);
    XtAddCallback(iw_cut,XmNvalueChangedCallback,
                 (XtCallbackProc) dendromen_buta,"0");

    /* Create toggle box for wantshow ---------------------------- wantshow */
    iw_show = wid_toggleg(iw_rowcolv, iw_show, "Show Averaged Images",
           wantshow,dendromen_d,cdum,-1,-1);

    /* Create text box for file pattern input  -------------- filpat */
    iw_patt = wid_textboxb(iw_rowcolv,iw_patt,"Template name of images:",filpat,12);

    /* if (!wantshow) XtUnmanageChild(XtParent(iw_patt)); */

    /* Create box for apply ----------------------------------- apply */
    iw_dums = wid_stdbut(iw_rowcolv, iw_dendromen, 
                        &iw_dums, &iw_pushc, &iw_pusha,  "CA",
                        fin_cb, fin_cb, dendromen_buta, NULL);
   }

 XtManageChild(iw_dendromen);
    if (!wantshow) XtUnmanageChild(XtParent(iw_patt));

}

 /************** dendromen_d (wantshow toggle callback) *************/

 void dendromen_d(Widget iw_temp, XtPointer data, XtPointer call_data)
  { 
  int showing;

  showing  = wantshow;
  wantshow = XmToggleButtonGadgetGetState(iw_show);

  if (wantshow && !showing)
     {
     /* XtManageChild(XtParent(iw_patt)); */
     XtManageChild(XtParent(iw_patt));
     }
  else if (!wantshow && showing)
     {
     /* XtUnmanageChild(XtParent(iw_patt)); */
     XtUnmanageChild(XtParent(iw_patt));
     }  
  } 

 /*********** Accept button callback **********************************/

 void dendromen_buta(Widget iw_temp, XtPointer data, XtPointer call_data)
 {
 XImage  *image;
 char    *string, str[5], filpatt[30];
 int      npix, ival, irect, nsr;
 float    fmint, fmaxt, scal, fm;
 float    pmin, xdis, yscale, vdiv;
 float    ypos, ylabel, ydiv, buffer[256];
 int      ii, x0, x1, y1, x2, y2, nkla, obj;
 int      i, j, k, yti, ytj, ht, wt, jgo, iform;
 int      icoloro;
 int      xll,yll, xul,yul,xlaboff, yoff ;


 // Get cutoff value (0...100)
 XmScaleGetValue(iw_cut, &icut);
 pmin = icut;

 if (wantshow) 
    { // Want to show image averages at bottem
    strcpy(filpat,XmTextGetString(iw_patt));
    if (strlen(filpat) == 0) 
       {spout("*** No file name template entered!");  return; }  
    } 

 /* Clear screen */
 webclear( NULL, NULL, NULL); 

 /* Show clock cursor (#79) */
 waitcursor(iwin,TRUE,79);

 if (wantshow)
    { // Want to show image averages at bottem
    strcpy(filpatt,filpat);
    filget(filpat,filpat,81,NUM[0]);

    /*  Add datexc (name extension ) to filpat if not already there  */
    if (strstr(filpat,datexc) == 0)
       {
       if ((strlen(filpat) + strlen(datexc) + 1) > 80)
          { spout("*** File name too long:"); spout(filpat); return;}  
       strcat(strcat(filpat,"."),datexc);
       }
    //printf("opening first: %d : %s, \n",NUM[0],filpat);

    /* Open the first one to get the dimensions of the images */
    filedata = openold(filpat,&nsam,&nrow,&nslice,&iform,"o"); 
    if (filedata == (void *)FALSE)
      { spout("*** Unable to open image file"); waitcursor(iwin,FALSE,0);   return;}
    closefile(filedata);

    nsr =  nsam * nrow;

    /* Allocate space for image arrays */
    if (newdoc)
       {
       newdoc = FALSE;
       if (((buf  = (float *) realloc((void *) buf, nsr * sizeof(float))) == 
                    (float *) NULL) ||
           ((cbuf = (char *) realloc((void *) cbuf, nsr * sizeof(char))) == 
                    (char *) NULL))
         { 
         spout("*** Unable to reallocate memory in dendromen_buta."); 
         waitcursor(iwin,FALSE,0); return; 
         }   
       }

    /* Initialize buf to zero  */
    for(i = 0; i < nsr; i++) *(buf+i) = 0.0;
    }  // End of: wantshow

  // Find window sizing:

  xlaboff = 60;             // offset from left axis label

  wt = 0.8 * iwidex - xlaboff -20 ;
  ht = 0.8 * ihighx ;  
  if (wantshow) ht = ht - nrow - 20; 

  xul = (iwidex - wt) / 2 ; 
  yul = (ihighx - ht) / 2 ;

  xll = xul ; 
  yll = yul + ht ;

  //printf("xul,yul: %d,%d  %d,%d, %d,%d, \n",xul,yul, ht,wt, xll,yll);

  /* Find y-scale for tree */
  yscale = (float) ht / (vmax - pmin); 

  /* Draw y-axis lines  */
  XDrawLine(idispl, iwin,    icontx, xll,yll,   xul,yul);
  XDrawLine(idispl, imagsav, icontx, xll,yll,   xul,yul);

  /* Label y-axis line  */
  ylabel  = pmin;  
  ypos    = yll;
  vdiv    = (vmax-pmin) / 5.0;  
  ydiv    = ht / 5.0;

  for (i = 0; i <= 5; i++)
     {
     XDrawLine(idispl, iwin,    icontx, xll,ypos,  xll+10,ypos);
     XDrawLine(idispl, imagsav, icontx, xll,ypos,  xll+10,ypos);
     sprintf(str,"%4.2f",ylabel);
     witext(icontx, str,  xll+10, ypos+3,  0,0, -9,2, FALSE);

     ylabel += vdiv;   
     ypos   -= ydiv;
     }

  nkla = 0;
  for( j = 0; j < maxpart; j++)
    {
    if (V[j] < pmin) continue;            /* Do not plot truncated lines */
    YT[nkla++] = (V[j] - pmin) * yscale;  /* Find node height (YT)       */
    }

  /* Find horizontal distance between leaves  */
  xdis = (float) wt / (nkla - 1.0);

  /* Center filename at the top */
  yoff = yul / 2; 
  witext(icontx, fname + 1, iwidex/2-25,yoff,  0,0, -9, 2, FALSE);
      
  obj = 1; jgo = 0;

  /* Start i sweep  */
  for ( i = 0; i < nkla; i++)
     {
     yti = YT[i];
     x0  = i * xdis + xlaboff;  // xlaboff is offset from left axis label
     x1  = i * xdis + xlaboff;  
     y1  = ht - yti;

     /* Draw vertical line to (X(i),yti)  */
     XDrawLine(idispl, iwin,    icontx, xul+x0, yul+ht,   xul+x1, yul+y1);
     XDrawLine(idispl, imagsav, icontx, xul+x0, yul+ht,   xul+x1, yul+y1);

     /* Determine and write # of objects for the tree node */
     for(j = jgo; j < maxpart; j++)
        {
        if (V[j] < pmin ) 
            obj++;
        else
           {
           if ( obj <= 9) 
              sprintf(str,"%1d",obj);
           else
              {
              if ( obj > 99)
                sprintf(str,"%3d",obj);
              else
                sprintf(str,"%2d",obj);
              }
              // Label for node
              witext(icontx, str, xul+x0-15,yul+ht+15,   0,0, -9,2, FALSE);
              obj = 1; 
 
            if ( wantshow )
              {              
              for(k = jgo; k <= j; k++)    /* Add the images */
                 {
                 strcpy(filpat,filpatt);
                 filget(filpat,filpat,81,NUM[k]);
                 // printf("***  %s %s \n", filpatt,filpat);
 
                 /*  Add datexc (name extension ) to filpat if needed  */
                 if (strstr(filpat,datexc) == 0)
                   {
                   if ((strlen(filpat) + strlen(datexc) + 1) > 80)
                     {spout("*** File name too long:"); spout(filpat); return; }  
                   strcat(strcat(filpat,"."),datexc);
                   }

                /* Try to open the file */
                filedata = openold(filpat,&nsam,&nrow,&nslice,&iform,"o"); 
                if (filedata == (void *)FALSE)
                  {
                  spout("*** Unable to open image file");
                  waitcursor(iwin,FALSE,0); return;
                  }

                npix = 0;  
                for (irect = 1; irect <= nrow; irect++)
                  {
                  if (redlin(filedata,buffer,nsam,irect) != TRUE) 
                      {
                      free(cbuf); free(buf); 
                      spout("redlin returned FALSE");
                      waitcursor(iwin,FALSE,0); return;
                      }

                  for(ii = 0; ii < nsam; ii++)
                     buf[npix++] += buffer[ii];
                  }
                 //printf("*** Summing k: %d  NUM: %d %s \n", k,NUM[k],filpat);
                 closefile(filedata);
                 }  /*  k  */


              /* Determine max and min of all images */
              fmint = fmaxt = buf[0] ;
              for(npix = 1; npix < nsr; npix++)
                 {
                 if (buf[npix] < fmint) fmint = buf[npix];
		 if (buf[npix] > fmaxt) fmaxt = buf[npix]; 
                 }
		 
              /* Conversion factor from floating point to logical * 1 RANGE */
              scal = (float) (imageend - imagego) / (fmaxt - fmint);
              fm   = -fmint * scal + imagego + 0.5;

              for(npix = 0; npix < nsr; npix++)
               {
                if (imageend < 128) 
                  { /* No need to convert numbers > 128 */   
                  ival = (*(buf + npix) * scal) + fm;
                  *(cbuf + npix) = ival;
                  }
                else
                  { /* Need to convert numbers > 128 */   
                  ival = (*(buf + npix) * scal) + fm;

                  /* convert 0..255  integers to -128...127 */
                  if (ival > 127) ival = ival - 256;
                     *(cbuf + npix) = ival;
                  }
               }

               /* Display the aaverage image */
               wipic(icontx,cbuf, xul+x0-nsam+15,yul+ht+40,  nsam,nrow,
                     TRUE,FALSE,&image);

               /* Initialize buf to zero  */
               for(ii = 0; ii < nsr; ii++)
                  *(buf+ii) = 0.0;

              } /* wantshow  */
             jgo = j + 1;  break;
         } /* else  */
      }  /*  for(j = jgo  */                               

     if (i == (nkla-1) ) break;  

   /* Find length of horizontal line   */
   for ( j = i+1; j < nkla; j++)
      {   
      ytj   = YT[j];
      if (ytj >= yti) 
         {
         /* Draw horizontal line to (X(j),yti)  */
         x2 = j * xdis + xlaboff;  
         y2 = ht - yti;
         XDrawLine(idispl, iwin,    icontx, xul+x1, yul+y1, xul+x2, yul+y2);
         XDrawLine(idispl, imagsav, icontx, xul+x1, yul+y1, xul+x2, yul+y2);
         break;
         }
      } /* for(j = i+1 */

  }  /*  i  */

 neednewmap = TRUE;

 /*  Remove clock cursor */
 waitcursor(iwin,FALSE,0);

 /* Display menu */
 showbutx("Recalls Menu.", 
          "Recalls Menu.",
          "Stops dendrogram.",FALSE); 

 /* Define buttons for menu retrieval or stop */
 actions(iw_win, "dendro_pop", dendro_pop, "123");

 }

 /*********** dendro_pop **********************************/

 void dendro_pop(Widget iw_t, XEvent *event, String *params,
                   Cardinal *num_params)
 {

 if (!(strcmp(*params,"3")))            /* Button 3 pushed */ 
    {

    /* uninstall translations */
    XtUninstallTranslations(iw_t);
   /* unmanage information box */
    showbutx("","","",TRUE);
    }

 else                                  /* Button 1 or 2 pushed */        
    {                                       
    XtManageChild(iw_dendromen);
    }

 }
