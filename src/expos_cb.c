
/*
 * expos_cb.c
 *                 backclear added              Feb 2011 ArDean Leith  *
 *                 True color                   Sep 2011 ArDean Leith  *
 ***********************************************************************
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
 C=* but WITHOUT ANY WAR//RANTY; without even the implied warranty of    *
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
 * expos_cb(iw_temp, data, call_data)
 * 
 * PURPOSE:     Handles exposures on draw window 
 *
 * RETURNS:     void
 * 
 * PARAMETERS:        
 *
 ***********************************************************************
*/

#include "x.h"
#include "std.h"

 // Functions used here
 void         spout(char *);
 extern void  backclear(Pixmap imagesavn, 
                        unsigned int iwidex, 
                        unsigned int highx);

 // External common variables
 extern Display        *idispl;
 extern Drawable       iwin, imagsav;
 extern unsigned int   iwidex, ihighx;
 extern int            ixulx,iyulx;
 extern GC             icontx, icontxb;
 extern int            drawing;

 extern int            icolorxb; 
 extern int            mapgo,mapend;       // Start & end in colormap 
 extern int            ngo,nend;           // Image start & end in colormap 

 // Internally defined common variables
 int                   vmap[256];          // Virtual colormap
 int                   colvmap[4][256];    // Virtual colormap

 unsigned int          iwidexb    = 0;
 unsigned int          ihighxb    = 0;
 Boolean               neednewmap = TRUE;  // Flag to create vimage
 Boolean               use3colors = FALSE; // Future Flag for cmap

/**********************  expos_cb **********************************/

#ifdef WEB_TRUE
// WEB_TRUE -----------------------------------------------------------

 XImage        *vimage = NULL;    // Virtual image
 Pixmap        imapsav = (Pixmap)NULL;    // Image backing store

 void expos_cb(Widget iw_temp, XtPointer data, XtPointer call_data)

 { 
 int           i,j,k,m;
 Window        iroot;
 unsigned int  iwidext,ihighxt,iborder,idepth,ixd,iyd;
 Pixmap        imagsavn;
 unsigned long ip_mask;            // For image ramp
 unsigned char cvalueold,cvaluenew;
 int           ipad;

 //printf(" use3col: %d  \n",use3colors);


 if (XGetGeometry(idispl,iwin,&iroot,&ixulx,&iyulx,&iwidext,&ihighxt,
                  &iborder,&idepth) == 0) 
     spout("*** Error getting geometry in expos_cb");

 if (iwidext > iwidexb || ihighxt > ihighxb)
    {
    /* Need to enlarge backing store for images */

    imagsavn = XCreatePixmap(idispl,iroot,iwidext,ihighxt,idepth);
    if ((int) imagsavn <= 0)  
      { spout("*** Can not create new backing store!"); return; } 

    if (imapsav != (Pixmap) NULL) XFreePixmap(idispl,imapsav);
    imapsav = XCreatePixmap(idispl,iroot,iwidext,ihighxt,idepth);
    if ((int) imagsavn <= 0)  
      { spout("*** Can not create new map backing store!"); return; } 

    // Clear background of new Pixmap   al 2011
    backclear(imagsavn,iwidext,ihighxt);
    //backclear(imapsav,iwidext,ihighxt);
    
    /* Copy old backing store to new backing store */
    ixd = MYMIN(iwidexb,iwidext);
    iyd = MYMIN(ihighxb,ihighxt);

    XCopyArea(idispl,imagsav,imagsavn,icontx,0,0,ixd,iyd,0,0);
    XFreePixmap(idispl,imagsav);
    imagsav = imagsavn;

    iwidexb = iwidext;
    ihighxb = ihighxt;

    //printf(" Resized backing store: %d x %d \n",iwidexb,ihighxb);

    if (vimage != NULL) XDestroyImage(vimage);
    vimage = NULL;
    }

 if (iwidext != iwidex || ihighxt != ihighx)
    {  /* Size of window changed but not larger than backing store */
    /* Remember size of current image window */
    iwidex = iwidext;
    ihighx = ihighxt;
    //printf(" Size changed: %d x %d \n",iwidex,ihighx);

    if (vimage != NULL) XDestroyImage(vimage);
    vimage = NULL;
    }

 //printf("mapgo: %d, mapend: %d  \n",        mapgo,    mapend);

 if (use3colors)
    {
    /* Just copy un-mapped backing store to image window */
    printf(" Using truecolor 3 color virtual colormap \n");

    if (vimage == NULL || neednewmap) 
       {
       if (vimage != NULL) XDestroyImage(vimage);
       //printf("New colormapped backing store: %d %d \n",neednewmap,vimage);

       // Get image from standard backing store.
       ip_mask = -1;                     
       vimage  = XGetImage(idispl, imagsav, 0,0, iwidex,ihighx,
                          ip_mask, ZPixmap);
       //printf(" vimage: bpp=%d x %d %d \n",
       //     vimage->bits_per_pixel, vimage->width, vimage->height);
       if (vimage == (XImage *) NULL) 
          { spout("*** Virtual colormap image formation failed"); return; }

       // Apply virtual colormap to image
       m = 0;
       for (i=1; i <= iwidex; i++)
          {
          for (j=0; j < ihighx; j++)   
             {
             for (k=0; k < 4; k++)   
                {
                cvalueold       = (unsigned char) vimage->data[j+k];
                cvaluenew       = colvmap[k][cvalueold];
                vimage->data[m] = cvaluenew;
                m++;
                }  // End of: for (k=0; j... 
             }     // End of: for (j=0; j... 
          }        // End of: for (i=1; i... 

       // Put virtual colormapped image in cmap backing store: imapsav
       XPutImage(idispl,imapsav, icontx,vimage,0,0, 0,0, iwidex, ihighx); 

       }        // End of: if (vimage == NULL...
    }

 else if (ngo == 0 && nend == 255)
    {
    /* Just copy un-mapped backing store to image window */
    XCopyArea(idispl,imagsav,iwin,icontx,0,0,iwidex,ihighx,0,0);
    //printf(" Using normal map: %d x %d \n",iwidex,ihighx);
    }
 else
    { // Must use vmap on imagsav backing store 

    //printf(" Using vmap: %d x %d \n",iwidex,ihighx);

    if (vimage == NULL || neednewmap) 
       {  // Create new colormapped backing store (size changed or imagesav changed)

       if (vimage != NULL) XDestroyImage(vimage);
       //printf("New colormapped backing store: %d %d \n",neednewmap,vimage);

       // Get image from standard backing store.
       ip_mask = -1;                     
       vimage  = XGetImage(idispl, imagsav, 0,0, iwidex,ihighx,
                          ip_mask, ZPixmap);
       //printf(" vimage: bpp=%d x %d %d \n",
       //     vimage->bits_per_pixel, vimage->width, vimage->height);
       if (vimage == (XImage *) NULL) 
          { spout("*** Virtual colormap image formation failed"); return; }

       // Apply virtual colormap to image
       k = 0;
       for (i=1; i <= iwidex; i++)
          {
          for (j=0; j < ihighx*4; j++)   // Why 4? al 9/2011
             {
             cvalueold       = (unsigned char) vimage->data[k];
             cvaluenew       = vmap[cvalueold];
             vimage->data[k] = cvaluenew;
             k++;
             }  // End of: for (j=0; j... 
          }     // End of: for (i=1; i... 

       // Put virtual colormapped image in cmap backing store: imapsav
       XPutImage(idispl,imapsav, icontx,vimage,0,0, 0,0, iwidex, ihighx); 

       }        // End of: if (vimage == NULL...

    // Copy colormapped backing store to display
    XCopyArea(idispl,imapsav,iwin,icontx,0,0,iwidex,ihighx,0,0);

    neednewmap = FALSE;
    }
  
 XFlush(idispl);

 drawing = FALSE;

 return;
 }


#ifdef NEVER
          ivalueold       = (int) vimage->data[k]; 
          ivaluenew       = vmap[ivalueold];
          if (ivalueold > 0)
            {  printf("vimage:[%d,%d]= %d %d \n", i,j, ivalueold,ivaluenew); }
 

    /* Copy backing store to image window */
    // XCopyArea(idispl,imagsav,iwin,icontx,0,0,iwidex,ihighx,0,0);
    //if (imagsavmap > 0) 
    //     XFreePixmap(imagsavmap); // Virtual cmap image backing store      
    //imagsavmap = 0;                              // Virtual cmap image backing store      

    if (imagsavmap <= 0)
       {  // Create new pixmap for virtual cmap image
       imagsavmap = XCreatePixmap(idispl,iroot, iwidex,ihighx,idepth);

       Bpp = (int) pow(2, (DefaultDepthOfScreen(iscreen) / 8) - 1);

       // vimage = XCreateImage(idispl,visual,DefaultDepthOfScreen(iscreen),
       //      ZPixmap, 0,  vbuf,    iwidex, ihighx, Bpp*4, iwidex*Bpp);
       }
#endif



#else
// WEB_24 -----------------------------------------------------------

/**********************  expos_cb **********************************/

void expos_cb(Widget iw_temp, XtPointer data, XtPointer call_data)

 { 

 Window       iroot;
 unsigned int iwidext,ihighxt,iborder,idepth,ixd,iyd;
 Pixmap       imagsavn;


 if (XGetGeometry(idispl,iwin,&iroot,&ixulx,&iyulx,&iwidext,&ihighxt,
                  &iborder,&idepth) == 0) 
     spout("*** Error getting geometry in expos_cb");

 if (iwidext > iwidexb || ihighxt > ihighxb)
    {
    /* Need to enlarge backing store for images */

    imagsavn = XCreatePixmap(idispl,iroot,iwidext,ihighxt,idepth);
    if ((int) imagsavn <= 0)  
      { spout("*** Can not create new backing store!"); return; } 

    // Clear background of new Pixmap   al 2011
    backclear(imagsavn,iwidext,ihighxt);
    
    /* Copy old backing store to new backing store */
    ixd = MYMIN(iwidexb,iwidext);
    iyd = MYMIN(ihighxb,ihighxt);
    XCopyArea(idispl,imagsav,imagsavn,icontx,0,0,ixd,iyd,0,0);
    XFreePixmap(idispl,imagsav);
    imagsav = imagsavn;

    iwidexb = iwidext;
    ihighxb = ihighxt;
    }

 if (iwidext != iwidex || ihighxt != ihighx)
       {  /* Size of window changed but not larger than backing store */
       /* Remember size of current image window */
       iwidex = iwidext;
       ihighx = ihighxt;
       }

 /* Copy backing store to image window */
 XCopyArea(idispl,imagsav,iwin,icontx,0,0,iwidex,ihighx,0,0); 
 XFlush(idispl);

 drawing = FALSE;

 return;
 }

#endif


 /*****************  backclear  ******************************/

 void backclear(Pixmap imagsavn, unsigned int iwidex, unsigned int ihighx)
 {

 XGCValues    gcval;

 if (icontxb <= (GC)0)
   { /* Need to create a new graphics context for blanking */
   icontxb = XCreateGC(idispl,iwin,0,&gcval);
   XCopyGC(idispl,icontx,0,icontxb);
   }

 if (icolorxb < 1)
     icolorxb = 0;

 XGetGCValues(idispl, icontx, GCForeground, &gcval); //??? al
 
 XSetForeground(idispl,icontx,icolorxb);    // Set clearing  color

 XFillRectangle(idispl,imagsavn,icontx,0,0,iwidex,ihighx);

 XFlush(idispl);

 XSetForeground(idispl, icontx, gcval.foreground);  // Restore color

 return;
 }
