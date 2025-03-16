package web;

import java.awt.*;
import java.awt.image.*;
import javax.swing.*;
import javax.swing.event.*;
import javax.media.jai.*;
import java.awt.image.renderable.*;
import java.awt.event.*;
//import java.text.*;

/**
 * Title:        WEB: A GUI for viewing and analyzing SPIDER images
 * Description:  WEB is a graphical user interface for viewing and analyzing SPIDER images.
 * Copyright:    Copyright (c) 2001
 * Company:      Health Research Inc.
 * @author Bimal K. Rath
 * @version 1.0
 */

public class UtilImageManipulate implements ActionListener{

   private RenderedOp ct;
   private RenderedImage outImage, outImage2;

   private JSlider interpolationSlider, xTransSlider, yTransSlider;
   private JSlider lowContrastSlider, highContrastSlider;

   private JRadioButton resizeLargeRB, resizeSmallRB;
   private ButtonGroup buttonGroup;
   final JTextField interpolationTextField = new JTextField(4);

   private WebDisplayPanel panel;
   private UtilParameters utilParameters;

   public boolean  translateFrameOpen = false;
   public boolean  interpolateFrameOpen = false;
   public boolean  contrastFrameOpen = false;
   private boolean reSizeLarge, reSizeSmall;

   public Interpolation interpolation = new InterpolationBilinear();

   public float scale = 1, xTrans = 0, yTrans = 0;
   public int   lowContrast, highContrast;
   final JTextField highContrastTextField = new JTextField(4);
   final JTextField lowContrastTextField = new JTextField(4);

   public JFrame  frame, frame2, frame3;
   private Dimension screenSize;

   //   DecimalFormat df2 = new DecimalFormat("0.00");
   //   DecimalFormat df1 = new DecimalFormat("#0.0");


   public UtilImageManipulate(WebDisplayPanel WDpanel, UtilParameters utilParam) {
      screenSize = Toolkit.getDefaultToolkit().getScreenSize();
      panel          = WDpanel;
      utilParameters = utilParam;
      this.lowContrast = utilParameters.contrastLowRamp;
      this.highContrast = utilParameters.contrastHighRamp;
      if (utilParameters.resizeLarge) {
         this.scale = utilParameters.resizeFactor;
      }
      else {
         this.scale = (1/utilParameters.resizeFactor);
      }
   }
   // do translation
   public void translate() {

      translateFrameOpen = true;
      Container contentPane;
      frame = new JFrame();
      contentPane = frame.getContentPane();
      GridBagLayout gridBagLayout = new GridBagLayout();
      contentPane.setLayout(gridBagLayout);
      GridBagConstraints gridBagConstraints = new GridBagConstraints();

      JLabel xTransLabel = new JLabel("X translation");
      addComponent(contentPane,xTransLabel,gridBagConstraints,0,0,2,2);

      int xSliderUpperVal = ((utilParameters.screenDimension.width / 1000)+1)*1000;
      xTransSlider = new JSlider(SwingConstants.HORIZONTAL,0,xSliderUpperVal,0);
      xTransSlider.setMajorTickSpacing(500);
      xTransSlider.setMinorTickSpacing(100);
      xTransSlider.setPaintTicks(true);
      xTransSlider.setPaintLabels(true);
      addComponent(contentPane,xTransSlider,gridBagConstraints,3,0,4,2);


      final JTextField xTransSliderTextField = new JTextField(4);
      xTransSliderTextField.setEditable(false);
      // set the initial display
      xTransSliderTextField.setText(""+0);

      xTransSlider.addChangeListener(new ChangeListener() {
         public void stateChanged(ChangeEvent ce) {
           // update display
            xTransSliderTextField.setText(""+xTransSlider.getValue());
            setXTrans((float)(xTransSlider.getValue()));
         }
      });
      addComponent(contentPane,xTransSliderTextField,gridBagConstraints,8,0,2,2);

      JLabel yTransLabel = new JLabel("Y translation");
      addComponent(contentPane,yTransLabel,gridBagConstraints,0,2,2,2);

      int ySliderUpperVal = ((utilParameters.screenDimension.height / 1000)+1)*1000;
      yTransSlider = new JSlider(SwingConstants.HORIZONTAL,0,
                                     ySliderUpperVal,0);
      yTransSlider.setMajorTickSpacing(500);
      yTransSlider.setMinorTickSpacing(100);
      yTransSlider.setPaintTicks(true);
      yTransSlider.setPaintLabels(true);
      addComponent(contentPane,yTransSlider,gridBagConstraints,3,2,4,2);


      final JTextField yTransSliderTextField = new JTextField(4);
      yTransSliderTextField.setEditable(false);
      // set the initial display
      yTransSliderTextField.setText(""+0);

      yTransSlider.addChangeListener(new ChangeListener() {
         public void stateChanged(ChangeEvent ce) {
           // update display
            yTransSliderTextField.setText(""+yTransSlider.getValue());
            setYTrans((float)(yTransSlider.getValue()));
         }
      });
      addComponent(contentPane,yTransSliderTextField,gridBagConstraints,8,2,2,2);

      frame.setTitle("Translation Slider");
      frame.setSize(400,170);
      frame.pack();
      frame.setLocation(screenSize.width-400, screenSize.height-170);
      frame.setVisible(true);
   }

   public void setXTrans(float xTrans) {
      this.xTrans = xTrans;
      // contrast changed, retain image with new contrast info
      if( lowContrast != 0 || highContrast != 255) {
         panel.outImage = scale(outImage2, scale, scale, xTrans, yTrans,
                                                               interpolation);
      }
      // contrast not changed. outImage2 doesn't exist so use panel.img
      else {
         panel.outImage = scale(panel.img, scale, scale, xTrans, yTrans,
                                                               interpolation);
      }
      panel.repaint();
   }

   public void setYTrans(float yTrans) {
      this.yTrans = yTrans;
      // contrast changed, retain image with new contrast info
      if( lowContrast != 0 || highContrast != 255) {
         panel.outImage = scale(outImage2, scale, scale, xTrans, yTrans,
                                                                interpolation);
      }
     // contrast not changed. outImage2 doesn't exist so use panel.img
      else {
         panel.outImage = scale(panel.img, scale, scale, xTrans, yTrans,
                                                               interpolation);
      }
      panel.repaint();
   }

   // do interpolation
   public void interpolate() {
      interpolateFrameOpen = true;

      frame2 = new JFrame();
      Container contentPane;
      contentPane = frame2.getContentPane();
      GridBagLayout gridBagLayout = new GridBagLayout();
      contentPane.setLayout(gridBagLayout);
      GridBagConstraints gridBagConstraints = new GridBagConstraints();


      JLabel scaleLabel = new JLabel("Resize:");
      addComponent(contentPane,scaleLabel,gridBagConstraints,0,0,2,2);


      resizeLargeRB = new JRadioButton("Large");
       if (utilParameters.resizeLarge) {
         resizeLargeRB.setSelected(true);
       }
       else {
         resizeLargeRB.setSelected(false);
       }
       addComponent(contentPane,resizeLargeRB,gridBagConstraints,2,0,1,1);

       // done this way so that close all button on main menu may execute
       // actionPerformed method defined in this class
       resizeLargeRB.addActionListener(new java.awt.event.ActionListener() {
             public void actionPerformed(ActionEvent e) {
                reSizeLarge = true;
                reSizeSmall = false;
                setScale ((float)(Float.parseFloat
                                  (interpolationTextField.getText().trim())));
                // update the scale variable so that it can be used from other classes
                // to retain the new value of scale variable.
                scale = (float) (Float.parseFloat
                                   (interpolationTextField.getText().trim()));
             }
       });

       resizeSmallRB = new JRadioButton("Small");
       if (utilParameters.resizeSmall) {
         resizeSmallRB.setSelected(true);
       }
       else {
         resizeSmallRB.setSelected(false);
       }
       addComponent(contentPane,resizeSmallRB,gridBagConstraints,2,1,1,1);

       // done this way so that close all button on main menu may execute
       // actionPerformed method (if defined) in this class
       resizeSmallRB.addActionListener(new java.awt.event.ActionListener() {
             public void actionPerformed(ActionEvent e) {
                reSizeSmall = true;
                reSizeLarge = false;
                setScale ((float)(1.0/Float.parseFloat
                                  (interpolationTextField.getText().trim())));
                // update the scale variable so that it can be used from other classes
                // to retain the new value of scale variable.
                scale = (float) (1.0/Float.parseFloat
                                   (interpolationTextField.getText().trim()));
             }
       });

       buttonGroup = new ButtonGroup();
       buttonGroup.add(resizeSmallRB);
       buttonGroup.add(resizeLargeRB);

      interpolationSlider = new JSlider(SwingConstants.HORIZONTAL,10,101,
                                        (int)utilParameters.resizeFactor*10);


      interpolationSlider.setMajorTickSpacing(20);
      interpolationSlider.setMinorTickSpacing(10);
      interpolationSlider.setPaintTicks(true);
      addComponent(contentPane,interpolationSlider,gridBagConstraints,4,0,4,2);

      interpolationTextField .addActionListener(new ActionListener() {
         public void actionPerformed(ActionEvent e) {
            String inputStr = e.getActionCommand();
            // resize large
            if (resizeLargeRB.isSelected()) {
               setScale( (float) (Float.parseFloat(inputStr)) );
            }
            // resize small
            else {
               setScale( (float) (1.0/(Float.parseFloat(inputStr)) ));
            }
            // update the slider
            interpolationSlider.setValue((int)(10.0*Float.parseFloat(inputStr)));

            // update the scale variable so that it can be used from other classes
            // to retain the new value of scale variable.
            if (reSizeLarge) {
               scale = (float) (Float.parseFloat(inputStr));
            }
            else if (reSizeSmall) {
               scale = (float) (1.0/Float.parseFloat(inputStr) );
            }
         }
      });

      // set the initial display
      interpolationTextField.setText(""+utilParameters.resizeFactor);

      interpolationSlider.addChangeListener(new ChangeListener() {
         public void stateChanged(ChangeEvent ce) {
            // update display
            interpolationTextField.setText(""+interpolationSlider.getValue()/10.0);
            // resize large
            if (resizeLargeRB.isSelected()) {
              setScale( (float) (interpolationSlider.getValue() / 10.0));
            }
            // resize small
            else {
               setScale( (float) (10.0/interpolationSlider.getValue() ));
            }
            // update the scale variable so that it can be used from other classes
            // to retain the new value of scale variable.
            if (reSizeLarge) {
               scale = (float) (interpolationSlider.getValue() / 10.0);
            }
            else if (reSizeSmall) {
               scale = (float) (10.0/interpolationSlider.getValue() );
            }


         }
      });

      addComponent(contentPane,interpolationTextField,gridBagConstraints,8,0,2,2);

      frame2.setTitle("Resize Slider");
      frame2.setSize(400,100);
      frame2.pack();
      frame2.setLocation(screenSize.width-400, screenSize.height-270);
      frame2.setVisible(true);
   }

   public void setScale(float scale) {
      this.scale = scale;
      // contrast changed, retain image with new contrast info
      if( lowContrast != 0 || highContrast != 255) {
         panel.outImage = scale(outImage2, scale, scale, xTrans, yTrans,
                                                                interpolation);
         // help activating scroll bar with new image
         panel.setPreferredSize(new Dimension(panel.outImage.getWidth(),
                                                 panel.outImage.getHeight()));
      }
      // contrast not changed. outImage2 doesn't exist so use panel.img
      else {
         panel.outImage = scale(panel.img, scale, scale, xTrans, yTrans,
                                                                interpolation);
         // help activating scroll bar with new image
         panel.setPreferredSize(new Dimension(panel.outImage.getWidth(),
                                                 panel.outImage.getHeight()));
      }
      // help activating scroll bar with new image
      panel.revalidate();
      panel.repaint();
   }

   RenderedImage scale(RenderedImage im, float xScale, float yScale,
                       float xTrans, float yTrans, Interpolation interpolation) {

      ParameterBlock pb = new ParameterBlock();
      pb.addSource(im);
      pb.add(xScale);
      pb.add(yScale);
      pb.add(xTrans);
      pb.add(yTrans);
      pb.add(interpolation);
      return JAI.create("Scale", pb, null);

   }

   // change contrast
   public void changeContrast() {
      contrastFrameOpen = true;
      frame3 = new JFrame();
      Container contentPane;
      contentPane = frame3.getContentPane();
      GridBagLayout gridBagLayout = new GridBagLayout();
      contentPane.setLayout(gridBagLayout);
      GridBagConstraints gridBagConstraints = new GridBagConstraints();

      JLabel lowContrastLabel = new JLabel("Low Ramp");
      addComponent(contentPane,lowContrastLabel,gridBagConstraints,0,0,2,2);

      lowContrastSlider = new JSlider(SwingConstants.HORIZONTAL,0,255,lowContrast);
      lowContrastSlider.setMajorTickSpacing(50);
      lowContrastSlider.setMinorTickSpacing(25);
      lowContrastSlider.setPaintTicks(true);
      lowContrastSlider.setPaintLabels(true);
      addComponent(contentPane,lowContrastSlider,gridBagConstraints,3,0,4,2);

      lowContrastTextField.setEditable(false);
      // set the initial display
      lowContrastTextField.setText(""+lowContrast);

      lowContrastSlider.addChangeListener(new ChangeListener() {
         public void stateChanged(ChangeEvent ce) {
            lowContrastTextField.setText(""+lowContrastSlider.getValue());

            lowContrast = lowContrastSlider.getValue();

            if(lowContrast >= highContrast) {
               lowContrastTextField.setForeground(Color.red);
            }
            else {
               lowContrastTextField.setForeground(Color.black);
               highContrastTextField.setForeground(Color.black);
               setLowContrast(lowContrast);
            }
         }
      });
      addComponent(contentPane,lowContrastTextField,gridBagConstraints,8,0,2,2);

      JLabel highContrastLabel = new JLabel("High Ramp");
      addComponent(contentPane,highContrastLabel,gridBagConstraints,0,2,2,2);

      highContrastSlider = new JSlider(SwingConstants.HORIZONTAL,0,255,highContrast);
      highContrastSlider.setMajorTickSpacing(50);
      highContrastSlider.setMinorTickSpacing(25);
      highContrastSlider.setPaintTicks(true);
      highContrastSlider.setPaintLabels(true);
      addComponent(contentPane,highContrastSlider,gridBagConstraints,3,2,4,2);



      highContrastTextField.setEditable(false);
      // set the initial display
      highContrastTextField.setText(""+highContrast);

      highContrastSlider.addChangeListener(new ChangeListener() {
         public void stateChanged(ChangeEvent ce) {
            highContrastTextField.setText(""+highContrastSlider.getValue());
            highContrast = highContrastSlider.getValue();
            if(lowContrast >= highContrast) {
               highContrastTextField.setForeground(Color.red);
            }
            else {
               lowContrastTextField.setForeground(Color.black);
               highContrastTextField.setForeground(Color.black);
               setHighContrast(highContrast);
            }
         }
      });

      addComponent(contentPane,highContrastTextField,gridBagConstraints,8,2,2,2);
      frame3.setTitle("Contrast Change Slider");
      frame3.setSize(400,170);
      frame3.pack();
      frame3.setLocation(screenSize.width-400, screenSize.height-440);
      frame3.setVisible(true);
   }

   public void setLowContrast(int lowContrast) {
      outImage2 = contrast(lowContrast,highContrast);
      // use current location of the image and current interpolated size too.
      panel.outImage = scale(outImage2, scale, scale, xTrans, yTrans, interpolation);
      // help activating scroll bar with new image
      panel.setPreferredSize(new Dimension(panel.outImage.getWidth(),
                                                 panel.outImage.getHeight()));
      panel.revalidate();
      panel.repaint();
   }

   public void setHighContrast(int highContrast) {
      //this.highContrast = highContrast;
      outImage2 = contrast(lowContrast,highContrast);
      // use current location of the image and current interpolated size too.
      panel.outImage = scale(outImage2, scale, scale, xTrans, yTrans, interpolation);
       // help activating scroll bar with new image
      panel.setPreferredSize(new Dimension(panel.outImage.getWidth(),
                                                 panel.outImage.getHeight()));
      panel.revalidate();
      panel.repaint();
   }

   PlanarImage  contrast(int lowContrast, int highContrast) {

      /*
         use of lookup table
         float [] table =new float[256] ;
         for (int j = 0; j <= 255; j++) {
            if (j <= lowContrast )
               table[j]=0.0F;
            else if (j >= highContrast)
               table[j]=1.0F;
            else {
               float a = (float)(j-lowContrast);
               float b = (1.0F/(highContrast-lowContrast+1.0F));
               table[j]= a*b;
            }
         }
         LookupTableJAI lookupTable = new LookupTableJAI(table);
         ct = JAI.create("lookup",panel.img,lookupTable);
      */


      int numBands =1;
      float [][][] bp = new float[numBands][2][];
      for(int b = 0; b < numBands; b++) {
         bp[b][0] = new float[] {(float)lowContrast, (float)highContrast};
         bp[b][1] = new float[] {0.0F, 255.0F};
      }
      ct = JAI.create("pieceWise",panel.img,bp);

      // Use the following if img is of type BufferedImage
      // ct = JAI.create("lookup",(RenderedImage)img,lookupTable);

      /* Either of the following lines don't work. It complains about data type.
         I don't know why. If it could have worked, I would have worked with
         PlanarImages all along after makePlanarImage().
         ct = JAI.create("lookup",(RenderedImage)outImage,lookupTable);
         ct = JAI.create("lookup",outImage,lookupTable);
      */
      return ct;
   }

   // Resets Image
   public void resetImageScale() {
      scale = 1;
      xTrans = 0;
      yTrans = 0;
      panel.outImage = scale(panel.img, scale, scale, xTrans, yTrans,
                                                                interpolation);
      panel.repaint();
   }

   public void disposeWindows() {
      if (translateFrameOpen == true) {
         frame.dispose();
      }

      if (interpolateFrameOpen == true) {
         frame2.dispose();
      }

      if (contrastFrameOpen == true) {
         frame3.dispose();
      }
   }

   public void addComponent(Container contentPane, Component comp,
                               GridBagConstraints gridBagConstraints, int gridX,
                               int gridY, int gridWidth, int gridHeight) {

      gridBagConstraints.gridx = gridX;
      gridBagConstraints.gridy = gridY;
      gridBagConstraints.gridwidth = gridWidth;
      gridBagConstraints.gridheight = gridHeight;
      // will grow to fill the space if the window size is changed
      gridBagConstraints.weightx = 100;
      gridBagConstraints.weighty = 100;
      // make it aligned
      // gridBagConstraints.fill = GridBagConstraints.BOTH;
      contentPane.add(comp,gridBagConstraints);
   }

   public void actionPerformed(ActionEvent evt) {
      // this.dispose;
      // note: you may try to close resize,contrast etc. windows here
      // by linking this class to closeall button on the main window of jweb
   }


}
