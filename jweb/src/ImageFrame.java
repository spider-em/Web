package web;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import java.awt.image.*;
import java.io.*;
import javax.media.jai.*;

/**
 * Title:        WEB: A GUI for viewing and analyzing SPIDER images
 * Description:  WEB is a graphical user interface for viewing and analyzing SPIDER images.
 * Copyright:    Copyright (c) 2001
 * Company:      Health Research Inc.
 * @author Bimal K. Rath
 * @version 1.0
 */

public class ImageFrame extends JFrame implements KeyListener, MouseListener,
                                                  ActionListener{

   JPanel contentPane;
   BorderLayout borderLayout1 = new BorderLayout();

   JMenuBar jMenuBar = new JMenuBar();

   JMenu fileMenu  = new JMenu();
   JMenu saveAsMenu = new JMenu();
   JMenuItem spiderMenuItem = new JMenuItem();
   JMenuItem rawMenuItem = new JMenuItem();
   JMenuItem bmpMenuItem = new JMenuItem();
   JMenuItem jpegMenuItem = new JMenuItem();
   JMenuItem pngMenuItem = new JMenuItem();
   JMenuItem pnmMenuItem = new JMenuItem();
   JMenuItem tiffMenuItem = new JMenuItem();

   JMenu editMenu  = new JMenu();
   JMenuItem translateMenuItem = new JMenuItem();
   JMenuItem interpolateMenuItem = new JMenuItem();
   JMenuItem contrastMenuItem = new JMenuItem();
   JMenuItem windowMenuItem = new JMenuItem();

   JMenu analysisMenu  = new JMenu();
   JMenuItem pixelMenuItem = new JMenuItem();
   JMenuItem annotateMenuItem = new JMenuItem();
   JMenuItem markerMenuItem = new JMenuItem();
   JMenuItem histogramMenuItem = new JMenuItem();
   JMenuItem distanceMenuItem = new JMenuItem();
   JMenuItem maskMenuItem = new JMenuItem();
   JMenuItem powerMenuItem = new JMenuItem();

   boolean singleImage    = false;
   private boolean montImage      = false;
   boolean montSlices     = false;
   boolean windowFrameOpen = false;
   boolean pixelFrameOpen = false;
   boolean annotateFrameOpen = false;
   boolean markerFrameOpen = false;
   private boolean montage = false;
   private boolean histogramFrameOpen = false;
   private boolean distanceFrameOpen = false;
   private boolean maskFrameOpen = false;
   boolean powerWinFrameOpen = false;

   private boolean translateActivatedForFirstTime = true;
   private boolean transFrameOpen = false;
   private boolean reSizeActivatedForFirstTime = true;
   private boolean reSzFrameOpen = false;
   private boolean contrastActivatedForFirstTime = true;
   private boolean contrFrameOpen = false;

   private float resizeFactor;

   WebDisplayPanel wdp;

   public UtilImageManipulate utilImageManipulate;

   String    s, fileStr;
   File []   files;

   private SpiderImage spiderimage;
   private UtilGUIWindow utilGUIWindow;
   private UtilGUIPixel utilGUIPixel;
   private UtilGUIAnnotate utilGUIAnnotate;
   private UtilGUIMarker utilGUIMarker;
   private HistogramFrame histogramFrame;
   private UtilGUIDistance utilGUIDistance;
   private UtilGUIMask utilGUIMask;
   // private UtilGUIPower utilGUIPower;
   private UtilGUIWindow utilGUIPowerWindow;

   private Dimension screenSize;
   private boolean insidePanel = false;
   private UtilParameters utilParameters;
   private TiledImage montagedSlices;
   private String [] filNames;


   // used for a single image
   public ImageFrame(String str, UtilParameters utilParam) {
     s = str;
     utilParameters = utilParam;
     //resize large
     if (utilParameters.resizeLarge) {
        resizeFactor = utilParameters.resizeFactor;
     }
     // resize small
     else {
        resizeFactor = 1/utilParameters.resizeFactor;
     }

     enableEvents(AWTEvent.WINDOW_EVENT_MASK);
      try {
         // used in jbInit()
         singleImage = true;
         jbInit();
      }
      catch(Exception e) {
         e.printStackTrace();
      }
   }

   // used for montage of an image series with unequal dimensions
   public ImageFrame(String [] filesStrings, UtilParameters utilParam) {
      utilParameters = utilParam;
      //resize large
      if (utilParameters.resizeLarge) {
          resizeFactor = utilParameters.resizeFactor;
      }
      // resize small
      else {
         resizeFactor = 1/utilParameters.resizeFactor;
      }

      filNames = filesStrings ;
      enableEvents(AWTEvent.WINDOW_EVENT_MASK);

      try {
         montImage = true;
         montage = montImage;
         jbInit();
      }
      catch(Exception e) {
         e.printStackTrace();
      }
   }

   // used for a volume OR
   // for montage of an image series with equal dimensions
   public ImageFrame(TiledImage tiledSlices, UtilParameters utilParam) {
      utilParameters  =  utilParam;
      //resize large
      if (utilParameters.resizeLarge) {
         resizeFactor = utilParameters.resizeFactor;
      }
      // resize small
      else {
         resizeFactor = 1/utilParameters.resizeFactor;
      }

      montagedSlices  =  tiledSlices;

      try {
         montSlices = true;
         montage = montSlices;
         jbInit();
      }

      catch(Exception e) {
         e.printStackTrace();
      }


   }

   /**Component initialization*/
   private void jbInit() throws Exception {
      // setIconImage(Toolkit.getDefaultToolkit().createImage(WebFrame.class.getResource("[Your Icon]")));
      screenSize = Toolkit.getDefaultToolkit().getScreenSize();
      contentPane = (JPanel) this.getContentPane();
      contentPane.setLayout(borderLayout1);
      contentPane.setBackground(Color.black);

      fileMenu.setText("File");
      fileMenu.setMnemonic('F');

      saveAsMenu.setText("SaveAs");
      saveAsMenu.setMnemonic('A');

      spiderMenuItem.setText("SPIDER");
      spiderMenuItem.setMnemonic('S');
      spiderMenuItem.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            spiderMenuItem_actionPerformed(e);
         }
      });

      rawMenuItem.setText("RAW");
      rawMenuItem.setMnemonic('R');
      rawMenuItem.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            rawMenuItem_actionPerformed(e);
         }
      });

      bmpMenuItem.setText("BMP");
      bmpMenuItem.setMnemonic('B');
      bmpMenuItem.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            bmpMenuItem_actionPerformed(e);
         }
      });

      jpegMenuItem.setText("JPEG");
      jpegMenuItem.setMnemonic('J');
      jpegMenuItem.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            jpegMenuItem_actionPerformed(e);
         }
      });

      pngMenuItem.setText("PNG");
      pngMenuItem.setMnemonic('P');
      pngMenuItem.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            pngMenuItem_actionPerformed(e);
         }
      });

      pnmMenuItem.setText("PNM");
      pnmMenuItem.setMnemonic('N');
      pnmMenuItem.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            pnmMenuItem_actionPerformed(e);
         }
      });

      tiffMenuItem.setText("TIFF");
      tiffMenuItem.setMnemonic('T');
      tiffMenuItem.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            tiffMenuItem_actionPerformed(e);
         }
      });

      editMenu.setText("Edit");
      editMenu.setMnemonic('E');

      translateMenuItem.setText("Translate");
      translateMenuItem.setMnemonic('T');
      translateMenuItem.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            translateMenuItem_actionPerformed(e);
         }
      });

      interpolateMenuItem.setText("Resize");
      interpolateMenuItem.setMnemonic('R');
      interpolateMenuItem.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            interpolateMenuItem_actionPerformed(e);
         }
      });

      contrastMenuItem.setText("Contrast");
      contrastMenuItem.setMnemonic('C');
      contrastMenuItem.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            contrastMenuItem_actionPerformed(e);
         }
      });

      windowMenuItem.setText("Window");
      windowMenuItem.setMnemonic('W');
      windowMenuItem.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            windowMenuItem_actionPerformed(e);
         }
      });

      analysisMenu.setText("Analysis");
      analysisMenu.setMnemonic('A');

      pixelMenuItem.setText("Pixel");
      pixelMenuItem.setMnemonic('P');
      pixelMenuItem.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            pixelMenuItem_actionPerformed(e);
         }
      });

      annotateMenuItem.setText("Annotate");
      annotateMenuItem.setMnemonic('A');
      annotateMenuItem.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            annotateMenuItem_actionPerformed(e);
         }
      });

      markerMenuItem.setText("Marker");
      markerMenuItem.setMnemonic('M');
      markerMenuItem.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            markerMenuItem_actionPerformed(e);
         }
      });

      histogramMenuItem.setText("Histogram");
      histogramMenuItem.setMnemonic('H');
      histogramMenuItem.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            histogramMenuItem_actionPerformed(e);
         }
      });

      distanceMenuItem.setText("Distance");
      distanceMenuItem.setMnemonic('D');
      distanceMenuItem.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            distanceMenuItem_actionPerformed(e);
         }
      });

      maskMenuItem.setText("Mask");
       maskMenuItem.setMnemonic('s');
       maskMenuItem.addActionListener(new java.awt.event.ActionListener() {
          public void actionPerformed(ActionEvent e) {
             maskMenuItem_actionPerformed(e);
          }
       });

       powerMenuItem.setText("Power Spectrum");
        powerMenuItem.setMnemonic('o');
        powerMenuItem.addActionListener(new java.awt.event.ActionListener() {
           public void actionPerformed(ActionEvent e) {
              powerMenuItem_actionPerformed(e);
           }
        });



      this.setJMenuBar(jMenuBar);
      jMenuBar.add(fileMenu);
      fileMenu.addSeparator();
      fileMenu.add(saveAsMenu);
      saveAsMenu.add(spiderMenuItem);
      saveAsMenu.addSeparator();
      saveAsMenu.add(rawMenuItem);
      saveAsMenu.addSeparator();
      saveAsMenu.add(bmpMenuItem);
      saveAsMenu.addSeparator();
      saveAsMenu.add(jpegMenuItem);
      saveAsMenu.addSeparator();
      saveAsMenu.add(pngMenuItem);
      saveAsMenu.addSeparator();
      saveAsMenu.add(pnmMenuItem);
      saveAsMenu.addSeparator();
      saveAsMenu.add(tiffMenuItem);
      saveAsMenu.addSeparator();

      jMenuBar.add(editMenu);
      editMenu.addSeparator();
      editMenu.add(translateMenuItem);
      editMenu.addSeparator();
      editMenu.add(interpolateMenuItem);
      editMenu.addSeparator();
      editMenu.add(contrastMenuItem);
      editMenu.addSeparator();
      editMenu.add(windowMenuItem);

      jMenuBar.add(analysisMenu);
      analysisMenu.addSeparator();
      analysisMenu.add(pixelMenuItem);
      analysisMenu.add(annotateMenuItem);
      analysisMenu.add(markerMenuItem);
      analysisMenu.add(histogramMenuItem);
      analysisMenu.add(distanceMenuItem);
      analysisMenu.add(maskMenuItem);
      analysisMenu.add(powerMenuItem);


      if (singleImage) {
         this.setTitle(s);
         showImage();
      }
      else if (montImage) {
         this.setTitle("Montaged Image");
         showMontage();
      }
      else if (montSlices) {
         showMontagedSlices();
      }
   }

   /* close all related windows when image window is closed*/
   protected void processWindowEvent(WindowEvent e) {
      super.processWindowEvent(e);
      if (e.getID() == WindowEvent.WINDOW_CLOSING) {
         disposeAuxiliaryWindows();
      }
   }

   void showImage() {
      try {
         TiledImage  tilImage;
         // if the following popular image formats are found
         if (s.endsWith(".jpg")|| s.endsWith(".jpeg")||s.endsWith(".gif")
                  || s.endsWith(".tiff") || s.endsWith(".tif")
                     || s.endsWith(".bmp") || s.endsWith(".png")
                        || s.endsWith(".pnm")) {
            // create a rendered image using JAI utilities
            RenderedImage sourceImage = JAI.create("fileload", s);

            // create a tiled image from the rendered image
            UtilImageCreate utilImageCreate = new UtilImageCreate(utilParameters);
            tilImage = utilImageCreate.getTiledImageFromRenderedImage(sourceImage);
            utilParameters.outputMessageTextArea.append(s + "\n");
            int imgNsam = tilImage.getWidth();
            int imgNrow = tilImage.getHeight();
            String outputStr =  " width = "+imgNsam+" height = "+imgNrow + "\n";
            utilParameters.outputMessageTextArea.append(outputStr);
            // get rid of the duplicate image
            sourceImage = null;
            // do run time garbage collection. Even though it is done three times still
            // sometimes it may not do the job. Something to do with JAVA features.
            Runtime.getRuntime().gc();
            Runtime.getRuntime().gc();
            Runtime.getRuntime().gc();
         }
         else {
            // get tiled spider image
            spiderimage = new SpiderImage(s,utilParameters);
            tilImage = spiderimage.getImage(1);
         }
// needs to be investigated how to make frame size that includes the frame borders
// and the image size to adjust height and menu bar width and the image size for
// adjusting width. Currently I have found the limits manually. It will change as
// the menu bar is expanded.
// you may use pack() after adding scrollpane to contentpane; however, for large
// images the bottom scroll bar is not visible at first. it gets visible by
// clicking on the middle icon on the right side of the window.

         // initialize frame size
         int frameWidth = (int)(tilImage.getWidth()*resizeFactor + 12);
         int frameHeight = (int)(tilImage.getHeight()*resizeFactor + 54);

         if (frameWidth >= screenSize.width) {
            frameWidth = screenSize.width-50;
         }
         if (frameHeight >= screenSize.height) {
            frameHeight = screenSize.height-50;
         }

         this.setSize(new Dimension(frameWidth, frameHeight));
         wdp = new WebDisplayPanel(tilImage);
         wdp.addKeyListener(this);
         wdp.addMouseListener(this);

         JScrollPane sp= new JScrollPane(wdp);
         contentPane.add(sp,"Center");

         // to be used for image manipulation
         utilImageManipulate = new UtilImageManipulate(wdp, utilParameters);
         // set contrast from global settings
         utilImageManipulate.setLowContrast(utilParameters.contrastLowRamp);
         utilImageManipulate.setHighContrast(utilParameters.contrastHighRamp);
         //repaint();
      }
      catch (Exception b) {
         System.err.println(b.getMessage());
      }
   }

   void showMontage() {
      try {

         int montagedImageWidth = 0;
         int montagedImageHeight = 0;
         int numFiles = filNames.length;
         int noOfImgPerRow = numFiles;

         // Don't show header info of the spider images. Only the montage
         // image width and height is calculated. Header info will be shown when the
         // image is actually read and montage image is constructed.
         utilParameters.showHeaderInfo = false;
         for (int i =0; i < numFiles; i++) {
            String fileName = filNames[i];
            SpiderImage spiImage = new SpiderImage(fileName,utilParameters);
            montagedImageWidth = montagedImageWidth + spiImage.nsam ;
            if (montagedImageHeight < spiImage.nrow) {
               montagedImageHeight = spiImage.nrow;
            }
         }
         // add extra one pixel for separating one image from another
         montagedImageWidth = montagedImageWidth + (numFiles - 1);
         // show header info (reset) of the spider images.
         utilParameters.showHeaderInfo = true;
         UtilImageCreate utilImageCreate = new UtilImageCreate(utilParameters);
         TiledImage montagedImage;
         // images are not of same dimension
         boolean imageStack = false;
         montagedImage = utilImageCreate.getMontagedImage(filNames, montagedImageWidth,
                                 montagedImageHeight,imageStack,noOfImgPerRow);

         // initialize frame size
         int frameWidth = (int)( montagedImage.getWidth()*resizeFactor + 12);
         int frameHeight = (int)(montagedImage.getHeight()*resizeFactor + 54);

         if (frameWidth >= screenSize.width)  {
            frameWidth = screenSize.width-50;
         }
         if (frameHeight >= screenSize.height) {
            frameHeight = screenSize.height-50;
         }

         this.setSize(new Dimension(frameWidth, frameHeight));

         wdp = new WebDisplayPanel(montagedImage);
         wdp.addKeyListener(this);
         wdp.addMouseListener(this);
         JScrollPane sp= new JScrollPane(wdp);
         contentPane.add(sp,"Center");
         // to be used for image manipulation
         utilImageManipulate = new UtilImageManipulate(wdp, utilParameters);
         // set contrast from global settings
         utilImageManipulate.setLowContrast(utilParameters.contrastLowRamp);
         utilImageManipulate.setHighContrast(utilParameters.contrastHighRamp);
         repaint();
      }
      catch (Exception b) {
         System.err.println(b.getMessage());
      }
   }

   void showMontagedSlices() {

      try {
         // initialize frame size
         int frameWidth = (int)(montagedSlices.getWidth()*resizeFactor + 12);
         int frameHeight = (int)(montagedSlices.getHeight()*resizeFactor + 54);

         if (frameWidth >= screenSize.width)  {
            frameWidth = screenSize.width-50;
         }
         if (frameHeight >= screenSize.height) {
            frameHeight = screenSize.height-50;
         }
         this.setSize(new Dimension(frameWidth, frameHeight));
         wdp = new WebDisplayPanel(montagedSlices);
         wdp.addKeyListener(this);
         wdp.addMouseListener(this);
         JScrollPane sp= new JScrollPane(wdp);
         contentPane.add(sp,"Center");
         // to be used for image manipulation
         utilImageManipulate = new UtilImageManipulate(wdp, utilParameters);
         // set contrast from global settings
         utilImageManipulate.setLowContrast(utilParameters.contrastLowRamp);
         utilImageManipulate.setHighContrast(utilParameters.contrastHighRamp);
         repaint();
      }
      catch (Exception b) {
         System.err.println(b.getMessage());
      }

   }

   void spiderMenuItem_actionPerformed(ActionEvent e) {
      String fileFormat = "spider";
      UtilImageConvert utilImageConvert = new UtilImageConvert
                                             (wdp, fileFormat, utilParameters);
   }

   void rawMenuItem_actionPerformed(ActionEvent e) {
      String fileFormat = "raw";
      UtilImageConvert utilImageConvert = new UtilImageConvert
                                             (wdp, fileFormat, utilParameters);
   }

   void bmpMenuItem_actionPerformed(ActionEvent e) {
      String fileFormat = "bmp";
      UtilImageConvert utilImageConvert = new UtilImageConvert
                                             (wdp, fileFormat, utilParameters);
   }

   void jpegMenuItem_actionPerformed(ActionEvent e) {
      String fileFormat = "jpeg";
      UtilImageConvert utilImageConvert = new UtilImageConvert
                                             (wdp, fileFormat, utilParameters);
   }

   void pngMenuItem_actionPerformed(ActionEvent e) {
      String fileFormat = "png";
      UtilImageConvert utilImageConvert = new UtilImageConvert
                                             (wdp, fileFormat, utilParameters);
   }

   void pnmMenuItem_actionPerformed(ActionEvent e) {
      String fileFormat = "pnm";
      UtilImageConvert utilImageConvert = new UtilImageConvert
                                             (wdp, fileFormat, utilParameters);
   }

   void tiffMenuItem_actionPerformed(ActionEvent e) {
      String fileFormat = "tiff";
      UtilImageConvert utilImageConvert = new UtilImageConvert
                                             (wdp, fileFormat, utilParameters);
   }

   void translateMenuItem_actionPerformed(ActionEvent e) {

     if (!translateActivatedForFirstTime) {
        transFrameOpen = utilImageManipulate.frame.isShowing();
        if (transFrameOpen) {
           if (utilImageManipulate.frame.getState() ==
                                       utilImageManipulate.frame.ICONIFIED) {
              utilImageManipulate.frame.setState(utilImageManipulate.frame.NORMAL);
           }
           else {
              utilImageManipulate.frame.show();
           }
        }
     }
     // if activated for the first time OR if the window is closed
     if (!transFrameOpen) {
        translateActivatedForFirstTime = false;
        utilImageManipulate.translate();
     }
  }

   void interpolateMenuItem_actionPerformed(ActionEvent e) {

      if (!reSizeActivatedForFirstTime) {
        reSzFrameOpen = utilImageManipulate.frame2.isShowing();
        if (reSzFrameOpen) {
          if (utilImageManipulate.frame2.getState() ==
              utilImageManipulate.frame2.ICONIFIED) {
              utilImageManipulate.frame2.setState(utilImageManipulate.frame2.NORMAL);
          }
          else {
            utilImageManipulate.frame2.show();
          }
        }
      }
      // if activated for the first time OR if the window is closed
      if (!reSzFrameOpen) {
         reSizeActivatedForFirstTime = false;
         utilImageManipulate.interpolate();
      }
   }

   void contrastMenuItem_actionPerformed(ActionEvent e) {

      if (!contrastActivatedForFirstTime) {
        contrFrameOpen = utilImageManipulate.frame3.isShowing();
        if (contrFrameOpen) {
           if (utilImageManipulate.frame3.getState() ==
                             utilImageManipulate.frame3.ICONIFIED) {
               utilImageManipulate.frame3.setState(utilImageManipulate.frame3.NORMAL);
            }
            else {
               utilImageManipulate.frame3.show();
            }
         }
      }
        // if activated for the first time OR if the window is closed
      if (!contrFrameOpen) {
         contrastActivatedForFirstTime = false;
         utilImageManipulate.changeContrast();
      }
   }

   void windowMenuItem_actionPerformed(ActionEvent e) {
      // should not work on montaged images or on volumes
      if (montage) {
          this.windowMenuItem.setEnabled(false);
          this.powerMenuItem.setEnabled(false);
         return;
      }
      if (!windowFrameOpen) {
         // prevents image to be resized
         this.editMenu.setEnabled(false);
         windowFrameOpen = true;
         // close the resize etc.. windows
         utilImageManipulate.disposeWindows();
         utilGUIWindow = new UtilGUIWindow(this,wdp, spiderimage,
                                                     montage, "crop", utilParameters);
         // set location of the Window Operation Menu
         Point imageFrameOrigin = new Point();
         imageFrameOrigin    = this.getLocation();
         int imageFrameWidth = this.getWidth();
         int windowFrameXCoordinate = (imageFrameWidth +
                                               (int) imageFrameOrigin.getX());
         int windowFrameYCoordinate = (int) (imageFrameOrigin.getY());

         if ( (windowFrameXCoordinate + 500) > screenSize.width
                     || (windowFrameYCoordinate + 400) > screenSize.height) {
            windowFrameXCoordinate = 0;
            windowFrameYCoordinate = 0;
         }
         utilGUIWindow.setLocation(windowFrameXCoordinate, windowFrameYCoordinate);
      }
      else if (utilGUIWindow.getState() == utilGUIWindow.ICONIFIED) {
         utilGUIWindow.setState(utilGUIWindow.NORMAL);
      }
      else {
         utilGUIWindow.setVisible(true);
      }
   }

   void pixelMenuItem_actionPerformed(ActionEvent e) {
      if (!pixelFrameOpen) {
         pixelFrameOpen = true;
         // close the resize etc.. windows
         utilImageManipulate.disposeWindows();
         utilGUIPixel = new UtilGUIPixel(this,wdp, spiderimage, utilImageManipulate,
                                       montage, utilParameters);
         // set location of the Pixel Operation Menu
         Point imageFrameOrigin = new Point();
         imageFrameOrigin = this.getLocation();
         int imageFrameWidth = this.getWidth();
         int pixelFrameXCoordinate = (imageFrameWidth +
                                    (int) imageFrameOrigin.getX());
         int pixelFrameYCoordinate = (int) (imageFrameOrigin.getY());

         if ( (pixelFrameXCoordinate + 500) > screenSize.width
           || (pixelFrameYCoordinate + 400) > screenSize.height) {
            pixelFrameXCoordinate = 0;
            pixelFrameYCoordinate = 0;
         }
         utilGUIPixel.setLocation(pixelFrameXCoordinate, pixelFrameYCoordinate);
      }
      else if (utilGUIPixel.getState() == utilGUIPixel.ICONIFIED) {
         utilGUIPixel.setState(utilGUIPixel.NORMAL);
      }
      else {
         utilGUIPixel.setVisible(true);
      }
   }

   void annotateMenuItem_actionPerformed(ActionEvent e) {
      if(!annotateFrameOpen){
         annotateFrameOpen = true;
         utilGUIAnnotate = new UtilGUIAnnotate(wdp,utilImageManipulate,spiderimage, utilParameters);
      }
      else if (utilGUIAnnotate.getState() == utilGUIAnnotate.ICONIFIED) {
         utilGUIAnnotate.setState(utilGUIAnnotate.NORMAL);
      }
      else {
         utilGUIAnnotate.setVisible(true);
      }
   }

   void markerMenuItem_actionPerformed(ActionEvent e) {
      if(!markerFrameOpen){
         markerFrameOpen = true;
         utilGUIMarker = new UtilGUIMarker(wdp, utilImageManipulate, utilParameters);
      }
      else if (utilGUIMarker.getState() == utilGUIMarker.ICONIFIED) {
         utilGUIMarker.setState(utilGUIMarker.NORMAL);
      }
      else {
         utilGUIMarker.setVisible(true);
      }
   }
   void histogramMenuItem_actionPerformed(ActionEvent e) {
      if(!histogramFrameOpen){
         histogramFrameOpen = true;
         histogramFrame = new HistogramFrame(wdp,utilParameters);
      }
      else if (histogramFrame.getState() == histogramFrame.ICONIFIED) {
         histogramFrame.setState(histogramFrame.NORMAL);
      }
      else {
         histogramFrame.setVisible(true);
      }
   }

   void distanceMenuItem_actionPerformed(ActionEvent e) {
      if(!distanceFrameOpen){
         distanceFrameOpen = true;
         utilGUIDistance = new UtilGUIDistance(wdp, utilParameters);
      }
      else if (utilGUIDistance.getState() == utilGUIDistance.ICONIFIED) {
         utilGUIDistance.setState(utilGUIDistance.NORMAL);
      }
      else {
         utilGUIDistance.setVisible(true);
      }
   }

   public void maskMenuItem_actionPerformed(ActionEvent e) {
      if(!maskFrameOpen){
         maskFrameOpen = true;
         utilGUIMask = new UtilGUIMask(wdp, utilParameters);
      }
      else if (utilGUIMask.getState() == utilGUIMask.ICONIFIED) {
         utilGUIMask.setState(utilGUIMask.NORMAL);
      }
      else {
         utilGUIMask.setVisible(true);
      }
   }

   public void powerMenuItem_actionPerformed(ActionEvent e) {
      // should not work on montaged images or on volumes
      if (montage) {
         this.powerMenuItem.setEnabled(false);
         this.windowMenuItem.setEnabled(false);
         return;
      }

      if (!powerWinFrameOpen) {
         // prevents image to be resized
         this.editMenu.setEnabled(false);
         powerWinFrameOpen = true;
         // close the resize etc.. windows
         utilImageManipulate.disposeWindows();
         utilGUIPowerWindow = new UtilGUIWindow(this,wdp, spiderimage,
                                               montage, "power", utilParameters);
         // set location of the Window Operation Menu
         Point imageFrameOrigin = new Point();
         imageFrameOrigin    = this.getLocation();
         int imageFrameWidth = this.getWidth();
         int windowFrameXCoordinate = (imageFrameWidth +
                                         (int) imageFrameOrigin.getX());
         int windowFrameYCoordinate = (int) (imageFrameOrigin.getY());

         if ( (windowFrameXCoordinate + 500) > screenSize.width
               || (windowFrameYCoordinate + 400) > screenSize.height) {
            windowFrameXCoordinate = 0;
            windowFrameYCoordinate = 0;
         }
         utilGUIPowerWindow.setLocation(windowFrameXCoordinate, windowFrameYCoordinate);
      }
      else if (utilGUIWindow.getState() == utilGUIWindow.ICONIFIED) {
         utilGUIWindow.setState(utilGUIWindow.NORMAL);
      }
      else {
         utilGUIWindow.setVisible(true);
      }
   }

   public void disposeAuxiliaryWindows() {
      // if more than one window is opened by selecting the same function
      // then only the latest window is closed.
      utilImageManipulate.disposeWindows();
      if (windowFrameOpen == true) utilGUIWindow.dispose();
      if (pixelFrameOpen == true) utilGUIPixel.dispose();
      if (annotateFrameOpen == true) utilGUIAnnotate.dispose();
      if (markerFrameOpen == true) utilGUIMarker.dispose();
      if (histogramFrameOpen == true) histogramFrame.dispose();
      if (powerWinFrameOpen == true) {
          utilGUIPowerWindow.doWindowClosingTasks();
          utilGUIPowerWindow.dispose();
      }
   }

   // if "Close all" menu item in WebFrame is selected
   public void actionPerformed(ActionEvent evt) {
      // should set image # = 0 and Image co-ordinate to 40,40
      disposeAuxiliaryWindows();
      this.dispose();
   }

   public void keyPressed(KeyEvent evt)
   {
      int keyCode = evt.getKeyCode();
      if ((insidePanel == true) && (keyCode == KeyEvent.VK_ESCAPE)) {
         disposeAuxiliaryWindows();
         this.dispose();
      }
   }

   public void keyReleased(KeyEvent evt) {
   }

   public void keyTyped(KeyEvent evt) {
   }

   public void mouseEntered(MouseEvent e) {
      insidePanel = true;
   }

   public void mouseExited(MouseEvent e) {
      insidePanel = false ;
   }

   public void mousePressed(MouseEvent e) {
   }

   public void mouseReleased(MouseEvent e) {
   }

   public void mouseClicked(MouseEvent e) {
   }
}
