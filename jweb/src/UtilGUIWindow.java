package web;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import java.io.*;
import javax.media.jai.*;
import java.awt.image.renderable.*;
import javax.media.jai.operator.DFTDescriptor;



/**
 * Title:        WEB: A GUI for viewing and analyzing SPIDER images
 * Description:  WEB is a graphical user interface for viewing and analyzing SPIDER images.
 * Copyright:    Copyright (c) 2001
 * Company:      Health Research Inc.
 * @author Bimal K. Rath
 * @version 1.0
 */

public class UtilGUIWindow extends JFrame implements ActionListener,
                                          MouseMotionListener, MouseListener{

   private JLabel     windowedFile, xCoordinate, yCoordinate, columns, rows;
   private JTextField windowedFileInput, xCoordinateInput,
                                  yCoordinateInput, columnsInput, rowsInput;
   private JRadioButton interactiveRadioButton, fixedSizeRadioButton,
                                              specifiedCoordinateRadioButton;
   private ButtonGroup windowButtonGroup;
   JButton jButtonSaveWindowedFile;
   private WebDisplayPanel wdp;
   private ImageFrame imageFrame;
   private SpiderImage spiderImage;
   private UtilParameters utilParameters;
   private String docName, op ;
   private int yCoordinateValue, xCoordinateValue, columnsValue, rowsValue;
   private int posX, posY;
   private float [] floatData;
   private boolean firstMouseClick = true;
   private boolean powSpecFrmActivatedForFirstTime = true;
   private boolean powerSpecFrameOpen = false;
   private ImageFrame powerSpecFrame;


   public UtilGUIWindow(ImageFrame imgFr, WebDisplayPanel panel,
             SpiderImage spiImage, boolean montage, String operation,
                                                  UtilParameters utilParam) {

      op = operation;
      utilParameters = utilParam;
      imageFrame = imgFr;
      wdp = panel;
      spiderImage = spiImage;
      if (!montage) {
         // if single image read Spider raw data and save in float [] spiderImage.fpix.
         try {
            spiderImage.readRawData();
         }
         catch (IOException e) {
            String str = "Error reading SpiderImage raw data: UtilGUIWindow " + e + "\n";
            utilParameters.outputMessageTextArea.append(str);
         }
      }
      if (op.compareTo("power") == 0) {
         setTitle("Power Spectrum Popup Menu");
      }
      else {
         setTitle("Window Popup Menu");
      }

      setSize(400,500);

      Container contentPane = getContentPane();
      GridBagLayout gridBagLayout = new GridBagLayout();
      contentPane.setLayout(gridBagLayout);
      GridBagConstraints gridBagConstraints = new GridBagConstraints();

      windowedFile = new JLabel("Windowed File Name: ");
      if (op.compareTo("crop") == 0) {
         addComponent(contentPane,windowedFile,gridBagConstraints,1,1,1,1);
      }

      int index_1 = utilParameters.currDirectory.lastIndexOf(".");
      String fileNameWOExtension = utilParameters.currDirectory.substring(0,index_1);

      windowedFileInput = new JTextField(fileNameWOExtension+".win",15);
      if (op.compareTo("crop") == 0) {
         addComponent(contentPane,windowedFileInput,gridBagConstraints,2,1,2,1);
      }
      xCoordinate = new JLabel("X Coordinate (in Pixels) : ");
      addComponent(contentPane,xCoordinate,gridBagConstraints,1,2,1,1);

      xCoordinateInput = new JTextField("1",5);
      addComponent(contentPane,xCoordinateInput,gridBagConstraints,2,2,1,1);

      yCoordinate = new JLabel("Y Coordinate (in Pixels) : ");
      addComponent(contentPane,yCoordinate,gridBagConstraints,3,2,1,1);

      yCoordinateInput = new JTextField("1",5);
      addComponent(contentPane,yCoordinateInput,gridBagConstraints,4,2,1,1);

      columns = new JLabel("Columns (in Pixels) : ");
      addComponent(contentPane,columns,gridBagConstraints,1,3,1,1);

      columnsInput = new JTextField("3",5);
      addComponent(contentPane,columnsInput,gridBagConstraints,2,3,1,1);

      rows = new JLabel("Rows (in Pixels) : ");
      addComponent(contentPane,rows,gridBagConstraints,3,3,1,1);

      rowsInput = new JTextField("4",5);
      addComponent(contentPane,rowsInput,gridBagConstraints,4,3,1,1);

      interactiveRadioButton = new JRadioButton("Interactive");
      interactiveRadioButton.setSelected(true);
      addComponent(contentPane,interactiveRadioButton,gridBagConstraints,1,4,1,1);
      interactiveRadioButton.addActionListener(this);

      fixedSizeRadioButton = new JRadioButton("Fixed Size");
      fixedSizeRadioButton.setSelected(false);
      addComponent(contentPane,fixedSizeRadioButton,gridBagConstraints,1,5,1,1);
      fixedSizeRadioButton.addActionListener(this);
      if (montage) fixedSizeRadioButton.setEnabled(false);

      specifiedCoordinateRadioButton = new JRadioButton("Specified Coordinate");
      specifiedCoordinateRadioButton.setSelected(false);
      addComponent(contentPane,specifiedCoordinateRadioButton,
                                                   gridBagConstraints,1,6,1,1);
      specifiedCoordinateRadioButton.addActionListener(this);

      windowButtonGroup = new ButtonGroup();
      windowButtonGroup.add(interactiveRadioButton);
      windowButtonGroup.add(fixedSizeRadioButton);
      windowButtonGroup.add(specifiedCoordinateRadioButton);

      jButtonSaveWindowedFile = new JButton("  Save Windowed File  ");
      if (op.compareTo("crop") == 0) {
         addComponent(contentPane,jButtonSaveWindowedFile,gridBagConstraints,3,6,1,1);
      }
      jButtonSaveWindowedFile.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            jButtonSaveWindowedFile_actionPerformed(e);
         }
      });
      setFirstSet(false);
      setSecondSet(false);

      wdp.addMouseMotionListener(this);
      wdp.addMouseListener(this);

      pack();
      setVisible(true);
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
      gridBagConstraints.fill = GridBagConstraints.BOTH;
      contentPane.add(comp,gridBagConstraints);
   }

   public void setFirstSet(boolean bool) {
      xCoordinate.setEnabled(bool);
      xCoordinateInput.setEnabled(bool);
      yCoordinate.setEnabled(bool);
      yCoordinateInput.setEnabled(bool);
   }

   public void setSecondSet(boolean bool) {
      columns.setEnabled(bool);
      columnsInput.setEnabled(bool);
      rows.setEnabled(bool);
      rowsInput.setEnabled(bool);
   }

   public void actionPerformed(ActionEvent e){

      if (interactiveRadioButton.isSelected()){
         setFirstSet(false);
         setSecondSet(false);
         // reset
         firstMouseClick = true;
         wdp.drawRubberBandRect = false;

      }
      else if (fixedSizeRadioButton.isSelected()){
         setFirstSet(false);
         setSecondSet(true);
         // reset
         firstMouseClick = true;
         wdp.drawRubberBandRect = false;
      }
      // Specified Coordinate
      else {
         setFirstSet(true);
         setSecondSet(true);
         // reset
         firstMouseClick = true;
         wdp.drawRubberBandRect = false;
      }
   }

   // stop the mouse listener
   protected void processWindowEvent(WindowEvent e) {
      super.processWindowEvent(e);
      if (e.getID() == WindowEvent.WINDOW_CLOSING) {
         doWindowClosingTasks();
      }
   }

   public void doWindowClosingTasks() {
      wdp.removeMouseMotionListener(this);
      wdp.removeMouseListener(this);
      imageFrame.editMenu.setEnabled(true);
      wdp.repaint();
      imageFrame.windowFrameOpen = false;
      imageFrame.powerWinFrameOpen = false;
      if (powerSpecFrameOpen) {
         powerSpecFrame.dispose();
      }
   }

   public void mouseMoved(MouseEvent e) {

      if ((wdp.drawRubberBandRect == true) && (firstMouseClick == false)) {
         Point point = e.getPoint();
         int x = (int) point.getX();
         int y = (int) point.getY();
         // if mouse is inside the image
         if ( (x < wdp.outImage.getWidth()) && (y < wdp.outImage.getHeight())) {
            if(interactiveRadioButton.isSelected()){
               if (x > posX) {
                  wdp.rubberBandX = posX;
                  wdp.rubberBandWidth = x - posX;
               }
               else {
                  wdp.rubberBandX = x;
                  wdp.rubberBandWidth = posX - x;
               }
               if (y > posY) {
                  wdp.rubberBandY = posY;
                  wdp.rubberBandHeight = y - posY;
               }
               else {
                  wdp.rubberBandY = y;
                  wdp.rubberBandHeight = posY - y;
               }
               wdp.repaint();
            }
            else if (fixedSizeRadioButton.isSelected()) {
               inputUpdate();
               wdp.rubberBandX = x;
               wdp.rubberBandY = y;
               wdp.rubberBandWidth  = columnsValue;
               wdp.rubberBandHeight = rowsValue;
               wdp.repaint();
            }
         }
      }
   }
   public void mousePressed(MouseEvent e) {
      if(!this.isShowing()) return;

      posX = e.getX();
      posY = e.getY();
      // if mouse is inside the image
      if ((posX < wdp.outImage.getWidth()) && (posY < wdp.outImage.getHeight())) {

         // first mouse click: enable rubberband drawing
         if (firstMouseClick == true) {
            wdp.drawRubberBandRect = true;
            firstMouseClick = false;
            if (fixedSizeRadioButton.isSelected()) {
               inputUpdate();
               wdp.rubberBandX = posX;
               wdp.rubberBandY = posY;
               wdp.rubberBandWidth  = columnsValue;
               wdp.rubberBandHeight = rowsValue;
               wdp.repaint();
            }
         }
         // second mouse click: disenable rubberband drawing
         else {
            firstMouseClick = true;
            if (op.compareTo("power") == 0) {
               showPowerSpectrum();
            }
         }
         if (specifiedCoordinateRadioButton.isSelected()) {
            wdp.drawRubberBandRect = true;
            inputUpdate();
            wdp.rubberBandX = xCoordinateValue;
            wdp.rubberBandY = yCoordinateValue;
            wdp.rubberBandWidth  = columnsValue;
            wdp.rubberBandHeight = rowsValue;
            wdp.repaint();
         }
      }
   }

   public void inputUpdate() {
      docName  = windowedFileInput.getText().trim();
      xCoordinateValue  = Integer.parseInt(xCoordinateInput.getText().trim());
      yCoordinateValue = Integer.parseInt(yCoordinateInput.getText().trim());
      columnsValue = Integer.parseInt(columnsInput.getText().trim());
      rowsValue = Integer.parseInt(rowsInput.getText().trim());
   }

   void jButtonSaveWindowedFile_actionPerformed(ActionEvent e) {

      inputUpdate();
      int nslice = 1;
      int nsam = wdp.rubberBandWidth;
      int nrow = wdp.rubberBandHeight;
      int iform = 1;
      int imami = 0;
      float fmax = 0;
      float fmin = 0;
      float avg = 0;
      float sig = 0;
      String sliceAxis = "z";
      int colLowVal = wdp.rubberBandX;
      int colHighVal = wdp.rubberBandX + nsam - 1;
      int rowLowVal = wdp.rubberBandY;
      int rowHighVal = wdp.rubberBandY + nrow -1;
      int sliceLowVal = 1;
      int sliceHighVal = 1;
      int volWidth = spiderImage.getnsam();
      int volHeight = spiderImage.getnrow();
      floatData = spiderImage.getFloatData(spiderImage.fpix,
                      sliceAxis,colLowVal,colHighVal,rowLowVal,rowHighVal,
                                    sliceLowVal,sliceHighVal,volWidth,volHeight);
      UtilFile utilFile = new UtilFile(utilParameters);
      try {
          utilFile.writeSpiderFile(docName, floatData,
                         nsam, nrow, nslice, imami, avg, sig, iform, fmax, fmin);
      }
      catch (IOException eoe) {
         String str = "Error writting SpiderImage: UtilGUIWindow " + eoe + "\n";
         utilParameters.outputMessageTextArea.append(str);
      }
   }

   public float [] getWinDataFloat() {
      inputUpdate();
      int nsam = wdp.rubberBandWidth;
      int nrow = wdp.rubberBandHeight;
      String sliceAxis = "z";
      int colLowVal = wdp.rubberBandX;
      int colHighVal = wdp.rubberBandX + nsam - 1;
      int rowLowVal = wdp.rubberBandY;
      int rowHighVal = wdp.rubberBandY + nrow -1;
      int sliceLowVal = 1;
      int sliceHighVal = 1;
      int volWidth = spiderImage.getnsam();
      int volHeight = spiderImage.getnrow();
      floatData = spiderImage.getFloatData(spiderImage.fpix,
                      sliceAxis,colLowVal,colHighVal,rowLowVal,rowHighVal,
                                    sliceLowVal,sliceHighVal,volWidth,volHeight);
      return floatData;
   }

   public int getNsam(){
      int nsam = wdp.rubberBandWidth;
      return nsam;
   }

   public int getNrow() {
      int nrow = wdp.rubberBandHeight;
      return nrow;
   }

   public void showPowerSpectrum() {

      float [] dataFloat = getWinDataFloat();
      int dataSize = dataFloat.length;
      double nmin = 0.0;
      double nmax = 255.0;
      double fcval, fncon;
      float pval;

      // sets fmin and fmax
      float fmin = dataFloat[0];
      float fmax = dataFloat[0];

      for (int i = 0; i<dataSize; i++){
         // resets fmin and fmax
         if (dataFloat[i] < fmin) fmin = dataFloat[i];
         if (dataFloat[i] > fmax) fmax = dataFloat[i];
      }

      fcval = (nmax - nmin) / (fmax - fmin);
      fncon = nmin - (fmin * fcval) ;

      byte [] dataByte = new byte [dataSize];
      for (int i = 0; i < dataSize; i++) {
         pval    = (int) (fcval * dataFloat[i] + fncon);
         dataByte[i] = (byte)pval;
      }

      int nsam = getNsam();
      int nrow = getNrow();
      TiledImage tilImg = spiderImage.makeTiledImageFromByteArray(dataByte,nsam,nrow) ;

      ParameterBlock pb = new ParameterBlock();
      pb.addSource(tilImg).add(DFTDescriptor.SCALING_DIMENSIONS);
      PlanarImage dft = JAI.create("dft", pb, null);
      pb = new ParameterBlock();
      pb.addSource(dft);
      PlanarImage powerSpectrum = JAI.create("magnitude",pb,null);

      int x_trans = 0;
      int y_trans = 0;
      int product = 2;
      while (product <= 100000) {
         product = 2 * product;
         if (product > nsam) {
            x_trans = product/2;
            break;
         }
      }
      product = 2;
      while (product <= 100000) {
         product = 2 * product;
         if (product > nrow) {
            y_trans = product/2;
            break;
         }
      }
      if ((x_trans < nsam) && (y_trans < nrow)) {
         pb = new ParameterBlock();
         pb.addSource(powerSpectrum);
         pb.add(x_trans);
         pb.add(y_trans);
         PlanarImage powerSpecShft = JAI.create("PeriodicShift", pb, null);

         int xLeft = x_trans - (nsam / 2);
         int yLeft = y_trans - (nrow / 2);

         pb = new ParameterBlock();
         pb.addSource(powerSpecShft);
         pb.add( (float) xLeft);
         pb.add( (float) yLeft);
         pb.add( (float) nsam);
         pb.add( (float) nrow);

         PlanarImage powerSpecShftCrop = JAI.create("crop", pb, null);
         pb = new ParameterBlock();
         pb.addSource(powerSpecShftCrop);
         pb.add( (float) - xLeft);
         pb.add( (float) - yLeft);
         PlanarImage powerSpecShftCropShft = JAI.create("translate", pb, null);

         UtilImageCreate utilImageCreate = new UtilImageCreate(utilParameters);
         TiledImage tilImageCropShft = utilImageCreate.
            getTiledImageFromRenderedImage(powerSpecShftCropShft);
         if (!powSpecFrmActivatedForFirstTime) {
            powerSpecFrame.dispose();
         }

         // if activated for the first time OR if the window is closed
         if (tilImageCropShft != null) {
            powerSpecFrame = new ImageFrame(tilImageCropShft, utilParameters);
            powerSpecFrame.setTitle(" Power Spectrum ");
            powerSpecFrame.setLocation(50, 50);
            powerSpecFrame.setVisible(true);
            powerSpecFrameOpen = true;
            powSpecFrmActivatedForFirstTime = false;
         }
         else {
            String str = "null image!\n ";
            utilParameters.outputMessageTextArea.setText(str);
         }
      }
      else {
         String str = "periodic shift values greater than or equal to image dimension ! ";
         utilParameters.outputMessageTextArea.setText(str);
      }
   }

   public void mouseDragged(MouseEvent e) {
   }
   public void mouseClicked(MouseEvent e) {
   }
   public void mouseReleased(MouseEvent e) {
   }
   public void mouseEntered(MouseEvent e) {
   }
   public void mouseExited(MouseEvent e) {
   }
}

