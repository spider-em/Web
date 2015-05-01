/*
C+**************************************************************************
C
C profile_cb.c                  DEC 92
C
C **************************************************************************
C    AUTHOR:  Mahieddine Ladjadj
C *  COPYRIGHT (C)1991, WADSWORTH CENTER FOR LABORATORIES AND             *
C *  RESEARCH, NEW YORK STATE DEPARTMENT OF HEALTH, ALBANY, NY 12201.      *
C *    THE CONTENTS OF THIS DOCUMENT ARE PROPRIETARY TO THE CENTER FOR     *
C *    LABORATORIES AND RESEARCH AND ARE NOT TO BE DISCLOSED TO OTHERS OR  *
C *    USED FOR PURPOSES OTHER THAN INTENDED WITHOUT WRITTEN APPROVAL OF   *
C *    THE CENTER FOR LABORATORIES AND RESEARCH                            *
C **************************************************************************
C
C profile_cb            runs when a toggle is pushed to
C                      change value of toggled variable
C
C PARAMETERS:    
C
C--*********************************************************************
*/

#include <Xm/ToggleBG.h>

 /********************  profile_cb   ****************************/

 void  profile_cb(Widget iw_temp, int *data, caddr_t call_data)
 {
   *data = XmToggleButtonGadgetGetState(iw_temp); 
 }

