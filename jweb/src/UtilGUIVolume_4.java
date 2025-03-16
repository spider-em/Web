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

public class UtilGUIVolume_4 extends JPanel implements Runnable{

   private SpiderImage spiderVolume;
   private UtilParameters utilParameters;
   private int nsam, nrow, nslice;

   private JCheckBox treatAsVolumeCheckBox;
   private JLabel  axisLabel, rowsLabel, columnsLabel;
   private JLabel  slicesLabel, zSliceLabel;
   private JRadioButton xRadioButton, yRadioButton, zRadioButton;
   private ButtonGroup axisButtonGroup;
   private JRadioButton twoWayJRadioButton, oneWayJRadioButton;
   private ButtonGroup whichWayMovieButtonGroup;
   private JTextField runCyclesJTextField, delayJTextField;

   private JLabel delayLabel;
   private JTextField rowsLowEndJTextField, rowsHighEndJTextField;
   private JTextField columnsLowEndJTextField, columnsHighEndJTextField;
   private JTextField slicesLowEndJTextField, slicesHighEndJTextField;
   private JTextField delaySliderJTextField;
   private JButton startMovieJButton, stopMovieJButton;
   private JSlider rowsLowEndSlider, rowsHighEndSlider, columnsLowEndSlider;
   private JSlider columnsHighEndSlider, slicesLowEndSlider, slicesHighEndSlider;
   private JSlider delaySlider;
   private JLabel dummyJLabel;

   private int columnsLVal, columnsHVal, rowsLVal, rowsHVal, slicesLVal, slicesHVal;
   private int slicesLValSave, slicesHValSave,columnsLValSave, columnsHValSave;
   private int rowsLValSave, rowsHValSave;
   private JButton jButtonCloseAll;
   private ImageFrame imageFrameX, imageFrameY, imageFrameZ;
   private String axisSelected, typeOfImage;
   private boolean imageFrameXOpen = false;
   private boolean imageFrameYOpen = false;
   private boolean imageFrameZOpen = false;
   private boolean showSliceXactivatedForFirstTime = true;
   private boolean showSliceYactivatedForFirstTime = true;
   private boolean showSliceZactivatedForFirstTime = true;
   private Thread animatorThread;
   private int delayTime;
   private boolean increment;
   private String str;
   private float scale, xTrans, yTrans;
   private int lowContrast, highContrast;
   private UtilParametersVolume utilParametersVolume;

   public UtilGUIVolume_4(SpiderImage spiVol, String imageType, JButton closeAll,
                UtilParameters utilParam, UtilParametersVolume utilParamVol) {

      spiderVolume = spiVol;
      utilParameters = utilParam;
      jButtonCloseAll = closeAll;
      utilParametersVolume = utilParamVol;
      typeOfImage = imageType;
      nsam = spiderVolume.getnsam();
      nrow = spiderVolume.getnrow();
      nslice = spiderVolume.getnslice();

      GridBagLayout gridBagLayout = new GridBagLayout();
      this.setLayout(gridBagLayout);
      GridBagConstraints gridBagConstraints = new GridBagConstraints();

      axisLabel = new JLabel("Axis");
      addComponent(axisLabel,gridBagConstraints,4,4,1,1);
      if (imageType == "ImageSeries") axisLabel.setEnabled(false);

      xRadioButton = new JRadioButton("X");
      xRadioButton.setSelected(false);
      addComponent(xRadioButton,gridBagConstraints,5,4,1,1);
      if (imageType == "ImageSeries")  xRadioButton.setEnabled(false);

      yRadioButton = new JRadioButton("Y");
      yRadioButton.setSelected(false);
      addComponent(yRadioButton,gridBagConstraints,9,4,1,1);
      if (imageType == "ImageSeries")  yRadioButton.setEnabled(false);

      zRadioButton = new JRadioButton("Z");
      zRadioButton.setSelected(true);
      addComponent(zRadioButton,gridBagConstraints,10,4,1,1);
      if (imageType == "ImageSeries")  zRadioButton.setEnabled(false);

      axisButtonGroup = new ButtonGroup();
      axisButtonGroup.add(xRadioButton);
      axisButtonGroup.add(yRadioButton);
      axisButtonGroup.add(zRadioButton);

      if (imageType == "ImageSeries") {
         treatAsVolumeCheckBox = new JCheckBox("Treat As Volume");
         addComponent(treatAsVolumeCheckBox,gridBagConstraints,15,4,3,1);
      }

      dummyJLabel = new JLabel(" ");
      addComponent(dummyJLabel,gridBagConstraints,15,5,1,1);

      columnsLabel = new JLabel("Columns");
      addComponent(columnsLabel,gridBagConstraints,1,7,2,1);

      columnsLowEndSlider = new JSlider(SwingConstants.HORIZONTAL,1,nsam,1);
      addComponent(columnsLowEndSlider,gridBagConstraints,3,7,6,1);
      columnsLowEndJTextField = new JTextField(4);
      columnsLowEndJTextField.setEditable(false);
      addComponent(columnsLowEndJTextField,gridBagConstraints,9,7,1,1);


      columnsHighEndSlider = new JSlider(SwingConstants.HORIZONTAL,1,nsam,nsam);
      addComponent(columnsHighEndSlider,gridBagConstraints,10,7,6,1);
      columnsHighEndJTextField = new JTextField(4);
      columnsHighEndJTextField.setEditable(false);
      addComponent(columnsHighEndJTextField,gridBagConstraints,16,7,1,1);

      rowsLabel = new JLabel("Rows");
      addComponent(rowsLabel,gridBagConstraints,1,11,3,1);

      rowsLowEndSlider = new JSlider(SwingConstants.HORIZONTAL,1,nrow,1);
      addComponent(rowsLowEndSlider,gridBagConstraints,3,11,6,1);
      rowsLowEndJTextField = new JTextField(4);
      rowsLowEndJTextField.setEditable(false);
      addComponent(rowsLowEndJTextField,gridBagConstraints,9,11,1,1);

      rowsHighEndSlider = new JSlider(SwingConstants.HORIZONTAL,1,nrow,nrow);
      addComponent(rowsHighEndSlider,gridBagConstraints,10,11,6,2);
      rowsHighEndJTextField = new JTextField(4);
      rowsHighEndJTextField.setEditable(false);
      addComponent(rowsHighEndJTextField,gridBagConstraints,16,11,1,1);

       if (imageType == "ImageSeries") {
         slicesLabel = new JLabel("Images");
      }
      else {
         slicesLabel = new JLabel("Slices");
      }
      addComponent(slicesLabel,gridBagConstraints,1,13,2,1);

      slicesLowEndSlider = new JSlider(SwingConstants.HORIZONTAL,1,nslice,1);
      addComponent(slicesLowEndSlider,gridBagConstraints,3,13,6,1);
      slicesLowEndJTextField = new JTextField(4);
      slicesLowEndJTextField.setEditable(false);
      addComponent(slicesLowEndJTextField,gridBagConstraints,9,13,1,1);

      slicesHighEndSlider = new JSlider(SwingConstants.HORIZONTAL,1,nslice,nslice);
      addComponent(slicesHighEndSlider,gridBagConstraints,10,13,6,1);
      slicesHighEndJTextField = new JTextField(4);
      slicesHighEndJTextField.setEditable(false);
      addComponent(slicesHighEndJTextField,gridBagConstraints,16,13,1,1);

      oneWayJRadioButton = new JRadioButton("One-way");
      oneWayJRadioButton.setSelected(true);
      addComponent(oneWayJRadioButton,gridBagConstraints,5,16,1,1);

      twoWayJRadioButton = new JRadioButton("Two-way");
      twoWayJRadioButton.setSelected(false);
      addComponent(twoWayJRadioButton,gridBagConstraints,10,16,1,1);

      whichWayMovieButtonGroup = new ButtonGroup();
      whichWayMovieButtonGroup.add(oneWayJRadioButton);
      whichWayMovieButtonGroup.add(twoWayJRadioButton);

      delayLabel = new JLabel("Delay(msec)");
      addComponent(delayLabel,gridBagConstraints,1,19,2,1);

      delaySlider = new JSlider(SwingConstants.HORIZONTAL,10,1024,10);
      addComponent(delaySlider,gridBagConstraints,3,19,6,1);
      delaySliderJTextField = new JTextField(4);
      delaySliderJTextField.setEditable(false);
      addComponent(delaySliderJTextField,gridBagConstraints,9,19,1,1);

      dummyJLabel = new JLabel("    ");
      addComponent(dummyJLabel,gridBagConstraints,1,20,10,1);

      startMovieJButton = new JButton("    START    ");
      addComponent(startMovieJButton,gridBagConstraints,2,21,7,1);

      stopMovieJButton = new JButton("     STOP    ");
      addComponent(stopMovieJButton,gridBagConstraints,9,21,7,1);
      stopMovieJButton.setEnabled(false);

      dummyJLabel = new JLabel(" ");
      addComponent(dummyJLabel,gridBagConstraints,15,22,1,1);

      columnsLowEndJTextField.setText("1");
      columnsLowEndSlider.addChangeListener(new ChangeListener() {
         public void stateChanged(ChangeEvent ce) {
            // update display
            columnsLowEndJTextField.setText(""+columnsLowEndSlider.getValue());
            checkLowEndSliderValidity(columnsLowEndSlider, columnsHighEndSlider,
                      columnsLowEndJTextField, columnsHighEndJTextField);
         }
      });

      columnsHighEndJTextField.setText(Integer.toString(nsam));
      columnsHighEndSlider.addChangeListener(new ChangeListener() {
         public void stateChanged(ChangeEvent ce) {
           // update display
           columnsHighEndJTextField.setText(""+columnsHighEndSlider.getValue());
           checkHighEndSliderValidity(columnsLowEndSlider, columnsHighEndSlider,
                      columnsLowEndJTextField, columnsHighEndJTextField);
         }
      });


      rowsLowEndJTextField.setText("1");
      rowsLowEndSlider.addChangeListener(new ChangeListener() {
         public void stateChanged(ChangeEvent ce) {
           // update display
           rowsLowEndJTextField.setText(""+rowsLowEndSlider.getValue());
           checkLowEndSliderValidity(rowsLowEndSlider, rowsHighEndSlider,
                      rowsLowEndJTextField, rowsHighEndJTextField);
         }
      });

      rowsHighEndJTextField.setText(Integer.toString(nrow));
      rowsHighEndSlider.addChangeListener(new ChangeListener() {
         public void stateChanged(ChangeEvent ce) {
           // update display
           rowsHighEndJTextField.setText(""+rowsHighEndSlider.getValue());
           checkHighEndSliderValidity(rowsLowEndSlider, rowsHighEndSlider,
                      rowsLowEndJTextField, rowsHighEndJTextField);
         }
      });

      slicesLowEndJTextField.setText("1");
      slicesLowEndSlider.addChangeListener(new ChangeListener() {
         public void stateChanged(ChangeEvent ce) {
            // update display
            slicesLowEndJTextField.setText(""+slicesLowEndSlider.getValue());
            checkLowEndSliderValidity(slicesLowEndSlider, slicesHighEndSlider,
                      slicesLowEndJTextField, slicesHighEndJTextField);
         }
      });

      slicesHighEndJTextField.setText(Integer.toString(nslice));
      slicesHighEndSlider.addChangeListener(new ChangeListener() {
         public void stateChanged(ChangeEvent ce) {
           // update display
           slicesHighEndJTextField.setText(""+slicesHighEndSlider.getValue());
           checkHighEndSliderValidity(slicesLowEndSlider, slicesHighEndSlider,
                      slicesLowEndJTextField, slicesHighEndJTextField);
         }
      });


      delaySliderJTextField.setText("10");
      delaySlider.addChangeListener(new ChangeListener() {
         public void stateChanged(ChangeEvent ce) {
            // update display
            delaySliderJTextField.setText(""+delaySlider.getValue());
         }
      });

      startMovieJButton.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
           startMovieJButton_actionPerformed(e);
         }
      });

      stopMovieJButton.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
           stopMovieJButton_actionPerformed(e);
         }
      });

      if (imageType == "ImageSeries") {
         treatAsVolumeCheckBox.addActionListener(
                                          new java.awt.event.ActionListener() {
            public void actionPerformed(ActionEvent e) {
              treatAsVolumeCheckBox_actionPerformed(e);
            }
         });
         enableAllSliders (false);
      }
   }

    public void addComponent(Component comp,
                               GridBagConstraints gridBagConstraints, int gridX,
                               int gridY, int gridWidth, int gridHeight) {

      gridBagConstraints.gridx = gridX;
      gridBagConstraints.gridy = gridY;
      gridBagConstraints.gridwidth = gridWidth;
      gridBagConstraints.gridheight = gridHeight;
      // will grow to fill the space if the window size is changed
      gridBagConstraints.weightx = 100;
      gridBagConstraints.weighty = 100;
      // make it aligned; didn't put the showSlice button in the middle,
      // so commented
      // gridBagConstraints.fill = GridBagConstraints.BOTH;
      this.add(comp,gridBagConstraints);
   }

   void treatAsVolumeCheckBox_actionPerformed(ActionEvent e) {
      if (treatAsVolumeCheckBox.isSelected()) {
         xRadioButton.setEnabled(true);
         yRadioButton.setEnabled(true);
         zRadioButton.setEnabled(true);
         axisLabel.setEnabled(true);
         resetSliders();
         enableAllSliders(true);
         slicesLabel.setText("Slices");
         if (imageFrameZOpen) {
            imageFrameZ.setTitle("'Z'" + " Slice");
         }
      }
      else {
         xRadioButton.setEnabled(false);
         yRadioButton.setEnabled(false);
         zRadioButton.setEnabled(false);
         zRadioButton.setSelected(true);
         axisLabel.setEnabled(false);
         resetSliders();
         enableAllSliders(false);
         slicesLabel.setText("Images");
         if (imageFrameZOpen) imageFrameZ.setTitle("Image");
      }
   }

   void startMovieJButton_actionPerformed(ActionEvent e) {

     columnsLVal = columnsLowEndSlider.getValue();
     columnsHVal = columnsHighEndSlider.getValue();
     rowsLVal = rowsLowEndSlider.getValue();
     rowsHVal = rowsHighEndSlider.getValue();
     slicesLVal = slicesLowEndSlider.getValue();
     slicesHVal = slicesHighEndSlider.getValue();

      if ((columnsLVal <= columnsHVal) && (rowsLVal <= rowsHVal) &&
                                             (slicesLVal <= slicesHVal)) {

         startMovieJButton.setEnabled(false);
         stopMovieJButton.setEnabled(true);
         enableAllSliders(false);

         axisSelected = new  String();
         if (xRadioButton.isSelected()) axisSelected = "x";
         if (yRadioButton.isSelected()) axisSelected = "y";
         if (zRadioButton.isSelected()) axisSelected = "z";

         if (axisSelected == "x") {
            columnsLValSave = columnsLVal;
            columnsHValSave = columnsHVal;
         }
         else if (axisSelected == "y") {
            rowsLValSave = rowsLVal;
            rowsHValSave = rowsHVal;
         }
         else if (axisSelected == "z") {
            slicesLValSave = slicesLVal;
            slicesHValSave = slicesHVal;
         }

         TiledImage montagedSlices = getImage();


         // if not activated for the first time, check if the window is closed
         // to use .isShowing method you need to create the object at least once
         // before OR you get an error.
         if (axisSelected == "x"){
            if (!showSliceXactivatedForFirstTime) {
               imageFrameXOpen = imageFrameX.isShowing();
               if (imageFrameXOpen) {
                  if (imageFrameX.getState() == imageFrameX.ICONIFIED) {
                     imageFrameX.setState(imageFrameX.NORMAL);
                  }
                  else {
                     // bring the frame to forefront
                     imageFrameX.show();
                  }
               }
            }
            // if activated for the first time OR if the window is not closed
            if (!imageFrameXOpen) {
               imageFrameX = new ImageFrame(montagedSlices, utilParameters);
               jButtonCloseAll.addActionListener(imageFrameX);
               imageFrameX.setTitle("'"+axisSelected.toUpperCase()+
                                                            "'" + " Slice");
               utilParametersVolume.movieXShown = true;
               imageFrameX.setLocation (50,50);
               imageFrameX.setVisible(true);
               imageFrameXOpen = true;
               showSliceXactivatedForFirstTime = false;
            }
         }
         if (axisSelected == "y"){
            // if not activated for the first time, check if the window is closed
            if (!showSliceYactivatedForFirstTime) {
               imageFrameYOpen = imageFrameY.isShowing();
               if (imageFrameYOpen) {
                  if (imageFrameY.getState() == imageFrameY.ICONIFIED) {
                     imageFrameY.setState(imageFrameY.NORMAL);
                  }
                  else {
                     // bring the frame to forefront
                     imageFrameY.show();
                  }
               }
            }
            // if activated for the first time OR if the window is not closed
            if (!imageFrameYOpen) {
               imageFrameY = new ImageFrame(montagedSlices, utilParameters);
               jButtonCloseAll.addActionListener(imageFrameY);
               imageFrameY.setTitle("'"+axisSelected.toUpperCase()+
                                                              "'" + " Slice");
               utilParametersVolume.movieYShown = true;
               imageFrameY.setLocation (60,60);
               imageFrameY.setVisible(true);
               imageFrameYOpen = true;
               showSliceYactivatedForFirstTime = false;
            }
         }

         if (axisSelected == "z"){
            // if not activated for the first time, check if the window is closed
            if (!showSliceZactivatedForFirstTime) {
               imageFrameZOpen = imageFrameZ.isShowing();
               if (imageFrameZOpen) {
                  if (imageFrameZ.getState() == imageFrameZ.ICONIFIED) {
                     imageFrameZ.setState(imageFrameZ.NORMAL);
                  }
                  else {
                     // bring the frame to forefront
                     imageFrameZ.show();
                  }
               }
            }
            // if activated for the first time OR if the window is not closed
            if (!imageFrameZOpen) {
               imageFrameZ = new ImageFrame(montagedSlices, utilParameters);
               jButtonCloseAll.addActionListener(imageFrameZ);
               if (typeOfImage == "ImageSeries" &&
                                        !treatAsVolumeCheckBox.isSelected())  {
                  imageFrameZ.setTitle("Image");
               }
               else {
                  imageFrameZ.setTitle("'Z'" + " Slice");
               }
               utilParametersVolume.movieZShown = true;
               imageFrameZ.setLocation (70,70);
               imageFrameZ.setVisible(true);
               imageFrameZOpen = true;
               showSliceZactivatedForFirstTime = false;
            }
         }
         // need a thread to do the repaint(), or else it shows the first frame and
         // then waits for the for loop to finish and the shows the last frame. This is
         // doing this way because the main thread in swing waits for the
         // actionPerformed method to return before doing the repaint()
         animatorThread = new Thread(this) ;
         animatorThread.start();
      }
      else {
         str = " Columns/Row/Slice low-end value is greater than" +
                                        " corresponding high-end value !\n ";
         utilParameters.outputMessageTextArea.setText(str);
      }
   }

   void stopMovieJButton_actionPerformed(ActionEvent e) {
      animatorThread = null;
      startMovieJButton.setEnabled(true);
      stopMovieJButton.setEnabled(false);
      if (typeOfImage == "ImageSeries" &&
                                        !treatAsVolumeCheckBox.isSelected())  {
         enableAllSliders(false);
      }
      else {
         enableAllSliders(true);
      }
   }

   public void run() {
   // thread runs here


      Thread currentThread = Thread.currentThread();
      while(currentThread == animatorThread) {
         if (axisSelected == "x") {
            imageFrameX.wdp.outImage = getImage();
            // copy out put image as input image in wdp so that EDIT and ANALYSIS
            // functions will take the image that is displayed as the input image.
            imageFrameX.wdp.img = imageFrameX.wdp.outImage;

            // note: if this makes movie display slow then then move
            // the code to find scale etc. to startMovieJButton_actionPerformed()
            // in which case one has to stop the movie before changing contrast
            // and resize etc.
            // find if scale, translation and contrast is changed
            scale         = imageFrameX.utilImageManipulate.scale;
            xTrans        = imageFrameX.utilImageManipulate.xTrans;
            yTrans        = imageFrameX.utilImageManipulate.yTrans;
            lowContrast   = imageFrameX.utilImageManipulate.lowContrast;
            highContrast  = imageFrameX.utilImageManipulate.highContrast;

            // keeps the interpolation, translation and contrast as
            // set in one image valid for all subsequent images.
            if (xTrans != 0 || yTrans != 0 || scale != 1 ||
                     lowContrast != 0 || highContrast != 255) {
              imageFrameX.utilImageManipulate.setLowContrast
                 (imageFrameX.utilImageManipulate.lowContrast);

              imageFrameX.utilImageManipulate.setHighContrast
                 (imageFrameX.utilImageManipulate.highContrast);
            }

            if (oneWayJRadioButton.isSelected()) {
               // oneway movie
               if (columnsLVal == columnsHValSave) {
                  columnsLVal = columnsLValSave;
               }
               else {
                  columnsLVal = columnsLVal + 1;
               }
            }
            else if (twoWayJRadioButton.isSelected()) {
               // twoway movie
               if (columnsLVal == columnsLValSave) increment = true;
               if (columnsLVal == columnsHValSave) increment = false;
               if (increment) {
                  columnsLVal = columnsLVal + 1;
               }
               else {
                  columnsLVal = columnsLVal - 1;
               }
            }
            delayTime = delaySlider.getValue();
            try {
               Thread.sleep((long)delayTime);
            }
            catch (InterruptedException ie) {
               String str = "Error in sleep: " + ie + "\n";
               utilParameters.outputMessageTextArea.append(str);
            }
            imageFrameX.wdp.repaint();
         }

         else if (axisSelected == "y") {
            imageFrameY.wdp.outImage = getImage();
            // copy out put image as input image in wdp so that EDIT and ANALYSIS
            // functions will take the image that is displayed as the input image.
            imageFrameY.wdp.img = imageFrameY.wdp.outImage;

            // note: if this makes movie display slow then then move
            // the code to find scale etc. to startMovieJButton_actionPerformed()
            // in which case one has to stop the movie before changing contrast
            // and resize etc.
            // find if scale, translation and contrast is changed
            scale         = imageFrameY.utilImageManipulate.scale;
            xTrans        = imageFrameY.utilImageManipulate.xTrans;
            yTrans        = imageFrameY.utilImageManipulate.yTrans;
            lowContrast   = imageFrameY.utilImageManipulate.lowContrast;
            highContrast  = imageFrameY.utilImageManipulate.highContrast;

            // keeps the interpolation, translation and contrast as
            // set in one image valid for all subsequent images.
            if (xTrans != 0 || yTrans != 0 || scale != 1 ||
                         lowContrast != 0 || highContrast != 255) {
               imageFrameY.utilImageManipulate.setLowContrast
                       (imageFrameY.utilImageManipulate.lowContrast);

               imageFrameY.utilImageManipulate.setHighContrast
                      (imageFrameY.utilImageManipulate.highContrast);
            }

            if (oneWayJRadioButton.isSelected()) {
               // oneway movie
               if (rowsLVal == rowsHValSave) {
                  rowsLVal = rowsLValSave;
               }
               else {
                  rowsLVal = rowsLVal + 1;
               }
            }
            else if (twoWayJRadioButton.isSelected()) {
               // twoway movie
               if (rowsLVal == rowsLValSave) increment = true;
               if (rowsLVal == rowsHValSave) increment = false;
               if (increment) {
                  rowsLVal = rowsLVal + 1;
               }
               else {
                  rowsLVal = rowsLVal - 1;
               }
            }
            delayTime = delaySlider.getValue();
            try {
               Thread.sleep((long)delayTime);
            }
            catch (InterruptedException ie) {
               String str = "Error in sleep: " + ie + "\n";
               utilParameters.outputMessageTextArea.append(str);
            }
            imageFrameY.wdp.repaint();
         }
         else if (axisSelected == "z") {
            imageFrameZ.wdp.outImage = getImage();
            // copy out put image as input image in wdp so that EDIT and ANALYSIS
            // functions will take the image that is displayed as the input image.
            imageFrameZ.wdp.img = imageFrameZ.wdp.outImage;

            // note: if this makes movie display slow then then move
            // the code to find scale etc. to startMovieJButton_actionPerformed()
            // in which case one has to stop the movie before changing contrast
            // and resize etc.
            // find if scale, translation and contrast is changed
            scale         = imageFrameZ.utilImageManipulate.scale;
            xTrans        = imageFrameZ.utilImageManipulate.xTrans;
            yTrans        = imageFrameZ.utilImageManipulate.yTrans;
            lowContrast   = imageFrameZ.utilImageManipulate.lowContrast;
            highContrast  = imageFrameZ.utilImageManipulate.highContrast;

            if (xTrans != 0 || yTrans != 0 || scale != 1 ||
                        lowContrast != 0 || highContrast != 255) {
             imageFrameZ.utilImageManipulate.setLowContrast
                  (imageFrameZ.utilImageManipulate.lowContrast);

             imageFrameZ.utilImageManipulate.setHighContrast
                  (imageFrameZ.utilImageManipulate.highContrast);
           }

            if (oneWayJRadioButton.isSelected()) {
               // oneway movie
               if (slicesLVal == slicesHValSave) {
                  slicesLVal = slicesLValSave;
               }
               else {
                  slicesLVal = slicesLVal + 1;
               }
            }
            else if (twoWayJRadioButton.isSelected()) {
               // twoway movie
               if (slicesLVal == slicesLValSave) increment = true;
               if (slicesLVal == slicesHValSave) increment = false;
               if (increment) {
                  slicesLVal = slicesLVal + 1;
               }
               else {
                  slicesLVal = slicesLVal - 1;
               }
            }
            delayTime = delaySlider.getValue();
            try {
               Thread.sleep((long)delayTime);
            }
            catch (InterruptedException ie) {
               String str = "Error in sleep: " + ie + "\n";
               utilParameters.outputMessageTextArea.append(str);
            }
            imageFrameZ.wdp.repaint();
         }
      }
      animatorThread = null;
   }

   public void enableAllSliders (boolean enb) {
      columnsLowEndSlider.setEnabled(enb);
      columnsHighEndSlider.setEnabled(enb);
      rowsLowEndSlider.setEnabled(enb);
      rowsHighEndSlider.setEnabled(enb);
      slicesLowEndSlider.setEnabled(enb);
      slicesHighEndSlider.setEnabled(enb);
   }

   public void resetSliders() {
      columnsLowEndSlider.setValue(1);
      columnsLowEndJTextField.setText(""+columnsLowEndSlider.getValue());
      columnsHighEndSlider.setValue(nsam);
      columnsHighEndJTextField.setText(""+columnsHighEndSlider.getValue());
      rowsLowEndSlider.setValue(1);
      rowsLowEndJTextField.setText(""+rowsLowEndSlider.getValue());
      rowsHighEndSlider.setValue(nrow);
      rowsHighEndJTextField.setText(""+rowsHighEndSlider.getValue());
      slicesLowEndSlider.setValue(1);
      slicesLowEndJTextField.setText(""+slicesLowEndSlider.getValue());
      slicesHighEndSlider.setValue(nslice);
      slicesHighEndJTextField.setText(""+slicesHighEndSlider.getValue());
   }

   public void checkLowEndSliderValidity(JSlider jSliderLow, JSlider jSliderHigh,
                      JTextField lowEndJTextField, JTextField highEndJTextField) {
      if (jSliderLow.getValue() > jSliderHigh.getValue()) {
         lowEndJTextField.setForeground(Color.red);
      }
      else {
         lowEndJTextField.setForeground(Color.black);
          highEndJTextField.setForeground(Color.black);
      }
   }

   public void checkHighEndSliderValidity(JSlider jSliderLow, JSlider jSliderHigh,
                      JTextField lowEndJTextField, JTextField highEndJTextField) {
      if (jSliderLow.getValue() > jSliderHigh.getValue()) {
         highEndJTextField.setForeground(Color.red);
      }
      else {
         lowEndJTextField.setForeground(Color.black);
         highEndJTextField.setForeground(Color.black);
      }
   }



   public TiledImage getImage() {
     // check that lowend value is not larger than highend values.
     // returns null if it is not true

      int noOfSlices = 1;
      int sliceWidth = 0;
      int sliceHeight = 0;
      int noOfImgInRow =1;
      if (axisSelected == "x"){
         columnsHVal = columnsLVal;
         sliceWidth   = rowsHVal    - rowsLVal    + 1;
         sliceHeight  = slicesHVal  - slicesLVal  + 1;
      }
      if (axisSelected == "y") {
         rowsHVal = rowsLVal;
         sliceWidth   = columnsHVal - columnsLVal + 1;
         sliceHeight  = slicesHVal  - slicesLVal  + 1;
      }
      if (axisSelected == "z") {
         slicesHVal  = slicesLVal;
         sliceWidth   = columnsHVal - columnsLVal + 1;
         sliceHeight  = rowsHVal    - rowsLVal    + 1;
      }

      int montagedSlicesWidth = sliceWidth;
      int montagedSlicesHeight = sliceHeight;

      UtilImageCreate utilImageCreate = new UtilImageCreate(utilParameters);
      TiledImage montagedSlices;
      montagedSlices = utilImageCreate.getMontagedSlices(spiderVolume,
             axisSelected, columnsLVal, columnsHVal, rowsLVal, rowsHVal,
                  slicesLVal, slicesHVal, noOfImgInRow, noOfSlices,sliceWidth,
                           sliceHeight,montagedSlicesWidth, montagedSlicesHeight);

      return montagedSlices;

   }

   public ImageFrame getImageFrameX() {
      return imageFrameX;
   }
   public ImageFrame getImageFrameY() {
      return imageFrameY;
   }
   public ImageFrame getImageFrameZ() {
      return imageFrameZ;
   }

}
