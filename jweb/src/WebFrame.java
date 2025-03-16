package web;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.border.*;
import java.io.*;

/**
 * Title:        WEB: A GUI for viewing and analyzing SPIDER images
 * Description:  WEB is a graphical user interface for viewing and analyzing SPIDER images.
 * Copyright:    Copyright (c) 2001
 * Company:      Health Research Inc.
 * @author Bimal K. Rath
 * @version 1.0
 */

public class WebFrame extends JFrame {

   Container contentPane;
   BorderLayout borderLayout1 = new BorderLayout();
   JPanel jPanel;


   JMenuBar jMenuBar = new JMenuBar();

   JMenu fileMenu = new JMenu();
   JMenu openMenu = new JMenu();
   JMenuItem imageMenuItem = new JMenuItem();
   JMenuItem imageSeriesMenuItem = new JMenuItem();
   JMenuItem volumeViewerMenuItem = new JMenuItem();
   JMenuItem docFileMenuItem = new JMenuItem();
   JButton jButtonCloseAll;
   JMenuItem exitMenuItem = new JMenuItem();

   JMenu optionsMenu = new JMenu();
   JMenuItem settingsMenuItem = new JMenuItem();

   ImageFrame imageFrame;
  // used for locating the image window
   private int imageNumber = 0;
   private int imageShift  = 20;
   private int imageFrameCoordinate = 0;
   private Dimension screenSize;

   private UtilParameters utilParameters;
   private UtilGUISettings utilGUISettings ;

   private boolean settingsActivatedForFirstTime = true;
   private boolean settingsFrameOpen = false;

   /**Construct the frame*/
   public WebFrame() {

      enableEvents(AWTEvent.WINDOW_EVENT_MASK);
      try {
         jbInit();
      }

      catch(Exception e) {
         e.printStackTrace();
      }
   }

   /**Component initialization*/
   private void jbInit() throws Exception {
      utilParameters = new UtilParameters();
      //setIconImage(Toolkit.getDefaultToolkit().createImage(WebFrame.class.getResource("Your Icon")));
      screenSize = Toolkit.getDefaultToolkit().getScreenSize();

      jPanel = new JPanel();
/*
      Border matteBorder = BorderFactory.createMatteBorder(1,1,1,1,Color.blue);
      Border titledBorder = BorderFactory.createTitledBorder(matteBorder,
                                                            "Output Messages");
      jPanel.setBorder(titledBorder);
*/
      contentPane = this.getContentPane();
      GridBagLayout gridBagLayout = new GridBagLayout();
      contentPane.setLayout(gridBagLayout);
      GridBagConstraints gridBagConstraints = new GridBagConstraints();

      JLabel dummyJLabel = new JLabel("     ");
      addComponent(contentPane,dummyJLabel,gridBagConstraints,0,1,1,1,"CENTER");

      JLabel outMsg = new JLabel("Output Messages:");
      addComponent(contentPane,outMsg,gridBagConstraints,0,6,3,1,"LEFT");

      utilParameters.outputMessageTextArea.setEditable(false);
      jPanel.add(new JScrollPane(utilParameters.outputMessageTextArea));
      addComponent(contentPane,jPanel,gridBagConstraints,0,7,10,10,"CENTER");

      JLabel dummyJLabel3 = new JLabel("     ");
      addComponent(contentPane,dummyJLabel3,gridBagConstraints,3,18,1,1,"CENTER");

      jButtonCloseAll = new JButton("  Close All Windows  ");
      addComponent(contentPane,jButtonCloseAll,gridBagConstraints,1,18,1,1,"LEFT");

      JButton jButtonClear = new JButton("Clear Output Messages");
      addComponent(contentPane,jButtonClear,gridBagConstraints,5,18,1,1,"LEFT");
      this.setSize(512,300);
      this.setTitle("JWeb -- A SPIDER image viewer and analyzer     COPYRIGHT (c) 1992,2001, Health Research Inc., Albany, NY");

      jButtonClear.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            jButtonClear_actionPerformed(e);
         }
      });

      this.setJMenuBar(jMenuBar);
      fileMenu.setText("File");
      fileMenu.setMnemonic('F');
      //fileMenu.setMnemonic(KeyEvent.VK_F);
      openMenu.setText("Open");
      //fileMenu.setMnemonic(KeyEvent.VK_O);
      openMenu.setMnemonic('P');

      imageMenuItem.setText("Image");
      imageMenuItem.setMnemonic('I');
      imageMenuItem.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            imageMenuItem_actionPerformed(e);
         }
      });

      imageSeriesMenuItem.setText("Image Series/ Image Stacks");
      imageSeriesMenuItem.setMnemonic('S');
      imageSeriesMenuItem.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            imageSeriesMenuItem_actionPerformed(e);
         }
      });

      volumeViewerMenuItem.setText("Volume");
      volumeViewerMenuItem.setMnemonic('V');
      volumeViewerMenuItem.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            volumeViewerMenuItem_actionPerformed(e);
         }
      });

      docFileMenuItem.setText("SPIDER Document File");
       docFileMenuItem.setMnemonic('D');
       docFileMenuItem.addActionListener(new java.awt.event.ActionListener() {
          public void actionPerformed(ActionEvent e) {
             docFileMenuItem_actionPerformed(e);
          }
       });


      exitMenuItem.setText("Exit");
      exitMenuItem.setMnemonic('x');
      exitMenuItem.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            exitMenuItem_actionPerformed(e);
         }
      });

      optionsMenu.setText("Options");
      optionsMenu.setMnemonic('O');

      settingsMenuItem.setText("Settings");
      settingsMenuItem.setMnemonic('S');
      settingsMenuItem.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            settingsMenuItem_actionPerformed(e);
         }
      });



      jMenuBar.add(fileMenu);
      fileMenu.addSeparator();
      fileMenu.add(openMenu);
      fileMenu.addSeparator();
      openMenu.add(imageMenuItem);
      openMenu.addSeparator();
      openMenu.add(imageSeriesMenuItem);
      openMenu.addSeparator();
      openMenu.add(volumeViewerMenuItem);
      openMenu.addSeparator();
      openMenu.add(docFileMenuItem);
      fileMenu.add(exitMenuItem);

      jMenuBar.add(optionsMenu);
      optionsMenu.addSeparator();
      optionsMenu.add(settingsMenuItem);

   }

   /**Overridden so we can exit when window is closed*/
   protected void processWindowEvent(WindowEvent e) {
      super.processWindowEvent(e);
      if (e.getID() == WindowEvent.WINDOW_CLOSING) {
         System.exit(0);
      }
   }

   // method for opening an image/volume bypassing the JWEB menu driven GUI
   // used when jweb is called from spidui or from command prompt
   public void openAnImage(String s, String strInterfaceType) {

      UtilFile utilFile = new UtilFile(utilParameters);
      boolean twoDImage = utilFile.check2DOr3D(s);
      if (twoDImage){
         // used for setting image frame location
         imageNumber++;
         imageFrame = new ImageFrame(s, utilParameters);
         // listen to Close all button in the WebFrame
         jButtonCloseAll.addActionListener(imageFrame);
         imageFrame.setVisible(true);
         // image Frame origin is set at
         // (imageFrameCoordinate, imageFrameCoordinate)
         imageFrameCoordinate = imageShift * imageNumber;
         if ( (imageFrameCoordinate + 400) > screenSize.width
                 || (imageFrameCoordinate + 400) > screenSize.height) {
            imageFrameCoordinate = 0;
            imageNumber = 0;
         }
         imageFrame.setLocation(imageFrameCoordinate, imageFrameCoordinate);
         // if window is closed then exit for inline command
         if (strInterfaceType == "inline") {
            imageFrame.addWindowListener(new WindowAdapter() {
               public void windowClosing (WindowEvent e) {
                  System.exit(0);
               }
            });
         }
      }
      else {
         //3D image or volume

         imageNumber ++ ;
         SpiderImage spiderVolume = new SpiderImage(s,utilParameters);
         // read the image data into memory
         try {
            spiderVolume.readImage();
         }
         catch (IOException er) {
            String str = "Error reading SpiderImage data: " + er + "\n";
            utilParameters.outputMessageTextArea.append(str);
         }
         UtilGUIVolume volGUI = new UtilGUIVolume(spiderVolume,
                               "Volume", jButtonCloseAll, utilParameters);

         // select the surface tab
         volGUI.jTabbedPane.setSelectedIndex(3);
         // programatically click the "Show Surface" button
         //volGUI.utilGUIVol2Surface.showSurfaceJButton.doClick();
         jButtonCloseAll.addActionListener(volGUI);
         // if window is closed then exit for inline command
         if (strInterfaceType == "inline") {
            volGUI.addWindowListener(new WindowAdapter() {
               public void windowClosing (WindowEvent e) {
                  System.exit(0);
               }
            });
         }
      }
   }

   void imageMenuItem_actionPerformed(ActionEvent e) {

      // used for setting image frame location
      imageNumber ++ ;
      UtilFile utilFile = new UtilFile(utilParameters);
      String s = utilFile.selectAFile();
      if (s != null) {
         try {
            imageFrame = new ImageFrame(s,utilParameters);
            // listen to Close all button in the WebFrame
            jButtonCloseAll.addActionListener(imageFrame);
            imageFrame.setVisible(true);
            // image Frame origin is set at
            // (imageFrameCoordinate, imageFrameCoordinate)
            imageFrameCoordinate = imageShift*imageNumber;
            if ((imageFrameCoordinate + 400) > screenSize.width
                 || (imageFrameCoordinate + 400) > screenSize.height) {
                 imageFrameCoordinate = 0;
                 imageNumber = 0;
            }
            imageFrame.setLocation(imageFrameCoordinate, imageFrameCoordinate);
         }
         catch (Exception b) {
            System.err.println(b.getMessage());
         }
      }
      else {
         utilParameters.outputMessageTextArea.append
                                               ("\n error in selecting image!\n");
      }
   }


   void imageSeriesMenuItem_actionPerformed(ActionEvent e) {

      String [] fileNames;
      byte [] stackedImagePixels;

      UtilFile utilFile = new UtilFile(utilParameters);
      // get an array of strings containing file name including file path
      fileNames = utilFile.selectMultipleFiles () ;
      if (fileNames != null) {
         int numFiles = fileNames.length;
         // if image files (not an image stack)
         if (numFiles != 1) {
            // test if the images are of same size
            boolean sameSizeImages = utilFile.areSameSizeImages(fileNames);

            // if images are of same size then treat them as slices of a spider volume
            if (sameSizeImages) {
               // make a byte array containing the data from the images. images
               // are treated as slices of a volume. 1st image is the 1st slice; 2nd
               // image is the  2nd slice and so on.
               stackedImagePixels = utilFile.makeVolumeFromImageSeries(fileNames);

               // Don't show header info of the spider images. Only the image width
               // and height is read. Header info will be shown when the
               // image data is actually read.
               utilParameters.showHeaderInfo = false;
               // make a temporary spider image to get the image dimension
               SpiderImage spiTempImage = new SpiderImage(fileNames[0],utilParameters);
               int imgWidth = spiTempImage.getnsam();
               int imgHeight = spiTempImage.getnrow();
               // show header info ("reset to default") of the spider images.
               utilParameters.showHeaderInfo = true;

               // make a spider image(volume) from the byte array
               SpiderImage spiderVolume = new SpiderImage(stackedImagePixels,
                                   imgWidth,imgHeight,numFiles,utilParameters);
               // make the GUI for the image series operations
               UtilGUIImageSeries imageSeriesGUI = new UtilGUIImageSeries(
                      spiderVolume, "ImageSeries", fileNames, jButtonCloseAll,
                                                               utilParameters);
               jButtonCloseAll.addActionListener(imageSeriesGUI);
            }
            // images are of different dimensions
            else {
               imageFrame = new ImageFrame(fileNames,utilParameters);
               // image Frame origin is set at
               // (imageFrameCoordinate, imageFrameCoordinate)
               imageFrameCoordinate = imageShift*imageNumber;
               // listen to Close all button in the WebFrame
               jButtonCloseAll.addActionListener(imageFrame);

               if ((imageFrameCoordinate + 400) > screenSize.width
                         || (imageFrameCoordinate + 400) > screenSize.height) {
                  imageFrameCoordinate = 0;
                  imageNumber = 0;
               }
               imageFrame.setLocation(imageFrameCoordinate,
                                                         imageFrameCoordinate);
               imageFrame.setVisible(true);
            }
         }
         // image stack
         else {
            String fileName = fileNames[0];
            SpiderImage stackImage = new SpiderImage(fileName, utilParameters);
            try {
               stackImage.readImageStack();
            }
            catch (IOException err) {
               String str = "Error reading data in SpiderImage: " + err + "\n";
               utilParameters.outputMessageTextArea.append(str);
            }
            // get pixels in bytes
            stackedImagePixels = stackImage.getBpixStackedImage();
            // get individual image width, height and image numbers that are used
            int imgWidth  = stackImage.getnsam();
            int imgHeight = stackImage.getnrow();
            fileNames = stackImage.getInuseImageNumbersAsStrings();
            // add a phony prefix and suffix to the stck numbers those are
            // used for storing image so that method
            // utilFile.findFileNumber(String s) will work correctly
            for (int i = 0; i < fileNames.length; i++) {
               fileNames[i] =  "spider" + fileNames[i] + ".dat";
            }
            numFiles  = fileNames.length;

            // make a spider image(volume) from the byte array
            SpiderImage spiderVolume = new SpiderImage(stackedImagePixels,
                                   imgWidth,imgHeight,numFiles,utilParameters);
            // make the GUI for the image series operations
            UtilGUIImageSeries imageSeriesGUI = new UtilGUIImageSeries(
                spiderVolume, "ImageSeries", fileNames, jButtonCloseAll,
                                                               utilParameters);
            jButtonCloseAll.addActionListener(imageSeriesGUI);
         }
      }
      else {
         utilParameters.outputMessageTextArea.append
                                     ("\n error in selecting image series!\n");
      }
   }

   void volumeViewerMenuItem_actionPerformed(ActionEvent e) {

      imageNumber ++ ;
      UtilFile utilFile = new UtilFile(utilParameters);
      String s = utilFile.selectAFile();
      if (s != null) {
         SpiderImage spiderVolume = new SpiderImage(s,utilParameters);
         // read the image data into memory
         try {
            spiderVolume.readImage();
         }
         catch (IOException er) {
            String str = "Error reading SpiderImage data: " + er + "\n";
            utilParameters.outputMessageTextArea.append(str);
         }
         UtilGUIVolume volGUI = new UtilGUIVolume(spiderVolume,
                                    "Volume", jButtonCloseAll, utilParameters);

         jButtonCloseAll.addActionListener(volGUI);
      }
      else {
         utilParameters.outputMessageTextArea.append
                                           ("\n error in selecting volume!\n");
      }
   }

   void docFileMenuItem_actionPerformed(ActionEvent e) {

      UtilFile utilFile = new UtilFile(utilParameters);
      String s = utilFile.selectAFile();
      if (s != null) {
         boolean KeyDuplicationAllowed = false;
         String op = "r";
         UtilDoc utilDoc = new UtilDoc(s,op,utilParameters);
         UtilGUIDocFile docGUI = new UtilGUIDocFile(utilDoc,
                                     jButtonCloseAll, utilParameters);
         jButtonCloseAll.addActionListener(docGUI);
      }
      else {
         utilParameters.outputMessageTextArea.append
                                           ("\n error in selecting SPIDER document file!\n");
      }
   }

   void jButtonClear_actionPerformed(ActionEvent e) {
      utilParameters.outputMessageTextArea.setText("");
   }

   void exitMenuItem_actionPerformed(ActionEvent e) {
      System.exit(0);
   }

   void settingsMenuItem_actionPerformed(ActionEvent e) {

      // if not activated for the first time, check if the window is closed.
      // to use .isShowing method you need to create the object at least once
      // before OR you get an error.
      if (!settingsActivatedForFirstTime) {
         settingsFrameOpen = utilGUISettings.isShowing();
         if (settingsFrameOpen) {
            if (utilGUISettings.getState() ==
                             utilGUISettings.ICONIFIED) {
               utilGUISettings.setState(utilGUISettings.NORMAL);
            }
            else {
               utilGUISettings.show();
            }
         }
      }
      // if activated for the first time OR if the window is closed
      if (!settingsFrameOpen) {
         settingsActivatedForFirstTime = false;
         utilGUISettings = new UtilGUISettings(utilParameters);
         jButtonCloseAll.addActionListener(utilGUISettings);
      }
   }

    public void addComponent(Container contentPane, Component comp,
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
      contentPane.add(comp,gridBagConstraints);
   }

}
