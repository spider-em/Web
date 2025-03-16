package web;

import java.awt.*;
import java.awt.image.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.event.*;
import javax.media.jai.*;
import java.io.*;
import java.util.*;

/**
 * Title:        WEB: A GUI for viewing and analyzing SPIDER images
 * Description:  WEB is a graphical user interface for viewing and analyzing SPIDER images.
 * Copyright:    Copyright (c) 2001
 * Company:      Health Research Inc.
 * @author Bimal K. Rath
 * @version 1.0
 */

public class UtilGUIImageSeries extends JFrame implements ChangeListener,
                                   ActionListener {

   private UtilParameters utilParameters;
   private JTabbedPane jTabbedPane;
   private UtilGUIVolume_2 utilGUIVolMontage;
   private UtilGUIVolume_3 utilGUIVolSlice;
   private UtilGUIVolume_4 utilGUIVolMovie;
   private UtilGUIImageSeriesCategorize utilGUIImageSeriesCategorize;
   private UtilGUIImageSeriesMarker utilGUIImageSeriesMarker;
   private UtilGUIImageSeriesTiltPair utilGUIImageSeriesTiltPair;


   private UtilParametersVolume utilParametersVolume;
   private UtilParametersImageSeries utilParametersImageSeries;

   public UtilGUIImageSeries(SpiderImage spiVol, String imageType,
              String [] fileNames, JButton closeAll,UtilParameters utilParam) {

      utilParameters = utilParam;
      utilParametersVolume = new UtilParametersVolume();
      utilParametersImageSeries = new UtilParametersImageSeries();
      jTabbedPane = new JTabbedPane(SwingConstants.TOP);

      utilGUIVolMontage =  new UtilGUIVolume_2(spiVol, imageType, closeAll,
                                              utilParam, utilParametersVolume);
      utilGUIVolSlice   =  new UtilGUIVolume_3(spiVol, imageType, closeAll,
                                              utilParam, utilParametersVolume);
      utilGUIVolMovie   =  new UtilGUIVolume_4(spiVol, imageType, closeAll,
                                              utilParam, utilParametersVolume);
      utilGUIImageSeriesCategorize = new UtilGUIImageSeriesCategorize(spiVol,
                    fileNames, closeAll, utilParam, utilParametersImageSeries);

      //initialize the marker editing tab pane
      utilGUIImageSeriesMarker   =  new UtilGUIImageSeriesMarker(spiVol,
          imageType, closeAll, utilParam, utilParametersImageSeries);

      try {

         //initialize the tilt pair particle picking tab pane
         utilGUIImageSeriesTiltPair   =  new UtilGUIImageSeriesTiltPair(spiVol,
                     imageType, closeAll, utilParam, utilParametersImageSeries);
      }
      catch (AWTException e) {
         String str = "Error in moving cursor: UtilGUIimageSeries" + e + "\n";
         utilParameters.outputMessageTextArea.append(str);
      }






      jTabbedPane.addTab("Montage", utilGUIVolMontage);
      jTabbedPane.addTab("Slide",  utilGUIVolSlice);
      jTabbedPane.addTab("Movie", utilGUIVolMovie);
      jTabbedPane.addTab("Pick Particle/Categorize", utilGUIImageSeriesCategorize);

      //add the Tomo/Marker editing tab pane
      jTabbedPane.addTab("Markers-Tomo", utilGUIImageSeriesMarker);

      //add the TiltPair/Marker editing tab pane
      jTabbedPane.addTab("Markers-Tilt Pair", utilGUIImageSeriesTiltPair);
      //if more than 2 images are selected then disable the "Markers-Tilt Pair" tabbed pane
      int index = jTabbedPane.getTabCount()-1;
      if (fileNames.length !=2) {
         jTabbedPane.setEnabledAt(index, false);
      }

      getContentPane().add(jTabbedPane, "Center");

      setTitle("Image Series Viewer");
      setSize(300,400);

      //set the window pop up location, so it will not
      //overlayed by Image Series Viewer window.
      setLocation(200,0);

//      setLocation(25,25);
      pack();
      setVisible(true);
  }

   public void stateChanged(ChangeEvent event) {

   }

   public void actionPerformed(ActionEvent evt) {
      // close window when closeAll button selected from the main WEB menu
      this.dispose();
   }

   protected void processWindowEvent(WindowEvent e) {
      super.processWindowEvent(e);
      if (e.getID() == WindowEvent.WINDOW_CLOSING) {
         if(utilParametersVolume.montageXYZShown) {
            // the following if statement will not create any error message
            // if the montage menu has been used to create a montage at least once.
            // this is ensured via variable utilParametersVolume.montageXYZShown.
            if (utilGUIVolMontage.getImageFrame().isShowing()) {
               utilGUIVolMontage.getImageFrame().disposeAuxiliaryWindows();
               utilGUIVolMontage.getImageFrame().dispose();
            }
         }
         if(utilParametersVolume.sliceXShown) {
            if (utilGUIVolSlice.getImageFrameX().isShowing()) {
               utilGUIVolSlice.getImageFrameX().disposeAuxiliaryWindows();
               utilGUIVolSlice.getImageFrameX().dispose();
            }
         }
         if(utilParametersVolume.sliceYShown) {
            if (utilGUIVolSlice.getImageFrameY().isShowing()) {
               utilGUIVolSlice.getImageFrameY().disposeAuxiliaryWindows();
               utilGUIVolSlice.getImageFrameY().dispose();
            }
         }
         if(utilParametersVolume.sliceZShown) {
            if (utilGUIVolSlice.getImageFrameZ().isShowing()) {
               utilGUIVolSlice.getImageFrameZ().disposeAuxiliaryWindows();
               utilGUIVolSlice.getImageFrameZ().dispose();
            }
         }
         if(utilParametersVolume.movieXShown) {
            if (utilGUIVolMovie.getImageFrameX().isShowing()) {
               utilGUIVolMovie.getImageFrameX().disposeAuxiliaryWindows();
               utilGUIVolMovie.getImageFrameX().dispose();
            }
         }
         if(utilParametersVolume.movieYShown) {
            if (utilGUIVolMovie.getImageFrameY().isShowing()) {
               utilGUIVolMovie.getImageFrameY().disposeAuxiliaryWindows();
               utilGUIVolMovie.getImageFrameY().dispose();
            }
         }
         if(utilParametersVolume.movieZShown) {
            if (utilGUIVolMovie.getImageFrameZ().isShowing()) {
               utilGUIVolMovie.getImageFrameZ().disposeAuxiliaryWindows();
               utilGUIVolMovie.getImageFrameZ().dispose();
            }
         }

         if(utilParametersImageSeries.categorizeShown) {
            // the following if statement will not create any error message
            // if the montage menu has been used to create a montage at least once.
            // this is ensured via variable utilParametersVolume.montageXYZShown.
            if (utilGUIImageSeriesCategorize.getImageFrame().isShowing()) {
               utilGUIImageSeriesCategorize.getImageFrame().disposeAuxiliaryWindows();
               utilGUIImageSeriesCategorize.getImageFrame().dispose();
            }
         }

         if(utilParametersImageSeries.markerSeriesShown) {
            if (utilGUIImageSeriesMarker.getImageFrameZ().isShowing()) {
              //Close the marker editing window
               utilGUIImageSeriesMarker.getImageFrameZ().disposeAuxiliaryWindows();
               utilGUIImageSeriesMarker.getImageFrameZ().dispose();
            }
         }

         if(utilParametersImageSeries.tiltPairShown) {
            if (utilGUIImageSeriesTiltPair.getImageFrameZ().isShowing()) {
              //Close the marker editing window
               utilGUIImageSeriesTiltPair.getImageFrameZ().disposeAuxiliaryWindows();
               utilGUIImageSeriesTiltPair.getImageFrameZ().dispose();
               utilGUIImageSeriesTiltPair.getImageFrameZ2().disposeAuxiliaryWindows();
               utilGUIImageSeriesTiltPair.getImageFrameZ2().dispose();
            }
         }




      }
   }
}

