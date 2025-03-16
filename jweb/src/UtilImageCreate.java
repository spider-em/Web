package web;

import java.awt.*;
import javax.media.jai.*;
import java.awt.image.renderable.*;
import java.awt.image.*;

/**
 * Title:        WEB: A GUI for viewing and analyzing SPIDER images
 * Description:  WEB is a graphical user interface for viewing and analyzing SPIDER images.
 * Copyright:    Copyright (c) 2001
 * Company:      Health Research Inc.
 * @author Bimal K. Rath
 * @version 1.0
 */

public class UtilImageCreate {
   TiledImage montagedImage;
   UtilParameters utilParameters;

   public UtilImageCreate(UtilParameters utilParam) {
      utilParameters = utilParam;
  }

   // This method creates a montaged image given a number of image names in an array.
   // The output image's height and width are also given as input. Works for
   // Spider Images only. A tiled image from one of the series of images is created to
   // extract sample model and color model. All the images should have similar sample
   // model and color model. Output Montaged image will have similar sample model and
   // color model. Tile height and width of the small
   // images should be same and it will be preserved for the montaged image.
   // imagestack is true means images are of equal dimensions.
   // width (montagedImageWidth) and height (montagedImageHeight) should include
   // extrapixels needed for a blank space around the image.
   public TiledImage getMontagedImage(String[] fileNameArray, int width, int height,
                                             boolean imgstack, int numOfImgInRow) {
      try {
         String [] fileNames = fileNameArray;
         int montagedImageWidth = width;
         int montagedImageHeight = height;
         int numFiles = fileNames.length;
         // don't dump spider image header info onto the output area
         utilParameters.showHeaderInfo = false;
         // Make a tiled image from one of the series of the images
         SpiderImage spiImage = new SpiderImage(fileNames[0], utilParameters);
         TiledImage tempImage = spiImage.getImage(1);
         // reset showHeaderInfo to true
         utilParameters.showHeaderInfo = true;
         // create an empty output tiled image using the sample model and the
         // color model from the above created tiled image.
         montagedImage = new TiledImage(0,0,montagedImageWidth,
                                              montagedImageHeight,
                                              0,0,tempImage.getSampleModel(),
                                              tempImage.getColorModel());
         //initialization
         float xScale = 1;
         float yScale = 1;
         float xTranslation = 0;
         float yTranslation = 0;
         Interpolation interpolation = new InterpolationNearest();
         int newX = 0;
         int newY = 0;

         // loop over all small images
         for (int i =0; i < numFiles; i++) {
            String fileName = fileNames[i];
            SpiderImage spiderImage = new SpiderImage(fileName, utilParameters);
            TiledImage smallImage = spiderImage.getImage(1);

            // find the dimension and define region of interest. This is copied to the
            // montaged image at the right location.
            Rectangle bounds = null;
            bounds = smallImage.getBounds();
            bounds.setLocation(newX,newY);
            ROI roi = new ROIShape(bounds);

            // create the translated image that is to be copied
            ParameterBlock pb = new ParameterBlock();
            pb.addSource(smallImage);
            pb.add(xScale);
            pb.add(yScale);
            pb.add(xTranslation);
            pb.add(yTranslation);
            pb.add(interpolation);
            PlanarImage translatedSmallImage = (PlanarImage)JAI.create("scale",pb,null);
            // copy the image to the montaged image
            montagedImage.setData(translatedSmallImage.getData(),roi);

            if (imgstack == true) {
               int j = (i+1) % numOfImgInRow;
               int k = (i+1)/numOfImgInRow;
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
            else {
            // set translation for the next image
            xTranslation = xTranslation + bounds.width+1;
            newX = newX + bounds.width+1;
            newY = 0;
            }
         }
      }
      catch (Exception b) {
         System.err.println(b.getMessage());
      }
      return montagedImage;

      /*
      You may use the following setup for displaying slices of a volume.
      If you want to use tiles directly, you need to change the
      tile size of this combined image equal to the size of the
      individual images (note: not equal to the tile size of the
      small images rather to the size of the individual images. Because,
      the method RenderedImage.getData() destroys the tile setting of the
      small images and returns the whole image as a single tile.)
      You cann't use different sizes of images
      to combine them to one. To change the tile size of this
      combined image you need to use ImageLayout and Rendering hints.
      For small images you will have small tiles which might hinder
      performance.
      // Change the tile size of the already created tempMontagedImage
      ImageLayout imageLayout = new ImageLayout();
      imageLayout.setTileWidth(smallImage.getWidth());
      imageLayout.setTileHeight(smallImage.getHeight());

      RenderingHints hints = new RenderingHints(JAI.KEY_IMAGE_LAYOUT,
                                                               imageLayout);
      float a =1;
      float b =1;
      float c =0;
      float d =0;
      Interpolation interpolation = new InterpolationNearest();

      ParameterBlock pb = new ParameterBlock();
      pb.addSource(tempMontagedImage);
      pb.add(a);
      pb.add(b);
      pb.add(c);
      pb.add(d);
      pb.add(interpolation);
      PlanarImage montagedImage = (PlanarImage)JAI.create("scale",pb,hints);
      // I am not sure if you need to cast RenderedOp as plannar Image
      // or leave it as it is. You cann't cast it as TiledImage. You
      // need to try out.
      for (int ty = 0; ty < newTiledImage.getNumYTiles(); ty++) {
         for (int tx = 0; tx < newTiledImage.getNumXTiles(); tx++) {
            montagedImage.getWritableTile(tx,ty).
                                   setRect(smallImage[].getData());
         }
      }
      // Due to some reason, the above loop does not work. It only
      // does the work for tile (0,0) and returns null while executing
      // setRect(smallImage[].getData()) for the next tile. The following set up
      // may be used instead.

      for (int ty = 0; ty < montgedImage.getNumYTiles(); ty++) {
         for (int tx = 0; tx < montagedImage.getNumXTiles(); tx++) {
            float a =1;
            float b =1;
            float c =smallImageX;
            float d =smallImageY;
            Interpolation interpolation = new InterpolationNearest();

            ParameterBlock pb = new ParameterBlock();
            pb.addSource(smallImg);
            pb.add(a);
            pb.add(b);
            pb.add(c);
            pb.add(d);
            pb.add(interpolation);
            PlanarImage smallImgNew = (PlanarImage)JAI.create("scale",pb,null);
            montagedImage.getWritableTile(tx,ty).
            setDataElements(smallImageX,smallImageY,smallImgnew.getData());
         }
      }
      Note 2: you can also set the tile width and tile height of the
      comined image as follows: But the draw back is that you are
      assuming the sample model of the small images is same as the one
      you are creating in smTiletemp. I would rather prefer to use the
      method suggested above i.e. inherit the sample model and color model
      from the small image and then change the tile size.

      ColorModel  cm = smallImage.getColorModel();
      SampleModel sm = smallImage.getSampleModel();

      int[] band = {0};
      SampleModel smTileTemp =
                    new PixelInterleavedSampleModel(sm.getDataType(),
                                             bounds1.width,bounds1.height,1,
                                             bounds1.width,band);
      SampleModel smTile = smTileTemp.createCompatibleSampleModel
                                             (tileWidth,tileHeight);

      TiledImage montagedImage = new TiledImage(0,0,bounds1.width,
                                              bounds1.height,0,0,smTile,cm);

      Note 3: You may use overlay function in JAI to achieve the same.
      However, people had problem with band combining while using this
      this function.

      Note: 4: I think the ROI method that I have implemented looks good to
      me. Since it can handle images of same size and also diffrent size, it
      is a better choice. Only if the performance is poor, then I should explore
      more about other options.
      */
   }
   public TiledImage getTiledImageFromRenderedImage(RenderedImage renderedImage) {
      /* images with width less than 512 will have tile width = image width;
      images with width greater than 512 will have tile width = 512 pixels.
      This criteria applies to image height too.
      */
      RenderedImage sourceImage = renderedImage;
      int tileX = 0;
      int tileY = 0;
      if (sourceImage.getWidth() <= 512) {
         tileX = sourceImage.getWidth();
      }
      else {
         tileX = 512;
      }
      if (sourceImage.getHeight() <= 512) {
         tileY = sourceImage.getHeight();
      }
      else {
         tileY = 512;
      }
      // get sample model and color model from the source image
      SampleModel sampModel = sourceImage.getSampleModel().
                                 createCompatibleSampleModel(tileX,tileY);
      ColorModel colModel = sourceImage.getColorModel();
      // create the tiled image
      TiledImage  tilImage = new TiledImage(0,0,sourceImage.getWidth(),
                                          sourceImage.getHeight(),0,0,sampModel,
                                          colModel);
      tilImage.set(sourceImage);

      // Tiles from the tiledimage are forced to be loaded from the RenderedImaged
      for(int ty = 0; ty < tilImage.getNumYTiles(); ty++) {
         for(int tx = 0; tx < tilImage.getNumXTiles(); tx++) {
            tilImage.getTile(tx,ty);
         }
      }
      return tilImage;
   }

   public TiledImage getMontagedSlices(SpiderImage spiVol,
             String axisSelected, int columnsLVal, int columnsHVal,
                int rowsLVal, int rowsHVal, int slicesLVal, int slicesHVal,
                    int noOfImgInRow, int noOfSlices, int sliceWidth,
                        int sliceHeight, int montagedSlicesWidth,
                                                  int montagedSlicesHeight) {

      int volWidth  =  spiVol.getnsam();
      int volHeight =  spiVol.getnrow();
      int volSlice  =  spiVol.getnslice();

      // get 1st z - slice quickly to make a temp tiled image
      byte [] zSliceByte = spiVol.getByteData(spiVol.bpix, "z",
            1, volWidth, 1, volHeight, 1, 1, volWidth, volHeight);

      TiledImage zTiledImg = spiVol.makeTiledImageFromByteArray
                                           (zSliceByte,volWidth, volHeight);

      // create an empty output tiled image using the sample model and the
      // color model from the above created tiled image.
      TiledImage montagedSlices = new TiledImage(0,0,montagedSlicesWidth,
                                              montagedSlicesHeight,
                                              0,0,zTiledImg.getSampleModel(),
                                              zTiledImg.getColorModel());
      //initialization
      float xScale = 1;
      float yScale = 1;
      float xTranslation = 0;
      float yTranslation = 0;
      Interpolation interpolation = new InterpolationNearest();
      int newX = 0;
      int newY = 0;

      // loop over all small images
      for (int i = 0; i < noOfSlices; i++) {

         if (axisSelected == "x") {
            // keep incrementing after first loop
            if (i != 0) {
               columnsLVal = columnsLVal + 1;
            }
            columnsHVal = columnsLVal;
         }

         if (axisSelected == "y") {
            // keep incrementing after first loop
            if (i != 0) {
               rowsLVal = rowsLVal + 1;
            }
            rowsHVal = rowsLVal;
         }

         if (axisSelected == "z") {
            // keep incrementing after first loop
            if (i != 0) {
            slicesLVal = slicesLVal + 1;
            }
            slicesHVal = slicesLVal;
         }

         byte [] sliceBytes = spiVol.getByteData(spiVol.bpix,axisSelected ,
                            columnsLVal, columnsHVal, rowsLVal, rowsHVal,
                                slicesLVal, slicesHVal, volWidth, volHeight);

         TiledImage smallImage = spiVol.makeTiledImageFromByteArray
                                           (sliceBytes, sliceWidth, sliceHeight);

         // find the dimension and define region of interest. This is copied to the
         // montaged image at the right location.
         Rectangle bounds = null;
         bounds = smallImage.getBounds();
         bounds.setLocation(newX,newY);
         ROI roi = new ROIShape(bounds);

         // create the translated image that is to be copied
         ParameterBlock pb = new ParameterBlock();
         pb.addSource(smallImage);
         pb.add(xScale);
         pb.add(yScale);
         pb.add(xTranslation);
         pb.add(yTranslation);
         pb.add(interpolation);
         PlanarImage translatedSmallImage = (PlanarImage)JAI.create("scale",pb,null);

         // copy the image to the montaged image
         montagedSlices.setData(translatedSmallImage.getData(),roi);

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
      return montagedSlices;
   }
}

