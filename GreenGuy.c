/* File: GreenGuy.c */

/**************************************************************************
 
			 CSc 405/505 Project

			GreenGuy Cartoon Maker

		      Submitted by Raymond Rusk
			Student Number 9413799

***************************************************************************/

/**************************************************************************
 * GreenGuy,  written by Raymond Rusk, May 1997.
 *
 * GreenGuy is an interactive program that uses the mouse to create
 * lines, circles, ellipses, rectangles and polygons.  These shapes
 * may then be re-positioned, re-ordered and deleted. An application
 * model, in the form of a list of objects, permits these operations
 * and allows the window to be refreshed when parts of it have been
 * damaged. A panel of push buttons is used to control program
 * execution.
 *
 * GreenGuy starts by initializing the canvas (i.e., drawing window)
 * and creating its own panel and menu. The panel consists of
 * rectangles on the edge of the canvas labeled with the GreenGuy
 * tools. The program is initially set up for creating filled
 * polygons.  After initialization the program waits for the user to
 * (1) select another tool from the panel or (2) to draw/edit
 * according to the tool currently selected. The program terminates if
 * Quit from the File pull-down menu is selected.
 *
 * The set of objects currently in existence is maintained in a singly
 * linked list.  Objects can be inserted, deleted, and traversed in
 * order, from the bottom object to the top object - thus, higher
 * objects obscure lower objects. Each time an object is created, it
 * is added as the top object.  Each object has an associated size,
 * color, line width and fill attribute that is saved as part of its
 * entry in the object list.
 *
 * In Edit mode an object is considered "under the mouse" if the
 * coordinates of the mouse lie in the object's rectangular extent
 * (which was specified when the object was created).  If more than
 * one object is "under the mouse", the topmost object is selected
 * for editing.
 *
 * Acknowledgements:
 *
 * GreenGuy uses the Simple X Library written by Dominic Giampaolo.
 * Some of the code in GreenGuy is based on examples distributed
 * with libsx.
 *
 * The linked list implementation is based on code in "Data Structures
 * and Algorithm Analysis in C, Second Edition", Mark Allen Weiss,
 * Addison-Wesley, 1997 and on code in "Computational geometry in C",
 * Joseph O'Rourke, Cambridge University Press, 1994.
 **********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <X11/cursorfont.h>
#include "libsx.h"
#include "creq.h"
#include "freq.h"
#include "bitmaps.h"
#include "object_list.h"
#include "vertex_list.h"

#define DEBUG 0

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

#define TRUE  1
#define FALSE 0

#define MAX_COLORS 256

#define STR_SIZ 512

static const char Version[] = "GreenGuy 1.0.0";

static const int DRAWWIDTH=500;
static const int DRAWHEIGHT=500;

static const int DEF_LINEWIDTH=1;
static const int MIN_LINEWIDTH=1;
static const int MAX_LINEWIDTH=64;

static const int MIN_MULT =  1;
static const int MAX_MULT = 10;

typedef enum {OFF, ON} ToggleValue;
typedef enum {EDIT, LINE, RECT, ELLIPSE, CIRCLE, POLYLINE, TEXT} PgmMode;
typedef enum {M_LEFT=1, M_MIDDLE=2, M_RIGHT=3} mousebutton;

typedef struct PgmData
{
  PgmMode mode;
  int writing;                      /* stores text writing state    */
  int drawing;                      /* polyline drawing state       */
  char buffer[STR_SIZ];             /* scratch buffer               */
  mousebutton down;                 /* records which button is down */
  int startx, starty, oldx, oldy;   /* used for drawing out boxes   */
  int LineWidth;                    /* stores foreground linewidth  */
  int color;                        /* stores foreground color      */
  int canvas_width, canvas_height;  /* stores draw area dimensions  */
  List L;                           /* stores object list           */
  ListPosition P_Top;               /* points to top object in L    */
  ElementType Obj;                  /* stores an object             */
  int BBRightEdge, BBBottomEdge;    /* for calculating polyline bb  */
  Widget LWLabel;                   /* linewidth widgets            */
  Widget LWScrollBar;
  Widget WLineWidth;
  Widget WscaleLabel;
  Widget Wdino[5];
  Widget Wedit;
  XFont TextFont;
  ToggleValue fill;
  ToggleValue reverse;
  char *FileName;
  int multiplier;                   /* used for scaling objects     */
  int divisor;                      /* used for scaling objects     */
} PgmData;

typedef struct {
  int CTidx;                  /* index into color table */
  unsigned char r,g,b;
  char *name;
} ColorRecord;

typedef struct {
  int CTidx;                  /* index into color table */
  unsigned char r,g,b;
} OtherColorRecord;

typedef enum 
{FirstColor, black = FirstColor, white,
 red4,   user1,
 red,    orange,
 user2,  yellow,
 blue4,  user3,
 blue,   cyan,
 cyan4,  green4,
 green,  magenta4,
 user4,  magenta,
 gray33, gray66,
 LastColor = gray66} ColorNum;

ColorRecord RGBColors[LastColor+1] = {
  {0,   0,  0,  0, "black"     },
  {0, 255,255,255, "white"     },
  {0, 139,  0,  0, "red4"      },
  {0, 201,  4, 60, "user1"     },   /* Red Guy    */
  {0, 255,  0,  0, "red"       },
  {0, 255,165,  0, "orange"    },   /* Yellow Guy */
  {0, 254,210,  0, "user2"     },
  {0, 255,255,  0, "yellow"    },
  {0,   0,  0,139, "blue4"     },
  {0,   8,129,167, "user3"     },   /* Blue Guy   */
  {0,   0,  0,255, "blue"      },
  {0,   0,255,255, "cyan"      },
  {0,   0,139,139, "cyan4"     },
  {0,   0,139,  0, "green4"    },   /* Green Guy  */
  {0,   0,255,  0, "green"     },
  {0, 139,  0,139, "magenta4"  },
  {0, 179,116,178, "user4"     },   /* Purple Guy */
  {0, 255,  0,255, "magenta"   },
  {0,  84, 84, 84, "gray33"    },
  {0, 168,168,168, "gray66"    }
};

OtherColorRecord MoreRGBColors[MAX_COLORS-LastColor-1];
static int OtherColors = 0;

static Widget canvas;
static Widget ColorMenu[3];
static int FgColor, BgColor;


OtherColorRecord
ColorTableIndexToRGB(int index)
{
  OtherColorRecord rgb;
  int i;

  /* Standard colors */
  for (i=FirstColor; i < LastColor; i++) {
    if (RGBColors[i].CTidx == index) {
      rgb.CTidx = index;
      rgb.r = RGBColors[i].r;
      rgb.g = RGBColors[i].g;
      rgb.b = RGBColors[i].b;
      return rgb;
    }
  }

  /* User created colors */
  for (i=0; i < OtherColors; i++) {
    if (MoreRGBColors[i].CTidx == index) {
      rgb.CTidx = index;
      rgb.r = MoreRGBColors[i].r;
      rgb.g = MoreRGBColors[i].g;
      rgb.b = MoreRGBColors[i].b;
      return rgb;
    }
  }
  fprintf(stderr, "An error occurred in ColorTableIndexToRGB().\n");
  rgb.CTidx = -index;
  rgb.r = 0;
  rgb.g = 0;
  rgb.b = 0;
  return rgb;
}

int
RGBtoColorTableIndex(unsigned char r, unsigned char g, unsigned char b)
{
  int i, index;

  /* Standard colors */
  for (i=FirstColor; i < LastColor; i++)
    if (RGBColors[i].r == r && RGBColors[i].g == g && RGBColors[i].b == b)
      return (RGBColors[i].CTidx);

  /* User created colors */
  for (i=0; i < OtherColors; i++)
    if (MoreRGBColors[i].r == r && MoreRGBColors[i].g == g &&
	MoreRGBColors[i].b == b)
      return (MoreRGBColors[i].CTidx);

  /* Else add to user colors */
  MoreRGBColors[OtherColors].r = r;
  MoreRGBColors[OtherColors].g = g;
  MoreRGBColors[OtherColors].b = b;
  index = MoreRGBColors[OtherColors].CTidx = GetRGBColor(r, g, b);
  if (index >= 0)
    OtherColors++;
  else
    fprintf(stderr,"Failed to allocate color r=%d, g=%d, b=%d.\n", r, g, b);
  return (index);
}

/* Print the linked-list of objects */
static void
PrintList( FILE *fp, const List L )
{
  ListPosition P = Header( L );
  ElementType Tmp;
  OtherColorRecord rgb;
  
  tVertex v;
  int cnt;
  
  if( IsEmpty( L ) )
    fprintf(fp,  "Empty list\n" );
  else
    {
      fprintf(fp,  "\n#Object Left Top Width Height   Color    Line- Fill" );
      fprintf(fp,  " Length of string/\n");
      fprintf(fp,  "# Type                       r    g    b width");
      fprintf(fp,  "      vertex list\n");
      do
        {
          P = Advance( P );
          Tmp = Retrieve( P );
	  rgb = ColorTableIndexToRGB(Tmp.color);
          fprintf(fp, "%4d"        , Tmp.object_type );
          fprintf(fp, "%5d %5d"    , Tmp.bb.left_edge, Tmp.bb.top_edge );
          fprintf(fp, "%5d %5d"    , Tmp.bb.width, Tmp.bb.height );
          fprintf(fp, "%4d %4d %4d", rgb.r, rgb.g, rgb.b);
	  fprintf(fp, "%4d %4d"    , Tmp.LineWidth, Tmp.fill_attribute);
	  fprintf(fp, "%4d"        , Tmp.npts );
	  if (Tmp.object_type == line || Tmp.object_type == polyline) {
	    v=Tmp.v;
	    cnt = 0;
	    do {
	      if (cnt % 6 == 0) fprintf(fp, "\n");
	      fprintf(fp, " (%4d,%4d)", v->xp.x, v->xp.y);
	      v=v->next; cnt++;
	    } while (v != Tmp.v);
	  } else if (Tmp.object_type == text && Tmp.txt != NULL) {
	    fprintf(fp, "\n%s", Tmp.txt);
	  }
	  fprintf(fp,"\n");
        } while( !IsLast( P, L ) );
      fprintf(fp,  "\n" );
    }
}

/* Determines whether the mouse (x,y)-coordinates lie within the
   bounding box of an object */
static int
inObject(ElementType Obj, int x, int y)
{
  int edge_width;

  edge_width = Obj.LineWidth / 2;

  if ( x >= Obj.bb.left_edge - edge_width &&
       y >= Obj.bb.top_edge - edge_width &&
       x <= Obj.bb.left_edge + Obj.bb.width + edge_width &&
       y <= Obj.bb.top_edge + Obj.bb.height + edge_width )
    return TRUE;
  else
    return FALSE;
}

/* Traverse the object list structure and redraw each object */
static void redisplay(Widget wdgt, int new_width, int new_height, void *data)
{
  PgmData *GG = (PgmData *)data;
  ListPosition P = Header( GG->L );
  ElementType Tmp;
  int x, y, w, h;
  XPoint *xpts;
  int i, npts;
  int offsetx, offsety;
  char *textline, buffer[STR_SIZ];
    
  GG->canvas_width = new_width;
  GG->canvas_height = new_height;

  if (DEBUG) PrintList(stdout, GG->L);
  ClearDrawArea();

  if( !IsEmpty( GG->L ) ) {
    do {
      P = Advance( P );
      Tmp = Retrieve( P );

      SetFgColor(canvas, Tmp.color);
      SetLineWidth(Tmp.LineWidth);

      x = Tmp.bb.left_edge;
      y = Tmp.bb.top_edge;
      w = Tmp.bb.width;
      h = Tmp.bb.height;

      if (Tmp.object_type == text) {
	if (Tmp.txt != NULL) {
	  offsetx = 0;
	  offsety = FontHeight(GG->TextFont);
	  strncpy(buffer, Tmp.txt, STR_SIZ);
	  textline = strtok(buffer, "\n");
	  while (textline != NULL) {
	    DrawText(textline, x+offsetx, y+offsety);
	    offsety += FontHeight(GG->TextFont);
	    textline = strtok(NULL, "\n");
	  }
	}
      } else if (Tmp.object_type == line) {             /* draw line       */
	DrawLine(Tmp.v->xp.x, Tmp.v->xp.y, 
		 Tmp.v->next->xp.x, Tmp.v->next->xp.y);
      } else if (Tmp.fill_attribute == OFF) {
	if (Tmp.object_type == polyline) {              /* draw polyline   */
	  npts = Tmp.npts;
	  xpts = (XPoint *)malloc(npts * sizeof(XPoint));
	  for (i=0; i< npts; i++) {
	    xpts[i] = Tmp.v->xp;
	    Tmp.v=Tmp.v->next;
	  }
	  DrawPolyline(xpts,npts);
	  free(xpts);
	} else if (Tmp.object_type == rectangle) {      /* draw rectangle  */
	  DrawBox(x, y, w, h);
	} else {                                /* draw an ellipse/circle  */
	  DrawArc(x, y, w, h, 0, 360);
	}
      } else {                                      /* draw filled objects */
	if (Tmp.object_type == polyline) {          /* draw filled polygon */
	  npts = Tmp.npts;
	  xpts = (XPoint *)malloc(npts * sizeof(XPoint));
	  for (i=0; i< npts; i++) {
	    xpts[i] = Tmp.v->xp;
	    Tmp.v=Tmp.v->next;
	  }
	  DrawFilledPolygon(xpts,npts);
	  free(xpts);
	} else if (Tmp.object_type == rectangle) {     /* draw filled rect */
	  DrawFilledBox(x, y, w, h);
	} else {                             /* draw filled ellipse/circle */
	  DrawFilledArc(x, y, w, h, 0, 360);
	}
      }
    } while( !IsLast( P, GG->L ) );

    if (P != GG->P_Top) {
      fprintf(stderr, "Program error - stray pointer to top object.\n");
      GG->P_Top = P;
    }

    SetFgColor(canvas, FgColor);
    SetLineWidth(GG->LineWidth);
  }
}

static void AddTextObjToList(PgmData *GG)
{
  int i, line_length;

  if(GG->buffer != NULL) {
    GG->Obj.txt = strdup(GG->buffer);
    if (GG->Obj.txt == NULL) {
      fprintf(stderr, "Could not allocate memory in AddTextObjToList for\n");
      fprintf(stderr, "[%s]\n", GG->buffer);
      return;
    }
    GG->Obj.bb.width = line_length = 0;
    for (i=0 ; i< strlen(GG->buffer); i++) {
      if(GG->buffer[i] != '\n') {
	line_length++;
	GG->Obj.bb.width = MAX(GG->Obj.bb.width, line_length);
      } else {
	line_length = 0;
	GG->Obj.bb.height++;
      }
    }
    if ( !(GG->Obj.bb.width == 0 && GG->Obj.bb.height == 0) ) {
      GG->Obj.bb.height++;
      GG->Obj.object_type = text;
      GG->Obj.bb.width  *= TextWidth(GG->TextFont, " ");
      GG->Obj.bb.height *= FontHeight(GG->TextFont);
      GG->Obj.v=NULL; 
      GG->Obj.npts = strlen(GG->Obj.txt);
      GG->Obj.color = FgColor;
      GG->Obj.LineWidth = GG->LineWidth;
      GG->Obj.fill_attribute = GG->fill;
      Insert( GG->Obj, GG->L, GG->P_Top );
      GG->P_Top = Advance( GG->P_Top );
    }
  }
}

static void SetCursorFont(unsigned int shape)
{
  Display *display;

  display = (Display *)XtDisplay(canvas);
  XDefineCursor(display, XtWindow(canvas),
                XCreateFontCursor(display, shape));
}

static void Edit(Widget w, void *data)
{
  PgmData *GG = (PgmData *)data;

  GG->mode = EDIT;
  SetKeypressCB(canvas, NULL);
  SetCursorFont(XC_hand2);
  if (GG->writing == TRUE) {
    AddTextObjToList(GG);
    GG->writing = FALSE;
  }

  return;
}

static void Line(Widget w, void *data)
{
  PgmData *GG = (PgmData *)data;

  GG->mode = LINE;
  SetKeypressCB(canvas, NULL);
  SetCursorFont(XC_crosshair);
  if (GG->writing == TRUE) {
    AddTextObjToList(GG);
    GG->writing = FALSE;
  }

  return;
}

static void Rect(Widget w, void *data)
{
  PgmData *GG = (PgmData *)data;

  GG->mode = RECT;
  SetKeypressCB(canvas, NULL);
  SetCursorFont(XC_crosshair);
  if (GG->writing == TRUE) {
    AddTextObjToList(GG);
    GG->writing = FALSE;
  }

  return;
}

static void Ellipse(Widget w, void *data)
{
  PgmData *GG = (PgmData *)data;

  GG->mode = ELLIPSE;
  SetKeypressCB(canvas, NULL);
  SetCursorFont(XC_crosshair);
  if (GG->writing == TRUE) {
    AddTextObjToList(GG);
    GG->writing = FALSE;
  }

  return;
}

static void Circle(Widget w, void *data)
{
  PgmData *GG = (PgmData *)data;

  GG->mode = CIRCLE;
  SetKeypressCB(canvas, NULL);
  SetCursorFont(XC_crosshair);
  if (GG->writing == TRUE) {
    AddTextObjToList(GG);
    GG->writing = FALSE;
  }

  return;
}

static void PolyLine(Widget w, void *data)
{
  PgmData *GG = (PgmData *)data;

  GG->mode = POLYLINE;
  SetKeypressCB(canvas, NULL);
  SetCursorFont(XC_crosshair);
  if (GG->writing == TRUE) {
    AddTextObjToList(GG);
    GG->writing = FALSE;
  }

  return;
}

static void KeyCBfunc(Widget w, char *input, int up_or_down, void *data)
{
  PgmData *GG = (PgmData *)data;

  if (input != NULL && up_or_down == 0 && strlen(input) == 1) {
    if (isprint(*input)) {
      DrawText(input, GG->oldx, GG->oldy);
      strncat(GG->buffer, input, STR_SIZ);
      GG->oldx += TextWidth(GG->TextFont, input);
    } else if (input[0] == '\r') {
      GG->oldx = GG->startx;
      GG->oldy += FontHeight(GG->TextFont);
      strncat(GG->buffer, "\n", STR_SIZ);
    } else if (input[0] == '\b' && GG->oldx > GG->startx) {
      GG->oldx -= TextWidth(GG->TextFont, " ");
      DrawText(" ", GG->oldx, GG->oldy);
      GG->buffer[strlen(GG->buffer)-1] = '\0';
    }
  }
}

static void Text(Widget w, void *data)
{
  PgmData *GG = (PgmData *)data;

  GG->mode = TEXT;
  SetCursorFont(XC_pencil);
  SetKeypressCB(canvas, KeyCBfunc);
}

static void Fill(Widget w, void *data)
{
  PgmData *GG = (PgmData *)data;

  GG->fill ^= ON;

  return;
}

static void ClearAll(Widget w, void *data)
{
  PgmData *GG = (PgmData *)data;

  GG->L = MakeEmpty(GG->L);
  GG->P_Top = Header(GG->L);
  redisplay(canvas, GG->canvas_width, GG->canvas_height, GG);
  CloseWindow();
}

static void ShutDown(Widget w, void *data)
{
  CloseWindow();
  exit(0);
}

static void Continue(Widget w, void *data)
{
  CloseWindow();
}

static void Clear(Widget w, void *data)
{
  Widget x[4];
  char s[]="Clear";

  x[0] = MakeWindow(s, SAME_DISPLAY, EXCLUSIVE_WINDOW);
  x[1] = MakeLabel("   Erase  \nEverything?");
  x[2] = MakeButton("Yes", ClearAll, data);
  x[3] = MakeButton(" No", Continue, NULL);
  SetWidgetPos(x[2], PLACE_UNDER, x[1], NO_CARE, NULL);
  SetWidgetPos(x[3], PLACE_UNDER, x[1], PLACE_RIGHT, x[2]);
  ShowDisplay();
  MainLoop();
}

static void Quit(Widget w, void *data)
{
  Widget x[4];
  char s[]="Quit";

  x[0] = MakeWindow(s, SAME_DISPLAY, EXCLUSIVE_WINDOW);
  x[1] = MakeLabel("Quitting?");
  x[2] = MakeButton("Yes", ShutDown, NULL);
  x[3] = MakeButton(" No", Continue, NULL);
  SetWidgetPos(x[2], PLACE_UNDER, x[1], NO_CARE, NULL);
  SetWidgetPos(x[3], PLACE_UNDER, x[1], PLACE_RIGHT, x[2]);
  ShowDisplay();
  MainLoop();
}

static void Message(char *string)
{
  Widget x[3];
  char s[]="Message";

  x[0] = MakeWindow(s, SAME_DISPLAY, EXCLUSIVE_WINDOW);
  x[1] = MakeLabel(string);
  x[2] = MakeButton("OK", Continue, NULL);
  SetWidgetPos(x[2], PLACE_UNDER, x[1], NO_CARE, NULL);
  ShowDisplay();
  MainLoop();
}

static void ColorFunc(Widget w, void *data)
{
  ColorNum Num = (ColorNum)data;
  int rOld, gOld, bOld;

  if (Num >= FirstColor && Num <= LastColor) {
    FgColor = RGBColors[Num].CTidx;
  } else if (OtherColors < MAX_COLORS - LastColor - 1) {
    if (OtherColors == 0) { /* initialize with default std color  */
      rOld = MoreRGBColors[OtherColors].r = RGBColors[gray66].r;
      gOld = MoreRGBColors[OtherColors].g = RGBColors[gray66].g;
      bOld = MoreRGBColors[OtherColors].b = RGBColors[gray66].b;
    } else {                /* initialize with previous selection */
      rOld = MoreRGBColors[OtherColors].r = MoreRGBColors[OtherColors-1].r;
      gOld = MoreRGBColors[OtherColors].g = MoreRGBColors[OtherColors-1].g;
      bOld = MoreRGBColors[OtherColors].b = MoreRGBColors[OtherColors-1].b;
    }
    if ( GetColor(&MoreRGBColors[OtherColors].r,
		  &MoreRGBColors[OtherColors].g,
		  &MoreRGBColors[OtherColors].b) ) {
      if (MoreRGBColors[OtherColors].r != rOld ||     /* Only allocate a   */
	  MoreRGBColors[OtherColors].g != gOld ||     /* color table index */
	  MoreRGBColors[OtherColors].b != bOld) {     /* when r, g, or b   */
	FgColor = MoreRGBColors[OtherColors].CTidx    /* changes.          */
	        = GetRGBColor(MoreRGBColors[OtherColors].r,
			      MoreRGBColors[OtherColors].g,
			      MoreRGBColors[OtherColors].b);
	OtherColors++;
      } else {
	FgColor = MoreRGBColors[OtherColors-1].CTidx;
      }
    }
  }
  SetBgColor(ColorMenu[0],FgColor);
  return;
}

static void LWscrollbarCBfunc(Widget w, float val, void *data)
{
  PgmData *GG = (PgmData *)data;
  char lw[20];

  if (val > MAX_LINEWIDTH) {
    GG->LineWidth = MAX_LINEWIDTH;
  } else if (val < MIN_LINEWIDTH) {
    GG->LineWidth = MIN_LINEWIDTH;
  } else {
    GG->LineWidth = (int)val;
  }
  SetLineWidth(GG->LineWidth);

  /* update LWLabel widget too */
  sprintf(lw, "Linewidth %2d ", GG->LineWidth);
  SetLabel(GG->LWLabel, lw);

  return;
}

static void ScaleIncr(Widget w, void *data)
{
  PgmData *GG = (PgmData *)data;
  char lw[20];

  if (GG->multiplier < MAX_MULT) {
    GG->multiplier += 1;
    GG->divisor     = 10;
    sprintf(lw,"%3d\n --\n%3d", GG->multiplier, GG->divisor);
    SetLabel(GG->WscaleLabel, lw);
  }
}

static void ScaleDecr(Widget w, void *data)
{
  PgmData *GG = (PgmData *)data;
  char lw[20];

  if (GG->multiplier > MIN_MULT) {
    GG->multiplier -= 1;
    GG->divisor     = 10;
    sprintf(lw,"%3d\n --\n%3d", GG->multiplier, GG->divisor);
    SetLabel(GG->WscaleLabel, lw);
  }
}

static void reverse(Widget w, void *data)
{
  PgmData *GG = (PgmData *)data;

  GG->reverse ^= ON;

  if (GG->reverse == OFF) {
    SetWidgetBitmap(GG->Wdino[0], ggr_bits, ggr_width, ggr_height);
    SetWidgetBitmap(GG->Wdino[1], bgr_bits, bgr_width, bgr_height);
    SetWidgetBitmap(GG->Wdino[2], ygr_bits, ygr_width, ygr_height);
    SetWidgetBitmap(GG->Wdino[3], pgr_bits, pgr_width, pgr_height);
    SetWidgetBitmap(GG->Wdino[4], rgr_bits, rgr_width, rgr_height);
  } else {
    SetWidgetBitmap(GG->Wdino[0], ggl_bits, ggl_width, ggl_height);
    SetWidgetBitmap(GG->Wdino[1], bgl_bits, bgl_width, bgl_height);
    SetWidgetBitmap(GG->Wdino[2], ygl_bits, ygl_width, ygl_height);
    SetWidgetBitmap(GG->Wdino[3], pgl_bits, pgl_width, pgl_height);
    SetWidgetBitmap(GG->Wdino[4], rgl_bits, rgl_width, rgl_height);
  }
  SetFgColor(GG->Wdino[0], RGBColors[green4].CTidx);
  SetFgColor(GG->Wdino[1], RGBColors[user3].CTidx);
  SetFgColor(GG->Wdino[2], RGBColors[orange].CTidx);
  SetFgColor(GG->Wdino[3], RGBColors[user4].CTidx);
  SetFgColor(GG->Wdino[4], RGBColors[user1].CTidx);
}

static void HandleEditMBLeft(int x, int y, void *data)
{
  PgmData *GG = (PgmData *)data;
  ElementType TopOne;

  if (GG->down == M_LEFT) {
    if( !IsEmpty( GG->L ) ) {
      TopOne = Retrieve(GG->P_Top);
      GG->P_Top = FindPrevious(TopOne, GG->L);
      Delete(TopOne, GG->L);
      TopOne.bb.left_edge = GG->Obj.bb.left_edge;
      TopOne.bb.top_edge = GG->Obj.bb.top_edge;
      Insert(TopOne, GG->L, GG->P_Top);
      GG->P_Top = Advance (GG->P_Top);
    }
  } else {
    fprintf(stderr, "HandleEditMBLeft() called for mouse button %d!\n",
	    GG->down);
  }
  return;
}

static void HandleEditMBCenterRight(int x, int y, void *data)
{
  PgmData *GG = (PgmData *)data;
  ListPosition P_TheOne = NULL, P_Tmp = Header( GG->L );
  ElementType Tmp, TheOne;
  tVertex t;
  int num = 0;

  if (GG->mode == EDIT && GG->down != M_LEFT) {
    if( !IsEmpty( GG->L ) ) {
      do {
	P_Tmp = Advance( P_Tmp );
	Tmp = Retrieve( P_Tmp );

	if ( inObject(Tmp, x, y) ) {
	  TheOne = Retrieve( P_Tmp );
	  P_TheOne = P_Tmp;
	  num++;
	}
      }  while( !IsLast( P_Tmp, GG->L ) );
    }
    if (P_TheOne != NULL) {             /* Just remove top item in bounding */
      if ( !IsLast(P_TheOne, GG->L) ) { /* box if item selected not at end  */
	Delete(TheOne, GG->L);          /* of list.  If item is at end of   */
      } else {                          /* list update P_Top first.         */
	GG->P_Top = FindPrevious(TheOne, GG->L);
	Delete(TheOne, GG->L);
      }
      if (GG->down == M_MIDDLE) {       /* Reinsert selected item at bottom */
	Insert(TheOne, GG->L, Header(GG->L));
	if (GG->P_Top == Header(GG->L)) {
	  GG->P_Top = Advance (GG->P_Top);
	}
      } else {       /* full deletion -  free vertex list */
	while ( (t=TheOne.v) != NULL ) { /* Free list of vertices */
	  TheOne.v=TheOne.v->next;
	  DELETE(TheOne.v,t);
	}
      }
    }
  } else if (GG->down == M_LEFT) {
    fprintf(stderr, "HandleEditMBCenterRight() called for left button!\n");
  } else if (GG->mode != EDIT) {
    fprintf(stderr, "HandleEditMBCenterRight() called but not editing!\n");
  }
  return;
}

/*
 * Called when DrawingArea gets mouse drawing events.  The arguments
 * x and y are the current position of the mouse in the window. 
 */
void DrawMotion(Widget w, int x, int y, void *data)
{
  PgmData *GG = (PgmData *)data;
  int sx, sy, owidth, oheight, radius;
  
  if(GG->down == M_LEFT) {                     /* Do nothing unless LEFT */
    sx = GG->startx;                           /* mouse button is down.  */
    sy = GG->starty;
    owidth  = GG->oldx - sx;
    oheight = GG->oldy - sy;
    if (GG->mode == LINE) {                    /* draw lines             */
      DrawLine(sx, sy, GG->oldx, GG->oldy);
      DrawLine(sx, sy, x, y);
    } else if (GG->mode == POLYLINE) {
      DrawLine(sx, sy, GG->oldx, GG->oldy);
      DrawLine(sx, sy, x, y);
    } else if (GG->mode == RECT) {             /* draw rectangles        */
      if (GG->fill == OFF) {
	DrawBox(sx, sy, owidth, oheight);
	DrawBox(sx, sy, (x - sx), (y - sy));
      } else {                                 /* draw filled rectangles */
	DrawFilledBox(sx, sy, owidth, oheight);
	DrawFilledBox(sx, sy, (x - sx), (y - sy));
      }
    } else if (GG->mode == CIRCLE) {           /* draw circles           */
      if (GG->fill == OFF) {
	radius = (int)sqrt(owidth*owidth+oheight*oheight);
	DrawArc(-radius + sx,-radius + sy, 2*radius, 2*radius, 0, 360);
	radius = (int)sqrt((x-sx)*(x-sx)+(y-sy)*(y-sy));
	DrawArc(-radius + sx,-radius + sy, 2*radius, 2*radius, 0, 360);
      } else {                                 /* draw filled circles    */
	radius = (int)sqrt(owidth*owidth+oheight*oheight);
	DrawFilledArc(-radius + sx,-radius + sy, 2*radius, 2*radius, 0, 360);
	radius = (int)sqrt((x-sx)*(x-sx)+(y-sy)*(y-sy));
	DrawFilledArc(-radius + sx,-radius + sy, 2*radius, 2*radius, 0, 360);
      }
    } else if (GG->mode == ELLIPSE) {          /* draw ellipses          */
      if (GG->fill == OFF) {
	DrawArc(sx, sy, owidth, oheight, 0, 360);
	DrawArc(sx, sy, (x-sx), (y-sy), 360, 360);
      } else {                                 /* draw filled ellipses   */
	DrawFilledArc(sx, sy, owidth, oheight, 0, 360);
	DrawFilledArc(sx, sy, (x-sx), (y-sy), 360,360);
      }
    } else if (GG->mode == EDIT) {
      fprintf(stderr, "DrawMotion() called in Edit mode.\n");
    } else {
      fprintf(stderr, "DrawMotion() called with no tool selected.\n");
    }
    GG->oldx = x;
    GG->oldy = y;
  } else {
    fprintf(stderr, "DrawMotion() called with left mouse button up.\n");
  }
}

/*
 * Called when mouse motion events indicate a dragging action in the
 * DrawingArea.  The arguments x and y are the current position of the
 * mouse in the window. 
 */
void DragMotion(Widget w, int x, int y, void *data)
{
  PgmData *GG=(PgmData *)data;
  XPoint *xpts;
  char *textline;
  int oldx, oldy, sx, sy, owidth, oheight, i, npts, offsety;
  
  /* Do nothing unless left mouse button is down in edit mode. */
  if(GG->down == M_LEFT && GG->mode == EDIT) {
    oldx = GG->Obj.bb.left_edge;
    oldy = GG->Obj.bb.top_edge;
    sx = x - GG->startx;
    sy = y - GG->starty;
    owidth = GG->Obj.bb.width;
    oheight = GG->Obj.bb.height;
    if (GG->Obj.object_type == text) {
      if (GG->Obj.txt != NULL) {
	offsety = FontHeight(GG->TextFont);
	strncpy(GG->buffer, GG->Obj.txt, STR_SIZ);
	textline = strtok(GG->buffer, "\n");
	while (textline != NULL) {
	  DrawText(textline, oldx, oldy + offsety);
	  DrawText(textline, sx, sy + offsety);
	  offsety += FontHeight(GG->TextFont);
	  textline = strtok(NULL, "\n");
	}
	oldx = x;
	oldy = y;
      }
    } else if (GG->Obj.object_type == line) {
      DrawLine(GG->Obj.v->xp.x,       GG->Obj.v->xp.y,
	       GG->Obj.v->next->xp.x, GG->Obj.v->next->xp.y);
      GG->Obj.v->xp.x += x - GG->oldx;
      GG->Obj.v->xp.y += y - GG->oldy;
      GG->Obj.v->next->xp.x += x - GG->oldx;
      GG->Obj.v->next->xp.y += y - GG->oldy;
      DrawLine(GG->Obj.v->xp.x,       GG->Obj.v->xp.y,
	       GG->Obj.v->next->xp.x, GG->Obj.v->next->xp.y);
      GG->oldx = x;
      GG->oldy = y;
    } else if (GG->Obj.fill_attribute == OFF) {
      if (GG->Obj.object_type == polyline) {
	npts = GG->Obj.npts;
	xpts = (XPoint *)malloc(npts * sizeof(XPoint));
	for (i=0; i< npts; i++) {
	  xpts[i] = GG->Obj.v->xp;
	  GG->Obj.v=GG->Obj.v->next;
	}
	DrawPolyline(xpts,npts);
	for (i=0; i< npts; i++) {
	  GG->Obj.v->xp.x += x - GG->oldx;
	  GG->Obj.v->xp.y += y - GG->oldy;
	  xpts[i] = GG->Obj.v->xp;
	  GG->Obj.v=GG->Obj.v->next;
	}
	DrawPolyline(xpts,npts);
	free(xpts);
	GG->oldx = x;
	GG->oldy = y;
      } else if (GG->Obj.object_type == rectangle) {  /* draw rectangle    */
	DrawBox(oldx, oldy, owidth, oheight);
	DrawBox(sx, sy, owidth, oheight);
      } else {                                  /* draw an ellipse/circle  */
	DrawArc(oldx, oldy, owidth, oheight, 0, 360);
	DrawArc(sx, sy, owidth, oheight, 0, 360);
      }
    } else {
      if (GG->Obj.object_type == polyline) {
	npts = GG->Obj.npts;
	xpts = (XPoint *)malloc(npts * sizeof(XPoint));
	for (i=0; i< npts; i++) {
	  xpts[i] = GG->Obj.v->xp;
	  GG->Obj.v=GG->Obj.v->next;
	}
	DrawFilledPolygon(xpts,npts);
	for (i=0; i< npts; i++) {
	  GG->Obj.v->xp.x += x - GG->oldx;
	  GG->Obj.v->xp.y += y - GG->oldy;
	  xpts[i] = GG->Obj.v->xp;
	  GG->Obj.v=GG->Obj.v->next;
	}
	DrawFilledPolygon(xpts,npts);
	free(xpts);
	GG->oldx = x;
	GG->oldy = y;
      } else if (GG->Obj.object_type == rectangle) {/* draw filled rect    */
	DrawFilledBox(oldx, oldy, owidth, oheight);
	DrawFilledBox(sx, sy, owidth, oheight);
      } else {                               /* draw filled ellipse/circle */
	DrawFilledArc(oldx, oldy, owidth, oheight, 0, 360);
	DrawFilledArc(sx, sy, owidth, oheight, 0, 360);
      }
    }
    GG->Obj.bb.left_edge = sx;
    GG->Obj.bb.top_edge = sy;
  } else {
    fprintf(stderr, "DragMotion() called while not in object-drag mode.\n");
  }
}

static void ButtonDown(Widget w, int which_button, int x, int y, void *data)
{
  PgmData *GG=(PgmData *)data;
  
  GG->down = which_button;

  if (which_button == M_LEFT && GG->mode == TEXT) {
    if (GG->writing == FALSE) {
      GG->writing = TRUE;
      GG->Obj.object_type = text;
    } else {
      AddTextObjToList(GG);
    }
    GG->Obj.bb.left_edge = GG->startx = GG->oldx = x;
    GG->Obj.bb.top_edge = y - FontHeight(GG->TextFont);
    GG->starty = GG->oldy = y;
    GG->Obj.bb.width = GG->Obj.bb.height = 0;
    SetColor(FgColor);
    GG->buffer[0] = '\0';
  } else if (which_button == M_LEFT && GG->mode == POLYLINE) {
    if(GG->drawing == FALSE) {
      GG->Obj.v = MakeVertex(NULL);
      GG->Obj.npts = 1;
      GG->Obj.v->xp.x = GG->oldx = GG->startx = x;
      GG->Obj.v->xp.y = GG->oldy = GG->starty = y;
      GG->Obj.bb.left_edge = GG->BBRightEdge = x;
      GG->Obj.bb.top_edge  = GG->BBBottomEdge = y;
      GG->drawing = TRUE;
      SetColor(FgColor);
      SetLineWidth(GG->LineWidth);
      SetMouseMotionCB(w, DrawMotion);
      SetDrawMode(SANE_XOR);
    } else {
      GG->Obj.bb.left_edge = MIN(GG->Obj.bb.left_edge,x);
      GG->Obj.bb.top_edge  = MIN(GG->Obj.bb.top_edge,y);
      GG->BBRightEdge  = MAX(GG->BBRightEdge,x);
      GG->BBBottomEdge = MAX(GG->BBBottomEdge,y);
      GG->Obj.v = MakeVertex(GG->Obj.v);
      GG->Obj.npts++;
      GG->Obj.v->xp.x = GG->oldx = GG->startx = x;
      GG->Obj.v->xp.y = GG->oldy = GG->starty = y;
    }
  } else if (which_button == M_MIDDLE && GG->mode == POLYLINE &&
	     GG->drawing == TRUE) {
    GG->Obj.object_type = polyline;
    GG->Obj.bb.left_edge = MIN(GG->Obj.bb.left_edge,x);
    GG->Obj.bb.top_edge  = MIN(GG->Obj.bb.top_edge,y);
    GG->BBRightEdge  = MAX(GG->BBRightEdge,x);
    GG->BBBottomEdge = MAX(GG->BBBottomEdge,y);
    GG->Obj.bb.width = abs(GG->Obj.bb.left_edge - GG->BBRightEdge);
    GG->Obj.bb.height = abs(GG->Obj.bb.top_edge - GG->BBBottomEdge);
    GG->Obj.v = MakeVertex(GG->Obj.v);
    GG->Obj.npts++;
    GG->Obj.v->xp.x = x;
    GG->Obj.v->xp.y = y;
    GG->Obj.color = FgColor;
    GG->Obj.LineWidth = GG->LineWidth;
    GG->Obj.fill_attribute = GG->fill;
    /* Don't clutter the linked-list with invisible */
    /* points.  Only store objects with extent.     */
    if ( !(GG->Obj.bb.width == 0 && GG->Obj.bb.height == 0) ) {
      Insert( GG->Obj, GG->L, GG->P_Top );
      GG->P_Top = Advance( GG->P_Top );
    }
    GG->drawing = FALSE;
    SetDrawMode(GXcopy);   /* sets things back to normal */
    SetMouseMotionCB(w, NULL);
    /* redisplay() draws the polygon for both filled and unfilled cases */
    redisplay(canvas, GG->canvas_width, GG->canvas_height, GG);
  } else if ( which_button == M_LEFT && GG->mode != EDIT &&
      (GG->mode == LINE ||
       GG->mode == RECT ||        /* If the left mouse button is down */
       GG->mode == CIRCLE ||      /* and a drawing tool is active,    */
       GG->mode == ELLIPSE) ) {   /* set up drawing mode.             */

    GG->oldx = GG->startx = x;
    GG->oldy = GG->starty = y;

    SetColor(FgColor);
    SetLineWidth(GG->LineWidth);
    SetMouseMotionCB(w, DrawMotion);
    SetDrawMode(SANE_XOR);

  } else if (which_button == M_LEFT &&
	     GG->mode == EDIT && ! IsEmpty(GG->L)) {
    int num = 0;                    /* If the left mouse button is down */
    ElementType Tmp;                /* in edit mode try to select item. */
    ListPosition P_TopOne = NULL, P_Tmp = Header( GG->L );

    do {
      P_Tmp = Advance( P_Tmp );
      Tmp = Retrieve( P_Tmp );
      if ( inObject(Tmp, x, y) ) {    /* Find uppermost item in the       */
	GG->Obj = Retrieve( P_Tmp );  /* bounding box of the object.      */
	P_TopOne = P_Tmp;
	num++;
      }
    }  while( !IsLast( P_Tmp, GG->L ) );
    if (P_TopOne != NULL) {
      if ( !IsLast(P_TopOne, GG->L) ) {        /* if selected item is not */
	Delete(GG->Obj, GG->L);                /* at top of list, move it */
	Insert(GG->Obj, GG->L, GG->P_Top);     /* to the top              */
	GG->P_Top = Advance( GG->P_Top );
      }

      GG->oldx = x;
      GG->oldy = y;
      GG->startx = x - GG->Obj.bb.left_edge; /* stores mouse offset within */
      GG->starty = y - GG->Obj.bb.top_edge;  /* bounding box of object     */

      SetFgColor(canvas, GG->Obj.color);
      SetLineWidth(GG->Obj.LineWidth);
      SetMouseMotionCB(w, DragMotion);
      SetDrawMode(SANE_XOR);
    }
  }
}

static void ButtonUp(Widget w, int which_button, int x, int y, void *data)
{
  PgmData *GG=(PgmData *)data;
  ElementType Obj;
  int radius;
  
  /* Check to see whether an object has been drawn. */
  if (GG->mode != EDIT && which_button == M_LEFT) {
    if (GG->mode == POLYLINE) {
      return;
    } else {
      switch(GG->mode) {
      case LINE:
	Obj.object_type = line;
	break;
      case RECT:
	Obj.object_type = rectangle;
	break;
      case ELLIPSE:
	Obj.object_type = ellipse;
	break;
      case CIRCLE:
	Obj.object_type = circle;
	break;
      default: /* No tool was selected. Just return. */
	GG->down = 0;
	return;
      }
      if (Obj.object_type == line) {
	Obj.bb.left_edge = MIN(GG->startx,x);
	Obj.bb.top_edge = MIN(GG->starty,y);
	Obj.bb.width = abs(GG->startx - x);
	Obj.bb.height = abs(GG->starty - y);
	Obj.v = MakeVertex(NULL);
	Obj.v->xp.x = GG->startx;
	Obj.v->xp.y = GG->starty;
	Obj.v = MakeVertex(Obj.v);
	Obj.v->xp.x = x;
	Obj.v->xp.y = y;
	Obj.npts = 2;
      } else if (Obj.object_type == circle) { /* save circle bounding box */
	radius = sqrt((GG->startx - x)*(GG->startx - x) +
		      (GG->starty - y)*(GG->starty - y));
	Obj.bb.left_edge = GG->startx - radius;
	Obj.bb.top_edge = GG->starty - radius;
	Obj.bb.width = 2*radius;
	Obj.bb.height = 2*radius;
	Obj.v=NULL; Obj.npts = 0;
      } else {
	Obj.bb.left_edge = MIN(GG->startx,x);
	Obj.bb.top_edge = MIN(GG->starty,y);
	Obj.bb.width = abs(GG->startx - x);
	Obj.bb.height = abs(GG->starty - y);
	Obj.v=NULL; Obj.npts = 0;
      }
      Obj.color = FgColor;
      Obj.LineWidth = GG->LineWidth;
      Obj.fill_attribute = GG->fill;
      /* Don't clutter the linked-list with invisible */
      /* points.  Only store objects with extent.     */
      if ( !(Obj.bb.width == 0 && Obj.bb.height == 0) ) {
	Insert( Obj, GG->L, GG->P_Top );
	GG->P_Top = Advance( GG->P_Top );
      }
    }
  } else if (GG->mode == EDIT) {
    if(which_button == M_LEFT) {
      HandleEditMBLeft(x, y, GG);
    } else {
      HandleEditMBCenterRight(x, y, GG);
    }
  }

  GG->down = 0;
  SetDrawMode(GXcopy);   /* sets things back to normal */
  SetMouseMotionCB(w, NULL);

  redisplay(canvas, GG->canvas_width, GG->canvas_height, GG);
}

static void FileLoad(Widget w, void *data)
{
  PgmData *GG = (PgmData *)data;
  FILE *fp;
  char *fname;
  char record[STR_SIZ];
  int r, g, b;
  int ObjectType, Left, Top, Width, Height, Color, LineWidth, Fill, Npts;
  int i, items, x, y, nlines = 0, tmp, sign, xtrans;
  int multiplier, divisor;
  char SEPCHARS[]="(,) \n";
  char *word;

  if (GG->FileName == NULL)
    fname = SimpleGetFile(NULL);
  else
    fname = GG->FileName;
  if(fname) {
    if ((fp = fopen(fname,"r")) != NULL) {
      fgets(record,STR_SIZ - 1,fp);
      nlines++;
      if (strncmp(&record[1], Version, strlen(Version)) != 0) {
	fprintf(stderr, "File [%s]\n", fname);
	fprintf(stderr, "has the following invalid data at line [%d]:\n",
		nlines);
	i = 0;
	while(isprint(record[i])) {
	  fprintf(stderr,"%c",record[i]);
	  i++;
	}
	if (i > 0) {
	  fprintf(stderr,"\n");
	} else {
	  fprintf(stderr, "Encountered non-printable characters.\n");
	}
	fclose(fp);
	if(fname != GG->FileName)
	  free(fname);
	return;
      } else {
	if (fname == GG->FileName) {
	  multiplier = GG->multiplier;
	  divisor    = GG->divisor;
	  if (GG->reverse == ON) {
	    sign = -1;
	    xtrans = GG->canvas_width;
	  } else {
	    sign = 1;
	    xtrans = 0;
	  }
	} else {
	  multiplier = 10;
	  divisor    = 10;
	  sign       =  1;
	  xtrans     =  0;
	}
	while (fgets(record, STR_SIZ - 1, fp) != NULL) {
	  nlines++;
	  if(record[0] != '#' && strcmp(record,"\n") != 0) {
	    items = sscanf(record, "%d %d %d %d %d %d %d %d %d %d %d \n",
			   &ObjectType, &Left, &Top, &Width, &Height,
			   &r, &g, &b, &LineWidth, &Fill, &Npts);
	    if (items == 11) {
	      GG->Obj.object_type    = ObjectType;
	      GG->Obj.bb.left_edge   =
		sign*(Left*multiplier)/divisor+xtrans;
	      GG->Obj.bb.top_edge    = (Top*multiplier)/divisor;
	      GG->BBRightEdge        =
		sign*((Left+Width)*multiplier)/divisor + xtrans;
	      GG->BBBottomEdge       =((Top+Height)*multiplier)/divisor;
	      if (sign == -1) {
		tmp = GG->Obj.bb.left_edge;
		GG->Obj.bb.left_edge = GG->BBRightEdge;
		GG->BBRightEdge  = tmp;
	      }
	      GG->Obj.bb.width       = GG->BBRightEdge - GG->Obj.bb.left_edge;
	      GG->Obj.bb.height      = GG->BBBottomEdge - GG->Obj.bb.top_edge;
	      if( (Color = RGBtoColorTableIndex(r, g, b)) >= 0 )
		GG->Obj.color        = Color;
	      else
		GG->Obj.color        = FgColor;
	      GG->Obj.LineWidth      = LineWidth;
	      GG->Obj.fill_attribute = Fill;
	      GG->Obj.npts           = Npts;
	      GG->Obj.v              = NULL;
	      GG->Obj.txt            = NULL;
	      if( Npts != 0 ) {
		if (Npts < 0) {
		  fprintf(stderr, "In [%s] at line [%d] ", fname, nlines);
		  fprintf(stderr, "encountered vertex length [%d]\n", Npts);
		  fclose(fp);
		  if(fname != GG->FileName)
		    free(fname);
		  return;
		} else {
		  GG->buffer[0]='\0';
		  while (Npts>0 && fgets(record, STR_SIZ - 1, fp) != NULL) {
		    if (GG->Obj.object_type == text) {
		      strncat(GG->buffer, record, STR_SIZ);
		      Npts -= strlen(record);
		    } else {
		      nlines++;
		      word = strtok(record, SEPCHARS);
		      while (word != NULL) {
			GG->Obj.v = MakeVertex(GG->Obj.v);
			if (sscanf(word, "%d", &x) != 1) {
			  fprintf(stderr, "In [%s] at line [%d]\n",
				  fname,nlines);
			  fprintf(stderr, "failed to locate x-coordinate in ");
			  fprintf(stderr, "word [%s].  Remainder of ", word);
			  fprintf(stderr, "record is:\n[%s]\n", record);
			  fclose(fp);
			  if(fname != GG->FileName)
			    free(fname);
			  return;
			}
			GG->Obj.v->xp.x = sign*(x*multiplier)/divisor+xtrans;
			if( GG->Obj.v->xp.x < GG->Obj.bb.left_edge ||
			    (GG->Obj.v->xp.x > GG->BBRightEdge) ) {
			  fprintf(stderr, "In [%s] at line [%d]\n",
				  fname,nlines);
			  fprintf(stderr, "found x-coordinate [%d] ", x);
			  fprintf(stderr, "outside bounding box range ");
			  fprintf(stderr, "[%d]-[%d]\n",Left,Left+Width);
			  fclose(fp);
			  if(fname != GG->FileName)
			    free(fname);
			  return;
			}
			word = strtok(NULL, SEPCHARS);
			if (sscanf(word, "%d", &y) != 1) {
			  fprintf(stderr, "In [%s] at line [%d]\n",
				  fname,nlines);
			  fprintf(stderr, "failed to locate y-coordinate in ");
			  fprintf(stderr, "word [%s].  Remainder of ", word);
			  fprintf(stderr, "record is:\n[%s]\n", record);
			  fclose(fp);
			  if(fname != GG->FileName)
			    free(fname);
			  return;
			}
			GG->Obj.v->xp.y = (y*multiplier)/divisor;
			if( GG->Obj.v->xp.y < GG->Obj.bb.top_edge ||
			    (GG->Obj.v->xp.y > GG->BBBottomEdge) ) {
			  fprintf(stderr, "In [%s] at line [%d]\n",
				  fname,nlines);
			  fprintf(stderr, "found y-coordinate [%d] ", y);
			  fprintf(stderr, "outside bounding box range ");
			  fprintf(stderr, "[%d]-[%d]\n",Top,Top+Height);
			  fclose(fp);
			  if(fname != GG->FileName)
			    free(fname);
			  return;
			}
			Npts--;
			word = strtok(NULL, SEPCHARS);
		      }
		    }
		  }
		}
	      }
	      if (GG->Obj.object_type != text) {
		Insert( GG->Obj, GG->L, GG->P_Top );
		GG->P_Top = Advance( GG->P_Top );
	      } else {
		GG->Obj.txt = strdup(GG->buffer);
		if (GG->Obj.txt != NULL) {
		  Insert( GG->Obj, GG->L, GG->P_Top );
		  GG->P_Top = Advance( GG->P_Top );
		} else {
		  fprintf(stderr, "Unable to allocate memory for string\n");
		  fprintf(stderr, "[%s]\n", GG->buffer);
		}
	      }
	    } else {
	      fprintf(stderr, "In [%s] at line [%d] ", fname, nlines);
	      fprintf(stderr, "expected a new object; encountered:\n[%s]\n",
		      record);
	    }
	  }
	}
      }
      fclose(fp);
    }
    if(fname != GG->FileName)
      free(fname);
    return;
  }
}

static void FileSave(Widget w, void *data)
{
  PgmData *GG = (PgmData *)data;
  FILE *fp;
  char *fname;
  char buffer[STR_SIZ];
  
  fname = SimpleGetFile(NULL);
  if(fname) {
    if ((fp = fopen(fname,"r")) != NULL) {         /* Don't allow the user  */
      sprintf(buffer, "Over-write %s?\n", fname);  /* to accidentally over- */
      if (GetYesNo(buffer) != TRUE) {              /* write existing files. */
	fclose(fp);
	free(fname);
	return;
      } else {
	fclose(fp);
      }
    }
    if ((fp = fopen(fname,"w")) != NULL) {
      fprintf(fp, "#%s\n",Version);
      fprintf(fp, "# Object types:\n# 0 - line; 1 - rectangle; ");
      fprintf(fp, "2 - ellipse; 3 - circle; 4 - polyline/polygon;");
      fprintf(fp, "# 5 - line");
      PrintList(fp, GG->L);
      fclose(fp);
    } else {
      sprintf(buffer, "Cannot write to [%s]\nCheck file permissions.", fname);
      Message(buffer);
    }
    free(fname);
  }
}

static void DinoDraw(Widget w, void *data)
{
  PgmData *GG = (PgmData *)data;
  char *tmp;

  tmp = GG->FileName;
  if (w == GG->Wdino[0])
    GG->FileName = strdup("./gg.guy");
  else if (w == GG->Wdino[1])
    GG->FileName = strdup("./bg.guy");
  else if (w == GG->Wdino[2])
    GG->FileName = strdup("./yg.guy");
  else if (w == GG->Wdino[3])
    GG->FileName = strdup("./pg.guy");
  else if (w == GG->Wdino[4])
    GG->FileName = strdup("./rg.guy");
  else
    GG->FileName = NULL;
  FileLoad(w,GG);
  free(GG->FileName);
  GG->FileName = tmp;
  SetToggleState(GG->Wedit, TRUE);
  redisplay(canvas, GG->canvas_width, GG->canvas_height, GG);
}

/*
 * This function sets up the display for us.
 */

int init_display(int argc, char **argv, PgmData *GG)
{
  Widget TopLeft, TopRight, Palette[LastColor+1], File[4], Space[3];
  Widget Wedit, Wline, Wrect, Wellipse, Wcircle, Wpolyline, Wclear;
  Widget Wdino[5];
  Widget WscaleLabel, WscaleIncr, WscaleDecr, Wreverse;
  Widget Wfill, LWLabel, LWScrollBar;
  Widget Wtext;
  char tmp[30];
  int i;

  argc = OpenDisplay(argc, argv);
  if (argc == FALSE)
    return argc;

  GG->canvas_width  = DRAWWIDTH;
  GG->canvas_height = DRAWHEIGHT;

  GG->FileName = NULL;

  GG->LineWidth = DEF_LINEWIDTH;
  GG->writing = FALSE;

  GG->multiplier = 6;
  GG->divisor    = MAX_MULT;

  GG->L     = MakeEmpty(NULL);
  GG->P_Top = Header(GG->L);

  TopLeft  = MakeLabel("    "); /* Sets width of left border  */
  TopRight = MakeLabel("    "); /* Sets width of right border */

  File[0] = MakeMenu("File");
  File[1] = MakeMenuItem(File[0], "Load ..", FileLoad, GG);
  File[2] = MakeMenuItem(File[0], "Save as ..", FileSave, GG);
  File[3] = MakeMenuItem(File[0], "Quit", Quit, GG);

  for (i=0; i < 3; i++)
    Space[i] = MakeLabel(NULL);

  ColorMenu[0] = MakeMenu("    ");
  ColorMenu[1] = MakeMenuItem(ColorMenu[0], "More Colors...", ColorFunc,
			      (void *)(LastColor+1));
  ColorMenu[2] = MakeMenuItem(ColorMenu[0], "Cancel", NULL, NULL);

  for (long i=FirstColor; i <= LastColor; i++)
    Palette[i] = MakeButton(" ", ColorFunc, (void *)i);

  canvas = MakeDrawArea(GG->canvas_width, GG->canvas_height, redisplay, GG);

  Wedit     = MakeToggle("Edit", FALSE, NULL,  Edit,     GG);
  Wline     = MakeToggle(NULL,   FALSE, Wedit, Line,     GG);
  Wrect     = MakeToggle(NULL,   FALSE, Wedit, Rect,     GG);
  Wellipse  = MakeToggle(NULL,   FALSE, Wedit, Ellipse,  GG);
  Wcircle   = MakeToggle(NULL,   FALSE, Wedit, Circle,   GG);
  Wpolyline = MakeToggle(NULL,   TRUE,  Wedit, PolyLine, GG);
  Wtext     = MakeToggle(NULL,   FALSE, Wedit, Text,     GG);

  Wfill     = MakeToggle("Fill", TRUE,  NULL, Fill, GG);

  Wclear    = MakeButton("Clear", Clear, GG);
  for (i=0; i<5; i++)
    Wdino[i]  = MakeButton(NULL, DinoDraw, GG);

  Wreverse    = MakeToggle(NULL, FALSE, NULL, reverse, GG);

  sprintf(tmp, "%3d\n --\n%3d", GG->multiplier, GG->divisor);
  WscaleLabel = MakeLabel(tmp);
  WscaleIncr  = MakeButton("+", ScaleIncr, GG);
  WscaleDecr  = MakeButton("-", ScaleDecr, GG);

  LWLabel = MakeLabel("Linewidth  1 ");
  LWScrollBar = MakeHorizScrollbar(MAX_LINEWIDTH+1,
				   (void *)LWscrollbarCBfunc, GG);

  SetWidgetPos(File[0], PLACE_RIGHT, TopLeft, NO_CARE, NULL);
  SetWidgetPos(TopRight, PLACE_RIGHT, canvas, NO_CARE, NULL);
  SetWidgetPos(ColorMenu[0], PLACE_UNDER, Palette[LastColor], NO_CARE, NULL);
  SetWidgetPos(Space[0], PLACE_UNDER, TopLeft, NO_CARE, NULL);
  SetWidgetPos(Palette[0], PLACE_UNDER, Space[0], NO_CARE, NULL);
  SetWidgetPos(Palette[1], PLACE_UNDER, Space[0], PLACE_RIGHT, Palette[0]);

  for (i=FirstColor+2; i<LastColor; i+=2) {
    SetWidgetPos(Palette[i], PLACE_UNDER, Palette[i-2], NO_CARE, NULL);
    SetWidgetPos(Palette[i+1], PLACE_UNDER, Palette[i-2],
		 PLACE_RIGHT, Palette[i]);
  }

  SetWidgetPos(canvas, PLACE_RIGHT, TopLeft, PLACE_UNDER, TopLeft);
  SetWidgetPos(Space[1], PLACE_UNDER, ColorMenu[0], NO_CARE, NULL);
  SetWidgetPos(Wline, PLACE_UNDER, Space[1], NO_CARE, NULL);
  SetWidgetPos(Wrect, PLACE_UNDER, Wline, NO_CARE, NULL);
  SetWidgetPos(Wellipse, PLACE_UNDER, Wrect, NO_CARE, NULL);
  SetWidgetPos(Wcircle, PLACE_UNDER, Wellipse, NO_CARE, NULL);
  SetWidgetPos(Wpolyline, PLACE_UNDER, Wcircle, NO_CARE, NULL);
  SetWidgetPos(Wtext, PLACE_UNDER, Wpolyline, NO_CARE, NULL);
  SetWidgetPos(Wedit, PLACE_RIGHT, File[0], NO_CARE, NULL);
  SetWidgetPos(Wclear, PLACE_RIGHT, Wedit, NO_CARE, NULL);
  SetWidgetPos(Wfill, PLACE_RIGHT, TopLeft, PLACE_UNDER, canvas);
  SetWidgetPos(LWLabel, PLACE_RIGHT, Wfill, PLACE_UNDER, canvas);
  SetWidgetPos(LWScrollBar, PLACE_RIGHT, LWLabel, PLACE_UNDER, canvas);
  SetWidgetPos(Space[2], PLACE_RIGHT, canvas, PLACE_UNDER, TopRight);
  SetWidgetPos(Wdino[0], PLACE_RIGHT, canvas, PLACE_UNDER, Space[2]);
  SetWidgetPos(Wdino[1], PLACE_RIGHT, canvas, PLACE_UNDER, Wdino[0]);
  SetWidgetPos(Wdino[2], PLACE_RIGHT, canvas, PLACE_UNDER, Wdino[1]);
  SetWidgetPos(Wdino[3], PLACE_RIGHT, canvas, PLACE_UNDER, Wdino[2]);
  SetWidgetPos(Wdino[4], PLACE_RIGHT, canvas, PLACE_UNDER, Wdino[3]);
  SetWidgetPos(Wreverse, PLACE_RIGHT, canvas, PLACE_UNDER, Wdino[4]);
  SetWidgetPos(WscaleLabel, PLACE_RIGHT, canvas,     PLACE_UNDER, Wreverse);
  SetWidgetPos(WscaleDecr,  PLACE_RIGHT, canvas,     PLACE_UNDER, WscaleLabel);
  SetWidgetPos(WscaleIncr,  PLACE_RIGHT, WscaleDecr, PLACE_UNDER, WscaleLabel);

  SetScrollbar(LWScrollBar, DEF_LINEWIDTH, MAX_LINEWIDTH, 1);

  for (i=0; i< 3; i++)
    SetWidgetBitmap(Space[i], space_bits, space_width, space_height);
  SetWidgetBitmap(Wline, line_bits, line_width, line_height);
  SetWidgetBitmap(Wrect, rect_bits, rect_width, rect_height);
  SetWidgetBitmap(Wellipse, ellipse_bits, ellipse_width, ellipse_height);
  SetWidgetBitmap(Wcircle, circle_bits, circle_width, circle_height);
  SetWidgetBitmap(Wpolyline, polyline_bits, polyline_width, polyline_height);
  SetWidgetBitmap(Wtext, abc_bits, abc_width, abc_height);
  SetWidgetBitmap(Wreverse, reverse_bits, reverse_width, reverse_height);
  SetWidgetBitmap(Wdino[0], ggr_bits, ggr_width, ggr_height);
  SetWidgetBitmap(Wdino[1], bgr_bits, bgr_width, bgr_height);
  SetWidgetBitmap(Wdino[2], ygr_bits, ygr_width, ygr_height);
  SetWidgetBitmap(Wdino[3], pgr_bits, pgr_width, pgr_height);
  SetWidgetBitmap(Wdino[4], rgr_bits, rgr_width, rgr_height);

  GG->Wedit       = Wedit;
  GG->LWLabel     = LWLabel;
  GG->LWScrollBar = LWScrollBar;
  GG->WscaleLabel = WscaleLabel;
  for (i=0; i < 5; i++)
    GG->Wdino[i] = Wdino[i];

  ShowDisplay();

  /* initialize the state of the toggle variables. */
  PolyLine(Wpolyline, GG);    /* Sets initial mode to */
  Fill(Wfill, GG);            /* draw filled polygons */
  GG->drawing = FALSE;

  for (i = FirstColor ; i <= LastColor ; i++) {
    RGBColors[i].CTidx =
      GetRGBColor(RGBColors[i].r,RGBColors[i].g,RGBColors[i].b);
    SetBgColor(Palette[i], RGBColors[i].CTidx);
  }

  ColorFunc(canvas, (void *)green4);  /* Set default foreground color */
  BgColor = RGBColors[white].CTidx;
  SetBgColor(canvas, BgColor);

  SetBorderColor(ColorMenu[0], RGBColors[red].CTidx);

  SetBgColor(File[0], RGBColors[gray66].CTidx);
  SetBorderColor(File[0], RGBColors[blue].CTidx);

  SetBgColor(Wedit, RGBColors[gray66].CTidx);
  SetBorderColor(Wedit, RGBColors[green].CTidx);

  SetBgColor(Wclear, RGBColors[gray66].CTidx);
  SetBorderColor(Wclear, RGBColors[red].CTidx);

  SetFgColor(Wdino[0], RGBColors[green4].CTidx);
  SetFgColor(Wdino[1], RGBColors[user3].CTidx);
  SetFgColor(Wdino[2], RGBColors[orange].CTidx);
  SetFgColor(Wdino[3], RGBColors[user4].CTidx);
  SetFgColor(Wdino[4], RGBColors[user1].CTidx);

  SetLineWidth(GG->LineWidth);
  GG->reverse = OFF;

  SetButtonUpCB(canvas, ButtonUp);
  SetButtonDownCB(canvas, ButtonDown);

  GG->TextFont = GetFont("10x20");
  if (GG->TextFont == NULL) {
    fprintf(stderr, "%s\n",
	    "Failed to load \"10x20\" font, loading \"fixed\" font.");
    GG->TextFont = GetFont("fixed");
  }
  SetWidgetFont(canvas, GG->TextFont);

  return argc;
}

int main(int argc, char **argv)
{
    PgmData GG;

    memset(&GG, '\0', sizeof(PgmData));  /* clear this out! */

    argc = init_display(argc, argv, &GG);  /* setup the display */
    if (argc == 0) {
        fprintf(stderr, "%s: unable to open display - exiting\n", argv[0]);
	exit(0);
    }

    MainLoop();        /* go right into the main loop, exits after quit() */

    return 0;          /* prevents compiler warnings about control reaching
                          end of non-void function */
}

/* End of File: GreenGuy.c */
