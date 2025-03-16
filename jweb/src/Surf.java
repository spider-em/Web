package web;

import javax.swing.*;
import java.io.*;
import java.math.*;


/*
C***************************************************************************
C
C   SURF.FOR  -- CREATED FEB 92 as WID_SURF.FOR
C   surf.c       CONVERTED TO C OCT 92 al
C                MODIFIED NOV 93 lo
C                MODIFIED JUNE 95 TO ADD COLOR al
C
C **************************************************************************
C *  AUTHOR: ArDean Leith                                                  *
C *  COPYRIGHT (C)1992, WADSWORTH CENTER FOR LABORATORIES AND RESEARCH     *
C *    NEW YORK STATE DEPARTMENT OF HEALTH, ALBANY, NY 12201.              *
C *    THE CONTENTS OF THIS DOCUMENT ARE PROPRIETARY TO THE CENTER FOR     *
C *    LABORATORIES AND RESEARCH AND ARE NOT TO BE DISCLOSED TO OTHERS OR  *
C *    USED FOR PURPOSES OTHER THAN INTENDED WITHOUT WRITTEN APPROVAL OF   *
C *    THE CENTER FOR LABORATORIES AND RESEARCH  			   *
C **************************************************************************
C
C SURF(SIRDIT,DISTIT)
C
C  PURPOSE:     READS SPIDER 3-D PICTURE FILE, CREATES 2-D
C               REFLECTION IMAGE OF LEFT SIDE VIEW
C
C  PARAMETERS:  NSAM1,NSAM2     SAMPLE RANGE
C               NROW1,NROW2     ROW RANGE
C               NSLICE1,NSLICE2 SLICE RANGE
C               THLEV           THRESHOLD DESIRED
C               REFL            REFLECTION %
C               FCT             ATTENUATION %
C               PHI,THETA,PSI   VIEWING ANGLES
C
C  NOTES:       FOR EACH PIXEL, THE PREVIOUS PIXEL ON THE ROW AND THE
C               PIXEL ONE COL. DOWN ARE USED TO FORM A TRIANGULAR TILE
C               THE TILE IS SHADED ACCORDING TO ITS ORIENTATION ACROSS
C               THE X AXIS AND ITS DISTANCE ALONG THE X AXIS FROM THE
C               LEFT END OF THE FILE.
C
C--*********************************************************************
*/

 /******************************   surf   ****************************/



class Surf {

   // externally defined variables used here !
   float phi, theta, psi, thlev, refl;
   int        imagego     = 0;
   int        imageend    = 255;
   float      fct         = (float)  1.0;

   int        nvolcolors = 1;
   double     scaleval   = 1.0;
   int        nsam1      = 1,  nrow1 = 1,  nslice1 = 1;
   int        nsam2, nrow2, nslice2;
   int        nsams, nrows, nslices;

   boolean    gotsurf    = false;

   //  variables for use here
   byte       lthlev, lthlevo = (byte) 0;
   float      fcto, reflo, thetao, phio, psio, csfact;
   double     scaleo     = 1.0;
   int        npixo      = 0, tnpixo = 0, nvoxelso = 0;
   int        nsam1o     = 0, nsam2o   = 0, nsamo = 0;
   int        nrowo      = 0, nsliceo  = 0;
   int        nslice1o   = 0, nslice2o = 0, nrow1o = 0, nrow2o = 0;
   int        tnpixsirdo = 0, nvolcolorso = -1, ncolrange;
   int        nsam, nrow, nslice;

   // variables that are used here
   byte  []  vol;
   byte  []  refmap;
   float []  distbuf;
   float []  idistl;
   float []  sirdbuf;
   byte  []  colvol;    // the 3D texture mapping vol  buffer //

   double    fd1, fd2;

   // variables that are used here (from surfrot.c
   int            nsr, vcx, vcy, vcz;
   double         xl, xh, yl, yh, zl, zh, rm11, rm21;
   double         rm31, rm12, rm22, rm32, rm13, rm23, rm33;
   boolean        newang;
   boolean        newsize = true;
   double         fn=0.0, fncon=0.0;

   SpiderImage    spiderimg;

   public  Surf(SpiderImage spiderImgIn, int nsamIn, int nrowIn, int nsliceIn) {

      spiderimg = spiderImgIn;
      // tricked the program, so that the re-ordered data (from SpiderImage.java)
      // will be used to create surface by looking from top to bottom
      nsam    = nsliceIn;
      nrow    = nsamIn;
      nslice  = nrowIn;
      nsam2   = nsam;
      nrow2   = nrow;
      nslice2 = nslice;
      nsams   = nsam;
      nrows   = nrow;
      nslices = nslice;

      fn    = spiderimg.getfcval();
      fncon = spiderimg.getfncon();

      //System.out.println("fncon , fn = "+fncon+"  " +fn);

   }
   /************************* surfrot ****************************/

   private   float surfrot(byte [] vol, int iy, int iz) {

      double         phit, psit, thetat;
      double         y, z, qr1, qr2, qr3, qout;
      double         u, v, w, bv, fv, xx, yy, zz;
      double         a1, a2, a3, a4, a5, a6, a7, a8;
      int            ix, ixgo, ixend, iox, ioy, ioz;
      int            p1, p2, p3, p4, p5, cptr, p6, p7, p8;

      if (newang) {
         thetat  = theta / 360.0 * 8.0 * Math.atan(1.0);
         phit    = phi   / 360.0 * 8.0 * Math.atan(1.0);
         psit    = psi   / 360.0 * 8.0 * Math.atan(1.0);

         /*
         thetat = theta * 8.0 * Math.atan(1.0) / 360.0;
         phit   = phi   * 8.0 * Math.atan(1.0) / 360.0;
         psit   = psi   * 8.0 * Math.atan(1.0) / 360.0;
         */

         rm11   =  Math.cos(thetat)*Math.cos(phit)*Math.cos(psit)
                                               -Math.sin(phit)*Math.sin(psit);
         rm21   = -Math.cos(thetat)*Math.cos(phit)*Math.sin(psit)
                                               -Math.sin(phit)*Math.cos(psit);
         rm31   =  Math.sin(thetat)*Math.cos(phit);
         rm12   =  Math.cos(thetat)*Math.sin(phit)*Math.cos(psit)+Math.cos(phit)*Math.sin(psit);
         rm22   = -Math.cos(thetat)*Math.sin(phit)*Math.sin(psit)+Math.cos(phit)*Math.cos(psit);
         rm32   =  Math.sin(thetat)*Math.sin(phit);
         rm13   = -Math.sin(thetat)*Math.cos(psit);
         rm23   =  Math.sin(thetat)*Math.sin(psit);
         rm33   =  Math.cos(thetat);
         newang = false;
      }

      if (newsize) {
         vcx     = nsam   / 2;
         vcy     = nrow   / 2;
         vcz     = nslice / 2;

         xl      = 1.0 - vcx;
         xh      = nsam - vcx;
         yl      = 1.0 - vcy;
         yh      = nrow - vcy;
         zl      = 1.0 - vcz;
         zh      = nslice - vcz;
         nsr     = nsam * nrow;
         newsize = false;
      }
      ixgo  = nsam1 - vcx;
      ixend = nsam2 - vcx;
      y     = iy - vcy;
      z     = iz - vcz;

      qr1   = rm11 * ixgo + rm21 * y + rm31 * z;
      qr2   = rm12 * ixgo + rm22 * y + rm32 * z;
      qr3   = rm13 * ixgo + rm23 * y + rm33 * z;

      for (ix = ixgo; ix <= ixend; ix++) {
         if (qr1 > xl && qr1 < xh && qr2 > yl &&
                                          qr2 < yh  && qr3 > zl && qr3 < zh){
            xx  = vcx + qr1;
            yy  = vcy + qr2;
            zz  = vcz + qr3;

            iox = (int) xx;  ioy = (int) yy;  ioz = (int) zz;
            u   = xx - iox;  v   = yy - ioy;   w   = zz - ioz;

            cptr = (ioz-1)*nsr + (ioy-1)*nsam + iox-1;

            p1 = cptr;            p5 = p1 + 1;
            p2 = p1 + nsam;       p6 = p2 + 1;
            p3 = p2 + nsr;        p7 = p3 + 1;
            p4 = p1 + nsr;        p8 = p4 + 1;

            a1 = vol[p1]; a2 = vol[p2]; a3 = vol[p3]; a4 = vol[p4];
            a5 = vol[p5]; a6 = vol[p6]; a7 = vol[p7]; a8 = vol[p8];

            bv = a1 + w*(a4 - a1) + v*(a2 - a1 + w*(a3 + a1 - a2 - a4));
            fv = a5 + w*(a8 - a5) + v*(a6 - a5 + w*(a7 + a5 - a6 - a8));

            qout = u*(fv - bv) + bv;
            if ( qout >= (float) lthlev) return((float)1.0);
         }
         qr1 += rm11;
         qr2 += rm12;
         qr3 += rm13;
      }
      return((float) 0.0);
   }

   /*************************************************************************/
   /*                                                                       */
   /* volval computes the value within the volume at position x,y,z (origin */
   /* shifted to middle of volume) using trilinear interpolation.           */
   /*                                                                       */
   /*************************************************************************/

   private   float volval(int ipos, int jpos, int kpos, byte [] vol) {

      int        iox, ioy, ioz;
      double     u, bv, fv, qr1, qr2, qr3;
      double     v, w, x, y, z, xx, yy, zz;
      double     a1, a2, a3, a4, a5, a6, a7, a8;
      int        cptr, p1, p2, p3, p4, p5, p6, p7, p8;

      x   = ipos - vcx;
      y   = jpos - vcy;
      z   = kpos - vcz;

      qr1 = rm11 * x + rm21 * y + rm31 * z;
      qr2 = rm12 * x + rm22 * y + rm32 * z;
      qr3 = rm13 * x + rm23 * y + rm33 * z;

      if (qr1 > xl && qr1 < xh && qr2 > yl &&
                                          qr2 < yh  && qr3 > zl && qr3 < zh) {
         xx = vcx + qr1;
         yy = vcy + qr2;
         zz = vcz + qr3;

         iox = (int) xx;       ioy = (int) yy;       ioz = (int) zz;
         u = xx - iox;   v = yy - ioy;   w = zz - ioz;

         cptr = (ioz-1)*nsr + (ioy-1)*nsam + iox-1;

         p1 = cptr;            p5 = p1 + 1;
         p2 = p1 + nsam;       p6 = p2 + 1;
         p3 = p2 + nsr;        p7 = p3 + 1;
         p4 = p1 + nsr;        p8 = p4 + 1;

         a1 = vol[p1]; a2 = vol[p2]; a3 = vol[p3]; a4 = vol[p4];
         a5 = vol[p5]; a6 = vol[p6]; a7 = vol[p7]; a8 = vol[p8];

         bv = a1 + w*(a4 - a1) + v*(a2 - a1 + w*(a3 + a1 - a2 - a4));
         fv = a5 + w*(a8 - a5) + v*(a6 - a5 + w*(a7 + a5 - a6 - a8));

         return( (float) (u*(fv - bv) + bv));
      }
      else
      /* will have to return original spot value.... */
      /* erroneous but satisfactory! */
      return( vol [(kpos-1) * nsr + (jpos-1) * nsam + ipos-1] );
   }

   /**************** surfmethod ***************************************/
   public byte[] surfmethod(float phiIn, float thetaIn, float psiIn,
                             float thlevIn,float reflIn, boolean sirdit,
                             boolean distit, boolean colorit, double imgScale)  {

      int            j, k, iloc, nsr, jlast, klast, iy;
      int            kgo, kend, jgo, jend, N1, N2, ixxp1=0;
      int            islicet, irow, jc, kc, jj, kk, irowp1, nvoxels;
      int            ixx, npix, tnpix, NP, NC, islicetp1;
      int            nsams, nrows, nsamwt;
      double          a1, a2, a3, a4, a5=0.0, a6=0.0, a7=0.0, a8=0.0;
      double          xb1, xf1, xb2, xf2, xb3, xf3, xb4, xf4;
      double          xb5, xf5=0.0, xb6=0.0, xf6=0.0, xb7, xf7=0.0, xb8, xf8=0.0;
      double          yb1, yf1, yb2, yf2=0.0, yb3, yf3=0.0, yb4, yf4;
      double          yb5, yf5, yb6, yf6=0.0, yb7, yf7=0.0, yb8, yf8;
      double          zb1, zf1, zb2, zf2, zb3, zf3=0.0, zb4, zf4=0.0;
      double          zb5, zf5, zb6, zf6, zb7, zf7=0.0, zb8, zf8=0.0;
      double          dj, dk, d1, d2, d3, d4, ft;
      double          of1, of2, of1c, of2c, jslope, kslope;
      double          nf1, nf2, nf1c, nf2c, nv, ov, u, v, w, dy;
      double          Nx, Ny, Nz, bgx, bgy, bgz, fgx, fgy, fgz, ivall;
      //String         output;

      boolean        first_time = true;
      boolean        rotate;

      int            cptr, ptr, cloc, floc, pos, posit, positsird=0;
      int            p1=0, p2=0, p3=0, p4=0, p5=0, p6=0, p7=0, p8=0;

      float          scaledis;
      int            kkfactor, islicetnsr, positindx, itemp, islicetfact;
      int            itempfact1, itempfact2, irowfact;
      //int            i;

      /**********debug ***********/
      int            nval   = 0;
      int            valmin = 10000,   valmax = -10000;
      int            valminc = 10000,  valmaxc = -10000;
      int            valminf = 10000 , valmaxf = -10000 ;
      int []         numcolt = new int[10];
      /********** end debug ***********/

      phi = phiIn;
      theta = thetaIn;
      psi = psiIn;
      thlev = thlevIn;
      refl = reflIn;
      scaleval = imgScale;
      // find number of pixels in an end view of the volume
      npix    = nrow * nslice;

      // find number of pixels on a slice
      nsr     = nsam * nrow;

      // find original volume size
      nvoxels = nsam * nrow * nslice;

      // find reflection image size
      tnpix   = (int) (scaleval * nrow * scaleval * nslice);

      jslope = (1.0 - nrow*scaleval)   / (1.0 - nrow);
      kslope = (1.0 - nslice*scaleval) / (1.0 - nslice);

      N1     = nrow2 - nrow1 + 1;
      N2     = nslice2 - nslice1 + 1;

      nsams  = (int) ((nrow2 - nrow1)*jslope + 1);
      nrows  = (int) ((nslice2 - nslice1)*kslope + 1);

      nsamwt = nsam2 - nsam1 + 1;

      // make clock cursor (#79)
      // waitcursor(iwin,true,79);

      if (sirdit) {
         // sirds output wanted
         if (tnpix > tnpixsirdo) {
            tnpixsirdo = tnpix;
            sirdbuf    = new float[tnpixsirdo];
         }
      }
      if (!gotsurf) {
         // do not have surface memory allocated
         first_time = true;

         // allocate space for arrays
         if (nvoxelso == 0 && npixo == 0 && tnpixo == 0) {
            vol      = new byte[nvoxels];
            distbuf  = new float[npix];
            refmap   = new byte[tnpix];
            colvol   = new byte[nvoxels];
            /* load volume data into 8 bit vol */
            vol = spiderimg.orderDataForSurfaceViewing(spiderimg.bpix);
            nvoxelso = nvoxels; npixo = npix; tnpixo = tnpix;
         }
         else {
            if (nvoxels > nvoxelso) {
               // must reallocate array space
               nvoxelso = nvoxels;
               vol      = new byte[nvoxels];
               colvol   = new byte[nvoxels];
               /* load volume data into 8 bit vol */
               vol = spiderimg.orderDataForSurfaceViewing(spiderimg.bpix);
            }

            if ( npix > npixo) {
               // number of pixels increased in distbuf
               npixo   = npix;
               distbuf = new float[npix];
            }
         }
         if (colorit) nvolcolorso = -1;
      }
      /*  (!gotsurf) */

      if (colorit && (nvolcolorso != nvolcolors)) {
         /* load color volume and set colormap */
         //if (!surfcolor(colvol, nvolcolors, &ncolrange)) return;!!!!!!!!
      }

      /* assume volume is small enough to rotate */
      if (theta > 0.0 || phi > 0.0 || psi > 0.0)
      // must rotate input volume
      rotate = true;
      else
      rotate = false;

      newang = false;

      if (theta != thetao || phi != phio || psi != psio) {
         /* rotation angle changed since last render */
         newang     = true;
         thetao     = theta;
         phio       = phi;
         psio       = psi;
         first_time = true;
      }

      if (nsam != nsamo || nrow != nrowo || nslice != nsliceo) {
         /* input vol. size changed since last render */
         newsize    = true;
         nsamo      = nsam;
         nrowo      = nrow;
         nsliceo    = nslice;
         first_time = true;
      }

      if (tnpix > tnpixo) {
         /* output image size changed since last render */
         tnpixo = tnpix;
         refmap = new byte[tnpix];
      }
      /* if (tnpix > tnpixo) */

      if (nrow1o != nrow1 || nrow2o != nrow2 || nslice1o != nslice1 ||
                 nslice2o != nslice2 || nsam1o != nsam1 || nsam2o != nsam2) {
         nrow1o     = nrow1;
         nrow2o     = nrow2;
         nslice1o   = nslice1;
         nslice2o   = nslice2;
         nsam1o     = nsam1;
         nsam2o     = nsam2;
         first_time = true;
      }

      /* NOTE: Distance shading is done according to original nsam     */
      /*       not the windowed nsam1 dimension                        */

      /* convert threshold to 8 bits */
      lthlev = (byte) (thlev * fn + fncon);
      // modified the bytes as required by Surf.java (-128...0....127)
      // see SpiderImage.orderDataForSurfaceViewing for deatails
      if (lthlev > 0) {
         lthlev = (byte) (lthlev - 128);
      }
      else {
         lthlev = (byte) (lthlev + 128);
      }

      if (lthlevo != lthlev) {
         lthlevo    = lthlev;
         first_time = true;
      }
      /* When (first_time) the following will compute and save the voxel
      depths in the nsam direction for the specified threshold, with
      scaling of 1. This enables speed-up when scaling > 1  */

      if (first_time) {
         iloc       = -1;
         itempfact1 = -nsr - nsam + nsam1 - 1;
         //---------------ok to here -------------------------
         for (islicet = nslice1; islicet <= nslice2; islicet++) {
            itempfact2 = islicet * nsr + itempfact1;

            /* set starting location for this row in reflection image */
            for (irow = nrow1; irow <= nrow2; irow++) {
               if (!rotate) {
                  /*  get ivall directly from data volume  */
                  /* cptr  = vol + (islicet - 1) * nsr
                                        + (irow -1) * nsam + nsam1 - 1;  */
                  /* islicet * nsr - nsr + irow * nsam - nsam + nsam1 - 1 */
                  cptr = itempfact2 + irow * nsam;
                  ivall = 0.0;
                  ixx   = nsam1 - 1;
                  for (ptr = cptr; ptr < (cptr + nsamwt); ptr++) {
                     ixx++;
                     if (vol[ptr] >= lthlev) {
                        ivall = ixx;
                        break;
                     }
                  }   /* for ptr  */
               }      /* if (!rotate) */
               else {
                  ivall = surfrot(vol, irow, islicet);
               }
               distbuf[++iloc] = (float) ivall;
            }  /*  irow    */
         }     /*  islicet */
      }        /* first_time*/

      if (!gotsurf || refl != reflo) {
         /* do not have a surface or have changed reflection ratio */
         gotsurf    = true;

         /* find normalization factors for refmap formation */
         /* changes by Bimal*/
//              fd1        = (127.0 - imagego) * (1.0 - refl) / (1.0 - nsam);
         fd1        = (255.0 - imagego) * (1.0 - refl) / (1.0 - nsam);
         fd2        = imagego - nsam * fd1;
//              csfact     = (float) (refl * (127.0 - imagego));
         csfact     = (float) (refl * (255.0 - imagego));

         if (colorit) {
            fd1     = (ncolrange-1) * (1.0 - refl) / (1.0 - nsam);
            fd2     = imagego - nsam * fd1;
            csfact  = refl * (ncolrange -1);
         }
         reflo      = refl;
         first_time = true;
      }
      if ( fcto != fct) {
         /* fct changed */
         fcto       = fct;
         first_time = true;
      }

      if ( colorit && (nvolcolorso != nvolcolors)) {
         /* nvolcolors changed */
         nvolcolorso = nvolcolors;
         first_time  = true;
         fd1         = (ncolrange-1) * (1.0 - refl) / (1.0 - nsam);
         fd2         = imagego - nsam * fd1;
         csfact      = refl * (ncolrange -1);
      }

      if (first_time || scaleo != scaleval) {
         /* first time or scale changed */
         first_time = false;
         scaleo     = scaleval;

         /***** alterd al junk 95!!!!!!!!!!!!!!
         // sprintf(output,
         //     "Phi:%d Theta:%d Psi:%d Scale:%4.1f Threshold:(%f,%d)
         //      Next_Threshold:(%f,%d)",
         //     (int)phi, (int)theta, (int)psi, scaleval, thlev,
         //    lthlev, (lthlev + 1.0 - fncon)/fn,(int)(thlev + 1));
         *****/

         //System.out.printf(
         //    "Phi:%d Theta:%d Psi:%d  Scale:%4.1f  Threshold:(%f,%d)",
         //    (int)phi, (int)theta, (int)psi, scaleval, thlev, lthlev);
         //spout(output);

         {
            /* assume we have enough space to rotate volume */
            /* initialize refmap to imagego at all pixels */
            for (cloc = 0; cloc < tnpix; cloc++) refmap[cloc] = (byte) imagego;

            /********** debug *********/
            for (iloc = 0; iloc < 10; iloc++) numcolt[iloc] = 0;
            /*********** end debug ********/

            if (sirdit) {
               /* want SIRD display */
               /* initialize sirdmap to 0 at all pixels */
               for (floc = 0; floc < tnpix; floc++) sirdbuf[floc] = (float) 0.0;
            }

            klast = kc = 0;
            kgo   = (int) (1 + kslope*(nslice1 - 1));

            /* For each level of islicet, kgo and kend are the local starting and
            ending rows of pixels in refmap  */

            for (islicet = nslice1; islicet < nslice2; islicet++) {
               islicetp1   = islicet + 1;
               kend        = (int) (1 + kslope * islicet);
               dk          = 1.0 / (kend - kgo);
               islicetnsr  = islicet * nsr;
               islicetfact = islicetnsr - nsam - nsr;
               irowfact    = islicetfact + nsam1 -1;

               if (islicet == (nslice2 -1)) kend++;
               kc++;

               jlast = jc = 0;
               jgo   = (int) (1 + jslope*(nrow1 - 1));

               /* For each irow, jgo and jend are the local starting and
               ending columns of pixels in refmap  */

               for (irow = nrow1; irow < nrow2; irow++) {
                  irowp1    = irow + 1;
                  jend      = (int) (1 + jslope*irow);
                  dj        = 1.0/(jend - jgo);
                  /* positindx = islicetnsr + nsam * irow - nsam - nsr */
                  positindx = islicetfact + nsam * irow;

                  if (irow == (nrow2 -1) ) jend++;
                  jc++;
                  pos = (kc - 1)*N1 + jc - 1;
                  d1  = distbuf[pos];
                  d2  = distbuf[pos + 1];
                  d3  = distbuf[pos + N1 + 1];
                  d4  = distbuf[pos + N1];

                  //  if (!d1 && !d2 && !d3 && !d4) /* the original corner pixels */
                  if (d1 == 0.0 && d2 == 0.0 && d3 == 0.0 && d4 == 0.0 ) {
                     /* the original corner pixels */
                     /* returned 0 depth therefore */
                     jlast += (jend - jgo);     /* surface is not within here */
                     jgo    = jend;
                     continue;
                  }
                  /* find pointer to position in original volume array */
                  /*** ptr = vol + (islicet - 1) * nsr
                                    + (irow - 1) * nsam + nsam1 - 1;   */
                  /*** ptr = vol + islicet * nsr - nsr + irow * nsam - nsam + nsam1 -1 */
                  ptr = irow * nsam + irowfact;

                  NP  = (kend - kgo)*(jend - jgo);
                  NC  = 0;

                  for (ixx = nsam1; ixx < nsam2; ixx++) {
                     /* step back along row through the volume */
                     if (NC == NP) break;

                     /* find value in volume at * corners */
                     if (!rotate) {
                        /* simple if rotation is zero! */
                        p1 = ptr;        p5 = p1 + 1;
                        p2 = p1 + nsam;  p6 = p2 + 1;
                        p3 = p2 + nsr;   p7 = p3 + 1;
                        p4 = p1 + nsr;   p8 = p4 + 1;

                        a1 = vol[p1]; a2 = vol[p2]; a3 = vol[p3]; a4 = vol[p4];
                        a5 = vol[p5]; a6 = vol[p6]; a7 = vol[p7]; a8 = vol[p8];
                     }
                     else {
                        /* must rotate */
                        ixxp1 = ixx + 1;
                        if ( ixx == nsam1) {
                           a1 = volval(ixx, irow, islicet, vol);
                           a2 = volval(ixx, irowp1, islicet, vol);
                           a3 = volval(ixx, irowp1, islicetp1, vol);
                           a4 = volval(ixx, irow, islicetp1, vol);
                        }
                        else {
                           a1 = a5;
                           a2 = a6;
                           a3 = a7;
                           a4 = a8;
                        }

                        a5 = volval(ixxp1, irow, islicet, vol);
                        a6 = volval(ixxp1, irowp1, islicet, vol);
                        a7 = volval(ixxp1, irowp1, islicetp1, vol);
                        a8 = volval(ixxp1, irow, islicetp1, vol);
                     }  /* can rotate */

                     /* if all 8 corner values are below threshold, the
                     surface is not here      */

                     if (a1 < lthlev && a2 < lthlev && a3 < lthlev &&
                               a4 < lthlev && a5 < lthlev && a6 < lthlev &&
                                                  a7 < lthlev && a8 < lthlev) {
                        ptr++;
                        continue;
                     }

                     /* Compute the forward difference parameters to use later
                     for gradient estimation */

                     xf1 = a5; xf2 = a6; xf3 = a7; xf4 = a8;
                     yf1 = a2; yf4 = a3; yf5 = a6; yf8 = a7;
                     zf1 = a4; zf2 = a3; zf5 = a8; zf6 = a7;

                     xb1 = yb1 = zb1 = a1; xb2 = yb2 = zb2 = a2;
                     xb3 = yb3 = zb3 = a3; xb4 = yb4 = zb4 = a4;
                     xb5 = yb5 = zb5 = a5; xb6 = yb6 = zb6 = a6;
                     xb7 = yb7 = zb7 = a7; xb8 = yb8 = zb8 = a8;

                     if (islicet == (nslice - 1)) /* bottommost */ {
                        /* use backward difference  */
                        zb3 = a2; zf3 = a3;
                        zb4 = a1; zf4 = a4;
                        zb7 = a6; zf7 = a7;
                        zb8 = a5; zf8 = a8;
                     }
                     else if (irow == (nrow-1))  /* rightmost  */ {
                        /* use backward difference  */
                        yb2 = a1; yf2 = a2;
                        yb3 = a4; yf3 = a3;
                        yb6 = a5; yf6 = a6;
                        yb7 = a8; yf7 = a7;
                     }
                     else if (ixx == (nsam - 1))  /* frontmost  */ {
                        /* use backward difference  */
                        xb5 = a1; xf5 = a5;
                        xb6 = a2; xf6 = a6;
                        xb7 = a3; xf7 = a7;
                        xb8 = a4; xf8 = a8;
                     }
                     else {
                        /* use forward difference  */
                        if (!rotate) {
                           xf5 = vol[p5 + 1];
                           xf6 = vol[p6 + 1];
                           xf7 = vol[p7 + 1];
                           xf8 = vol[p8 + 1];

                           yf2 = vol[p2 + nsam];
                           yf3 = vol[p3 + nsam];
                           yf6 = vol[p6 + nsam];
                           yf7 = vol[p7 + nsam];

                           zf3 = vol[p3 + nsr];
                           zf4 = vol[p4 + nsr];
                           zf7 = vol[p7 + nsr];
                           zf8 = vol[p8 + nsr];
                        }
                        else {
                           xf5 = volval(ixxp1+1, irow, islicet, vol);
                           xf6 = volval(ixxp1+1, irowp1, islicet, vol);
                           xf7 = volval(ixxp1+1, irowp1, islicetp1, vol);
                           xf8 = volval(ixxp1+1, irow, islicetp1, vol);

                           yf2 = volval(ixx, irowp1+1, islicet, vol);
                           yf3 = volval(ixx, irowp1+1, islicetp1, vol);
                           yf6 = volval(ixxp1, irowp1+1, islicet, vol);
                           yf7 = volval(ixxp1, irowp1+1, islicetp1, vol);

                           zf3 = volval(ixx, irowp1, islicetp1+1, vol);
                           zf4 = volval(ixx, irow, islicetp1+1, vol);
                           zf7 = volval(ixxp1, irowp1, islicetp1+1, vol);
                           zf8 = volval(ixxp1, irow, islicetp1+1, vol);
                        }
                     }  /* else   use forward difference  */

                     of1  = a1;           of2 = a2 - a1;
                     of1c = (a4 - a1)*dk; of2c = (a3 + a1 - a2 - a4)*dk;

                     nf1  = a5;           nf2 = a6 - a5;
                     nf1c = (a8 - a5)*dk; nf2c = (a7 + a5 - a6 - a8)*dk;

                     kk = klast;
                     /* Process all the pixels on the rectangle per ptr  */
                     for (k = kgo; k < kend; k++) {
                        /* scan down thru the rectangle */
                        kk++;
                        jj = jlast;
                        kkfactor = (kk -1) * nsams - 1;

                        /* Scan Across Rectangle  */
                        for (j = jgo; j < jend; j++) {
                           jj++;

                           /* set position in output image(s) */
                           /* posit  = refmap + (kk - 1) * nsams + jj - 1; */
                           posit     = kkfactor + jj ;
                           if (sirdit) positsird = kkfactor + jj;

                           /* if have already set this pixel, skip */
                           if (refmap[posit] > (byte) imagego) continue;

                           /* find surface depth within these 8 corners */
                           /* at this region of the k,j rectangle       */
                           ivall = 0.0;

                           /* dy is %distance across rectangle */
                           dy = (j - jgo)*dj;
                           ov = of1 + dy*of2;
                           nv = nf1 + dy*nf2;

                           if ( ov >= lthlev ) ivall = ixx;
                           if ( ov <= lthlev && nv >= lthlev) {
                              ivall = ixx;
                              if (ov != nv) ivall = ixx + (lthlev - ov)/(nv - ov);
                           }

                           /* if surface not found, skip rest of pixel assignment */
                           if (ivall == 0.0)  continue;

                           /* surface located, now find value for the reflection map buffer */

                           NC++;
                           /* (u,v,w) coressponds to fractional location */
                           /* within original source voxel (8 corners)   */
                           u = ivall - ixx;        /* along row */
                           v = dy;                 /* across rectangle */
                           w = (k - kgo)*dk;       /* down   rectangel */

                           /* compute gradient at the back face for x, y & z */
                           /* y gradient */
                           d1 = yf1 - yb1;
                           d2 = yf2 - yb2;
                           d3 = yf3 - yb3;
                           d4 = yf4 - yb4;
                           bgy = d1 + w*(d4-d1) + v*(d2-d1 + w*(d3-d2-d4+d1));

                           /* z gradient */
                           d1 = zf1 - zb1;
                           d2 = zf2 - zb2;
                           d3 = zf3 - zb3;
                           d4 = zf4 - zb4;
                           bgz = d1 + w*(d4-d1) + v*(d2-d1 + w*(d3-d2-d4+d1));

                           /* x gradient */
                           d1 = xf1 - xb1;
                           d2 = xf2 - xb2;
                           d3 = xf3 - xb3;
                           d4 = xf4 - xb4;
                           bgx = d1 + w*(d4-d1) + v*(d2-d1 + w*(d3-d2-d4+d1));

                           /* compute gradient at the front face for x, y & z */
                           /* y gradient */
                           d1 = yf5 - yb5;
                           d2 = yf6 - yb6;
                           d3 = yf7 - yb7;
                           d4 = yf8 - yb8;
                           fgy = d1 + w*(d4-d1) + v*(d2-d1 + w*(d3-d2-d4+d1));

                           /* z gradient */
                           d1 = zf5 - zb5;
                           d2 = zf6 - zb6;
                           d3 = zf7 - zb7;
                           d4 = zf8 - zb8;
                           fgz = d1 + w*(d4-d1) + v*(d2-d1 + w*(d3-d2-d4+d1));

                           /* x gradient */
                           d1 = xf5 - xb5;
                           d2 = xf6 - xb6;
                           d3 = xf7 - xb7;
                           d4 = xf8 - xb8;
                           fgx = d1 + w*(d4-d1) + v*(d2-d1 + w*(d3-d2-d4+d1));

                           /* find normals for this tile */
                           Nx = u*(fgx - bgx) + bgx;

                           if (Math.abs(Nx) >= 0.0005) {
                              /* set reflection contribution */
                              /************
                              Ny = (u*(fgy - bgy) + bgy) / Nx;
                              Nz = (u*(fgz - bgz) + bgz) / Nx;
                              ft = Ny*Ny + Nz*Nz;
                              *************/

                              Ny = (u*(fgy - bgy) + bgy);
                              Nz = (u*(fgz - bgz) + bgz);
                              ft = (Ny*Ny + Nz*Nz) / ( Nx * Nx) ;

                              refmap[posit] = (byte) (fd1 * ivall + fd2
                                        + csfact/Math.sqrt((fct * ft) + 1.0));
                           }
                           else {
                              /* ignore reflection contribution (div. by 0) */
                              refmap[posit] = (byte) (fd1 * ivall + fd2);
                           }

                           if (sirdit) {
                              /* save distance in sirds buffer, if needed */
                              sirdbuf[positsird] = (float)
                                         (1.0 - ((float) ivall / (float)nsam));
                           }
                           else if (colorit) {
                              /* set correct color and intensity  */
                              itemp = (int) refmap[posit];
                              /************
                              refmap[posit] = (refmap[posit]) + ncolrange *
                                              colvol[positindx + (int) ivall] ;
                              refmap[posit] = (refmap[posit])
                                            + colvol[(islicet -1 +w+0.5)*nsr
                                            + (irow -1 +v+0.5)*nsam + ivall+0.5
                              ***********/

                              /* find colvol value at closest voxel for this k,j location */
                              refmap[posit] = (byte) ((refmap[posit]) + ncolrange *
                                  colvol[positindx + (int)(w + 0.5) * nsr  +
                                                     (int)(v + 0.5) * nsam +
                                                     (int)(ivall + 0.5)]);

                              /****** debug ********/
                              nval++;
                              if (colvol[positindx + (int) ivall] < valminc)
                                    valminc = colvol[positindx + (int) ivall];
                              if (colvol[positindx + (int) ivall] > valmaxc)
                                    valmaxc = colvol[positindx + (int) ivall];
                              numcolt[colvol[positindx + (int) ivall]] ++;
                              if (refmap[posit] < (byte) valmin)
                                                       valmin = refmap[posit];
                              if (refmap[posit] > (byte) valmax)
                                                       valmax = refmap[posit];

                              if (itemp < valminf) valminf = itemp;
                              if (itemp > valmaxf) valmaxf = itemp;

                              /**************** removed
                              if (nval > 3800 && nval < 4000) {
                                 printf(" Color from (%d,%d,%d) = %d  inten: %d -->%d \n",
                                 ixx,irow,islicet,
                                 colvol[positindx + (int) ivall],  itemp,  refmap[posit]);
                              }  ***************/
                              /********* end debug ************/
                           }      /* colorit   */
                        }         /* for (j... */
                        of1 += of1c;   of2 += of2c;
                        nf1 += nf1c;   nf2 += nf2c;
                     }            /* for (k... */
                     ptr++;
                  }                /* for (ixx... */
                  jlast += (jend - jgo);
                  jgo = jend;
               }                     /* for (irow... */
               klast += (kend - kgo);
               kgo = kend;
            }                     /* for (islicet...  */
         }                        /* if (canrotate  */
      } /* if (first_time or scaleo != scaleval) */

      /*  find location for reflection map display !!!!!!!!  */
      // !!! locc(&ixul,&iyul,nsams,nrows,&ntop,&nbot,&nrit,
      // !!!    marx,mary,marlef,martop,iwidex,ihighx,placed,
      // !!!    newline,&nuscrn,force);

      if (distit) {    /* want distance map from sirdbuf */
         scaledis =  (imageend - imagego) * imagego;
         for (iy = 0; iy < nsams*nrows; iy++) {
            refmap[iy]= (byte) (sirdbuf[iy] * scaledis);
         }
      }
      //else if (sirdit) !!!!!!!!!!!!!!!!!!!!!!!!!!!!!
      //   {   /* want to convert distances in sirdbuf to a sirdmap */
      //   posit     = refmap;
      //   positsird = sirdbuf;
      //   /* allocate memory used in sird1 */
      //  sird(positsird, posit, nsams, nrows, 0, -1);
      //  for (iy = 0; iy < nsams; iy++)
      //    {  /* call sird1 for each line of image */
      //    sird(positsird, posit, nsams, nrows,iy,0);
      //    posit     += nsams;
      //    positsird += nsams;
      //    }
      /* free memory used in sird1 */
      // sird(positsird, posit, nsams, nrows, 0, 1);!!!!!!!!!!!!!!!!!!
      //   }

      /*  display the reflection, sird, or distance map  */ //!!!!!!!!!!
      //wipic(icontx,refmap,ixul,iyul,nsams,nrows,true,false,&imagel);

      return (refmap);

   } // end of surfmethod

}







