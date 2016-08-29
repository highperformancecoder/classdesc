#include <stdio.h>
#include <stdlib.h>
#include <tcl.h>
#include <tk.h>
#define TK3 (TK_MAJOR_VERSION<8)

static Tcl_Interp *interp=NULL;
static Tk_Window mainWin;


void raster(int *data, int xsize, int ysize)
{
  static char cmap[256][3], cmd[100];
  int i,j,k;
//  unsigned int min_val, max_val;
  Tk_PhotoImageBlock block;

  if (interp==NULL)
    {
      interp=Tcl_CreateInterp();
#if TK3
      mainWin = Tk_CreateMainWindow(interp,NULL,"raster","raster");
#else
      Tk_Init(interp);
      mainWin = Tk_MainWindow(interp);
      /*
      Tcl_SetVar(interp,"tcl_library","/usr/local/lib/tcl8.0",0);
      Tcl_SetVar(interp,"tk_library","/usr/local/lib/tk8.0",0);
      */
#endif
      if (Tcl_EvalFile(interp,"raster.tcl")!=TCL_OK)
	{
	  printf("%s\n",Tcl_GetStringResult(interp)); /* print out trace */
	  printf("%s\n",Tcl_GetVar(interp,"errorInfo",0));
	}
      Tk_GeometryRequest(mainWin,500,500);  /* default, in case window geom not
					 specified */
      Tk_MapWindow(mainWin);

      /* setup colourmap */
      for (i = 0; i < 256; i++)
	{
	  float r,g,b,ii=(float)i/256;
	  switch ((i*7)/256)
	    {
	    case 0: r=g=0; b=3.5*(ii+1/7.0); break;
	    case 1: case 2: r=0; g=3.5*(ii-1/7.0); b=1; break;
	    case 3: case 4: r=3.5*(ii-3/7.0); g=1; b=3.5*(3/7.0-ii); break;
	    case 5: case 6: r=1; g=3.5*(5/7.0-ii); b=0;
	    }
	  cmap[i][0]=r*255; cmap[i][1]=g*255; cmap[i][2]=b*255;
	}
     }

  /* configure Canvas to be big enough */
  sprintf(cmd,".raster configure -width %d -height %d",xsize,ysize);
  Tcl_Eval(interp,cmd);

  block.pixelPtr = (unsigned char*) malloc(3*xsize*ysize);
  block.width = xsize;
  block.height = ysize;
  block.pitch = xsize * 3;
  block.pixelSize = 3;
  block.offset[0]=0; block.offset[1]=1; block.offset[2]=2;

//  min_val = data[0];
//  max_val = data[0];
  for (i=0; i<xsize; i++)
  { for (j=0; j<ysize; j++)
    {
//       int d = data[i+j*xsize];
//       if (d > max_val) max_val = d;
//       if (d < min_val) min_val = d;
       for (k=0; k<3; k++)
       {
	  int d = data[i+j*xsize];
	  block.pixelPtr[(i+xsize*j)*3+k] =
	    (d>=0)? ((d<256)? cmap[d][k] : 255): 0;
       }
    }
  }
//  printf("min=%d max=%d\n", min_val, max_val);
#if TK3
  Tk_PhotoPutBlock(Tk_FindPhoto(".pixmap"), &block, 0, 0, xsize, ysize);
#elif TK_MAJOR_VERSION==8 && TK_MINOR_VERSION < 4 
  Tk_PhotoPutBlock(Tk_FindPhoto(interp,".pixmap"), &block, 0, 0, xsize, ysize);
#elif TK_MAJOR_VERSION==8 && TK_MINOR_VERSION < 5
  Tk_PhotoPutBlock(Tk_FindPhoto(interp,".pixmap"), &block, 0, 0, xsize, ysize,
		   TK_PHOTO_COMPOSITE_SET);
#else
  Tk_PhotoPutBlock(interp,Tk_FindPhoto(interp,".pixmap"), &block, 0, 0, xsize, ysize,
		   TK_PHOTO_COMPOSITE_SET);
#endif
  free(block.pixelPtr);
  Tcl_Eval(interp,"update");
  Tk_DoOneEvent(TK_ALL_EVENTS | TK_DONT_WAIT);
}


void raster_pause()
{
  int paused;
  Tcl_SetVar(interp,"paused","true",TCL_GLOBAL_ONLY);
  while (Tcl_GetBoolean(interp,
			Tcl_GetVar(interp,"paused",TCL_GLOBAL_ONLY),
			&paused),paused)
      Tk_DoOneEvent(TK_ALL_EVENTS);

}

/* Fortran hooks */
void raster_(int *data, int *xsize, int *ysize) {raster(data,*xsize,*ysize);}
void fraster(int *data, int *xsize, int *ysize) {raster(data,*xsize,*ysize);}
void fraster_(int *data, int *xsize, int *ysize) {raster(data,*xsize,*ysize);}
void raster_pause_() {raster_pause();}

