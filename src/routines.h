 /* routines.h */
 /* Subroutine prototypes */

#ifndef ROUTINESH
#define ROUTINESH

#include "x.h"
#include "files.h"

 extern void     action       (Widget, char *, char *, char *);
 extern void     actions      (Widget, char*, XtActionProc, char*);
 extern int      addext       (char*, char*, int);
 extern float    angle        (float);
 extern void     angle_cb     (Widget, XtPointer, XtPointer);
 extern int      annote       (char*);
 extern void     annotemen    (void);
 extern float    antiang      (float);
 extern int      average      (float *, float *, int, float);
 extern int      avgsave      (char *, int, int, float *);
 extern void     backmen      (int);
 extern void     bar          (Widget, void *, void *);
 extern void     barmen       (Widget, XtPointer, XtPointer);
 extern void     bldr	      (float, float, float, float*);
 extern float* 	 cald	      (float*, float*);
 extern void     cantrot      (int, int, float *, unsigned char *, float,
                               float, float);
 extern void     cat          (void);
 extern void     catmen       (void);
 extern void     changedir    (Widget, XtPointer, XtPointer);
 extern void     changepat    (Widget, void *, void *);
 extern int      closefile    (FILEDATA*);
 extern int      closefilep   (FILEDATA**);
 extern void     colmen       (Widget, XtPointer, XtPointer);
 extern void     contrastmen  (Widget, XtPointer, XtPointer);
 extern void     copyarea     (Display *, Drawable, Drawable, GC, int,
                               int, unsigned int, unsigned int, int,
                               int);
 extern void     curmen       (Widget, XtPointer, XtPointer);
 extern void     comap        (void);
 extern void     comapsav     (int);
 extern void     comapmen2    (void);
 extern void     comapmen5    (Widget, void *, void *);
 extern void     comapmen     (Widget, XtPointer, XtPointer);
 extern void     comapmen2_buts(Widget, XtPointer, XtPointer);
 extern void     comapmen3    (Widget, void *, void *);
 extern void     corr_mask    (int);
 extern void     ctfmen0      (char *);
 extern Colormap defluts      (int); 
 extern Colormap defluts_true (int); 
 extern void     dendromen    (char *);
 extern int      denoise      (float *, int, int, float *, int, int,
                               float *, float *, float *, float);
 extern void     denoisemen   (Widget, XtPointer, XtPointer);
 extern void     dent         (void);
 extern void     dentmen      (void);
 extern void     dentmen2     (void);
 extern int      det_tilt     (int, int);
 extern void     dispmen      (Widget, void *, void *);
 extern void     dispsize     (int, int, int, int, int, int, int,
                               int*, int*, int*);
 extern void     dist         (void);
 extern void     distmen      (Widget, XtPointer, XtPointer);
 extern void     distmen_buts (Widget , XtPointer, XtPointer);
 extern void     distoref     (int, int, int, int *, unsigned char *,
                               float, float, float, float, int, int, int,
                               int, int); 
 extern void     docmontmen   (char *);
 extern void     docmontmen0  (char *);
 extern void     erodemen     (Widget, XtPointer, XtPointer);
 extern void     eul_to_xyz   (float *, float *, float *);
 extern void     exitexit     (Widget, void *, void *);
 extern void     expos_cb     (Widget, XtPointer, XtPointer);
 extern char  *  extractstr   (XmString);
 extern int      fftmcf       (float *, float *, int, int, int, int *);
 extern int      filget       (char *, char *, int, int);
 extern int      filinc       (char*, char*, int*, int*);
 extern void     fillmen      (Widget, XtPointer, XtPointer);
 extern int      filset       (float *, int, int, float, float, float *,
                               float *, float *);
 extern int      filter       (float *, int, int, float *, int, int, 
                               float *, float *, float *);
 extern int      filtere      (float *, int, int, int, int, float, 
                               float *, float *, float *);
 extern int      filterv      (float *, int, int, float *, int, int, 
                               float *, float *, float *);
 extern void     filtermen    (void);
 extern int      filterfill   (float *, int, int, 
                               int, int, float, float, 
                               float *, float *, float *, int);
 extern void     filter_but   (Widget, XtPointer, XtPointer);
 extern int      filterskel   (float *, int, int, 
                               float *, float *, float *);
 extern int      filtestb     (float *, int, int, float, float, float *,
                               float *, float *);
 extern void     fin_cb       (Widget, void *, void *);
 void            fit_butdraw2 (Widget, XtPointer, XtPointer);
 void            fit_butsavang(Widget, XtPointer, XtPointer);
 extern int      fitdoc       (Boolean,Boolean,Boolean,Boolean);
 extern int      fitdoc_addpart(int, int, int, int, int, int);
 extern void     fitdocmen    (Widget, XtPointer, XtPointer);
 extern void     fitmen_adv   (Widget, XtPointer, XtPointer);
 extern void     fitsav       (int);
 extern int      fmr_1        (float*, int, float *, int *);
 extern int      fmr_2        (float*, int, int, int *);
 extern void     fontmen      (Widget, void *, void *);
 extern int      getcorner    (Widget, Widget , int *, int *);
 extern int      getdoc       (FILE *, char *, char *,  
                               int, int, float **, int *);
 extern int      getdoc_f      (FILE *, char *, char *,  
                               int, int, float **, int *,int);
 extern void     get_imsiz    (Widget, XtPointer, XtPointer);
 extern void     getloc       (XEvent *, char, int *, int *);
 extern int      getoldstack  (FILEDATA*, int, int, int);
 extern void     glass        (Window, Window, int, int, int, int,
                               int *, int *, int);
 extern void     gold         (void);
 extern void     gold_pop     (Widget, XEvent *, String *, Cardinal *);
 extern void     goldmen      (void);
 extern void     goldmen_butm (Widget, XtPointer, XtPointer);
 extern void     goldmen_buts (Widget, XtPointer, XtPointer);
 extern int      goldmen_mark (void);
 extern int      gongon       (float, float, float *, float *, float *);
 extern int      his          (FILEDATA*, int*);
 extern int      hist         (FILEDATA*);
 extern void     imagedis     (FILEDATA *, int, XImage **,
                               int, int, float **, int, int);
 extern void     imagemen     (Widget, XtPointer, XtPointer);
 extern int      imagemen1    (char *, int);
 extern void     imoptmen     (Widget, void *, void *);
 extern int      invert       (float *, int, int,float *, float *, 
                               float *);
 extern void     itex         (Widget, void *, void *);
 extern char *   itoa         (int);
 extern int      iskipadj     (int, int, int, int, int, int*, int *);
 extern int      iskipundo    (int, int, int, int, int, int*, int *);
 extern void     lahemen      (Widget, XtPointer, XtPointer);
 extern int      localareaeq  (float *,int, int, float *, float *,
                               int, int, float *);
 extern void     locc         (int*, int*, int, int, 
                               int*, int*, int*, int, int, int, int, 
                               int, int, int, int, int*, int);
 extern int      lut          (char *, Colormap, int, int);
 extern int      lut_info     (char *, Colormap, int, int, int);
 extern void     lutmen       (Widget, XtPointer, XtPointer);
 extern void     mag          (Widget, void *, void *);
 extern void     mask         (void);
 extern void     mask_draw_polys(Pixmap, GC, int, int);
 extern void     mask_pop     (Widget, XEvent *, String *, Cardinal *);
 extern void     mask_reset   (void);
 extern void     maskmen      (void);
 extern int      mircol       (int, int, int, float (*)[5], float, float *);
 extern int      monn         (GC, FILEDATA *, int, int, int, int, int, int);
 extern void     mont2        (int, int);
 extern int      mont3        (FILEDATA *, int, int);
 extern void     montmen3     (void);
 extern void     montmen2     (void);
 extern void     movearea     (void);
 extern void     movecur      (int, int);
 extern void     moviemen     (int);
 extern void     moviemen_buts(Widget, void *, void *);
 extern Cursor   newcursor    (Cursor, int);
 extern void     newext       (Widget,XtPointer, XtPointer );
 extern void     newvmap      (int, int, int* );
 extern int      norm         (FILEDATA*);
 extern int      numdig       (int, int);
 extern FILEDATA *opennew     (char*, int, int, int, int, char*);
 extern FILEDATA *openold     (char*, int*, int*, int*, int*, char*);
 extern void     pickback     (int);
 extern int      pickbackave  (char *);
 extern void     pickbackmen  (void);
 extern void     pickdisp     (int, int);
 extern void     pickdraw     (int, int, int, int, int, int);
 extern void     pickmen      (void);
 extern void     pickmen_butdet(Widget, XtPointer, XtPointer);
 extern void     pickmen_butl (Widget, XtPointer, XtPointer);
 extern void     pickmen_butn (Widget, XtPointer, XtPointer);
 extern void     pickp        (int);
 extern void     pixel_in     (void);
 extern void     pixel_in_pop (Widget, XEvent *, String *, Cardinal *);
 extern void     pixel_sc     (void);
 extern void     pixelmen_in_buts(Widget, XtPointer, XtPointer);
 extern void     pixelmen_in_doc(Widget, XtPointer, XtPointer);
 extern void     pixelmen_in_nod(void);
 extern void     pixelmen_sc_buts(Widget, XtPointer, XtPointer);
 extern void     pixelmen_sc_doc(Widget, XtPointer, XtPointer);
 extern void     pixelmen_sc_nod(Widget, XtPointer, XtPointer);
 extern int      pixlen       (GC, char *);
 extern unsigned long planeforcolor(int);
 extern void     pop_errmsg   (char *);
 extern int      power        (float*, int, int);
 extern void     powermen     (void);
 extern void     powermena    (void);
 extern void     profile      (void);
 extern void     profile_pop  (Widget, XEvent *, String *, Cardinal *);
 extern void     profilemen   (void);
 extern void     profilemen_buts (Widget, void *, void *);
 extern void     projmovie    (int);
 extern int      querylut     (Colormap, Visual *, int *, int * ,int *, int *);
 extern void     radcur       (int);
 extern void     rcaver       (FILEDATA*, int, int, int, 
                               int, int, int, float *);
 extern int      rdprx        (Widget, char, int *, float *);
 extern int      rdpriw       (int *, int, int, Widget, char *, char *);
 extern int      rdprxc       (Widget, char *, int);
 extern int      redlin       (FILEDATA *, float*, int , int);
 extern int      redlin16f    (FILEDATA *, float *, int, int);
 extern int      redlin8      (FILEDATA *, unsigned char*, int, int);
 extern int      redlin8f     (FILEDATA *, float *, int, int);
 extern int      redvol       (FILEDATA *, int,  char  **, int, float **,
                               int,  int, int, int, int, int, int, int, 
                               float, float, 
                               float*,  float*, int);
 extern int      redvol8      (FILEDATA *, unsigned char  *,
                               int, int, int, int,  int, int, int, int, 
                               float *, float *);
 extern int      redvol8_reform(FILEDATA *, unsigned char * , int, int,
                               int, int, int,  float *, float *); 
 extern void     reflmen      (void);
 extern void     foumen       (void);
 extern float    round_spi    (float);
 extern float    roundang     (float);
 extern void     rubberline   (Widget, XtPointer, XtPointer);
 extern int      scale32to8   (float, float, int, int, int,
                               float *, float *);
 extern int      scaleimg     (float *, int, int, int, int,
                               float, float, char *);
 extern FILE   * savdn1       (char*, char*, FILE**, float*, 
			       int, int*,int,int);
 extern FILE   * savdnc       (char*, char*, FILE**, float*, 
			       int, int*,int,int,  char *);
 extern void     setacursor   (int,    int, int);
 extern void     Setbgfg      (Arg *, int, int * );
 extern void     setamap      (Colormap);
 extern void     setangles    (int, float, float, float, float, float,
                               float);
 extern void     setline      (Widget, void *, void *);
 extern void     setmag       (Widget, XtPointer, XtPointer);
 extern void     setmen       (Widget, XtPointer, XtPointer);
 extern int      setnprow     (int);
 extern int      setprm       (FILEDATA *, char *);
 extern GC       setxor       (GC);
 extern int      showbimage   (float *, int, char**,
                               int, XImage **, int, int,float, float);
 extern void     showbuts_str (Widget *, Widget *, char *, char *,
                               char *, int);
 extern int      showimage    (FILEDATA*, int, char**, 
                               int, XImage**,
                               int, float**, int, float, float, 
                               int, int);
 extern void     showbutx     (char*, char*, char*, int);
 extern void     showbutxmes  (char*, char*, char*);
 extern void     showmoviemen (int);
 extern void     showmovie    (void);
 extern void     showmovmen2  (void);
 extern void     showmovmen3  (void);
 extern void     shift        (int);
 extern int      sird         (float *, unsigned char *, int, 
                               int, int, int);
 extern void     spout        (char*);
 extern void     spouts       (char*);
 extern void     spouto       (char*);
 extern void     spouti       (int);
 extern int      spoutfile    (int);
 extern void     stackmen     (FILEDATA*);
 extern void     stackdis     (int, int);
 extern int      strtoint     (Widget, char*, int*);
 extern int      sumum        (float *, float *, float *, int);
 extern void     surf         (int, int, int, int);
 extern int      surfcolor    (unsigned char *, int, int *);
 extern void     surfmen      (int, int, int);
 extern void     surfmovie    (int);
 extern void     surf_pop     (Widget, XEvent *, String *, Cardinal *);
 extern float    surfrot      (int *, int *, float, float, float,
                               unsigned char *, unsigned char, int, int);
 extern int      thresh       (float *, int, int, 
                               float, float, float, float,
                               float *, float *, float *);
 extern void     threshmen    (Widget, XtPointer, XtPointer);
 extern int      tiltang      (float *, float *, float *, float *,
	                       int, float *, int *, float, int);
 extern void     tiltp_deletemen(void);
 extern void     toggle2_cb   (Widget, XtPointer, XtPointer);
 extern void     trafc        (float, float, float, float, float, float,
                               float, float, float, float, int, float *);
 extern int      unsdal       (char *, int *, int, float *,
                               int, float **, int *, int, int);
 extern void     visual_info  (Display*, Visual*, int *, int *);
 extern void     visuals      (Display*, int);
 extern float    volval       (int, int, int, unsigned char *);    
 extern void     waitcursor   (Window, int, int);
 extern void     webclear     (Widget, XtPointer, XtPointer);
 extern void     web_com      (Widget);
 extern void     web_com2     (Widget);
 extern void     web_info     (Widget, void *, void *);
 extern Widget   web_main     (Widget);
 extern void     web_man      (Widget, void *, void *);
 extern int      wicolor      (GC, int);
 extern Widget   widean       (Widget, Widget, char *, char *,
                               int, int, int);
 extern int      widisp       (GC, FILEDATA*, int, int, int, int, int, 
                               double, double ,int, int, int, int, int,
                               int, XImage**, int);
 extern Widget   wid_angles   (Widget, Widget[3], int,int, int, int, int, int);
 extern Widget   wid_contrast (Widget,  char*,  int, int);
 extern Widget   wid_dialog   (Widget, Widget, char*, int, int);
 extern Widget   wid_file     (Widget, Widget, char*, char*, char*,
                               XtCallbackProc, int, int);
 extern void     wid_fileset  (Widget, char*);
 extern void     wid_filedirset(Widget, char*);
 extern void     wid_info     (void);
 extern Widget   wid_labelg   (Widget, Widget, char*, int, int);
 extern Widget   wid_label    (Widget, Widget, char*, Colormap,
                               int, int);
 extern int      wid_lutinfo  (Colormap, int);
 extern Widget   wid_mess     (Widget, Widget, char*, int, int);
 extern void     wid_movie    (void);
 extern Widget   wid_menubar  (Widget); 
 extern Widget   wid_posiz    (Widget, Widget, int, int, 
                               int, int, char, void (*)(void));
 extern int      wid_power    (int, int*);
 extern Widget   wid_pulldown (Widget, char *, int);
 extern Widget   wid_push     (Widget, Widget, char *, XtCallbackProc,
                               char *, Colormap, int, int);  //UNUSED??
 extern Widget   wid_pushg    (Widget, Widget, char*, XtCallbackProc,
                               char*,int, int); 
 extern Widget   wid_rowcol   (Widget, char , int, int);
 extern void     wid_sep      (Widget); 
 extern Widget   wid_seper    (Widget, Widget, char *, int, int);
 extern Widget   wid_scale    (Widget, Widget, char*, 
                               int, int, int,  int, int, int, int);
 extern Widget   wid_skip     (Widget,  char*,  int, int);
 extern Widget   wid_stdbut   (Widget, Widget,  Widget*, Widget*, Widget*, 
                               char *, XtCallbackProc, XtCallbackProc, XtCallbackProc,
                               void *);
 extern Widget   wid_stdbut_str(Widget, Widget,  Widget*, Widget*, Widget*, 
                               char *,     char *,     char *,    
                               char *, XtCallbackProc, XtCallbackProc, XtCallbackProc,
                               void *);
 extern Widget   wid_text     (Widget, Widget, char*, char*,
                               XtCallbackProc, void *, int, int);
 extern Widget   wid_textboxb (Widget iw_parent, Widget iw_text, 
                               char* prompt, char* string,  int icol);
 extern Widget   wid_togmen   (Widget, Widget, char*, int, int, int);
 extern Widget   wid_toggleg  (Widget, Widget, char *, int,
                               XtCallbackProc, void *, int, int);
 extern void     wid_win      (Widget, Widget *, int, int, int);
 extern void     widpos       (int *, int *);
 extern int      willsq       (float*, float*, float*, float*,
                               int, float, float*, float*);
 extern int      wimakecur    (Cursor *, char * , char *, XColor *,
                               XColor *, int, int);
 extern void     winbox       (int);
 extern void     windata      (int, int, int, int);
 extern void     windowsp     (FILEDATA *, FILEDATA *, int, int, int,
                               int, int, int, int,  int,  int, float);
 extern void     winfile      (void);
 extern void     winmov       (void);
 extern void     winread      (void);
 extern void     winread1     (char *,int, int, unsigned long);
 extern void     winsel       (void);
 extern void     winsiz       (Widget, XtPointer, XtPointer);
 extern void     winsiz1      (void);
 extern void     wipic        (GC, char *, int, int, int, int, int, int,
                               XImage **);
 extern void     wirubberline (Drawable win, GC icontxt, 
                               Boolean draw, Boolean perm,
                               int ixs,  int iys, int ixe,   int iye);
 extern void     witext       (GC, char*, int, int, 
                                int, int, int, int, int);
 extern void     witran       (float *, float *, float *, float *, int,
			       float, float, float);
 extern void     witran_rev   (float *, float *, float, float, int,
                               float, float, float);
 extern int      wrtlin       (FILEDATA *, float *, int, int);
 extern int      wrtname      (int, int, char *, int);
 extern int      wrtvol       (FILEDATA *,  float *);
 extern void     xorc         (Window, GC, int, int, int, int);
 extern void     xorbox       (Window, GC, int, int, int, int, int);
 extern void     xorboxt      (Window, GC, Boolean, Boolean, Boolean,
                               int, int, int, int);
 extern void     xorline      (Window, GC, int, int , int, int, int);
 extern void     xorlin       (Window, GC, int, int, 
                               int *, int *, int *, int *,
                               int, int, int, int);
 extern void 	 xyz_to_eul   (float *, float *, float *);
 extern int      ztoxy        (FILEDATA *, char *, char, float, float, int);
 extern int      ztoxyn       (FILEDATA *, char *, char, int, int, int);
 extern int      setnprow     (int);
#endif
