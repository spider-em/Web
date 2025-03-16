package web;


import javax.swing.*;
import javax.media.jai.*;
import java.awt.*;
import java.awt.*;
import javax.swing.*;
import java.awt.event.*;

/**
 * <p>Title: WEB: A GUI for viewing and analyzing SPIDER images</p>
 * <p>Description: WEB is a graphical user interface for viewing and analyzing SPIDER images.</p>
 * <p>Copyright: Copyright (c) 2001</p>
 * <p>Company: Health Research Inc.</p>
 * @author Bimal K. Rath
 * @version 1.0
 */

public class UtilGUIDendrogram extends JPanel implements ActionListener{

   UtilDoc utilDocument;
   UtilParameters utilParam;
   UtilPlotGraphPanel utilPlotGraphPanel;
   float [] imgNumbers;
   int noOfParticles;
   float [] heightVal;
   float thresholdHeight;
   int maxNoOfClass = 500;
   float [][] classInfoArray ;
   float [][] plotArray;
   int noOfClasses = 0;
   int [][] dendrogramClass;
   private JLabel templFile, thresNum;
   private String templFileName;
   private JTextField tmplFileInput, thresNumInput;
   private JCheckBox showAvgImg;
   String fileNameTempl;

   public UtilGUIDendrogram(UtilDoc utilDoc, UtilParameters utilParameters) {
      utilDocument = utilDoc;
      utilParam = utilParameters;



      GridBagLayout gridBagLayout = new GridBagLayout();
      this.setLayout(gridBagLayout);
      GridBagConstraints gridBagConstraints = new GridBagConstraints();



      thresNum = new JLabel("Threshold : ");
      addComponent(thresNum,gridBagConstraints,1,1,1,1,"LEFT");
      thresNumInput = new JTextField("0.0004",10);
      addComponent(thresNumInput,gridBagConstraints,2,1,2,1,"CENTER");

      templFile = new JLabel("Template File : ");
      addComponent(templFile,gridBagConstraints,1,2,2,1,"LEFT");
      templFile.setEnabled(false);

      // set a default template file name
      int index_1 = utilParameters.currDirectory.lastIndexOf("\\");
      String fileNameDir = utilParameters.currDirectory.substring(0,index_1+1);
      templFileName = fileNameDir+"img***.dat";
      tmplFileInput = new JTextField(templFileName,25);
      addComponent(tmplFileInput,gridBagConstraints,3,2,4,1,"LEFT");
      tmplFileInput.setEnabled(false);

      showAvgImg = new JCheckBox("Show Class Average Images");
      showAvgImg.setSelected(false);
      addComponent(showAvgImg,gridBagConstraints,1,3,3,1,"LEFT");
      showAvgImg.addActionListener(this);

      JButton jButtonShowDend = new JButton("Show Dendrogram");
      addComponent(jButtonShowDend,gridBagConstraints,2,4,2,1,"LEFT");

      jButtonShowDend.addActionListener(new java.awt.event.ActionListener() {
         public void actionPerformed(ActionEvent e) {
            jButtonShowDend_actionPerformed(e);
         }
      });







   }

   // reads document file and retrieves image numbers and heights into two
   // dirrent float arrays
   private void readDendDocFile() {

      boolean KeyDuplicationAllowed = false;
      int imgNumReg = 1;
      int heightReg = 2;
      imgNumbers = utilDocument.readDoc
                                      (imgNumReg, KeyDuplicationAllowed);

      heightVal = utilDocument.readDoc
                                      (heightReg, KeyDuplicationAllowed);
      makeClassArray (imgNumbers, heightVal, thresholdHeight, maxNoOfClass );
   }

   // makes an array of size (no. of classes  X 5) with the following information
   // ( Begining key #, End key #, class #, # of Particles in the class, Height)

   private void makeClassArray (float [] imgNumbers, float [] heightVal,
                                  float thresholdHeight, int maxNoOfClass) {

      noOfParticles = imgNumbers.length;
      classInfoArray = new float [maxNoOfClass][5];
      float begKeyNo = 1;
      float endKeyNo = noOfParticles;
      float classNo = 0;
      float htOfClass = -99;
      int rowNo = 0;
      float noOfPartInClass = 0;

      for (int i = 0; i < noOfParticles; i++) {


         if (heightVal[i] < thresholdHeight) {
            noOfPartInClass = noOfPartInClass + 1;
         }
         else {
            classNo = classNo + 1;
            if (classNo >= maxNoOfClass) {
             String str = "No. of Classes Exceed " +  maxNoOfClass;
             utilParam.outputMessageTextArea.append(str);
             return ;
            }
            htOfClass = heightVal[i];
            endKeyNo = i+1;
            rowNo = (int)classNo - 1 ;
            classInfoArray[rowNo][0] = begKeyNo ;
            classInfoArray[rowNo][1] = endKeyNo ;
            classInfoArray[rowNo][2] = classNo;
            classInfoArray[rowNo][3] = noOfPartInClass +1 ;
            classInfoArray[rowNo][4] = htOfClass;
            begKeyNo = i + 2;
            noOfPartInClass = 0;
         }
         noOfClasses = (int)classNo;
      }

   makePlotArray (classInfoArray, noOfClasses) ;
   }

   private void makePlotArray(float [][] classArray, float noOfCls) {
      int noRows = 4*((int)noOfCls - 1) + 2;
      int noColumns = 2;
      plotArray = new float [noRows][noColumns];

      int rowNum = 0;
      int nextI = 0;
      boolean foundNext = false;


      for (int i = 0; i < noOfCls; i ++) {
         foundNext = false;

         plotArray[rowNum][0] = classArray[i][2];
         plotArray[rowNum][1]= (float)0;

         rowNum= rowNum+1;

         plotArray[rowNum][0] = classArray[i][2];
         plotArray[rowNum][1] = classArray[i][4];
         rowNum = rowNum + 1;

         nextI = nextI + 1;

         for (int j = nextI; j < noOfCls; j++) {
            if ((classArray[j][4] >= classArray[i][4]) && ( i != (noOfCls -1))){
               plotArray[rowNum][0] = classArray[j][2];
               plotArray[rowNum][1] = classArray[i][4];
               rowNum = rowNum + 1;
               plotArray[rowNum][0] = classArray[j][2];
               plotArray[rowNum][1] = 0;
               rowNum = rowNum + 1;
               foundNext = true;
            }
            if (foundNext) break;
         }
      }
   }

   void makeDendroClass() {
      dendrogramClass = new int [noOfClasses][2];
      for (int i = 0; i < noOfClasses; i++) {
         dendrogramClass[i][0] = i;
         dendrogramClass[i][1] = (int)classInfoArray[i][3];
     }
   }

   void showClassAverage() {
      // not complete
      UtilFile utilFile = new UtilFile(utilParam);
//      int[] fileNums = new int[noOfClasses];
      int beginKeyForClass;
      int endKeyForClass;
      int[] fileNoArray;
      String[] fileNames;


      byte [] avgImg;
      int startIndex = 0;
      int imgWidth = 1, imgHeight = 1;
      byte [] stackedImagePixels;


      // find array length required to hold the class avg images
      // begin
      beginKeyForClass = (int)classInfoArray[0][0];
      int [] fileNoArrayTemp = new int [1];
      fileNoArrayTemp[0] = (int)imgNumbers[beginKeyForClass-1];
      fileNames = utilFile.makeFileNameArrayFromNumbers (fileNameTempl,fileNoArrayTemp);
      int numFiles = noOfClasses;
      utilParam.showHeaderInfo = false;
      //System.out.println("filename = " +fileNames[0] );
      SpiderImage spiTempImage = new SpiderImage(fileNames[0],utilParam);
      imgWidth = spiTempImage.getnsam();
      imgHeight = spiTempImage.getnrow();
      int stackedImageSize = imgWidth * imgHeight * numFiles;
      // show header info ("reset to default") of the spider images.
      utilParam.showHeaderInfo = true;
      stackedImagePixels = new byte[stackedImageSize];
      // end

      for (int i = 0; i < noOfClasses; i++) {
         beginKeyForClass = (int)classInfoArray[i][0];
         endKeyForClass = (int)classInfoArray[i][1];
         int k = 0;
         int fileNoArrayLen = (int)classInfoArray[i][3];
         fileNoArray = new int [fileNoArrayLen];

         for (int j = beginKeyForClass; j <= endKeyForClass; j++) {
           fileNoArray[k] = (int)imgNumbers[j-1];
           k = k+1;
         }
         fileNames = utilFile.makeFileNameArrayFromNumbers (fileNameTempl,fileNoArray);

         // find avg. of the images in byte array
         avgImg = utilFile.makeAnAverageFromImageSeries (fileNames);

         for (int j = 0; j < imgWidth * imgHeight; j++) {
            // copy individual image byte data into the array.
            // this large array holds data the same way a spider
            // volume data is stored
            stackedImagePixels [startIndex + j] = avgImg[j];
         }
         //the array index starts with 0. do necessary increment to the index
         startIndex = startIndex + (imgWidth * imgHeight) ;
      }
      // make a spider image(volume) from the byte array
      SpiderImage spiderVolume = new SpiderImage(stackedImagePixels,
                                   imgWidth,imgHeight,numFiles,utilParam);

      float scale = 1;
      int scrnWid = 0;
      int noOfSlices = noOfClasses;
      //int sliceWidth = 0;
      //int sliceHeight = 0;
      int noOfImgInRow = 0;
      String axisSelected = "z";
      int columnsLVal = 1, columnsHVal = imgWidth;
      int  rowsLVal = 1, rowsHVal = imgHeight;
      int slicesLVal = 1, slicesHVal = noOfClasses;

      // find scaling of the image
      if (utilParam.resizeLarge) {
         scale = utilParam.resizeFactor;
      }
      else {
         scale = (1/utilParam.resizeFactor);
      }
      scrnWid = utilParam.screenDimension.width - 50;
      // reset the screen width as per the image scaling
      // the image will be scaled in ImageFrame.java
      scrnWid = (int)(scrnWid/scale) ;

      noOfImgInRow = scrnWid / imgWidth;
      if (noOfImgInRow == 0) noOfImgInRow = 1;
      if (noOfSlices <= noOfImgInRow) noOfImgInRow = noOfSlices;
      // add noOfImgInRow -1 pixels for a blank pixel after each image
      int montagedSlicesWidth = noOfImgInRow * imgWidth + noOfImgInRow - 1;

      int k = noOfSlices / noOfImgInRow;
      int l = noOfSlices % noOfImgInRow;
      ImageFrame imageFrame;
      if (l != 0) k = k + 1;
      int noOfRows = k;
      // add k-1 pixels for a blank pixel after each image
      int montagedSlicesHeight = k * imgHeight + k - 1;

      UtilImageCreate utilImageCreate = new UtilImageCreate(utilParam);
      TiledImage montagedSlices;
      montagedSlices = utilImageCreate.getMontagedSlices(spiderVolume,
                   axisSelected, columnsLVal, columnsHVal, rowsLVal, rowsHVal,
                    slicesLVal, slicesHVal, noOfImgInRow, noOfSlices, imgWidth,
                          imgHeight, montagedSlicesWidth, montagedSlicesHeight);

      imageFrame = new ImageFrame(montagedSlices, utilParam);
      //jButtonCloseAll.addActionListener(imageFrame);
      imageFrame.setTitle("Class Averages");
      imageFrame.setLocation(50, 50);
      imageFrame.setVisible(true);
      imageFrame.wdp.drawString = true;
      imageFrame.wdp.stringNumber = noOfSlices;
      for (int i = 0; i <noOfSlices; i++) {
         imageFrame.wdp.strings[i] = Float.toString(classInfoArray[i][3]);
      }
      for (int j = 0; j <  noOfRows; j++) {
         for (int m = 0; m < noOfImgInRow; m++) {
            imageFrame.wdp.stringPos[m+j*noOfImgInRow] =
               new Point((imgWidth/2 )+(m*imgWidth),
                                         (imgHeight*(j+1)-10));
         }
      }
      imageFrame.wdp.repaint();
      //utilParametersVolume.montageXYZShown = true;

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
   void jButtonShowDend_actionPerformed(ActionEvent e) {

      thresholdHeight = Float.parseFloat(thresNumInput.getText().trim());
      readDendDocFile();
      //makeClassArray (imgNumbers, heightVal, thresholdHeight, maxNoOfClass );
      //makePlotArray (classInfoArray, noOfClasses) ;
      makeDendroClass();

      int margin = 70;
      // usable space from jframe

      JFrame dendogramFrame = new JFrame();
      dendogramFrame.setSize(500,500);
      dendogramFrame.setLocation(200,200);
      int frameSizeX = dendogramFrame.getWidth();
      int frameSizeY = dendogramFrame.getHeight();
      int noOfXTicks = noOfClasses;
      int noOfYTicks = 10;

      String graphType = "dendrogram";
      utilPlotGraphPanel = new UtilPlotGraphPanel
                   (plotArray, margin,frameSizeX, frameSizeY, noOfXTicks,
                                            noOfYTicks, graphType, utilParam);
      utilPlotGraphPanel.plotXAxis = false;
      utilPlotGraphPanel.dendroClass= dendrogramClass;

      dendogramFrame.getContentPane().add(utilPlotGraphPanel);
      String frameTitle = "Dendrogram:   "+utilParam.currDirectory;
      dendogramFrame.setTitle(frameTitle);
      dendogramFrame.setVisible(true);
      if (showAvgImg.isSelected()) {
         fileNameTempl  = tmplFileInput.getText().trim();
         showClassAverage();
      }
   }
   public void actionPerformed(ActionEvent e){
      if (showAvgImg.isSelected()) {
         templFile.setEnabled(true);
         tmplFileInput.setEnabled(true);
      }
      else {
         templFile.setEnabled(false);
         tmplFileInput.setEnabled(false);
      }
   }
}



