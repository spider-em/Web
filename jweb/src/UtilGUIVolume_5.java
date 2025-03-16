package web;

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import javax.media.jai.*;
import javax.swing.event.*;
import java.awt.image.renderable.*;
import java.io.*;
import java.util.*;
import java.awt.image.*;


/**
 * <p>Title: WEB: A GUI for viewing and analyzing SPIDER images</p>
 * <p>Description: WEB is a graphical user interface for viewing and analyzing SPIDER images.</p>
 * <p>Copyright: Copyright (c) 2001</p>
 * <p>Company: Health Research Inc.</p>
 * @author Bimal K. Rath
 * @version 1.0
 */

public class UtilGUIVolume_5 extends JPanel {
   private ImageFrame imageFrameSurface, imageFrameMontagedSurface ;
   public JButton showSurfaceJButton, showMovieJButton;
   private Surf surf;
   private SpiderImage spiderImage;
   private DataOutputStream  dout;
   private int nsam, nrow, nslice, nsamScaled, nrowScaled;
   private float min, max;
   private double imgScale;
   private byte [] surfImgByte ;
   private boolean sirdit, distit, colorit;

   private TiledImage surfaceImage, surfaceSmallImage;
   private UtilParameters utilParameters;
   private UtilParametersVolume utilParametersVolume;
   private JButton jButtonCloseAll;
   private JLabel  thresholdLabel, scaleLabel, reflectionLabel, eulerAnglesLabel, phiLabel,
                       thetaLabel, psiLabel;
   private JCheckBox movieCheckBox, saveToFileCheckBox;
   private JLabel  startValueLabel, endValueLabel, incrementLabel;
   private JRadioButton phiRadioButton, thetaRadioButton, psiRadioButton,
                                                          thresholdRadioButton;
   private ButtonGroup movieSelectionButtonGroup;
   private JLabel firstOutputFileNameLabel;
   private JSlider reflectionSlider, phiSlider, thetaSlider, psiSlider;
   private JTextField reflectionJTextField, phiJTextField,thetaJTextField,
                                                                psiJTextField;
   private JTextField thresholdJTextField, scaleJTextField,
                                                firstOutputFileNameJTextField;
   private JTextField startValueJTextField, endValueJTextField,
                                                           incrementJTextField;
   private float phi, theta, psi, thlev, refl;
   private float scale, xTrans, yTrans;
   private int lowContrast, highContrast;
   private String outputMsgStr = new String();
   private boolean imageFrameSurfaceOpen = false;
   private boolean showSurfaceActivatedForFirstTime = true;
   private int noOfImages, scrnWid, montagedSurfImgWidth, montagedSurfImgHeight;
   private float startValue, endValue, increment, size;
   private int noOfImgInRow, surfImgWidth, surfImgHeight;
   private String fileName, fileSeriesTemplate, fileSeriesExtension;

   public UtilGUIVolume_5(SpiderImage spiVol, String imageType, JButton closeAll,
                 UtilParameters utilParam, UtilParametersVolume utilParamVol) {

      utilParameters = utilParam;
      utilParametersVolume = utilParamVol;
      jButtonCloseAll = closeAll;

      spiderImage = spiVol;
      nsam = spiderImage.getnsam();
      nrow = spiderImage.getnrow();
      nslice = spiderImage.getnslice();
      min = spiderImage.getmin();
      max = spiderImage.getmax();

      surf = new Surf(spiderImage, nsam, nrow, nslice);
      sirdit = false;
      distit = false;
      colorit = false;

      GridBagLayout gridBagLayout = new GridBagLayout();
      this.setLayout(gridBagLayout);
      GridBagConstraints gridBagConstraints = new GridBagConstraints();

      thresholdLabel = new JLabel("   Threshold");
      addComponent(thresholdLabel,gridBagConstraints,1,1,1,1,"LEFT");
      thresholdJTextField = new JTextField(8);
      thresholdJTextField.setText((min+((max-min)/2))+"");
      addComponent(thresholdJTextField,gridBagConstraints,2,1,1,1,"LEFT");

      scaleLabel = new JLabel("Scale");
      addComponent(scaleLabel,gridBagConstraints,3,1,1,1,"LEFT");
      scaleJTextField = new JTextField(4);
      scaleJTextField.setText("1.0");
      addComponent(scaleJTextField,gridBagConstraints,4,1,1,1,"LEFT");

      reflectionLabel = new JLabel("   Reflection %");
      addComponent(reflectionLabel,gridBagConstraints,1,2,1,1,"LEFT");
      reflectionSlider = new JSlider(SwingConstants.HORIZONTAL,1,100,50);
      addComponent(reflectionSlider,gridBagConstraints,2,2,1,1,"LEFT");
      reflectionJTextField = new JTextField(4);
      reflectionJTextField.setEditable(false);
      addComponent(reflectionJTextField,gridBagConstraints,3,2,1,1,"LEFT");
      reflectionJTextField.setText("50");

      reflectionSlider.addChangeListener(new ChangeListener() {
         public void stateChanged(ChangeEvent ce) {
            // update display
            reflectionJTextField.setText(""+reflectionSlider.getValue());
         }
      });

      eulerAnglesLabel = new JLabel("EulerAngles : ");
      addComponent(eulerAnglesLabel,gridBagConstraints,1,3,1,1,"LEFT");

      phiLabel = new JLabel("   Phi");
      addComponent(phiLabel,gridBagConstraints,1,4,1,1,"LEFT");
      phiSlider = new JSlider(SwingConstants.HORIZONTAL,0,360,0);
      addComponent(phiSlider,gridBagConstraints,2,4,1,1,"LEFT");
      phiJTextField = new JTextField(4);
      phiJTextField.setEditable(false);
      addComponent(phiJTextField,gridBagConstraints,3,4,1,1,"LEFT");
      phiJTextField.setText("0");
      phiSlider.addChangeListener(new ChangeListener() {
         public void stateChanged(ChangeEvent ce) {
            // update display
            phiJTextField.setText(""+phiSlider.getValue());
         }
      });

      thetaLabel = new JLabel("   Theta");
      addComponent(thetaLabel,gridBagConstraints,1,5,1,1,"LEFT");
      thetaSlider = new JSlider(SwingConstants.HORIZONTAL,0,360,0);
      addComponent(thetaSlider,gridBagConstraints,2,5,1,1,"LEFT");
      thetaJTextField = new JTextField(4);
      thetaJTextField.setEditable(false);
      addComponent(thetaJTextField,gridBagConstraints,3,5,1,1,"LEFT");
      thetaJTextField.setText("0");
      thetaSlider.addChangeListener(new ChangeListener() {
         public void stateChanged(ChangeEvent ce) {
            // update display
            thetaJTextField.setText(""+thetaSlider.getValue());
         }
      });

      psiLabel = new JLabel("   Psi");
      addComponent(psiLabel,gridBagConstraints,1,6,1,1,"LEFT");
      psiSlider = new JSlider(SwingConstants.HORIZONTAL,0,360,0);
      addComponent(psiSlider,gridBagConstraints,2,6,1,1,"LEFT");
      psiJTextField = new JTextField(4);
      psiJTextField.setEditable(false);
      addComponent(psiJTextField,gridBagConstraints,3,6,1,1,"LEFT");
      psiJTextField.setText("0");
      psiSlider.addChangeListener(new ChangeListener() {
         public void stateChanged(ChangeEvent ce) {
            // update display
            psiJTextField.setText(""+psiSlider.getValue());
         }
      });

      movieCheckBox = new JCheckBox("Movie");
      addComponent(movieCheckBox,gridBagConstraints,5,1,1,1,"LEFT");
      movieCheckBox.addActionListener(
      new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            movieCheckBox_actionPerformed(e);
         }
      });

      phiRadioButton = new JRadioButton("Phi");
      phiRadioButton.setSelected(true);
      addComponent(phiRadioButton,gridBagConstraints,5,2,1,1,"LEFT");

      phiRadioButton.addActionListener(
      new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            phiRadioButton_actionPerformed(e);
         }
      });

      thetaRadioButton = new JRadioButton("Theta");
      thetaRadioButton.setSelected(false);
      addComponent(thetaRadioButton,gridBagConstraints,5,3,1,1,"LEFT");

      thetaRadioButton.addActionListener(
      new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            thetaRadioButton_actionPerformed(e);
         }
      });


      startValueLabel = new JLabel("Start Value");
      addComponent(startValueLabel,gridBagConstraints,6,3,1,1,"LEFT");
      endValueLabel = new JLabel("End Value");
      addComponent(endValueLabel,gridBagConstraints,7,3,1,1,"LEFT");
      incrementLabel = new JLabel("Increment");
      addComponent(incrementLabel,gridBagConstraints,8,3,1,1,"LEFT");

      psiRadioButton = new JRadioButton("Psi");
      psiRadioButton.setSelected(false);
      addComponent(psiRadioButton,gridBagConstraints,5,4,1,1,"LEFT");

      psiRadioButton.addActionListener(
      new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            psiRadioButton_actionPerformed(e);
         }
      });

      startValueJTextField = new JTextField(4);
      addComponent(startValueJTextField,gridBagConstraints,6,4,1,1,"LEFT");
      endValueJTextField = new JTextField(4);
      addComponent(endValueJTextField,gridBagConstraints,7,4,1,1,"LEFT");
      incrementJTextField = new JTextField(4);
      addComponent(incrementJTextField,gridBagConstraints,8,4,1,1,"LEFT");

      thresholdRadioButton = new JRadioButton("Threshold");
      thresholdRadioButton.setSelected(false);
      addComponent(thresholdRadioButton,gridBagConstraints,5,5,1,1,"LEFT");

      thresholdRadioButton.addActionListener(
      new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            thresholdRadioButton_actionPerformed(e);
         }
      });

      movieSelectionButtonGroup = new ButtonGroup();
      movieSelectionButtonGroup.add(phiRadioButton);
      movieSelectionButtonGroup.add(thetaRadioButton);
      movieSelectionButtonGroup.add(psiRadioButton);
      movieSelectionButtonGroup.add(thresholdRadioButton);

      saveToFileCheckBox = new JCheckBox("Save to File");
      addComponent(saveToFileCheckBox,gridBagConstraints,4,6,1,1,"LEFT");
      saveToFileCheckBox.addActionListener( new java.awt.event.ActionListener() {
          public void actionPerformed(ActionEvent e) {
             saveToFileCheckBox_actionPerformed(e);
          }
       });

      firstOutputFileNameLabel = new JLabel("First output file name : ");
      addComponent(firstOutputFileNameLabel,gridBagConstraints,4,7,3,1,"LEFT");
      firstOutputFileNameJTextField = new JTextField(15);
      addComponent(firstOutputFileNameJTextField,gridBagConstraints,6,7,2,1,"LEFT");

      JLabel dummyJLabel = new JLabel("     ");
      addComponent(dummyJLabel,gridBagConstraints,1,8,1,1,"CENTER");

      showSurfaceJButton = new JButton("Show Surface");
      addComponent(showSurfaceJButton,gridBagConstraints,2,9,2,1,"LEFT");
      showSurfaceJButton.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            showSurfaceJButton_actionPerformed(e);
         }
       });

      showMovieJButton = new JButton("Show Movie");
      addComponent(showMovieJButton,gridBagConstraints,5,9,2,1,"LEFT");
      showMovieJButton.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            showMovieJButton_actionPerformed(e);
         }
      });

      movieRelatedOptionsIEnabled(false);
   }

   public void showSurfaceJButton_actionPerformed(ActionEvent e) {

      // update input parameters
      thlev  = Float.parseFloat(thresholdJTextField.getText().trim());
      phi    = phiSlider.getValue();
      theta  = thetaSlider.getValue();
      psi    = psiSlider.getValue();
      refl   = (float)reflectionSlider.getValue()/100;
      imgScale = (double)Float.parseFloat(scaleJTextField.getText().trim());

      // if not activated for the first time, check if the window is closed
      // to use .isShowing method you need to create the object at least once
      // before OR you get an error.
      if (!showSurfaceActivatedForFirstTime) {
         imageFrameSurfaceOpen = imageFrameSurface.isShowing();
         if (imageFrameSurfaceOpen) {
            if (imageFrameSurface.getState() == imageFrameSurface.ICONIFIED) {
               imageFrameSurface.setState(imageFrameSurface.NORMAL);
            }
            else {
               // bring the frame to forefront
               imageFrameSurface.show();
            }

            // find if scale, translation and contrast is changed
            scale         = imageFrameSurface.utilImageManipulate.scale;
            xTrans        = imageFrameSurface.utilImageManipulate.xTrans;
            yTrans        = imageFrameSurface.utilImageManipulate.yTrans;
            lowContrast   = imageFrameSurface.utilImageManipulate.lowContrast;
            highContrast  = imageFrameSurface.utilImageManipulate.highContrast;

            // lowramp should be > than highramp
            if (lowContrast < highContrast) {
               imageFrameSurface.wdp.outImage = getSurfaceImage(phi, theta,
                                                     psi, thlev,refl,imgScale);
               // if output image is null display error message else display image
               if (imageFrameSurface.wdp.outImage != null) {
                  // copy out put image as input image in wdp so that EDIT and ANALYSIS
                  // functions will take the image that is displayed as the input image.
                  imageFrameSurface.wdp.img = imageFrameSurface.wdp.outImage;

                  // keeps the interpolation, translation and contrast as
                  // set in one image valid for all subsequent images.
                  if (xTrans != 0 || yTrans != 0 || scale != 1 ||
                                   lowContrast != 0 || highContrast != 255) {
                     imageFrameSurface.utilImageManipulate.setLowContrast
                           (imageFrameSurface.utilImageManipulate.lowContrast);
                     imageFrameSurface.utilImageManipulate.setHighContrast
                          (imageFrameSurface.utilImageManipulate.highContrast);
                  }
               }
            }
            else {
               outputMsgStr = " Low contrast ramp is greater than high contrast ramp !\n ";
               utilParameters.outputMessageTextArea.setText(outputMsgStr);
            }
            imageFrameSurface.wdp.repaint();
         }
      }

      // if activated for the first time OR if the window is closed
      if (!imageFrameSurfaceOpen) {
         TiledImage surfaceImgForDisplay = getSurfaceImage(phi, theta,
                                                   psi, thlev, refl, imgScale);
         imageFrameSurface = new ImageFrame(surfaceImgForDisplay, utilParameters);
         jButtonCloseAll.addActionListener(imageFrameSurface);
         imageFrameSurface.setTitle(utilParameters.currDirectory+" *Surface*");
         utilParametersVolume.surfaceShown = true;
         imageFrameSurface.setLocation (50,50);
         imageFrameSurface.setVisible(true);
         imageFrameSurfaceOpen = true;
         showSurfaceActivatedForFirstTime = false;
      }
   }

   public void showMovieJButton_actionPerformed(ActionEvent e) {

      // update input parameters
      thlev  = Float.parseFloat(thresholdJTextField.getText().trim());
      phi    = phiSlider.getValue();
      theta  = thetaSlider.getValue();
      psi    = psiSlider.getValue();
      refl   = (float)reflectionSlider.getValue()/100;
      imgScale = (double)Float.parseFloat(scaleJTextField.getText().trim());
      startValue = Float.parseFloat(startValueJTextField.getText().trim());
      endValue = Float.parseFloat(endValueJTextField.getText().trim());
      increment = Float.parseFloat(incrementJTextField.getText().trim());
      if((startValue <= endValue && increment > 0)||
                                (startValue >= endValue && increment < 0)) {

         // if saveToFileCheckBox is checked then find the file series template
         if (saveToFileCheckBox.isSelected()) {
            String fileSeriesName = firstOutputFileNameJTextField.getText().trim();
            int indexDot = fileSeriesName.lastIndexOf(".");
            fileSeriesTemplate = fileSeriesName.substring(0,indexDot);
            fileSeriesExtension = fileSeriesName.substring(indexDot,
                                                   fileSeriesName.length()) ;
         }
         noOfImages = (int) ((endValue - startValue)/increment) + 1;

         // find scaling of the image
         if (utilParameters.resizeLarge) {
            size = utilParameters.resizeFactor;
         }
         else {
            size = (1/utilParameters.resizeFactor);
         }

         scrnWid = utilParameters.screenDimension.width - 50;
         // reset the screen width as per the image scaling
         // the image will be scaled in ImageFrame.java
         scrnWid = (int)(scrnWid/size) ;

         surfaceSmallImage = getSurfaceImage(phi, theta, psi, thlev,refl,imgScale);
         surfImgWidth = surfaceSmallImage.getWidth();
         surfImgHeight =  surfaceSmallImage.getHeight();

         noOfImgInRow = scrnWid / surfImgWidth;
         if (noOfImgInRow == 0) noOfImgInRow = 1;
         if (noOfImages <= noOfImgInRow) noOfImgInRow = noOfImages;
         // add noOfImgInRow -1 pixels for a blank pixel after each image
         montagedSurfImgWidth = noOfImgInRow * surfImgWidth + noOfImgInRow - 1;
         int m = noOfImages / noOfImgInRow;
         int l = noOfImages % noOfImgInRow;

         if (l != 0) m = m + 1;
         // add k-1 pixels for a blank pixel after each image
         montagedSurfImgHeight = m * surfImgHeight + m - 1;

         // create an empty output tiled image using the sample model and the
         // color model from the earlier created tiled image surfaceSmallImage .
         TiledImage surfaceMontagedImage = new TiledImage(0,0,montagedSurfImgWidth,
                 montagedSurfImgHeight,0,0,surfaceSmallImage.getSampleModel(),
                                             surfaceSmallImage.getColorModel());
         //initialization
         float xScale = 1;
         float yScale = 1;
         float xTranslation = 0;
         float yTranslation = 0;
         Interpolation interpolation = new InterpolationNearest();
         int newX = 0;
         int newY = 0;

         outputMsgStr = "Begining of Surface Movie Parameters:" + "\n";
         utilParameters.outputMessageTextArea.append(outputMsgStr);

         // loop over all small images
         for (int i = 0; i < noOfImages; i++) {

            if (thresholdRadioButton.isSelected()) {
               thlev = startValue + i*increment ;
            }
            if (phiRadioButton.isSelected()) {
               phi = startValue + i*increment ;
            }
            if (thetaRadioButton.isSelected()) {
               theta = startValue + i*increment ;
            }
            if (psiRadioButton.isSelected()) {
               psi = startValue + i*increment ;
            }
            outputMsgStr = "Phi " + phi + " Theta  " + theta + " Psi  " +psi
                                              + " Threshold  " + thlev + "\n";
            utilParameters.outputMessageTextArea.append(outputMsgStr);

            surfaceSmallImage = getSurfaceImage(phi, theta, psi, thlev,refl,imgScale);
            // if saveToFileCheckBox is checked then save the image
            if (saveToFileCheckBox.isSelected()) {
               int ij = i +1 ;
               fileName = fileSeriesTemplate + ij + fileSeriesExtension;
               try {
                  dout = new DataOutputStream
                    (new BufferedOutputStream(new FileOutputStream(fileName)));

                  int wid = surfaceSmallImage.getWidth();
                  int ht =   surfaceSmallImage.getHeight();
                  int length= wid*ht;
                  int nslice = 1;
                  int nsam = wid;
                  int nrow = ht;
                  int iform = 1;
                  int imami = 0;
                  float fmax = 0;
                  float fmin = 0;
                  float av = 0;
                  float sig = 0;

                  int lenbyt = nsam*4;
                  int labrec = 1024/lenbyt;
                  if ((1024 % lenbyt) != 0) labrec = labrec + 1;
                  int labbyt = labrec * lenbyt;

                  float [] headerArrayOne = new float [211];
                  for (int k = 0; k < 211; k++) {
                     headerArrayOne[k] = 0;
                  }
                  headerArrayOne[0] = (float)nslice;
                  headerArrayOne[1] = (float)nrow;
                  headerArrayOne[4] = (float)iform;
                  headerArrayOne[5] = (float)imami;
                  headerArrayOne[6] = fmax;
                  headerArrayOne[7] = fmin;
                  headerArrayOne[8] = av;
                  headerArrayOne[9] = sig;
                  headerArrayOne[11] = (float)nsam;
                  headerArrayOne[12] = (float)labrec;
                  headerArrayOne[21] = (float)labbyt;
                  headerArrayOne[22] = (float)lenbyt;

                  for (int j = 0; j < 211; j++) {
                     dout.writeFloat(headerArrayOne[j]);
                  }

                  Date date = new Date();
                  String str = new String();
                  str = date.toString() ;

                  // added a space at the end just to make it 12 bytes i.e. divisible
                  // by 4. may not confirm to spider format

                  String dateString = str.substring(8,10) + "-" + str.substring(4,7)
                       + "-" + str.substring(str.length()-4,str.length())+ " ";
                  String timeString = str.substring(11,19);
                  char [] dateCharArray = dateString.toCharArray();
                  char [] timeCharArray = timeString.toCharArray();
                  dout.writeBytes(dateString);
                  dout.writeBytes(timeString);

                  for (int n = 217; n <= labbyt/4; n++) {
                     dout.writeFloat(0);
                  }

                  Raster ras = surfaceSmallImage.getData();
                  float [] dataFloat = new float [length];
                  ras.getPixels(0,0,wid,ht,dataFloat);
                  for (int p = 0; p < length; p++) {
                     dout.writeFloat(dataFloat[p]);
                  }
                  dout.close();
                  /*
                  how to use format operator to change the data type after loading
                  an image and then store the image
                  ParameterBlock pb;
                  pb = new ParameterBlock();
                  pb.addSource(sourceImage);
                  pb.add(DataBuffer.TYPE_FLOAT);
                  RenderedImage outImage = JAI.create("format", pb, null);
                  JAI.create("filestore",outImage,"c:\\bimal\\data\\test2.tiff", "tiff",null);
                  */
               }
               catch (Exception b) {
                  System.err.println(b.getMessage());
                  utilParameters.outputMessageTextArea.append("Error writting to file");
               }
            }

            // find the dimension and define region of interest. This is copied to the
            // montaged image at the right location.
            Rectangle bounds = null;
            bounds = surfaceSmallImage.getBounds();
            bounds.setLocation(newX,newY);
            ROI roi = new ROIShape(bounds);

            // create the translated image that is to be copied
            ParameterBlock pb = new ParameterBlock();
            pb.addSource(surfaceSmallImage);
            pb.add(xScale);
            pb.add(yScale);
            pb.add(xTranslation);
            pb.add(yTranslation);
            pb.add(interpolation);
            PlanarImage translatedSmallImage = (PlanarImage)JAI.create("scale",pb,null);

            // copy the image to the montaged image
            surfaceMontagedImage.setData(translatedSmallImage.getData(),roi);
            int j = (i+1) % noOfImgInRow;
            int k = (i+1)/noOfImgInRow;

            if((j == 0) && (k != 0)) {
               // if last image and not the first row reset X and update Y
               xTranslation = 0;
               newX = 0;
               yTranslation = yTranslation + bounds.height+1;
               newY = newY + bounds.height+1;
            }
            else {
               // set translation for the next image
               xTranslation = xTranslation + bounds.width+1;
               newX = newX + bounds.width+1;
            }
         }
         outputMsgStr = "End of Surface Movie Parameters:" + "\n";
         utilParameters.outputMessageTextArea.append(outputMsgStr);

         imageFrameMontagedSurface = new ImageFrame(surfaceMontagedImage,
                                                               utilParameters);

         jButtonCloseAll.addActionListener(imageFrameMontagedSurface);
         imageFrameMontagedSurface.setTitle(utilParameters.currDirectory
                                                       +" *Montaged Surface*");
         utilParametersVolume.montagedSurfaceShown = true;

         imageFrameMontagedSurface.setLocation (70,70);
         imageFrameMontagedSurface.setVisible(true);
      }
      else {
         outputMsgStr = "Wrong set of StartValue/EndValue/increment" + "\n";
         utilParameters.outputMessageTextArea.append(outputMsgStr);
      }
   }

   public TiledImage getSurfaceImage(float phiIn, float thetaIn, float psiIn,
                                float thlevIn, float reflIn, double imgScale) {

      surfImgByte = surf.surfmethod(phiIn, thetaIn, psiIn, thlevIn, reflIn,
                                      sirdit, distit, colorit, imgScale);
      nsamScaled = (int)(nsam * imgScale);
      nrowScaled = (int)(nrow * imgScale);
      surfaceImage = spiderImage.makeTiledImageFromByteArray(
                                        surfImgByte, nsamScaled, nrowScaled);
      return surfaceImage;
   }

   void movieCheckBox_actionPerformed(ActionEvent e) {
      if (movieCheckBox.isSelected()) {
         movieRelatedOptionsIEnabled(true);
                  movieRelatedOptionsIIEnabled(true);
      }
      else {
         movieRelatedOptionsIEnabled(false);
                  movieRelatedOptionsIIEnabled(false);
      }
   }

   void phiRadioButton_actionPerformed(ActionEvent e) {

      movieRelatedOptionsIIEnabled(true);
   }

   void thetaRadioButton_actionPerformed(ActionEvent e) {

      movieRelatedOptionsIIEnabled(true);
   }

   void psiRadioButton_actionPerformed(ActionEvent e) {

      movieRelatedOptionsIIEnabled(true);
   }

   void thresholdRadioButton_actionPerformed(ActionEvent e) {
    //  if (thresholdRadioButton.isSelected()) {
         movieRelatedOptionsIIEnabled(true);
     // }
   }

   void saveToFileCheckBox_actionPerformed(ActionEvent e) {
      if (saveToFileCheckBox.isSelected()) {
         firstOutputFileNameJTextField.setEditable(true);
         firstOutputFileNameLabel.setEnabled(true);
         int indexDot = utilParameters.currDirectory.lastIndexOf(".");
         String fileNameWOExtension = utilParameters.currDirectory.substring(0,indexDot);
         firstOutputFileNameJTextField.setText(fileNameWOExtension+".mov");
      }
      else {
         firstOutputFileNameJTextField.setText("");
         firstOutputFileNameJTextField.setEditable(false);
         firstOutputFileNameLabel.setEnabled(false);
      }
   }


   public void addComponent(Component comp,
                GridBagConstraints gridBagConstraints, int gridX, int gridY,
                              int gridWidth, int gridHeight, String align) {

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
      if (align == "LEFT") {
      // align to the left
         gridBagConstraints.anchor = GridBagConstraints.WEST;
      }
      else if (align == "CENTER"){
         // align to the center
         gridBagConstraints.anchor = GridBagConstraints.CENTER;
      }
      else {
         // align to the right
         gridBagConstraints.anchor = GridBagConstraints.EAST;
      }
      this.add(comp,gridBagConstraints);
   }

   public ImageFrame getImageFrame() {
      return imageFrameSurface;
   }

   public ImageFrame getMontagedImageFrame() {
       return imageFrameMontagedSurface;
   }

   public void movieRelatedOptionsIEnabled(boolean bool) {

      phiRadioButton.setEnabled(bool);
      thetaRadioButton.setEnabled(bool);
      psiRadioButton.setEnabled(bool);
      thresholdRadioButton.setEnabled(bool);
      startValueLabel.setEnabled(bool);
      endValueLabel.setEnabled(bool);
      incrementLabel.setEnabled(bool);
      showMovieJButton.setEnabled(bool);

      if (movieCheckBox.isSelected()) {
         startValueJTextField.setText("0");
         endValueJTextField.setText("360");
         incrementJTextField.setText("60");

      }
      else {
         // reset
         startValueJTextField.setText("");
         endValueJTextField.setText("");
         incrementJTextField.setText("");
      }
      startValueJTextField.setEditable(bool);
      endValueJTextField.setEditable(bool);
      incrementJTextField.setEditable(bool);
      saveToFileCheckBox.setSelected(false);
      saveToFileCheckBox.setEnabled(bool);
      firstOutputFileNameJTextField.setText("");
      firstOutputFileNameJTextField.setEditable(false);
      firstOutputFileNameLabel.setEnabled(false);
      showSurfaceJButton.setEnabled(!bool);
   }

   public void movieRelatedOptionsIIEnabled(boolean bool) {

      thresholdJTextField.setEditable(true);
      phiSlider.setEnabled(true);
      thetaSlider.setEnabled(true);
      psiSlider.setEnabled(true);
      if (thresholdRadioButton.isSelected()) {
         thresholdJTextField.setEditable(!bool);
      }
      if (phiRadioButton.isSelected()) {
         phiSlider.setEnabled(!bool);
      }
      if (thetaRadioButton.isSelected()) {
         thetaSlider.setEnabled(!bool);
      }
      if (psiRadioButton.isSelected()) {
         psiSlider.setEnabled(!bool);
      }
   }
}
