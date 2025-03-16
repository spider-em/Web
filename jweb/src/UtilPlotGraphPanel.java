package web;

import java.awt.Graphics2D;
import java.awt.Color;
import java.awt.*;
import javax.swing.*;
import java.awt.font.*;
import java.awt.geom.*;
import java.text.*;



/**
 * <p>Title: WEB: A GUI for viewing and analyzing SPIDER images</p>
 * <p>Description: WEB is a graphical user interface for viewing and analyzing SPIDER images.</p>
 * <p>Copyright: Copyright (c) 2001</p>
 * <p>Company: Health Research Inc.</p>
 * @author Bimal K. Rath
 * @version 1.0
 */
   public class UtilPlotGraphPanel extends JPanel {

   float[] []data;

   String typeOfPlot;
   Font font;
   UtilParameters utilParameters;
   int margin, frameSizeX, frameSizeY, noOfXTicks, noOfYTicks;
   public boolean plotXAxis = true;
   int [] [] dendroClass;


   public UtilPlotGraphPanel(float [][] dataPassed, int marginIN,
              int frameSizeXIN, int frameSizeYIN, int noOfXTicksIN,
              int noOfYTicksIN, String typeOfGraph, UtilParameters utilParam) {

      typeOfPlot = typeOfGraph;
      data = dataPassed;
      margin = marginIN;
      frameSizeX = frameSizeXIN;
      frameSizeY = frameSizeYIN;
      noOfXTicks = noOfXTicksIN;
      noOfYTicks = noOfYTicksIN;
      utilParameters = utilParam;

      font = new Font("lucida sans regular", Font.PLAIN, 16);
      setBackground(Color.white);
   }

   protected void paintComponent(Graphics g) {
      super.paintComponent(g);
      Graphics2D g2 = (Graphics2D)g;
      g2.setFont(font);
      FontRenderContext frc = g2.getFontRenderContext();

      // usable space from jframe
      int frameWidth = frameSizeX;
      int frameHeight = frameSizeY;

      // calculate tick interval
      float xTickIntervalPixel = (frameWidth - 2*margin) / noOfXTicks;
      float yTickIntervalPixel = (frameHeight - 2*margin) / noOfYTicks;
      float[] dataVals = findStatistics();
      float yTickIntervalValue = dataVals[2] / noOfYTicks;
      float xTickIntervalValue = dataVals[5]/noOfXTicks;
      g2.setColor(Color.blue);

      // draw y-axis / origin is at left hand upper corner
      g2.draw(new Line2D.Double(margin, margin, margin, frameHeight - margin));

      // draw y tick marks
      float x1 = margin, y1 = margin, x2 = margin - 3, y2;
      for (int j = 0; j <= noOfYTicks; j++) {
        g2.draw(new Line2D.Double(x1, y1, x2, y1));
        y1 = y1 + yTickIntervalPixel;
      }

      // draw y labels
      String text;
      LineMetrics lm;
      float xPos, yPos;
      float textWidth, height;
      DecimalFormat df1 = new DecimalFormat("####0.0");

      for (int j = 0; j <= noOfYTicks; j++) {
         //text = String.valueOf(dataVals[1] - (int)(j * yTickIntervalValue));
         text = df1.format(dataVals[1] - (j * yTickIntervalValue));
         textWidth = (float) font.getStringBounds(text, frc).getWidth();
         lm = font.getLineMetrics(text, frc);
         height = lm.getAscent();
         xPos = margin - textWidth - 7;
         yPos = margin + (j * yTickIntervalPixel) + height / 2;
         g2.drawString(text, xPos, yPos);
      }

      // draw x-axis / origin is at left hand upper corner
      if (plotXAxis)   {
         g2.draw(new Line2D.Double(margin, frameHeight - margin,
                                    frameWidth - margin, frameHeight - margin));
         // draw x tick marks
         x1 = margin; y1 = frameHeight - margin; y2 = y1 + 3;
         for(int j = 0; j <= noOfXTicks; j++) {
            g2.draw(new Line2D.Double(x1, y1, x1, y2));
            x1 = x1 + xTickIntervalPixel;
         }

         // draw x labels
         for(int j = 0; j <= noOfXTicks; j++) {
            text = String.valueOf(0 + (int)(j * xTickIntervalValue));
            textWidth = (float)font.getStringBounds(text, frc).getWidth();
            lm = font.getLineMetrics(text, frc);
            height = lm.getHeight();
            xPos = margin + j * xTickIntervalPixel - textWidth/2;
            yPos = frameHeight - margin;
            g2.drawString(text, xPos, yPos + height);
         }
      }
      // plot data
      x1 = margin;
      y1 = frameHeight - margin;
      float  yScale = (float)((frameHeight - 2.0*margin) / dataVals[2]);
      float xScale = (float)((frameWidth - 2.0*margin) / dataVals[5]);
      int rectWidth = 0;
      g2.setColor(Color.green);

      if( typeOfPlot == "bar") {
         for(int j = 0; j < data.length; j++) {
            x2 = x1;
            y2 = margin + ((frameHeight - 2*margin)-yScale*data[j][1]) ;
            rectWidth = (int)xScale+1;
            if(j > 0) g2.fillRect((int)x2,(int)y2,rectWidth,(int)(y1-y2));
            x1 = margin + xScale*data[j][0];
         }
      }
      else if (  typeOfPlot == "line") {
         for(int j = 0; j < data.length-1; j++) {
            x1 = margin + (xScale*data[j][0]) ;
            y1 = margin + ((frameHeight - 2*margin)-yScale*data[j][1]) ;
            x2 = margin + (xScale*data[j+1][0]) ;
            y2 = margin + ((frameHeight - 2*margin)-yScale*data[j+1][1]) ;
            if(j > 0) g2.draw(new Line2D.Double(x1,y1,x2,y2));
         }
      }
      else if (  typeOfPlot == "dendrogram") {
         int countLoop = 0;
         for(int j = 0; j < data.length-1; j++) {
            x1 = margin + (xScale*data[j][0]) ;
            y1 = margin + ((frameHeight - 2*margin)-yScale*data[j][1]) ;
            x2 = margin + (xScale*data[j+1][0]) ;
            y2 = margin + ((frameHeight - 2*margin)-yScale*data[j+1][1]) ;
            countLoop = countLoop + 1;

            // skip a line connection after every third line is drawn
            if (countLoop != 4) {
               if (j > 0) g2.draw(new Line2D.Double(x1, y1, x2, y2));
            }
            else {
               countLoop = 0;
            }
         }

         // draw x labels
         for(int j = 0; j <= dendroClass.length-1; j++) {
            xTickIntervalPixel = (frameWidth - 2*margin) / (dendroClass.length - 1);
            text = String.valueOf(dendroClass[j][1]);
            textWidth = (float)font.getStringBounds(text, frc).getWidth();
            lm = font.getLineMetrics(text, frc);
            height = lm.getHeight();
            xPos = margin + (j+1) * xTickIntervalPixel - textWidth/2;
            yPos = frameHeight - margin;
            g2.drawString(text, xPos, yPos + height);
         }
      }

      else {
         String str = "\n "+ typeOfPlot + " Graph is not supported yet ! \n";
         utilParameters.outputMessageTextArea.append(str);
      }
   }
   private float[] findStatistics() {
      float xMax = Float.MIN_VALUE;
      float xMin = Float.MAX_VALUE;
      float yMax = Float.MIN_VALUE;
      float yMin = Float.MAX_VALUE;

      for(int j = 0; j < data.length; j++) {
         if(data[j][1] < yMin) yMin = data[j][1];
         if(data[j][1] > yMax) yMax = data[j][1];
      }
      float ySpan = yMax - yMin;

      for(int j = 0; j < data.length; j++) {
         if(data[j][0] < xMin) xMin = data[j][0];
         if(data[j][0] > xMax) xMax = data[j][0];
      }
      float xSpan = xMax - xMin;

      return new float[] { yMin, yMax, ySpan, xMin, xMax, xSpan, };
   }
}
