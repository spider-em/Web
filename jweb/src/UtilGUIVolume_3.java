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

public class UtilGUIVolume_3 extends JPanel implements ActionListener {

   private SpiderImage spiderVolume;
   private UtilParameters utilParameters;
   private int nsam, nrow, nslice;

   private JCheckBox treatAsVolumeCheckBox;
   private JLabel  axisLabel, rowsLabel, columnsLabel, slicesLabel;;
   private JRadioButton xRadioButton, yRadioButton, zRadioButton;
   private ButtonGroup axisButtonGroup;
   private JTextField rowsLowEndJTextField, rowsHighEndJTextField;
   private JTextField columnsLowEndJTextField, columnsHighEndJTextField;
   private JTextField slicesLowEndJTextField, slicesHighEndJTextField;
   private JButton showSliceJButton;
   private JSlider rowsLowEndSlider, rowsHighEndSlider, columnsLowEndSlider;
   private JSlider columnsHighEndSlider, slicesLowEndSlider, slicesHighEndSlider;
   private JScrollBar rowsLowEndScrollBar;
   private JLabel dummyJLabel;
   private JTabbedPane jTabbedPane;
   private int columnsLVal, columnsHVal, rowsLVal, rowsHVal, slicesLVal, slicesHVal;
   private JButton jButtonCloseAll;
   private ImageFrame imageFrameX, imageFrameY, imageFrameZ;
   private String axisSelected, typeOfImage;
   private boolean imageFrameXOpen = false;
   private boolean imageFrameYOpen = false;
   private boolean imageFrameZOpen = false;
   private boolean showSliceXactivatedForFirstTime = true;
   private boolean showSliceYactivatedForFirstTime = true;
   private boolean showSliceZactivatedForFirstTime = true;
   private UtilParametersVolume utilParametersVolume;
   private float scale, xTrans, yTrans;
   private int lowContrast, highContrast;
   private String str = new String();

   public UtilGUIVolume_3(SpiderImage spiVol, String imageType, JButton closeAll,
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
      xRadioButton.addActionListener(this);
      if (imageType == "ImageSeries")  xRadioButton.setEnabled(false);

      yRadioButton = new JRadioButton("Y");
      yRadioButton.setSelected(false);
      addComponent(yRadioButton,gridBagConstraints,9,4,1,1);
      yRadioButton.addActionListener(this);
      if (imageType == "ImageSeries")  yRadioButton.setEnabled(false);

      zRadioButton = new JRadioButton("Z");
      zRadioButton.setSelected(true);
      axisSelected = "z";
      addComponent(zRadioButton,gridBagConstraints,11,4,1,1);
      zRadioButton.addActionListener(this);
      if (imageType == "ImageSeries")  zRadioButton.setEnabled(false);

      axisButtonGroup = new ButtonGroup();
      axisButtonGroup.add(xRadioButton);
      axisButtonGroup.add(yRadioButton);
      axisButtonGroup.add(zRadioButton);

      if (imageType == "ImageSeries") {
         treatAsVolumeCheckBox = new JCheckBox("Treat As Volume");
         addComponent(treatAsVolumeCheckBox,gridBagConstraints,15,4,3,1);
      }

      columnsLabel = new JLabel("Columns");
      addComponent(columnsLabel,gridBagConstraints,1,7,2,1);

      columnsLowEndSlider = new JSlider(SwingConstants.HORIZONTAL,1,nsam,1);
      addComponent(columnsLowEndSlider,gridBagConstraints,3,7,6,1);
      columnsLowEndJTextField = new JTextField(4);
      columnsLowEndJTextField.setEditable(false);
      addComponent(columnsLowEndJTextField,gridBagConstraints,9,7,1,1);

      columnsLowEndJTextField.addActionListener(new ActionListener() {
         public void actionPerformed(ActionEvent e) {
            String inputStr = e.getActionCommand();
            int inputVal = Integer.parseInt(inputStr);
            if (inputVal > nsam ) inputVal = nsam;
            rowsLowEndSlider.setValue(inputVal);
            showSliceJButton_actionPerformed(e);
         }
      });


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

      rowsLowEndJTextField.addActionListener(new ActionListener() {
         public void actionPerformed(ActionEvent e) {
            String inputStr = e.getActionCommand();
            int inputVal = Integer.parseInt(inputStr);
            if (inputVal > nrow ) inputVal = nrow;
            rowsLowEndSlider.setValue(inputVal);
            showSliceJButton_actionPerformed(e);
         }
      });



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
      slicesLowEndJTextField.setEditable(true);
      addComponent(slicesLowEndJTextField,gridBagConstraints,9,13,1,1);

      slicesLowEndJTextField.addActionListener(new ActionListener() {
         public void actionPerformed(ActionEvent e) {
            String inputStr = e.getActionCommand();
            int inputVal = Integer.parseInt(inputStr);
            if (inputVal > nslice ) inputVal = nslice;
            slicesLowEndSlider.setValue(inputVal);
            showSliceJButton_actionPerformed(e);
         }
      });



      slicesHighEndSlider = new JSlider(SwingConstants.HORIZONTAL,1,nslice,nslice);
      addComponent(slicesHighEndSlider,gridBagConstraints,10,13,6,1);
      slicesHighEndJTextField = new JTextField(4);
      slicesHighEndJTextField.setEditable(false);
      addComponent(slicesHighEndJTextField,gridBagConstraints,16,13,1,1);

      enableAllSliders(false);

      // added this dummy label for creating space above the Show Montage button
      // and also to center the button
      dummyJLabel = new JLabel("    ");
      addComponent(dummyJLabel,gridBagConstraints,15,15,1,2);
      if (imageType == "ImageSeries") {
         showSliceJButton = new JButton("Show Slide");
      }
      else {
         showSliceJButton = new JButton("Show " + axisSelected + " Slice");
      }
      addComponent(showSliceJButton,gridBagConstraints,5,18,7,1);

      columnsLowEndJTextField.setText("1");
      columnsLowEndSlider.addChangeListener(new ChangeListener() {
         public void stateChanged(ChangeEvent ce) {
            // update display
            columnsLowEndJTextField.setText(""+columnsLowEndSlider.getValue());
            checkLowEndSliderValidity(columnsLowEndSlider, columnsHighEndSlider,
                      columnsLowEndJTextField, columnsHighEndJTextField);
            if (axisSelected == "x" && imageFrameXOpen) {

               // find if scale, translation and contrast is changed
               scale         = imageFrameX.utilImageManipulate.scale;
               xTrans        = imageFrameX.utilImageManipulate.xTrans;
               yTrans        = imageFrameX.utilImageManipulate.yTrans;
               lowContrast   = imageFrameX.utilImageManipulate.lowContrast;
               highContrast  = imageFrameX.utilImageManipulate.highContrast;

               if (lowContrast < highContrast) {
                  imageFrameX.wdp.outImage = getImage();

                  // if output image is null display error message else display image
                  if (imageFrameZ.wdp.outImage != null) {
                     // copy out put image as input image in wdp so that EDIT and ANALYSIS
                     // functions will take the image that is displayed as the input image.
                     imageFrameX.wdp.img = imageFrameX.wdp.outImage;

                     // keeps the interpolation, translation and contrast as
                     // set in one image valid for all subsequent images.
                     if (xTrans != 0 || yTrans != 0 || scale != 1 ||
                              lowContrast != 0 || highContrast != 255) {
                        imageFrameX.utilImageManipulate.setLowContrast
                          (imageFrameX.utilImageManipulate.lowContrast);

                        imageFrameX.utilImageManipulate.setHighContrast
                          (imageFrameX.utilImageManipulate.highContrast);
                     }
                  }
                  else {
                     str = " Columns/Row/Slice low-end value is greater than" +
                              " corresponding high-end value !\n ";
                    // utilParameters.outputMessageTextArea.setText(str);
                    utilParameters.outputMessageTextArea.setText(str);
                  }
               }
               else {
                  str = " Low contrast ramp is greater than high contrast ramp !\n ";
                  utilParameters.outputMessageTextArea.setText(str);
               }
                  imageFrameX.wdp.repaint();
            }
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
            if (axisSelected == "y" && imageFrameYOpen ) {

               // find if scale, translation and contrast is changed
               scale         = imageFrameY.utilImageManipulate.scale;
               xTrans        = imageFrameY.utilImageManipulate.xTrans;
               yTrans        = imageFrameY.utilImageManipulate.yTrans;
               lowContrast   = imageFrameY.utilImageManipulate.lowContrast;
               highContrast  = imageFrameY.utilImageManipulate.highContrast;
               if (lowContrast < highContrast) {
                  imageFrameY.wdp.outImage = getImage();

                  // if output image is null display error message else display image
                  if (imageFrameZ.wdp.outImage != null) {
                     // copy out put image as input image in wdp so that EDIT and ANALYSIS
                     // functions will take the image that is displayed as the input image.
                     imageFrameY.wdp.img = imageFrameY.wdp.outImage;

                     // keeps the interpolation, translation and contrast as
                     // set in one image valid for all subsequent images.
                     if (xTrans != 0 || yTrans != 0 || scale != 1 ||
                                  lowContrast != 0 || highContrast != 255) {
                        imageFrameY.utilImageManipulate.setLowContrast
                                (imageFrameY.utilImageManipulate.lowContrast);

                        imageFrameY.utilImageManipulate.setHighContrast
                               (imageFrameY.utilImageManipulate.highContrast);
                     }
                  }
                  else {
                     str = " Columns/Row/Slice low-end value is greater than" +
                             " corresponding high-end value !\n ";
                     utilParameters.outputMessageTextArea.setText(str);
                  }
               }
               else {
                  str = " Low contrast ramp is greater than high contrast ramp !\n ";
                  utilParameters.outputMessageTextArea.setText(str);
               }
               imageFrameY.wdp.repaint();
            }
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
            if (axisSelected == "z" && imageFrameZOpen) {
               // find if scale, translation and contrast is changed
               scale         = imageFrameZ.utilImageManipulate.scale;
               xTrans        = imageFrameZ.utilImageManipulate.xTrans;
               yTrans        = imageFrameZ.utilImageManipulate.yTrans;
               lowContrast   = imageFrameZ.utilImageManipulate.lowContrast;
               highContrast  = imageFrameZ.utilImageManipulate.highContrast;

               // lowramp should be > than highramp
               if (lowContrast < highContrast) {
                  imageFrameZ.wdp.outImage = getImage();

                  // if output image is null display error message else display image
                  if (imageFrameZ.wdp.outImage != null) {
                     // copy out put image as input image in wdp so that EDIT and ANALYSIS
                     // functions will take the image that is displayed as the input image.
                     imageFrameZ.wdp.img = imageFrameZ.wdp.outImage;

                     // keeps the interpolation, translation and contrast as
                     // set in one image valid for all subsequent images.
                     if (xTrans != 0 || yTrans != 0 || scale != 1 ||
                                  lowContrast != 0 || highContrast != 255) {
                        imageFrameZ.utilImageManipulate.setLowContrast
                            (imageFrameZ.utilImageManipulate.lowContrast);

                        imageFrameZ.utilImageManipulate.setHighContrast
                            (imageFrameZ.utilImageManipulate.highContrast);
                     }
                  }
                  else {
                     str = " Columns/Row/Slice low-end value is greater than" +
                                         " corresponding high-end value !\n ";
                     utilParameters.outputMessageTextArea.setText(str);
                  }
               }
               else {
                 str = " Low contrast ramp is greater than high contrast ramp !\n ";
                 utilParameters.outputMessageTextArea.setText(str);
               }
               imageFrameZ.wdp.repaint();
            }
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

      showSliceJButton.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
           showSliceJButton_actionPerformed(e);
         }
      });

      if (imageType == "ImageSeries") {
         treatAsVolumeCheckBox.addActionListener(
                                          new java.awt.event.ActionListener() {
            public void actionPerformed(ActionEvent e) {
              treatAsVolumeCheckBox_actionPerformed(e);
            }
         });
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
      //gridBagConstraints.fill = GridBagConstraints.BOTH;
      this.add(comp,gridBagConstraints);
   }

   void treatAsVolumeCheckBox_actionPerformed(ActionEvent e) {
      if (treatAsVolumeCheckBox.isSelected()) {
         xRadioButton.setEnabled(true);
         yRadioButton.setEnabled(true);
         zRadioButton.setEnabled(true);
         axisLabel.setEnabled(true);
         slicesLabel.setText("Slices");
         showSliceJButton.setText("Show z Slice");
         if (imageFrameZOpen) imageFrameZ.setTitle("'Z' Slice");
         resetSliders();
         enableAllSliders (true);
         slicesHighEndSlider.setEnabled(false);
      }
      else {
         xRadioButton.setEnabled(false);
         yRadioButton.setEnabled(false);
         zRadioButton.setEnabled(false);
         zRadioButton.setSelected(true);
         axisLabel.setEnabled(false);
         slicesLabel.setText("Images");
         showSliceJButton.setText("Show Slide");
         resetSliders();

         axisSelected = "z";
         if (!showSliceZactivatedForFirstTime) {
            imageFrameZOpen = imageFrameZ.isShowing();
         }
         if (!imageFrameZOpen) {
            resetSliders();
            enableAllSliders(false);
         }
         else {
            resetSliders();
            imageFrameZ.setTitle("Slide");
            imageFrameZ.wdp.outImage = getImage();
            // copy out put image as input image in wdp so that EDIT and ANALYSIS
            // functions will take the image that is displayed as the input image.
            imageFrameZ.wdp.img = imageFrameZ.wdp.outImage;
            imageFrameZ.wdp.repaint();
            enableAllSliders (false);
            slicesLowEndSlider.setEnabled(true);
         }
      }
   }

   void showSliceJButton_actionPerformed(ActionEvent e) {
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
         // if activated for the first time OR if the window is closed
         if (!imageFrameXOpen) {
            if (montagedSlices != null) {
               imageFrameX = new ImageFrame(montagedSlices, utilParameters);
               jButtonCloseAll.addActionListener(imageFrameX);
               imageFrameX.setTitle("'" + axisSelected.toUpperCase() +
                                                            "'" + " Slice");
               utilParametersVolume.sliceXShown = true;
               imageFrameX.setLocation(50, 50);
               imageFrameX.setVisible(true);
               imageFrameXOpen = true;
               showSliceXactivatedForFirstTime = false;
            }
            else {
               str = "Columns/Row/Slice low-end value is greater than" +
                                         " corresponding high-end value !\n ";
               utilParameters.outputMessageTextArea.setText(str);
            }
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
         // if activated for the first time OR if the window is closed
         if (!imageFrameYOpen) {
            if (montagedSlices != null) {
               imageFrameY = new ImageFrame(montagedSlices, utilParameters);
               jButtonCloseAll.addActionListener(imageFrameY);
               imageFrameY.setTitle("'" + axisSelected.toUpperCase() +
                                   "'" + " Slice");
               utilParametersVolume.sliceYShown = true;
               imageFrameY.setLocation(60, 60);
               imageFrameY.setVisible(true);
               imageFrameYOpen = true;
               showSliceYactivatedForFirstTime = false;
            }
            else {
               str = "Columns/Row/Slice low-end value is greater than" +
                                       " corresponding high-end value !\n ";
               utilParameters.outputMessageTextArea.setText(str);
            }
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
         // if activated for the first time OR if the window is closed
         if (!imageFrameZOpen) {
            if (montagedSlices != null) {
               imageFrameZ = new ImageFrame(montagedSlices, utilParameters);
               jButtonCloseAll.addActionListener(imageFrameZ);
               if (typeOfImage == "ImageSeries" &&
                            !treatAsVolumeCheckBox.isSelected()) {
                  imageFrameZ.setTitle("Slide");
               }
               else {
                  imageFrameZ.setTitle("'" + axisSelected.toUpperCase() +
                                               "'" + " Slice");
               }
               utilParametersVolume.sliceZShown = true;
               imageFrameZ.setLocation(70, 70);
               imageFrameZ.setVisible(true);
               imageFrameZOpen = true;
               showSliceZactivatedForFirstTime = false;
            }
            else {
               str = "Columns/Row/Slice low-end value is greater than" +
                             " corresponding high-end value !\n ";
               utilParameters.outputMessageTextArea.setText(str);
            }
         }
      }
   }

   public void actionPerformed(ActionEvent e){
      if (xRadioButton.isSelected()) {
         axisSelected = "x";
         if (!showSliceXactivatedForFirstTime) {
            imageFrameXOpen = imageFrameX.isShowing();
         }
         if (!imageFrameXOpen) {
            resetSliders();
            enableAllSliders(false);
         }
         else {
            resetSliders();
            imageFrameX.wdp.outImage = getImage();
            // copy out put image as input image in wdp so that EDIT and ANALYSIS
            // functions will take the image that is displayed as the input image.
            imageFrameX.wdp.img = imageFrameX.wdp.outImage;

            // use set translation, scale and contrast; done in utilImageManipulate
            // since it is done only once while selecting radio buttons it is not
            // time consuming and hence it is not checked if scale, translation and
            // contrast is changed before calling the .setLowContrast and
            // .setHighContrast functions

            imageFrameX.utilImageManipulate.setLowContrast
                                (imageFrameX.utilImageManipulate.lowContrast);
            imageFrameX.utilImageManipulate.setHighContrast
                               (imageFrameX.utilImageManipulate.highContrast);

            imageFrameX.wdp.repaint();
            enableAllSliders (true);
            columnsHighEndSlider.setEnabled(false);
         }
         setSliderLowEndJTextField(axisSelected);
      }
      if (yRadioButton.isSelected()) {
         axisSelected = "y";
         if (!showSliceYactivatedForFirstTime) {
            imageFrameYOpen = imageFrameY.isShowing();
         }
         if (!imageFrameYOpen) {
            resetSliders();
            enableAllSliders(false);
         }
         else {
            resetSliders();
            imageFrameY.wdp.outImage = getImage();
            // copy out put image as input image in wdp so that EDIT and ANALYSIS
            // functions will take the image that is displayed as the input image.
            imageFrameY.wdp.img = imageFrameY.wdp.outImage;

            // use set translation, scale and contrast; done in utilImageManipulate
            // since it is done only once while selecting radio buttons it is not
            // time consuming and hence it is not checked if scale, translation and
            // contrast is changed before calling the .setLowContrast and
            // .setHighContrast functions

            imageFrameY.utilImageManipulate.setLowContrast
                                 (imageFrameY.utilImageManipulate.lowContrast);
            imageFrameY.utilImageManipulate.setHighContrast
                                (imageFrameY.utilImageManipulate.highContrast);

            imageFrameY.wdp.repaint();
            enableAllSliders (true);
            rowsHighEndSlider.setEnabled(false);
         }
         setSliderLowEndJTextField(axisSelected);
      }
      if (zRadioButton.isSelected()) {
         axisSelected = "z";
         if (!showSliceZactivatedForFirstTime) {
            imageFrameZOpen = imageFrameZ.isShowing();
         }
         if (!imageFrameZOpen) {
            resetSliders();
            enableAllSliders(false);
         }
         else {
            resetSliders();
            imageFrameZ.wdp.outImage = getImage();
            // copy out put image as input image in wdp so that EDIT and ANALYSIS
            // functions will take the image that is displayed as the input image.
            imageFrameZ.wdp.img = imageFrameZ.wdp.outImage;

            // use set translation, scale and contrast; done in utilImageManipulate
            // since it is done only once while selecting radio buttons it is not
            // time consuming and hence it is not checked if scale, translation and
            // contrast is changed before calling the .setLowContrast and
            // .setHighContrast functions

            imageFrameZ.utilImageManipulate.setLowContrast
                                (imageFrameZ.utilImageManipulate.lowContrast);
            imageFrameZ.utilImageManipulate.setHighContrast
                               (imageFrameZ.utilImageManipulate.highContrast);

            imageFrameZ.wdp.repaint();
            enableAllSliders (true);
            slicesHighEndSlider.setEnabled(false);
         }
         setSliderLowEndJTextField(axisSelected);
      }
      showSliceJButton.setText("Show " + axisSelected + " Slice");
   }

   private void setSliderLowEndJTextField(String axis) {
      if (axis == "x") {
         columnsLowEndJTextField.setEditable(true);
         rowsLowEndJTextField.setEditable(false);
         slicesLowEndJTextField.setEditable(false);
      }
      if (axis == "y") {
         columnsLowEndJTextField.setEditable(false);
         rowsLowEndJTextField.setEditable(true);
         slicesLowEndJTextField.setEditable(false);

      }
      if (axis == "z") {
         columnsLowEndJTextField.setEditable(false);
         rowsLowEndJTextField.setEditable(false);
         slicesLowEndJTextField.setEditable(true);
      }
   }

   public void enableAllSliders (boolean enb) {
      columnsLowEndSlider.setEnabled(enb);
      columnsHighEndSlider.setEnabled(enb);
      rowsLowEndSlider.setEnabled(enb);
      rowsHighEndSlider.setEnabled(enb);
      slicesLowEndSlider.setEnabled(enb);
      slicesHighEndSlider.setEnabled(enb);
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

//      columnsLVal = columnsLowEndSlider.getValue();
      columnsLVal = Integer.parseInt(columnsLowEndJTextField.getText().trim());
      columnsLowEndSlider.setValue(columnsLVal);
      columnsHVal = columnsHighEndSlider.getValue();
//      rowsLVal = rowsLowEndSlider.getValue();
      rowsLVal = Integer.parseInt(rowsLowEndJTextField.getText().trim());
      rowsLowEndSlider.setValue(rowsLVal);
      rowsHVal = rowsHighEndSlider.getValue();
//      slicesLVal = slicesLowEndSlider.getValue();
      slicesLVal = Integer.parseInt(slicesLowEndJTextField.getText().trim());
      slicesLowEndSlider.setValue(slicesLVal);
      slicesHVal = slicesHighEndSlider.getValue();

      // check that lowend value is not larger than highend values.
      // returns null if it is not true
      if ((columnsLVal <= columnsHVal) && (rowsLVal <= rowsHVal) &&
                                              (slicesLVal <= slicesHVal)) {
         int noOfSlices = 1;
         int sliceWidth = 0;
         int sliceHeight = 0;
         int noOfImgInRow =1;
         if (axisSelected == "x"){
            columnsHVal = columnsLVal;
            sliceWidth   = rowsHVal    - rowsLVal    + 1;
            sliceHeight  = slicesHVal  - slicesLVal  + 1;
            enableAllSliders (true);
            columnsHighEndSlider.setEnabled(false);
         }
         if (axisSelected == "y") {
            rowsHVal = rowsLVal;
            sliceWidth   = columnsHVal - columnsLVal + 1;
            sliceHeight  = slicesHVal  - slicesLVal  + 1;
            enableAllSliders (true);
            rowsHighEndSlider.setEnabled(false);
         }
         if (axisSelected == "z") {
            slicesHVal  = slicesLVal;
            sliceWidth   = columnsHVal - columnsLVal + 1;
            sliceHeight  = rowsHVal    - rowsLVal    + 1;
            if (typeOfImage == "ImageSeries" &&
                                        !treatAsVolumeCheckBox.isSelected()) {
               enableAllSliders (false);
               slicesLowEndSlider.setEnabled(true);
            }
            else {
               enableAllSliders (true);
               slicesHighEndSlider.setEnabled(false);
            }
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
      return null;
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
