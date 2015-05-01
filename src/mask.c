
/*$Header: /usr8/web/src/RCS/mask.c,v 1.9 2011/09/23 19:47:03 leith Exp $*/
/*
C***********************************************************************
C
C  WID_MASK.FOR  -- CREATED FEB 90
C  mask.c                   DEC 92
C                                                              
C **********************************************************************
C *  AUTHOR: ArDean Leith                                              *
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
C    MASK
C
C    PURPOSE:      Creates a mask by drawing around areas with mouse
C
C    CALLED BY:    imagemen 
C
C--*********************************************************************
*/

#include "common.h"
#include "routines.h"

// Max # of mask outline pts.
#define  nsize 300
// Max # of mask polygons
#define poly_maxpoly  5000   
// Max # of mask polygon pts
#define poly_maxpts  50000
 
 /* Internal subroutine prototypes */
 void             mask_pop     (Widget, XEvent *, String *, Cardinal *);
 void             mask_add_poly(XPoint *, int);
 void             mask_draw_polys(Pixmap, GC, int, int);
 void             mask_reset   ();
 void             mask_clear   ();

 /* External common variables used here */
 extern char      outstr[80];

 /* Global variable used by maskmen also */
 int              filler     = TRUE;
 GC               icontxmask = (GC) NULL;       // Mask GC 
 Pixmap           masksav    = (Pixmap) NULL;   // Mask store 

 /* File variables */
 static int       ix0,iy0;
 static int       ix2,iy2, ix1,iy1;
 static XPoint *  points;
 static Boolean   gotcontxt = FALSE;

 int              firstpt, ipts;
 XPoint *         poly_points;
 int *            poly_go;
 int *            poly_npts;

 int              poly_totnpts;
 int              poly_num;

 //----------------------  mask   ----------------------------------

 void mask(void)

 {
 XGCValues    gcval;
 int          idt = 1;        // Overlay pixmap depth

 /* Refresh screen */
 expos_cb(NULL,NULL,NULL);

 /* Create single bit mask same size as last image & GC */
 if (masksav != ((Pixmap) NULL)) XFreePixmap(idispl,masksav);
 masksav    = XCreatePixmap(idispl,iroot,nsam,nrow,idt);
 //printf(" Created masksav: %d Color: %d = %#8x \n",
 //           masksav,icolorov,ispicol[icolorov]);

 // Create graphics context for the single bit mask image
 if (! gotcontxt) icontxmask = XCreateGC(idispl,masksav,0,&gcval);
 gotcontxt = TRUE; 

 // Set clearing  color and clear the single bit mask
 mask_clear();

 /* Set current draw color to overlay color */
 wicolor(icontx,colorgo+icolorov);

 if ((points = (XPoint *)      malloc(nsize *       sizeof(XPoint))) == (int) 0)
    {  spout("*** Unable to malloc points in mask.c.");    return;}

 if ((poly_points = (XPoint *) malloc(poly_maxpts * sizeof(XPoint))) == (int) 0)
    {  spout("*** Unable to malloc poly_points in mask.c."); return;}

 if ((poly_go = (int *)        malloc(poly_maxpoly * sizeof(int)))   == (int) 0)
    {  spout("*** Unable to malloc poly_go in mask.c.");  return;}

 if ((poly_npts = (int *)      malloc(poly_maxpoly * sizeof(int)))   == (int) 0)
    {  spout("*** Unable to malloc poly_npts in mask.c.");  return;}

 /*  Set flags for new polygons */
 mask_reset();

 showbutx("Starts / sets line segment.",
          "Menu.",
          "Close this polygon.",FALSE);

 actions(iw_win, "mask_pop", mask_pop, "M123");
 }


 // ------------------- mask_pop  -----------------------------------

 void mask_pop(Widget iw_t, XEvent *event, String *params,
               Cardinal *num_params)
 {

 int  i;

 if (!(strcmp(*params,"M")))                 /* Cursor moved */
    {
    if (!firstpt)
       {
       /* Get mouse position ix2,iy2  */
       getloc(event,'M',&ix2,&iy2);

       /* Already have line, mouse moved, so erase and draw new line   */
       /* Copy backing store to image window & draw new line on screen */
       wirubberline(iwin,icontx,TRUE,FALSE,ix1,iy1,ix2,iy2);
       }
    }

 else if (!(strcmp(*params,"1")))            /* Button 1 pushed */ 
    {
    /* Get mouse position ix2,iy2  */
    getloc(event,'M',&ix2,&iy2);

    if (firstpt)
       {
       firstpt = FALSE;
       ix0     = ix2;
       iy0     = iy2;
       }
    else
       {  
       /*  Set current color to overlay color */
       wicolor(icontx,colorgo+icolorov);

       /* Draw permanent line to backing store and copy to screen */
       wirubberline(iwin,icontx,TRUE,TRUE,ix1,iy1,ix2,iy2);
       }

    ix1 = ix2;
    iy1 = iy2;

    if (ipts >= nsize)
       {  // Overflow
       sprintf(outstr,"Polygon length limit: %d \n",nsize);
       spout(outstr);
       return;
       }

    // Save points in mask polygon list
    points[ipts].x = ix1 - ixull + 1;
    points[ipts].y = iy1 - iyull + 1;
    ipts++;
    }
 
 else if (!(strcmp(*params,"2")))          /* Button 2 pushed */ 
    {
    /* Mask menu callup */

    ipts    = 0;
    firstpt = TRUE;

    // Display masking menu
    maskmen();
    }

 else if (!(strcmp(*params,"3")))          /* Button 3 pushed */        
    {  
    //   Finished with this mask polygon 
    if (ipts > 0) 
       {
       points[ipts].x = ix0 - ixull + 1;
       points[ipts].y = iy0 - iyull + 1;
       ipts++;

       // Draw permanent line to backing store and copy to screen 
       wirubberline(iwin,icontx,TRUE,TRUE,ix1,iy1,ix0,iy0);

       // Save this polygon in polygon list
       mask_add_poly(points, ipts);
       }

    firstpt = TRUE;
    ipts    = 0;             // Finished with current polygon

    XFlush(idispl);
    }

 return;
 }


// ------------  mask_add_poly --- add polygon to mask list -------- 

 void mask_add_poly(XPoint * points, int npts)
 {
 int  i,igo;

 if (poly_num >= poly_maxpoly) 
    {  // Overflow
    sprintf(outstr,"Too many polygons: %d in mask", poly_num);
    spout(outstr);   
    return;
    }

 poly_go  [poly_num] = poly_totnpts;      // Starting index of this poly
 poly_npts[poly_num] = npts;              // # of points on this poly

 igo = poly_go[poly_num];
 //printf(" Adding polygon: %d with: %d points go: %d\n", poly_num, npts, igo);

 poly_num++;                              // Increment polygon numbers

 for ( i = 0; i < npts; i++)
    {
    if (poly_totnpts >= poly_maxpts)
       { // Overflow
       sprintf(outstr,"Number of mask points limited to: %d \n",poly_maxpts);
       spout(outstr);
       return;
       }

    poly_points[poly_totnpts].x = points[i].x ;
    poly_points[poly_totnpts].y = points[i].y ;

    //printf(" Point: %d %d = %d , %d \n",
    //         i, poly_totnpts,poly_points[poly_totnpts].x,poly_points[poly_totnpts].y );

    poly_totnpts++;
    }

 //printf(" \n");
 //printf(" Added polygon: %d with: %d points shifted: %d %d Start: %d %d \n",
 //              poly_num-1, npts,    ixull, iyull, 
 //              poly_points[igo].x,  poly_points[igo].y);

 }

 // ------------------  mask_draw_polys   -------------------------------

void  mask_draw_polys(Pixmap imaget, GC icontxt, 
                      int add_shift ,int filled)
{ 

int      ipoly, npts, igo,ipt,i;
XPoint   pointst[nsize];

if (poly_num > 0)
   {
   for (ipoly = 0; ipoly < poly_num; ipoly++)
      {
      igo  = poly_go  [ipoly];
      npts = poly_npts[ipoly];   

      i = 0;

      for (ipt = igo; ipt <= igo + npts -1; ipt++)
         {
         //printf("  ipt: %d  npts: %d i: %d  \n",ipt,npts,i);

         if (add_shift) 
            {
            pointst[i].x = poly_points[ipt].x + ixull ;  // ixull from shift.c
            pointst[i].y = poly_points[ipt].y + iyull ;  // iyull from shift.c
            }
         else
            {
            pointst[i].x = poly_points[ipt].x ;  
            pointst[i].y = poly_points[ipt].y ;  
            }

         i++;
         }


      if (filled)
         {
         // Draw filled polygons on the mask image 
         XFillPolygon(idispl,imaget,icontxt, pointst,npts,
                      Complex,CoordModeOrigin);
         neednewmap = TRUE;

         //printf(" Drew Filled polygon: %d with: %d points shifted: %d %d Start: %d %d igo: %d\n",
         //    ipoly, npts, ixull,iyull, 
         //    pointst[0].x,pointst[0].y, igo);
         }
      else
         {
         // Draw empty polygons on the mask image 
         XDrawLines(idispl,imaget,icontxt,pointst,npts,
                    CoordModeOrigin);
         neednewmap = TRUE;

         //printf(" Drew Empty polygon: %d with: %d points shifted: %d %d Start: %d %d igo: %d\n",
         //      ipoly, npts, ixull,iyull, 
         //      pointst[0].x,pointst[0].y),igo;
         }

      }
   }
   XFlush(idispl);
} 
 
// --------------  mask_clear --- clear mask image ------------------ 

 void mask_clear()
 {
 // Set clearing  color  to black
 XSetForeground(idispl,icontxmask,0x00);             

 // Clear the single bit mask
 XFillRectangle(idispl,masksav,icontxmask, 0,0,nsam,nrow); 

 // Set single bit mask color (white)
 XSetForeground(idispl,icontxmask,0xff);      
 }


// --------------  mask_reset --- zero mask list ------------------ 

 void mask_reset()
 {

 poly_num     = 0;
 poly_totnpts = 0;

 firstpt      = TRUE;
 ipts         = 0;

 if (masksav != ((Pixmap) NULL)) mask_clear(); 
 }
