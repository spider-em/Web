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

public class UtilGUIVolume extends JFrame implements ChangeListener,
                                                           ActionListener {

   private    UtilParameters utilParameters;
   protected  JTabbedPane jTabbedPane;
   private    UtilGUIVolume_2 utilGUIVol2Montage;
   private    UtilGUIVolume_3 utilGUIVol2Slice;
   private    UtilGUIVolume_4 utilGUIVol2Movie;
   protected  UtilGUIVolume_5 utilGUIVol2Surface;

   private UtilParametersVolume utilParametersVolume;

   public UtilGUIVolume(SpiderImage spiVol, String imageType,
                                JButton closeAll, UtilParameters utilParam) {

      utilParameters = utilParam;
      utilParametersVolume = new UtilParametersVolume();
      jTabbedPane = new JTabbedPane(SwingConstants.TOP);

      utilGUIVol2Montage =  new UtilGUIVolume_2(spiVol, imageType, closeAll,
                                                utilParam,utilParametersVolume);
      utilGUIVol2Slice   =  new UtilGUIVolume_3(spiVol, imageType, closeAll,
                                                utilParam,utilParametersVolume);
      utilGUIVol2Movie   =  new UtilGUIVolume_4(spiVol, imageType, closeAll,
                                                utilParam,utilParametersVolume);
      utilGUIVol2Surface =  new UtilGUIVolume_5(spiVol, imageType, closeAll,
                                                utilParam,utilParametersVolume);

      jTabbedPane.addTab("Montage", utilGUIVol2Montage);
      jTabbedPane.addTab("Slicer",  utilGUIVol2Slice);
      jTabbedPane.addTab("Movie", utilGUIVol2Movie);
      jTabbedPane.addTab("Surface", utilGUIVol2Surface);

      getContentPane().add(jTabbedPane, "Center");

      setTitle("Volume Viewer");
      setSize(300,400);

      //set the window position properly, so that windows will not be overlayed.
      setLocation(200,5);

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
            if (utilGUIVol2Montage.getImageFrame().isShowing()) {
               utilGUIVol2Montage.getImageFrame().disposeAuxiliaryWindows();
               utilGUIVol2Montage.getImageFrame().dispose();
            }
         }
         if(utilParametersVolume.sliceXShown) {
            if (utilGUIVol2Slice.getImageFrameX().isShowing()) {
               utilGUIVol2Slice.getImageFrameX().disposeAuxiliaryWindows();
               utilGUIVol2Slice.getImageFrameX().dispose();
            }
         }
         if(utilParametersVolume.sliceYShown) {
            if (utilGUIVol2Slice.getImageFrameY().isShowing()) {
               utilGUIVol2Slice.getImageFrameY().disposeAuxiliaryWindows();
               utilGUIVol2Slice.getImageFrameY().dispose();
            }
         }
         if(utilParametersVolume.sliceZShown) {
            if (utilGUIVol2Slice.getImageFrameZ().isShowing()) {
               utilGUIVol2Slice.getImageFrameZ().disposeAuxiliaryWindows();
               utilGUIVol2Slice.getImageFrameZ().dispose();
            }
         }
         if(utilParametersVolume.movieXShown) {
            if (utilGUIVol2Movie.getImageFrameX().isShowing()) {
               utilGUIVol2Movie.getImageFrameX().disposeAuxiliaryWindows();
               utilGUIVol2Movie.getImageFrameX().dispose();
            }
         }
         if(utilParametersVolume.movieYShown) {
            if (utilGUIVol2Movie.getImageFrameY().isShowing()) {
               utilGUIVol2Movie.getImageFrameY().disposeAuxiliaryWindows();
               utilGUIVol2Movie.getImageFrameY().dispose();
            }
         }
         if(utilParametersVolume.movieZShown) {
            if (utilGUIVol2Movie.getImageFrameZ().isShowing()) {
               utilGUIVol2Movie.getImageFrameZ().disposeAuxiliaryWindows();
               utilGUIVol2Movie.getImageFrameZ().dispose();
            }
         }

         if(utilParametersVolume.surfaceShown) {
            // the following if statement will not create any error message
            // if the montage menu has been used to create a montage at least once.
            // this is ensured via variable utilParametersVolume.montageXYZShown.
            if (utilGUIVol2Surface.getImageFrame().isShowing()) {
               utilGUIVol2Surface.getImageFrame().disposeAuxiliaryWindows();
               utilGUIVol2Surface.getImageFrame().dispose();
            }
         }

         if(utilParametersVolume.montagedSurfaceShown) {
            // the following if statement will not create any error message
            // if the montage menu has been used to create a montage at least once.
            // this is ensured via variable utilParametersVolume.montageXYZShown.
            if (utilGUIVol2Surface.getMontagedImageFrame().isShowing()) {
                utilGUIVol2Surface.getMontagedImageFrame().disposeAuxiliaryWindows();
                utilGUIVol2Surface.getMontagedImageFrame().dispose();
            }
         }
      }
   }
}
