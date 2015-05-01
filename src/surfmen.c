
/*
C++********************************************************************
  *                                                                   *
  * surfmen  -- Created as wid_surfmen.f                              *
  *             Converted to C                  Oct 1992 ArDean Leith *
  *             Rewrite                         Sep 2011 ArDean Leith *
  *                                                                   *
C *********************************************************************
C=* AUTHOR: A. LEITH                                                  *
C=* FROM: WEB - VISUALIZER FOR SPIDER MODULAR IMAGE PROCESSING SYSTEM *
C=* Copyright 1985-2012  Health Research Inc.,                        *
C=* Riverview Center, 150 Broadway, Suite 560, Menands, NY 12204.     *
C=* Email:  spider@wadsworth.org                                      *
C=*                                                                   *
C=* This program is free software; you can redistribute it and/or     *
C=* modify it under the terms of the GNU General Public License as    *
C=* published by the Free Software Foundation; either version 2 of    *
C=* the License, or (at your option) any later version.               *
C=*                                                                   *
C=* This program is distributed in the hope that it will be useful,   *
C=* but without any warranty; without even the implied warranty of    *
C=* merchantability or fitness for a particular purpose.  See the GNU *
C=* General Public License for more details.                          *
C=* You should have received a copy of the GNU General Public License *
C=* along with this program. If not, see <http://www.gnu.org/licenses>*
C=*                                                                   *
C *********************************************************************
C
C    surfmen
C
C    PURPOSE:         Display surface menu
C
C    CALLED BY:       imagemen
C
C        v----------------imnum=1---------------------------^
C        v                                                  ^
C    imagemen --> wid_files --> imagemen_cb ---> imagemen1 -^
C                                                      v
C               v -------------(imnum=2)---------------'
C               v
C           surfmen --> surfmen_buta --> surfmovie
C               ^           v
C               ^           v
C               ^         surf ---------> surfcolor
C               ^           v                v
C               ^---------surf_pop        redvol
C                           v
C                         (end)   
C
C**********************************************************************
*/

#include <Xm/Scale.h>
#include <Xm/Text.h>
#include <Xm/ToggleBG.h>

#include "common.h"
#include "routines.h"

#define MAX_ARGS 5
#define EULER	 1
#define XYZ	 0

 /* Function prototypes */
 extern void   	surfmovie    (int);

 /* Internal function prototypes */
 void  surf_pop      (Widget, XEvent *, String *, Cardinal *);
 void  surfmen_butf  (Widget, XtPointer, XtPointer);
 void  surfmen_butc  (Widget, XtPointer, XtPointer);
 void  surfmen_buta  (Widget, XtPointer, XtPointer);
 void  angle_cb      (Widget, XtPointer, XtPointer);
 void  savesurf_cb   (Widget, XtPointer, XtPointer);
 void  surfmen_th_but(Widget, XtPointer, XtPointer);
 void  setangles     (int, float, float, float, float, float, float);
 
 /* Common variables defined elsewhere*/
 extern unsigned char * refmap;

 /* Common variables defined here*/
 float          phi, theta, psi, thlev;
 float          scalef, offsetf;  
 int            canrotate;
 int            nvolcolors = 3;
 Widget         iw_thlev   = (Widget) 0;      // Float threshold value
 int            ithreshmin = 0;
 int            ithreshmax = 255;

 /* Internal variables */
 static Widget   iw_scales[6],iw_angs[6],iw_angsxyz[6],iw_rc,iw_mov;
 static Widget   iw_refl, iw_fct, iw_fast;
 static Widget   iw_pushf, iw_pushc, iw_pusha, iw_sc, iw_angle;
 static Widget   iw_lab, iw_lab_nr, iw_rowcolh1, iw_rowcolh2, iw_rowcolv;
 static Widget	 iw_euler, iw_xyz;
 static Widget	 iw_text;		   /* Filename widget */
 static Widget   iw_surfmen   = (Widget) 0;
 static Widget   iw_thlevscal = (Widget) 0;    
 static Widget   iw_colors    = (Widget) 0;

 static int      ithresh;
 static int      sirdit, distit, colorit;
 static float	 ang_x, ang_y, ang_z;
 static int	 first = TRUE;
 char            cval[15];
 Arg             args[MAX_ARGS];

 /***************************   surfmen   ****************************/

 void surfmen(int sirditt, int distitt, int coloritt)

 { 
 int     i, n, ifct, irefl, iscale;
 Widget  iw_labb1,  iw_dum,  iw_dums;
 char    cval[50];
 
 /* Set flags for sirds or distance display */
 sirdit = sirditt;   distit = distitt; colorit = coloritt;

 //printf("colorit:%d\n",colorit);

 canrotate = TRUE;
 if (nsam * nrow * nslice > 512 * 512 * 512)
    {   /* Probably too big to allocate rotation memory */
    canrotate = FALSE;
    }

 // Conversion factor from floating point to integer
 scalef   = (float) (ithreshmax - ithreshmin) / 
           (filedata->fmax_spi - filedata -> fmin_spi);
 offsetf = -filedata->fmax_spi * scalef + ithreshmax + 0.5;

 if (iw_surfmen <= (Widget)0)
    {   /* Create surface menu widget first */

    iw_surfmen  = wid_dialog(iw_win, 0, "Surface options", -1, -1);
    iw_rowcolv  = wid_rowcol(iw_surfmen, 'v', -1, -1);

    phi      = 0.0;
    theta    = 0.0;
    psi      = 0.0;
    gotsurf  = FALSE;

    /* Create label box for view dir.  ---------------------- View dir */
    sprintf(cval," Views top of volume now!! (nsam x nrow plane) ");
    iw_labb1  = wid_labelg(iw_rowcolv, 0, cval, -1, -1);

    /* Create horizontal rowcol for range and fast  */
    iw_rowcolh1  = wid_rowcol(iw_rowcolv, 'h', -1, -1);

    /* Create label box for image range ------------------------ Range */
    sprintf(cval,"Intensity range:  %f ...%f",filedata->fmin_spi,
                                       filedata->fmax_spi);
    iw_lab  = wid_labelg(iw_rowcolh1, 0, cval, -1, -1);

    /* Create label box for spacing */
    wid_labelg(iw_rowcolh1, 0,"   ", -1, -1);

    /* Create toggle box for movie ----------------------------- Movie */
    iw_mov = wid_togmen(iw_rowcolh1,0,"MOVIE",False,0,0);

    /* Create label box for spacing */
    iw_dum = wid_labelg(iw_rowcolh1, 0,"   ", -1, -1);

    /* Create toggle box for angle conversion ------------- Angle type */
    iw_angle = wid_toggleg (iw_rowcolh1,0,"Euler angles",
                            False,angle_cb,"E",0,0);

    /* Create horizontal rowcol for threshold and scaling */
    iw_rc  = wid_rowcol(iw_rowcolv, 'h', -1, -1);

    /* Create scale widget for threshold ------------------- Threshold */
    ithresh = (ithreshmax - ithreshmin) / 2;
    iw_thlevscal = wid_scale(iw_rc, 0, "Threshold: ",
                      ithreshmin,ithreshmax,ithresh,200,50,-1,-1); 

    XtAddCallback(iw_thlevscal,XmNvalueChangedCallback,
                 (XtCallbackProc) surfmen_th_but,"0");

    // Create text box for thresh., default thresh. is mid range 
    thlev = filedata -> fmin_spi + 
           ((filedata->fmax_spi - filedata->fmin_spi) / 2.0);
    sprintf(cval,"%11g",thlev);
    iw_thlev = wid_textboxb(iw_rc,iw_thlev,"",cval,12);

    /* Create scale widget for size scaling ------------------ Scaling */
    iscale = scaleval * 10;
    iw_sc = wid_scale(iw_rc, 0,"Size\nScaling: ",10,200,
                      iscale,250,50,-1,-1); 

    XtSetArg(args[0], XmNdecimalPoints, 1);
    XtSetValues(iw_sc, args, 1);
    XtAddCallback(iw_sc,XmNvalueChangedCallback,
                 (XtCallbackProc) surfmen_buta,"0");

    if (colorit) 
       {
       /* Create text box  for colors    ---------------------  Colors */
       sprintf(cval,"%3d",nvolcolors);
       iw_colors = wid_textboxb(iw_rowcolv,0,
                                "Number of colors:",cval,10);
       }

    /* Create horizontal rowcol for refl and fct  */
    iw_rowcolh2  = wid_rowcol(iw_rowcolv, 'h', -1, -1);

    /* Create scale widget for % reflection --------------------- Refl */
    irefl   = 100 * refl;   
    iw_refl = wid_scale(iw_rowcolh2, 0, "% Reflection ", 
                        0,100,  irefl,  180,50, -1,-1);
    XtAddCallback(iw_refl,XmNvalueChangedCallback,
                 (XtCallbackProc) surfmen_buta,"0");

    /* Create scale widget for fct  ------------------------------ Fct */
    ifct = 100.0 * fct;
    iw_fct  = wid_scale(iw_rowcolh2, 0, "Depth Attenuation(%)", 
                        0,100,  ifct, 180,50, -1,-1);
    XtAddCallback(iw_fct,XmNvalueChangedCallback,
                 (XtCallbackProc) surfmen_buta,"0");

    /* Create scale widgets for windowing volume -------------- Scales */
    for (i=0; i < 6; iw_scales[i++]  = (Widget) 0);
    iw_dums = wid_win(iw_rowcolv, &iw_scales[0], 
                       nsam, nrow, nslice);

    for (i=0; i < 6; i++)
       {
       XtAddCallback(iw_scales[i],XmNvalueChangedCallback,
                    (XtCallbackProc) surfmen_buta,"0");
       }

    if (canrotate)
       {
       /* Create scale widgets for angles --------------------- Angles */
       for (i=0; i < 6; iw_angs[i++]  = (Widget) 0);
       iw_euler = wid_angles(iw_rowcolv, &iw_angs[0], EULER,
                     (int) phi, (int) theta, (int) psi, -1,-1);

       /* Create a sub-widget of XYZ angles for later use */
       for (i=0; i < 6; iw_angsxyz[i++]  = (Widget) 0);
       iw_xyz = wid_angles(iw_rowcolv, &iw_angsxyz[0],XYZ, 
                 (int) ang_x, (int) ang_y, (int) ang_z, -1,-1);

       for (i=0; i < 6; i++)
          {
          XtAddCallback(iw_angs[i],XmNvalueChangedCallback,
                       (XtCallbackProc) surfmen_buta,"0");
          XtAddCallback(iw_angsxyz[i],XmNvalueChangedCallback,
                       (XtCallbackProc) surfmen_buta,"0");
          }

       XtUnmanageChild(iw_xyz);
       }
    else
       {  /* Create non-rotate label */
       for (i=0; i < 3; i++)
          XtUnmanageChild(iw_angs[i]);
       iw_lab_nr  = wid_labelg(iw_rowcolv, 0, 
                             "Too large to rotate", -1, -1);
       }

    /* Create box for apply ----------------------------------- Apply */
    iw_dums = wid_stdbut(iw_rowcolv, iw_surfmen, 
                 &iw_pushf, &iw_pushc, &iw_pusha, "KCA",
                 surfmen_butf,surfmen_butc, surfmen_buta, NULL);
    }   /* (iw_surfmen <= (Widget)0)  */

 else
    {   /* Surface menu widget already exists, update it */

    /* Update label box for image range ----------------------  Range */
    sprintf(cval,"Range:     %f ...%f",filedata->fmin_spi,
                                       filedata->fmax_spi);
    iw_lab  = wid_labelg(iw_rowcolh1, iw_lab, cval, -1, -1);


    //printf(" nsam: %d %d %d %d\n", nsam,nrow,iw_scales[0],iw_scales[1]);

    /* Update scale widgets for windowing volume ------------- Scales */
    iw_dums = wid_win(iw_rowcolv, &iw_scales[0], 
                       nsam, nrow, nslice);

    /* Update scale for threshold ------------------------  Threshold */
    if (!gotsurf)
       {
       /* Must reset default threshold */
       ithresh = (ithreshmax - ithreshmin) / 2;
       iw_thlevscal = wid_scale(iw_rc, iw_thlevscal, "Threshold: ",
                ithreshmin,ithreshmax,ithresh,200,50,-1,-1); 

       thlev = filedata -> fmin_spi + 
              ((filedata->fmax_spi - filedata->fmin_spi) / 2.0);
       sprintf(cval,"%11g",thlev);
       iw_thlev = wid_textboxb(iw_rc,iw_thlev,"",cval,12);
       }

    if (canrotate)
       {
       /* Update scale widgets for angles -------------------- Angles */
       if (XmToggleButtonGadgetGetState(iw_angle)) 
	   {XtManageChild(iw_xyz);}
       else
	   {XtManageChild(iw_euler);}       
       }
    else
       {  /* Update non-rotate label */
       for (i=0; i < 3; i++)
          XtUnmanageChild(iw_angs[i]);

       iw_lab_nr  = wid_labelg(iw_rowcolv, iw_lab_nr, 
                             "Too large to rotate", -1, -1);
       }
    }   

 XtManageChild(iw_surfmen);
 }

 /************* Threshold slider callback *****************************/

 void surfmen_th_but(Widget iw_temp, XtPointer data, 
                                     XtPointer call_data)

 {
 int  n;

 // Get current threshold scale value (ithreshmin ... ithreshmax)
 XmScaleGetValue(iw_temp, &ithresh);

 // Update thlev
 thlev = filedata->fmin_spi + 
           (filedata->fmax_spi - filedata->fmin_spi) *
           ((float)ithresh / (float) (ithreshmax - ithreshmin));

 // Update surfmen text box for thlev
 sprintf(cval,"%11g",thlev);
 n = 0;
 XtSetArg(args[n], XmNvalue, cval);         n++;
 XtSetArg(args[n], XmNcolumns, (short) 12); n++;
 XtSetValues(iw_thlev, args, n);

 // Apply threshold and draw 
 surfmen_buta(NULL, NULL, NULL);
 }

 /*********** File save button callback ********************************/

 void surfmen_butf(Widget iw_temp, XtPointer data, 
                                   XtPointer call_data)
  {
  /* Input the filename*/
  iw_text = wid_text(iw_win, (Widget)0, 
                  "Enter the filename:", docnam,
                  savesurf_cb, NULL, ixulw+nsamw, iyulw+nroww);
  }


 /*********** Cancel button callback **********************************/

 void surfmen_butc(Widget iw_temp, XtPointer data, 
                                   XtPointer call_data)
   {
   if (first)
      {
      if ( filedata && filedata->fp ) 
           closefile(filedata);
      }
   XtUnmanageChild(iw_surfmen);
   }

 /*********** Accept button callback **********************************/

 void surfmen_buta(Widget iw_temp, XtPointer data, 
                                   XtPointer call_data)
 {
 char * string;
 int    iphi, itheta, ipsi, irefl, ifct, iscale;
 int    iang_x,iang_y,iang_z; 		/* XYZ angles */
 int    i, iang[6];
 float  ang1[3], ang2[3], ang3[3], *angtmp, fiang[6];
 float  anga[3], angb[3], angc[3], angd[3], ange[3];
 float  t0,t1,t2;
 float  phi1,theta1,psi1;
 float  phi2,theta2,psi2;
 float  phi3,theta3,psi3;
 char   output[100];

 /* Set the first open flag*/
 first = FALSE;
 
 /* Get nsam1 & nsam2 */
 XmScaleGetValue(iw_scales[0],&nsam1);
 XmScaleGetValue(iw_scales[1],&nsam2);
 if (nsam2 < nsam1)
    { spout("*** ENDING COL MUST BE > STARTING COL"); return; }

 /* Get nrow1 & nrow2 */
 XmScaleGetValue(iw_scales[2],&nrow1);
 XmScaleGetValue(iw_scales[3],&nrow2);
 if (nrow1 > nrow2)
    { spout("*** ENDING ROW MUST BE > STARTING ROW"); return; }

 /* Get nslice1 & nslice2 */
 XmScaleGetValue(iw_scales[4],&nslice1);
 XmScaleGetValue(iw_scales[5],&nslice2);
 if (nslice1 > nslice2)
    { spout("*** ENDING SLICE MUST BE > STARTING SLICE"); return; }

 if (canrotate)
    {
    /* Get rotation angles */

    if (XmToggleButtonGadgetGetState(iw_angle))
        {                                  /* XYZ angles */
        for (i=0; i<6; i++) 
    	    XmScaleGetValue(iw_angsxyz[i],&iang[i]);
	    
        for (i=0; i<3; i++) 
            {
	    ang1[i] = (float)iang[i];
	    ang2[i] = (float)iang[i+3];
	    }

        t0      = ang1[0];
        t1      = ang1[1];
        t2      = ang1[2];

	//ang1[0] = t0; // z
	//ang1[1] = t1; // x
	//ang1[2] = t2; // y

	//ang1[0] = t1; // y
	//ang1[1] = t2; // z
	//ang1[2] = t0; // x

	ang1[0] = t2; // z-->x
	ang1[1] = t0; // x-->y
	ang1[2] = t1; // y-->z
        //printf(" 0Euler: %f %f %f\n",ang1[0],ang1[1],ang1[2]);

    	xyz_to_eul(&ang1[0], &ang1[1], &ang1[2]);
    	xyz_to_eul(&ang2[0], &ang2[1], &ang2[2]);
        //printf(" 1Euler: %f %f %f\n",ang1[0],ang1[1],ang1[2]);

        angtmp = cald(ang2, ang1);  // Combine euler angles
	for (i=0; i<3; i++)
            {
	    ang3[i] = *angtmp; // Just sets ang3 = angtmp!!?
	    angtmp++;
	    }

	phi = ang3[0]; theta = ang3[1]; psi = ang3[2];
        //printf(" 2Euler: %f %f %f\n",phi,theta,psi);
	
    	eul_to_xyz(&ang3[0], &ang3[1], &ang3[2]);
	ang_x = ang3[0]; ang_y = ang3[1]; ang_z = ang3[2];

        //printf(" 3Euler: %f %f %f\n",phi,theta,psi);

        /* Reset the angle values */
        //setangles(XYZ, ang_x, ang_y, ang_z, 0, 0, 0);
    	}
    else
        {                               /* Euler angles */
        for (i=0; i<6; i++)
    	    {XmScaleGetValue(iw_angs[i],&iang[i]);}
	    
        for (i=0; i<3; i++)
            { // Convert scale values to float 
	    ang1[i] = (float)iang[i];   // 1..3
	    ang2[i] = (float)iang[i+3]; // 4..6
	    }

        // Phi is now rotation around x
        anga[0]=ang1[0];  anga[1]=0;  anga[2]=0;
      	xyz_to_eul(&anga[0], &anga[1], &anga[2]);

        // Theta is now rotation around 
        angb[0]=0;  angb[1]=0;  angb[2]=ang1[1];
       xyz_to_eul(&angb[0],&angb[1], &angb[2]);

        // Psi is now rotation around x
        angc[0]=ang1[2];  angc[1]=0;  angc[2]=0;
        xyz_to_eul(&angc[0],&angc[1],&angc[2]);

        //printf(" aEuler: %f %f %f\n",anga[0],anga[1],anga[2]);
        //printf(" bEuler: %f %f %f\n",angb[0],angb[1],angb[2]);
        //printf(" cEuler: %f %f %f\n",angc[0],angc[1],angc[2]);

        // This order gives wrong summation
        //angtmp = cald(anga, angb);  // Combine euler angles a+b->a
        //angtmp = cald(anga, angc);  // Combine euler angles a+c->a
        //phi = anga[0]; theta = anga[1]; psi = anga[2];

        angtmp = cald(angb, anga);  // Combine euler angles b+a->b
        angtmp = cald(angc, angb);  // Combine euler angles a+v->c
	phi = angc[0]; theta = angc[1]; psi = angc[2];
          
        //printf(" Euler:  %f %f %f\n\n",phi,theta,psi);

	if (ang2[0]!=0 || ang2[1]!=0 || ang2[2]!=0) 
            {	// Increment is not zero, add it to current angle

    	    xyz_to_eul(&ang2[0], &ang2[1], &ang2[2]); // Returns:phi...
            angtmp = cald(ang2, angc);  // Combine euler angles

	    phi = ang2[0]; theta = ang2[1]; psi = ang2[2];
	    }

        /* Reset the angle values */
        //setangles(EULER, phi, theta, psi, 0, 0, 0);
	}
    }

 /* Get scaling value */
 XmScaleGetValue(iw_sc,&iscale);
 scaleval = iscale / 10.0;

 /* Get reflection percentage */
 XmScaleGetValue(iw_refl,&irefl);
 refl = (float) irefl / 100.0;

 /* Get fct percentage */
 XmScaleGetValue(iw_fct,&ifct);
 fct = (float) ifct / 100.0; 

 /* Find threshold */
 string = XmTextGetString(iw_thlev);
 sscanf(string,"%f",&thlev);
 if (thlev > filedata -> fmax_spi) thlev = filedata -> fmax_spi;
 if (thlev < filedata -> fmin_spi) thlev = filedata -> fmin_spi;
 if (string) free(string);

 if (colorit) 
    {   /* Get number of colors to be used on surface */
    string = XmTextGetString(iw_colors);
    sscanf(string,"%d",&nvolcolors);
    if (nvolcolors < 1)
       { spout("*** NUMBER OF SURFACE COLORS MUST BE > 0"); return; }
    if (nvolcolors > 99)
       { spout("*** NUMBER OF SURFACE COLORS MUST BE < 100"); return; }
    if (nvolcolors > 7)
       { spout("*** SURFACE COLORMAP ONLY SUPPORTS 7 COLORS!!");}
    }

 /* Refresh screen */
 expos_cb(NULL,NULL,NULL); 
 XFlush(idispl);

 XmUpdateDisplay(iw_win);

 /* Display surfaces */
 if (XmToggleButtonGadgetGetState(iw_mov))
     {surfmovie(canrotate);}
 else
   {
   sprintf(output,
        "Phi:%d Theta:%d Psi:%d  Scale:%4.1f  Threshold:(%f,%d)",
        (int)phi, (int)theta, (int)psi, scaleval, thlev, ithresh);
   spout(output);

   surf(canrotate,sirdit,distit,colorit);

   /* Display menu */
   showbutx("Recalls Menu.", 
          "Save the current image.",
          "Stop surfacing.",FALSE); 

   /* Define buttons for menu retrieval or stop */
   actions(iw_win, "surf_pop", surf_pop, "123");
   }
 }

 

 /*************************** surf_pop *********************************/

 void surf_pop(Widget iw_t, XEvent *event, String *params,
                   Cardinal *num_params)
 {

 if (!(strcmp(*params,"1")))                  /* Button 1 pushed */ 
    {                                       
    surfmen(sirdit, distit, colorit);
    }

 else if (!(strcmp(*params,"2")))             /* Button 2 pushed */ 
    {
    /* Input the filename*/
    iw_text = wid_text(iw_win, (Widget)0, 
                    "Enter the filename:", docnam,
                    savesurf_cb, NULL, 
                    ixulw+nsamw, iyulw+nroww);
    }
                                       
 else if (!(strcmp(*params,"3")))	      /* Button 3 pushed */ 
    {
    /* Uninstall translations */
    XtUninstallTranslations(iw_t);
 
    if ( filedata && filedata->fp!=NULL)
        {
	closefile(filedata);	   
        first = TRUE;
        }

    /* Unmanage information box */
    showbutx("","","",TRUE);
    }
 }

 /*************************** savesurf_cb ******************************/
 void savesurf_cb(Widget iw_temp, XtPointer data, 
                                  XtPointer call_data)
   {
   float        jslope, kslope;
   int		nsams,nrows;
   float        *bufs;
   int          i, j, k, kk, nlet, numfil, irec;
   FILEDATA     *filedatat;
   char		output[100];

   XmSelectionBoxCallbackStruct * cbs;

   cbs = (XmSelectionBoxCallbackStruct *) call_data;

   /* Check the input name */
   strcpy(docnam,extractstr(cbs -> value));

   /* Check input filename */
   if (strlen(docnam) == 0) 
       {
       spout("*** Must specify name for the file.");
       return;
       }

   /* Add datexc to docnam if not already there */
   if (strstr(docnam,datexc) == 0)
      {
      strcat(docnam,".");
      strcat(docnam,datexc);
      }                      
         
  XtUnmanageChild( iw_text );

  /* Find reflection image size */
  jslope = (1.0 - nrow*scaleval)/(1.0 - nrow);
  kslope = (1.0 - nslice*scaleval)/(1.0 - nslice);
  nsams  = (nrow2 - nrow1)*jslope + 1;
  nrows  = (nslice2 - nslice1)*kslope + 1;

  /* Locate the space for the saving image */
  if((bufs = (float *) malloc(sizeof(float)*nsams)) == NULL)
    {
    spout("*** Allocation failed for writing to file in surfmovie!");
    return;
    }

  /* Open new output file */
  filedatat = opennew(docnam, nsams, nrows, 1, 1, "u");  

  /* Put newimage in output file */
  kk = 0;
  for (irec = 1; irec <= nrows; irec++)
     {
     for(k = 0; k < nsams; k++) bufs[k] = (float) refmap[kk++];

     if (!wrtlin(filedatat, bufs, nsams, irec)) 
        {
	sprintf(output,"*** Error writing to file in %s!", docnam);
	spout(output);
        closefile(filedatat); free(bufs); return;
        }
     }

  closefile(filedatat);     
  free(bufs);
}

// *********************** angle_cb ****************************

// Convert between Euler angles and XYZ angles

void angle_cb(Widget iw_temp, XtPointer data, 
                              XtPointer callback)
    {
    Arg        	args[MAX_ARGS];   	/* Arg list */
    int		iphi,itheta,ipsi; 	/* Euler angles */
    int		iang_x,iang_y,iang_z; 	/* Xyz angles */
    int		icang[3],i; 		/* Increased angles */
    float	fcang[3]; 		/* Increased angles float*/

    if (XmToggleButtonGadgetGetState(iw_angle))
        {    /* Using XYZ angle */
	/* Set label for toggle button */
 	XtSetArg(args[0], XmNlabelString, XmStringCreate("XYZ Angles",
                              XmSTRING_DEFAULT_CHARSET));
	XtSetValues(iw_angle, args, 1);
	
	/* Calculate the xyz angle */
    	XmScaleGetValue(iw_angs[0],&iphi);
    	XmScaleGetValue(iw_angs[1],&itheta);
    	XmScaleGetValue(iw_angs[2],&ipsi);
    	phi   = (float) iphi;		ang_x = phi;
    	theta = (float) itheta;		ang_y = theta;
    	psi   = (float) ipsi; 		ang_z = psi;

	eul_to_xyz(&ang_x, &ang_y, &ang_z);

	/* Get the increased angles */
	for (i=0; i<3; i++)
            {
    	    XmScaleGetValue(iw_angs[i+3],&icang[i]);
	    fcang[i] = (float) icang[i];
	    }

	eul_to_xyz(&fcang[0], &fcang[1], &fcang[2]);

	setangles(XYZ, ang_x,ang_y,ang_z, 
                  fcang[0], fcang[1], fcang[2]);

	XtUnmanageChild(iw_euler);
	XtManageChild(iw_xyz);
        
        printf(" To Euler: %f %f %f\n",phi,theta,psi);
	}
    else
        {    /* Using Euler angle */
 	XtSetArg(args[0], XmNlabelString, XmStringCreate("Euler",
                              XmSTRING_DEFAULT_CHARSET));
	XtSetValues(iw_angle, args, 1);

	/* Calculate the xyz angle */
    	XmScaleGetValue(iw_angsxyz[0],&iang_x);
    	XmScaleGetValue(iw_angsxyz[1],&iang_y);
    	XmScaleGetValue(iw_angsxyz[2],&iang_z);
    	ang_x = (float)iang_x;	phi   = ang_x;
    	ang_y = (float)iang_y;	theta = ang_y;
    	ang_z = (float)iang_z;  psi   = ang_z;

	xyz_to_eul(&phi, &theta, &psi);

	/* Get the increased angles */
	for (i=0; i<3; i++)
           {
    	   XmScaleGetValue(iw_angs[i+3],&icang[i]);
	   fcang[i] = (float) icang[i];
	   }

	xyz_to_eul(&fcang[0], &fcang[1], &fcang[2]);

	setangles(XYZ, ang_x,ang_y,ang_z, 
                  fcang[0], fcang[1], fcang[2]);

	XtUnmanageChild(iw_xyz);
	XtManageChild(iw_euler);

        printf(" To XYZ: %f %f %f\n",phi,theta,psi);
	}
   }

 /*************************** setangles ********************************/

void setangles(int angtype, float t1,float t2,float t3, 
	     		    float t4,float t5,float t6)
    {
    Arg        	args[MAX_ARGS];   	/* Arg list */
    float	angles[6];
    int 	i;

    angles [0] = t1;
    angles [1] = t2;
    angles [2] = t3;
    angles [3] = t4;
    angles [4] = t5;
    angles [5] = t6;

    for (i=0; i<6; i++)
        {
	XtSetArg(args[0], XmNvalue, angles[i]);

    	if (angtype) 	   /* Euler angtype */
	   {XtSetValues(iw_angs[i], args, 1);}

    	else 		  /* XYZ angtype */
	   { XtSetValues(iw_angsxyz[i], args, 1);}
	}
   }
	
