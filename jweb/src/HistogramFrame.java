package web;

import javax.media.jai.*;
import java.awt.image.*;
import javax.swing.*;
import java.awt.image.*;
import java.awt.image.renderable.*;



/**
 * <p>Title: WEB: A GUI for viewing and analyzing SPIDER images</p>
 * <p>Description: WEB is a graphical user interface for viewing and analyzing SPIDER images.</p>
 * <p>Copyright: Copyright (c) 2001</p>
 * <p>Company: Health Research Inc.</p>
 * @author Bimal K. Rath
 * @version 1.0
 */

public class HistogramFrame extends JFrame{
   WebDisplayPanel panel;
   RenderedImage image;
   Histogram hist;

   public HistogramFrame(WebDisplayPanel wdp, UtilParameters utilParam) {
      panel = wdp;
      image = wdp.img;

      int [] bins = {256};
      double[] low = {0};
      double[] high = {255};
      ROI roi = null;
      hist = new Histogram(bins,low,high);
      ParameterBlock pb = new ParameterBlock();
      pb.addSource(image);
      pb.add(roi);
      pb.add(1);
      pb.add(1);
      pb.add(bins);
      pb.add(low);
      pb.add(high);
      PlanarImage histogram = (PlanarImage) JAI.create("histogram",pb);
      hist = (Histogram) histogram.getProperty("histogram");
      int [] numPix = hist.getBins(0);
      float [][] xyDataArray = new float [numPix.length] [2];
      // x data values
      for (int i = 0; i < numPix.length; i++) {
         xyDataArray[i][0] = (float)i;
      }

      // y data values
      for (int i = 0; i < numPix.length; i++) {
         xyDataArray[i][1] = (float)numPix[i];
      }

      this.setSize(500,500);
      this.setLocation(200,200);

      int margin = 70;
      // usable space from jframe
      int frameSizeX = this.getWidth();
      int frameSizeY = this.getHeight();
      int noOfXTicks = 10;
      int noOfYTicks = 10;

      String graphType = "bar";
      this.getContentPane().add(new UtilPlotGraphPanel(xyDataArray, margin,
                   frameSizeX, frameSizeY, noOfXTicks, noOfYTicks, graphType,
                                                                   utilParam));
      String frameTitle = "Histogram:   "+utilParam.currDirectory;
      this.setTitle(frameTitle);
      this.setVisible(true);
   }


}
