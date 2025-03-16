package web;

import java.awt.*;
import java.awt.image.*;
import javax.swing.*;
import javax.media.jai.*;
import java.awt.geom.*;
import java.awt.event.*;

/**
 * Title:        WEB: A GUI for viewing and analyzing SPIDER images
 * Description:  WEB is a graphical user interface for viewing and analyzing SPIDER images.
 * Copyright:    Copyright (c) 2001
 * Company:      Health Research Inc.
 * @author Bimal K. Rath
 * @version 1.0
 */

public class WebDisplayPanel extends JPanel implements MouseMotionListener,
                                             KeyListener, MouseListener{
   TiledImage tiledImage;
   RenderedImage img, outImage;
   AffineTransform identityTransform = new AffineTransform();
   RenderedOp ct;
   public boolean drawString      = false;
   public boolean drawMarker      = false, drawRectMarker = false;
   public boolean drawRubberBandRect  = false;
   public boolean drawRubberBandLine  = false;
   public boolean drawPolygon  = false;
   public boolean drawFilledPolygon  = false;
   public int rubberBandX, rubberBandY, rubberBandWidth, rubberBandHeight;
   public int markerLength       = 10;
   private int maxNumberOfMarkers = 2000;
   private int maxNumberOfStrings = 2000;
   public Point[] markers         = new Point[maxNumberOfMarkers];
   public Point[] rectMarkers         = new Point[maxNumberOfMarkers];
   public int markerNumber        = 0, rectMarkerNumber = 0;
   public int stringNumber        = 0;
   public String [] strings       = new String [maxNumberOfStrings];
   public Point  [] stringPos     = new Point [maxNumberOfStrings];
   private int maxNumberOfPolyPoints = 2000;
   public int numPolyPoints       = 0;
   public int [] polyX            = new int [maxNumberOfPolyPoints];
   public int [] polyY            = new int [maxNumberOfPolyPoints];
   public Polygon polyObj;

   public WebDisplayPanel() {
   }

   public WebDisplayPanel(RenderedImage image) {
      super();
      img = image ;
      tiledImage = (TiledImage)image;
      // setPreferredSize is needed. Without it the scrolling function of the
      // images will not work.
      setPreferredSize(new Dimension(img.getWidth(), img.getHeight()));
      this.setBackground(Color.black);
      outImage = image;
   }

   // display method
   public void paintComponent(Graphics g) {
      super.paintComponent(g);
      if (outImage == null) System.out.println("outImage is null");
      if (outImage != null) {
         try {
            ((Graphics2D)g).drawRenderedImage(outImage,identityTransform);
            if (drawMarker == true) {
               g.setColor(Color.green);
               for (int i = 0; i < markerNumber; i++) {
                  paintMarker(g,i);
               }
            }
            if (drawRectMarker == true) {
               g.setColor(Color.green);
               for (int i = 0; i < rectMarkerNumber; i++) {
                  paintRectMarker(g,i);
               }
            }

            if (drawString == true) {
               Font font = new Font("SanSerif",Font.BOLD,18);
               g.setFont(font);
               g.setColor(Color.green);
               for (int i = 0; i < stringNumber; i++) {
                  paintString(g,i);
               }
            }
            if (drawRubberBandRect == true) {
               g.setColor(Color.green);
               drawRubbBandRect(g);
            }
            if (drawRubberBandLine == true) {
               g.setColor(Color.green);
               drawRubbBandLine(g);
            }
            if (drawPolygon == true) {
               g.setColor(Color.green);
               drawPoly(g);
            }
            if (drawFilledPolygon == true) {
              g.setColor(Color.green);
              drawFilledPoly(g);
           }

         }
         catch (OutOfMemoryError e) {
            System.out.println("out of memory error from paintComponent");
         }
      }
      /* For a buffered image we need to do like this.
         if (img != null)
         g.drawImage(img,0,0,null);
      */
   }

   // for overlay
   public void drawRubbBandRect(Graphics g) {
      g.drawRect(rubberBandX, rubberBandY, rubberBandWidth, rubberBandHeight);
   }
   public void drawRubbBandLine(Graphics g) {
      g.drawPolyline(polyX,polyY,numPolyPoints);
   }

   public void drawPoly(Graphics g) {
      polyObj = new Polygon(polyX,polyY,numPolyPoints);
      //g.drawPolygon(polyX,polyY,numPolyPoints);
      g.drawPolygon(polyObj);
   }

   public void drawFilledPoly(Graphics g) {
       polyObj = new Polygon(polyX,polyY,numPolyPoints);
       //g.fillPolygon(polyX,polyY,numPolyPoints);
       g.fillPolygon(polyObj);
   }

   // for overlay
   public void paintMarker(Graphics g, int i) {
//      g.drawRect(markers[i].x - markerLength/2,markers[i].y - markerLength/2,
//                       markerLength,markerLength );
     g.drawOval(markers[i].x - markerLength/2,markers[i].y - markerLength/2,
                                                   markerLength,markerLength );
   }

   // for overlay
   public void paintRectMarker(Graphics g, int i) {
      g.drawRect(rectMarkers[i].x - markerLength/2,rectMarkers[i].y
                                - markerLength/2, markerLength,markerLength );
   }

   // for overlay
   public void paintString(Graphics g, int i) {
      g.drawString(strings[i],  stringPos[i].x - strings[i].length(),
                                                                stringPos[i].y);
   }

  // this method is used to make the display fast for Buffered Image.
  // Read comments inside the code. This method is not currently used for
  // this purpose since SpiderImage doesnot return a BufferedImage anymore.
  // This method was used from UtilImageStatistics to get pixel values from
  // a byte image, however it is not used any more. This method is left alone
  // incase there is a need for it in the future. However, remember that in
  // SGI machines use of lookup table makes the graphics repaint()
  // extremely slow.
   public void makePlanarImage() {

      float [] table =new float[256] ;
      for (int j = 0; j <= 255; j++) {
         table[j] = (float)(j)/255.0F;
      }
      LookupTableJAI lookupTable = new LookupTableJAI(table);

      /* The following lines of code can also be used in lieu
         of ct = JAI.create("lookup",(RenderedImage)img,lookupTable);

         RenderedImageAdapter ria = new RenderedImageAdapter(img);
         ct = JAI.create("lookup",ria,lookupTable);
      */

      // Use this if img is of type BufferedImage
      // ct = JAI.create("lookup",(RenderedImage)img,lookupTable);

      ct = JAI.create("lookup",img,lookupTable);
      outImage = (PlanarImage)ct;

      /* The following line of code can also be used
         in lieu of outImage = (PlanarImage)ct;

         outImage = PlanarImage.wrapRenderedImage(ct);
      */

      /*  The following lines of code could be properly used to generate the
          PlanarImage needed for Display. However it is very slow. I don't
          know the exact reason. I think the use of a lookup table
          makes it quite fast. From the e-mail archieves of JAI interest group
          I got the impression that there is something funny about the gray scale
          color/sample model. There is no optimization done for grayScale images
          in paintComponent. It has been advised that it will be faster,
          if you use RGB model with same data copied to all RGB channels. However, this
          has the effect that the memory need becomes 3 times more, since you copy
          the Graycale value to RGB channels (3). Supposedly, this bug is fixed in
          JAVA 1.4. I need to try it though.

          outImage = PlanarImage.wrapRenderedImage(ria);
          OR
          outImage = PlanarImage.wrapRenderedImage((RenderedImage)img);
      */
   }

   //to draw onto the graphics
   public void drawMarkers(int x, int y) {
      // tiledImage.createGraphics().drawRect(x,y,10,10);
   }

   // Causes the focus to be on this panel for KeyStroke input. Keystrokes
   // will not be listened without the following method.
//   public boolean isFocusTraversable() {
    public boolean isFocusable() {
      return true;
   }

   public void keyPressed(KeyEvent e) {
   }
   public void keyReleased(KeyEvent e){
   }
   public void keyTyped(KeyEvent e)     {
   }
   public void mousePressed(MouseEvent e) {
   }
   public void mouseReleased(MouseEvent e) {
   }
   public void mouseMoved(MouseEvent e) {
   }
   public void mouseDragged(MouseEvent e) {
   }
   public void mouseClicked(MouseEvent e) {
   }
   public void mouseEntered(MouseEvent e) {
   }
   public void mouseExited(MouseEvent e) {
   }
}
