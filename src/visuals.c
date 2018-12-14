
/*$Header: /usr16/software/web/src/RCS/visuals.c,v 1.3 2018/12/07 17:03:34 leith Exp $*/
#include "std.h"
#include "x.h"
#include "routines.h"

 void visuals(Display *idispl, int iscreen)
 {

 XVisualInfo   vTemplate;
 XVisualInfo   *visualList;
 int           visualsMatched;
 int           class, i, number;


 vTemplate.screen = iscreen;

 visualList = XGetVisualInfo(idispl,VisualScreenMask,&vTemplate,
                             &visualsMatched);
 if (visualsMatched == 0)
     {
     printf("no matching visuals \n");
     return;
     }

 printf("For screen: %d   Matching visuals: %d ------- \n",
         iscreen,visualsMatched);

 for (i=0; i<visualsMatched; i++)
   {
   class  = visualList[i].class;

   if (class == DirectColor)
      {
      printf("DirectColor: %3d, Depth: %3d,  Map: %5d  %8p \n",
        class, visualList[i].depth, visualList[i].colormap_size,
        (void *)visualList[i].visual);
      }
   else if (class == GrayScale)
      {
      printf("GreyScale    %3d, Depth: %3d,  Map: %5d  %8p \n",
        class, visualList[i].depth ,visualList[i].colormap_size,
        (void *)visualList[i].visual);
      }
   else if (class == PseudoColor)
      {
      printf("PseudoColor: %3d, Depth: %3d,  Map: %5d  %8p \n",
         class, visualList[i].depth ,visualList[i].colormap_size,
         (void *)visualList[i].visual);;
      }
   else if (class == StaticColor)
      {
      printf("StaticColor: %3d, Depth: %3d,  Map: %5d  %8p \n",
         class, visualList[i].depth, visualList[i].colormap_size,
         (void *)visualList[i].visual);
      }
   else if (class == StaticGray)
      {
      printf("StaticGray:  %3d, Depth: %3d,  Map: %5d  %8p \n",
         class, visualList[i].depth, visualList[i].colormap_size,
         (void *)visualList[i].visual);
      }
   else if (class == TrueColor)
      {
      printf("TrueColor:   %3d, Depth: %3d,  Map: %5d  %8p \n",
         class, visualList[i].depth, visualList[i].colormap_size,
         (void *)visualList[i].visual);
      }
   else if (class == GrayScale)
      {
      printf("DirectColor: %3d, Depth: %3d,  Map: %5d  %8p \n",
         class, visualList[i].depth ,visualList[i].colormap_size,
         (void *)visualList[i].visual);
      }
   else if (class == GrayScale)
      {
      printf("DirectColor: %3d, Depth: %3d,  Map: %5d  %8p \n",
         class, visualList[i].depth ,visualList[i].colormap_size,
         (void *)visualList[i].visual);
      }

   }
 XFree((char *)visualList);

 vTemplate.visual = XDefaultVisual(idispl,iscreen);
 vTemplate.screen = iscreen;

 visualList = XGetVisualInfo(idispl,VisualScreenMask | VisualNoMask,
                             &vTemplate, &visualsMatched);
 
 class = visualList[0].class;

 printf("Default    : %3d, Depth: %3d,  Map: %5d %8p\n ",
         class, visualList[0].depth, visualList[0].colormap_size,
         (void *)visualList[i].visual);
 XFree((char *) visualList);

}

