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

public class UtilGUIVolume_2 extends JPanel {

   private SpiderImage spiderVolume;
   private UtilParameters utilParameters;
   private int nsam, nrow, nslice;
   private UtilParametersVolume utilParametersVolume;

   private JCheckBox treatAsVolumeCheckBox;
   private JRadioButton montageViewerJRadioButton, sliceViewerJRadioButton;
   private ButtonGroup montageSlicesButtonGroup;
   private JLabel  axisLabel, rowsLabel, xSliceLabel, columnsLabel;
   private JLabel  slicesLabel;
   private JRadioButton xRadioButton, yRadioButton, zRadioButton;
   private ButtonGroup axisButtonGroup;
   private JTextField rowsLowEndJTextField, rowsHighEndJTextField;
   private JTextField columnsLowEndJTextField, columnsHighEndJTextField;
   private JTextField slicesLowEndJTextField, slicesHighEndJTextField;
   private JButton showMontageJButton;
   private JSlider rowsLowEndSlider, rowsHighEndSlider, columnsLowEndSlider;
   private JSlider columnsHighEndSlider, slicesLowEndSlider, slicesHighEndSlider;
   private JScrollBar rowsLowEndScrollBar;
   private JLabel dummyJLabel;
   private int columnsLVal, columnsHVal, rowsLVal, rowsHVal, slicesLVal, slicesHVal;
   private JButton jButtonCloseAll;
   private ImageFrame imageFrame;
   private String str;
   private float scale;

   public UtilGUIVolume_2(SpiderImage spiVol, String imageType, JButton closeAll,
                 UtilParameters utilParam, UtilParametersVolume utilParamVol) {

      spiderVolume = spiVol;
      utilParameters = utilParam;
      jButtonCloseAll = closeAll;
      utilParametersVolume = utilParamVol;
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
      addComponent(zRadioButton,gridBagConstraints,11,4,1,1);
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

      // added this dummy label for creating space above the Show Montage button
      // and also to center the button
      dummyJLabel = new JLabel("    ");
      addComponent(dummyJLabel,gridBagConstraints,15,15,1,2);
      //addComponent(dummyJLabel,gridBagConstraints,7,18,3,2);
      showMontageJButton = new JButton("Show Montage");
      addComponent(showMontageJButton,gridBagConstraints,5,18,7,1);

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

      showMontageJButton.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
           showMontageJButton_actionPerformed(e);
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
      // make it aligned; didn't put the showMontage button in the middle,
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
         resetSliders();
         enableAllSliders (true);
         slicesLabel.setText("Slices");
      }
      else {
         xRadioButton.setEnabled(false);
         yRadioButton.setEnabled(false);
         zRadioButton.setEnabled(false);
         zRadioButton.setSelected(true);
         axisLabel.setEnabled(false);
         resetSliders();
         enableAllSliders (false);
         slicesLabel.setText("Images");
      }
   }

   void showMontageJButton_actionPerformed(ActionEvent e) {
      columnsLVal = columnsLowEndSlider.getValue();
      columnsHVal = columnsHighEndSlider.getValue();

      rowsLVal = rowsLowEndSlider.getValue();
      rowsHVal = rowsHighEndSlider.getValue();

      slicesLVal = slicesLowEndSlider.getValue();
      slicesHVal = slicesHighEndSlider.getValue();

      // check that lowend value is not larger than highend values.
      // check that lowend value is not larger than highend values.
      // returns null if it is not true
      if ((columnsLVal <= columnsHVal) && (rowsLVal <= rowsHVal) &&
                                           (slicesLVal <= slicesHVal)) {

         String axisSelected = new String();
         if (xRadioButton.isSelected()) axisSelected = "x";
         if (yRadioButton.isSelected()) axisSelected = "y";
         if (zRadioButton.isSelected()) axisSelected = "z";

         int noOfSlices = 0;
         int sliceWidth = 0;
         int sliceHeight = 0;
         int noOfImgInRow = 0;
         int scrnWid = 0;

         if (axisSelected == "x") {
            noOfSlices = columnsHVal - columnsLVal + 1;
            sliceWidth = rowsHVal - rowsLVal + 1;
            sliceHeight = slicesHVal - slicesLVal + 1;
         }
         if (axisSelected == "y") {
            noOfSlices = rowsHVal - rowsLVal + 1;
            sliceWidth = columnsHVal - columnsLVal + 1;
            sliceHeight = slicesHVal - slicesLVal + 1;
         }
         if (axisSelected == "z") {
            noOfSlices = slicesHVal - slicesLVal + 1;
            sliceWidth = columnsHVal - columnsLVal + 1;
            sliceHeight = rowsHVal - rowsLVal + 1;
         }

         // find scaling of the image
         if (utilParameters.resizeLarge) {
            scale = utilParameters.resizeFactor;
         }
         else {
            scale = (1/utilParameters.resizeFactor);
         }

         scrnWid = utilParameters.screenDimension.width - 50;
         // reset the screen width as per the image scaling
         // the image will be scaled in ImageFrame.java
         scrnWid = (int)(scrnWid/scale) ;


         noOfImgInRow = scrnWid / sliceWidth;
         if (noOfImgInRow == 0) noOfImgInRow = 1;
         if (noOfSlices <= noOfImgInRow) noOfImgInRow = noOfSlices;
         // add noOfImgInRow -1 pixels for a blank pixel after each image
         int montagedSlicesWidth = noOfImgInRow * sliceWidth + noOfImgInRow - 1;
         int k = noOfSlices / noOfImgInRow;
         int l = noOfSlices % noOfImgInRow;

         if (l != 0) k = k + 1;
         // add k-1 pixels for a blank pixel after each image
         int montagedSlicesHeight = k * sliceHeight + k - 1;

         UtilImageCreate utilImageCreate = new UtilImageCreate(utilParameters);
         TiledImage montagedSlices;
         montagedSlices = utilImageCreate.getMontagedSlices(spiderVolume,
         axisSelected, columnsLVal, columnsHVal, rowsLVal, rowsHVal,
         slicesLVal, slicesHVal, noOfImgInRow, noOfSlices, sliceWidth,
         sliceHeight, montagedSlicesWidth, montagedSlicesHeight);

         imageFrame = new ImageFrame(montagedSlices, utilParameters);
         jButtonCloseAll.addActionListener(imageFrame);
         imageFrame.setTitle("Montaged " + "'" + axisSelected.toUpperCase() +
                                                              "'" + " Slices");
         utilParametersVolume.montageXYZShown = true;
                                                imageFrame.setLocation(50, 50);
         imageFrame.setVisible(true);
      }
      else {
         str = " Columns/Row/Slice low-end value is greater than" +
                                         " corresponding high-end value !\n ";
         utilParameters.outputMessageTextArea.setText(str);
      }
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

   public ImageFrame getImageFrame() {
      return imageFrame;
   }
}
