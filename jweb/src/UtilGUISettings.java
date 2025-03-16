package web;

import javax.swing.*;
import javax.swing.event.*;
import java.awt.*;
import java.awt.event.*;

/**
 * <p>Title: WEB: A GUI for viewing and analyzing SPIDER images</p>
 * <p>Description: WEB is a graphical user interface for viewing and analyzing SPIDER images.</p>
 * <p>Copyright: Copyright (c) 2001</p>
 * <p>Company: Health Research Inc.</p>
 * @author Bimal K. Rath
 * @version 1.0
 */

public class UtilGUISettings extends JFrame implements ActionListener {

   private UtilParameters utilParameters;
   private JLabel fileExtnLabel, resizeLabel, contrastLabel, lowContrastLabel,
                                                              highContrastLabel;
   private JTextField fileExtnInput ;
   private JSlider resizeSlider, lowContrastSlider, highContrastSlider;
   private JButton jButtonOK, jButtonCancel, jButtonApply;
   private JRadioButton resizeLargeRB, resizeSmallRB;
   private ButtonGroup buttonGroup;
   private int lowContrastSliderValue = 0, highContrastSliderValue = 255;
   final JTextField lowContrastTextField;
   final JTextField highContrastTextField;
   private String dataExtnStr ;
   private int lowConVal, highConVal;
   private float reSzFact;
   private boolean reSzSmall, reSzLarge;


   public UtilGUISettings(UtilParameters utilParam) {

      utilParameters = utilParam;
      setTitle("Global Settings Popup Menu");
      setSize(400,500);

      // Save current settings to be retrieved if Cancel button is pressed
      dataExtnStr = utilParameters.dataFileExtension;
      reSzFact   = utilParameters.resizeFactor;
      lowConVal  = utilParameters.contrastLowRamp;
      highConVal = utilParameters.contrastHighRamp ;
      reSzSmall  = utilParameters.resizeSmall;
      reSzLarge  = utilParameters.resizeLarge ;

      Container contentPane = getContentPane();
      GridBagLayout gridBagLayout = new GridBagLayout();
      contentPane.setLayout(gridBagLayout);
      GridBagConstraints gridBagConstraints = new GridBagConstraints();

      fileExtnLabel = new JLabel("File Extension : ");
      addComponent(contentPane,fileExtnLabel,gridBagConstraints,1,1,1,1);

      fileExtnInput = new JTextField(utilParameters.dataFileExtension,10);
      addComponent(contentPane,fileExtnInput,gridBagConstraints,3,1,2,1);

      JLabel dummyJLabel1 = new JLabel("     ");
      addComponent(contentPane,dummyJLabel1,gridBagConstraints,1,2,1,1);

      resizeLabel = new JLabel("Resize : ");
      addComponent(contentPane,resizeLabel,gridBagConstraints,1,3,1,1);

      resizeLargeRB = new JRadioButton("Large");
      if (utilParameters.resizeLarge) {
        resizeLargeRB.setSelected(true);
      }
      else {
        resizeLargeRB.setSelected(false);
      }
      addComponent(contentPane,resizeLargeRB,gridBagConstraints,2,3,1,1);

      // done this way so that close all button on main menu will execute
      // actionPerformed method defined in this class
      resizeLargeRB.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(ActionEvent e) {
            }
      });

      resizeSmallRB = new JRadioButton("Small");
      if (utilParameters.resizeSmall) {
        resizeSmallRB.setSelected(true);
      }
      else {
        resizeSmallRB.setSelected(false);
      }
      addComponent(contentPane,resizeSmallRB,gridBagConstraints,2,4,1,1);

      // done this way so that close all button on main menu will execute
      // actionPerformed method defined in this class
      resizeSmallRB.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(ActionEvent e) {
            }
      });

      buttonGroup = new ButtonGroup();
      buttonGroup.add(resizeSmallRB);
      buttonGroup.add(resizeLargeRB);

      resizeSlider = new JSlider(SwingConstants.HORIZONTAL,
                   10,101,(int)utilParameters.resizeFactor*10);
      resizeSlider.setMajorTickSpacing(20);
      resizeSlider.setMinorTickSpacing(10);
      resizeSlider.setPaintTicks(true);
      resizeSlider.setPaintLabels(false);
      addComponent(contentPane,resizeSlider,gridBagConstraints,3,3,1,2);

      final JTextField resizeTextField = new JTextField(4);
      resizeTextField.setEditable(false);
      // set the initial display
      resizeTextField.setText(""+utilParameters.resizeFactor);

      resizeSlider.addChangeListener(new ChangeListener() {
            public void stateChanged(ChangeEvent ce) {
               resizeTextField.setText(""+resizeSlider.getValue()/10.0);
            }
      });
      addComponent(contentPane,resizeTextField,gridBagConstraints,4,3,1,1);

      // dummyJLabel(s) are used to get GUI look nice
      JLabel dummyJLabel2 = new JLabel("     ");
      addComponent(contentPane,dummyJLabel2,gridBagConstraints,1,5,1,1);

      contrastLabel = new JLabel("Contrast: ");
      addComponent(contentPane,contrastLabel,gridBagConstraints,1,7,1,1);

      lowContrastLabel = new JLabel("Low Ramp ");
      addComponent(contentPane,lowContrastLabel,gridBagConstraints,2,7,1,1);

      lowContrastSlider = new JSlider(SwingConstants.HORIZONTAL,
                                      0,255,utilParameters.contrastLowRamp);
      lowContrastSlider.setMajorTickSpacing(50);
      lowContrastSlider.setMinorTickSpacing(25);
      lowContrastSlider.setPaintTicks(true);
      lowContrastSlider.setPaintLabels(true);
      addComponent(contentPane,lowContrastSlider,gridBagConstraints,3,7,1,2);

      lowContrastTextField = new JTextField(4);
      lowContrastTextField.setEditable(false);
      // set the initial display
      lowContrastTextField.setText(""+utilParameters.contrastLowRamp);

      lowContrastSlider.addChangeListener(new ChangeListener() {
            public void stateChanged(ChangeEvent ce) {
               lowContrastTextField.setText(""+lowContrastSlider.getValue());
               lowContrastSliderValue = lowContrastSlider.getValue();
               if(lowContrastSliderValue >= highContrastSliderValue) {
                  lowContrastTextField.setForeground(Color.red);
               }
               else {
                  lowContrastTextField.setForeground(Color.black);
                  highContrastTextField.setForeground(Color.black);
               }


            }
      });
      addComponent(contentPane,lowContrastTextField,gridBagConstraints,4,7,1,1);

      JLabel dummyJLabel3 = new JLabel("     ");
      addComponent(contentPane,dummyJLabel3,gridBagConstraints,1,8,1,1);

      highContrastLabel = new JLabel("High Ramp ");
      addComponent(contentPane,highContrastLabel,gridBagConstraints,2,9,1,1);

      highContrastSlider = new JSlider(SwingConstants.HORIZONTAL,
                                       0,255,utilParameters.contrastHighRamp);
      highContrastSlider.setMajorTickSpacing(50);
      highContrastSlider.setMinorTickSpacing(25);
      highContrastSlider.setPaintTicks(true);
      highContrastSlider.setPaintLabels(true);
      addComponent(contentPane,highContrastSlider,gridBagConstraints,3,9,1,2);

      highContrastTextField = new JTextField(4);
      highContrastTextField.setEditable(false);
      // set the initial display
      highContrastTextField.setText(""+utilParameters.contrastHighRamp);

      highContrastSlider.addChangeListener(new ChangeListener() {
            public void stateChanged(ChangeEvent ce) {
               highContrastSliderValue = highContrastSlider.getValue();
               highContrastTextField.setText(""+highContrastSlider.getValue());
               if(highContrastSliderValue <= lowContrastSliderValue) {
                 highContrastTextField.setForeground(Color.red);
               }
               else {
                  highContrastTextField.setForeground(Color.black);
                  lowContrastTextField.setForeground(Color.black);
               }
            }
      });
      addComponent(contentPane,highContrastTextField,gridBagConstraints,4,9,1,1);

      JLabel dummyJLabel4 = new JLabel("              ");
      addComponent(contentPane,dummyJLabel4,gridBagConstraints,6,10,1,1);
      JLabel dummyJLabel5 = new JLabel("              ");
      addComponent(contentPane,dummyJLabel5,gridBagConstraints,1,11,1,1);

      jButtonOK = new JButton("    OK    ");
      addComponent(contentPane,jButtonOK,gridBagConstraints,1,12,1,1);
      jButtonOK.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(ActionEvent e) {
               jButtonOK_actionPerformed(e);
            }
      });

      jButtonApply = new JButton("    Set    ");
      addComponent(contentPane,jButtonApply,gridBagConstraints,3,12,1,1);
      jButtonApply.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(ActionEvent e) {
               jButtonApply_actionPerformed(e);
            }
      });


      jButtonCancel = new JButton(" Cancel ");
      addComponent(contentPane,jButtonCancel,gridBagConstraints,5,12,2,1);
      jButtonCancel.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(ActionEvent e) {
               jButtonCancel_actionPerformed(e);
            }
      });

      pack();
      setVisible(true);
   }

   void jButtonOK_actionPerformed(ActionEvent e) {
      updateSettingsParameters();
      this.dispose();
   }

   void jButtonApply_actionPerformed(ActionEvent e) {
      updateSettingsParameters();
   }

   void jButtonCancel_actionPerformed(ActionEvent e) {
      revertSettingsParameters();
      this.dispose();
   }


   void revertSettingsParameters() {
      utilParameters.dataFileExtension = dataExtnStr;
      utilParameters.resizeFactor = reSzFact;
      utilParameters.contrastLowRamp = lowConVal;
      utilParameters.contrastHighRamp = highConVal;
      utilParameters.resizeSmall = reSzSmall;
      utilParameters.resizeLarge = reSzLarge;
   }

   void updateSettingsParameters() {

      if(highContrastSliderValue <= lowContrastSliderValue) {
         String str = "contrast low ramp is >= contrast high ramp !" + "\n";
         utilParameters.outputMessageTextArea.append(str);

      }
      else {
         // update dataFileExtension, resizeFactor, contrastLowRamp, contrastHighRamp
         // resizeSmall and resizeLarge in utilParameters
         utilParameters.dataFileExtension = fileExtnInput.getText().trim();
         utilParameters.resizeFactor = (float)(resizeSlider.getValue()/10.0);
         utilParameters.contrastLowRamp  = lowContrastSlider.getValue();
         utilParameters.contrastHighRamp = highContrastSlider.getValue();
         if (resizeLargeRB.isSelected()) {
            utilParameters.resizeLarge = true;
            utilParameters.resizeSmall = false;
         }
         if (resizeSmallRB.isSelected()) {
            utilParameters.resizeSmall = true;
            utilParameters.resizeLarge = false;
         }
      }
   }

   // if "Close all" menu item in WebFrame is selected close this frame
   public void actionPerformed(ActionEvent evt) {
      this.dispose();
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

}
