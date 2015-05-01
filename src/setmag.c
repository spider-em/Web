/*$Header: /usr8/web/src/RCS/setmag.c,v 1.8 2011/09/23 12:46:42 leith Exp $*/
/*
C++*************************************************************************
C
C setmag.c -- Created                                          Jul  92
C setmag.c                                                     Nov  92                                                                    
C
C **********************************************************************
C *  AUTHOR: YANHONG LI                                                      *
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
C    setmag(Widget iw_temp, XtPointer data, XtPointer call_data)
C
C    PARAMETERS:   iw_temp, data, call_data (usual callback data)
C
C    PURPOSE:      Set magnification parameters
C
C    CALLED BY:    web_main
C
C--********************************************************************
*/

#include "common.h"
#include <Xm/RowColumn.h>
#include <Xm/Scale.h>

#define  MAX_ARGS 13

 /* External function  prototypes */
 extern Widget wid_labelg   (Widget, Widget, char *, int, int);
 extern Widget wid_dialog   (Widget,...);
 extern Widget wid_stdbut   (Widget,...);
 extern void   fin_cb	    (Widget, caddr_t, caddr_t);
 
 /* Internal function  prototypes */
 Widget scalev	(Widget iw_parent,Widget iw_its,int imin,int imax,
		int inow,int iwid,int ihi,int ixdum,int iydum);

 void   pushbuta(Widget iw_temp, XtPointer data, XtPointer callback);

 /* Internal file variables */
 static  Widget	 iw_magx, iw_magy, iw_magf, iw_setmag = 0;

/***********************  setmag  **************************/

 void setmag(Widget iw_temp, XtPointer data, XtPointer call_data)
 {
 Widget	 iw_but, ipushc, ipusha, rowcolh;
 Arg     args[MAX_ARGS];         /* Arg list  */
 int     n;

 if (iw_setmag <= (Widget)0)
    {
    iw_setmag = wid_dialog( iw_main, 0, "setmag", 0, 0 );
 
    /* Create the labels of magsize and magfac	*/
    wid_labelg(iw_setmag, 0, "size",60, 30);
    wid_labelg(iw_setmag, 0, "x",40, 60);
    wid_labelg(iw_setmag, 0, "y",120, 60);
    wid_labelg(iw_setmag, 0, "factor ", 180, 30);

    /* Create the magsize scale	*/
    iw_magx  = scalev(iw_setmag,0, 2,512,magsizx,50,200, 10,80);
    iw_magy  = scalev(iw_setmag,0, 2,512,magsizy,50, 200,90,80);
    iw_magf  = scalev(iw_setmag,0, 2,32,magfac,55, 210,180, 70);

    /* Create standard buttons for set mag varibles */
    /* Set the position for standard buttons	*/
    n = 0;
    Setbgfg(args, MAX_ARGS, &n);       // Handles BGFG Args
    XtSetArg(args[n], XmNx, 10);                       n++;                
    XtSetArg(args[n], XmNy, 300);                      n++;
    XtSetArg(args[n], XmNdefaultPosition, False );     n++;              
    rowcolh = XmCreateRowColumn(iw_setmag, "sss", args, n);
    XtManageChild( rowcolh );

    /* Create the buttons	*/
    iw_but = wid_stdbut(rowcolh,iw_setmag, &ipushc,&ipushc,
		      &ipusha,"CA",fin_cb,fin_cb,pushbuta,NULL);
    }

    XtManageChild(iw_setmag);
 
 }

/*****************  scalev  ***************************/

 Widget scalev(Widget iw_parent,Widget iw_its,int imin,int imax,
		int inow,int iwid,int ihi,int ixdum,int iydum)
{   
    Arg          args[MAX_ARGS];    /* arg list  */
    Widget       iw_lab;
    Widget       iw_rowcolh;
    int          n;

    if (iw_its == (Widget)0) 
    { 
    	/* Create scale widget */
        n = 0;
        Setbgfg(args, MAX_ARGS, &n);       // Handles BGFG Args
    	XtSetArg(args[n], XmNwidth, iwid);              n++;                  
    	XtSetArg(args[n], XmNheight, ihi);              n++;              
    	XtSetArg(args[n], XmNscaleWidth, iwid-30);      n++;          
    	XtSetArg(args[n], XmNscaleHeight, ihi);         n++;       
    	XtSetArg(args[n], XmNminimum, imin);            n++;              
    	XtSetArg(args[n], XmNmaximum, imax);            n++;            
    	XtSetArg(args[n], XmNdecimalPoints, 0);         n++;            
    	XtSetArg(args[n], XmNvalue, inow);              n++;                
    	XtSetArg(args[n], XmNorientation, XmVERTICAL);  n++;       
    	XtSetArg(args[n], XmNshowValue, TRUE);          n++;         
     	XtSetArg(args[n], XmNx, ixdum);                 n++;                
    	XtSetArg(args[n], XmNy, iydum);                 n++;             

    	iw_its = XmCreateScale(iw_parent, "iw_scale", args, n);
    }

    else 
    { /* Alter scale size in existing widget */

    	XtSetArg(args[0], XmNminimum, imin);       
    	XtSetArg(args[1], XmNmaximum, imax);       
    	XtSetArg(args[2], XmNvalue,   inow);
       
    	XtSetValues(iw_its, args, 3);
    }

 XtManageChild(iw_its);
 return iw_its;
 }

 /*****************  pushbuta  ***************************/

 void  pushbuta(Widget iw_temp, XtPointer  data, XtPointer callback)
    {
    /* Get values of magsizx, magsizy and magfac	*/
    XmScaleGetValue(iw_magx,&magsizx);
    XmScaleGetValue(iw_magy,&magsizy);
    XmScaleGetValue(iw_magf,&magfac);

    XtUnmanageChild(iw_setmag);
    }



