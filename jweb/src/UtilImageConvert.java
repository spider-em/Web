package web;

import javax.swing.*;
import java.awt.image.*;
import java.io.*;
import javax.media.jai.*;
import java.util.*;

/**
 * Title:        WEB: A GUI for viewing and analyzing SPIDER images
 * Description:  WEB is a graphical user interface for viewing and analyzing SPIDER images.
 * Copyright:    Copyright (c) 2001
 * Company:      Health Research Inc.
 * @author Bimal K. Rath
 * @version 1.0
 */

public class UtilImageConvert extends JFrame{

   private WebDisplayPanel webPanel;
   private String fileFormat;

   public UtilImageConvert() {
   }

   public UtilImageConvert(WebDisplayPanel webDisplayPanel, String fileType,
                                   UtilParameters utilParameters) {
      webPanel = webDisplayPanel;
      fileFormat = fileType;

      DataOutputStream  dout;

      RenderedImage sourceImage = webPanel.img;
//    Will save the original image not the manipulated image with
//    (contrast, interpolated etc.. changed)

      JFileChooser jFileChooser1 = new JFileChooser();
      jFileChooser1.setCurrentDirectory(new File(utilParameters.currDirectory));
      jFileChooser1.setFileSelectionMode(JFileChooser.FILES_ONLY);
      if (JFileChooser.APPROVE_OPTION == jFileChooser1.showSaveDialog(this)) {
         String fileName = jFileChooser1.getSelectedFile().getPath();
         if (fileFormat == "spider" || fileFormat == "raw") {
            try {
               dout = new DataOutputStream
                    (new BufferedOutputStream(new FileOutputStream(fileName)));
               int wid = sourceImage.getWidth();
               int ht =   sourceImage.getHeight();
               int length= wid*ht;
               if (fileFormat == "spider") {
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
                  for (int i = 0; i < 211; i++) {
                      headerArrayOne[i] = 0;
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

                  for (int i = 0; i < 211; i++) {
                     dout.writeFloat(headerArrayOne[i]);
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

                  for (int i = 217; i <= labbyt/4; i++) {
                     dout.writeFloat(0);
                  }
               }
               Raster ras = sourceImage.getData();
               float [] dataFloat = new float [length];
               ras.getPixels(0,0,wid,ht,dataFloat);
               for (int i = 0; i < length; i++) {
                  dout.writeFloat(dataFloat[i]);
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

         else if (fileFormat == "bmp") {
            JAI.create("filestore",sourceImage,fileName,"bmp",null);
         }
         else if (fileFormat == "jpeg") {
            JAI.create("filestore",sourceImage,fileName,"jpeg",null);
         }
         else if (fileFormat == "png") {
            JAI.create("filestore",sourceImage,fileName,"png",null);
         }
         else if (fileFormat == "pnm") {
            JAI.create("filestore",sourceImage,fileName,"pnm",null);
         }
         else if (fileFormat == "tiff") {
            JAI.create("filestore",sourceImage,fileName,"tiff",null);
         }
         else {
         }
      }
   }
}