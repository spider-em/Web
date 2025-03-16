package web;

import javax.media.jai.*;
import javax.media.jai.iterator.*;
import java.awt.*;
import java.awt.event.*;
/**
 * Title:        WEB: A GUI for viewing and analyzing SPIDER images
 * Description:  WEB is a graphical user interface for viewing and analyzing SPIDER images.
 * Copyright:    Copyright (c) 2001
 * Company:      Health Research Inc.
 * @author Bimal K. Rath
 * @version 1.0
 */

public class UtilImageStatistics {
   // for iterative use
   private RandomIter randomIter;
   // for RGB data, color pixels
   // private float[] pixelData = new float[3];
   private int[] pixelData = new int[3];
   private WebDisplayPanel wdp;

   public UtilImageStatistics() {
   }

   // construct is not used anywhere
   public UtilImageStatistics(PlanarImage planarImage) {
     this.randomIter = RandomIterFactory.create(planarImage,
                                                   planarImage.getBounds());
   }

   // construct is not used anywhere
   public UtilImageStatistics(WebDisplayPanel wdp) {
     // convert byte image using lookup table operation
     // so getPixel(int,int,float[]) method works.
     // wdp.makePlanarImage();
     this.randomIter = RandomIterFactory.create((PlanarImage)wdp.outImage,
                                  ((PlanarImage)wdp.outImage).getBounds());
   }

  public UtilImageStatistics(WebDisplayPanel wdp, UtilImageManipulate uim) {
     // convert byte image using lookup table
     // so getPixel(int,int,float[]) method works.
     // wdp.makePlanarImage();
     // reset the scale, translation and keep the contrast as set by the user
     uim.resetImageScale();
     uim.setHighContrast(uim.highContrast);
     uim.setLowContrast(uim.lowContrast);

     this.randomIter = RandomIterFactory.create((PlanarImage)wdp.outImage,
                                  ((PlanarImage)wdp.outImage).getBounds());
   }

   public synchronized Color getPixel(int x, int y) {
      randomIter.getPixel(x, y, pixelData);
      return new Color(pixelData[0], pixelData[1], pixelData[2]);
   }

}
