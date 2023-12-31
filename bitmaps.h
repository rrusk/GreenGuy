#ifndef __BITMAP_H__
#define __BITMAP_H__

#define line_width 28
#define line_height 24
static unsigned char line_bits[] = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0xff, 0x7f, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

#define rect_width 28
#define rect_height 24
static unsigned char rect_bits[] = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0xc0, 0xff, 0x3f, 0x00, 0x40, 0x00, 0x20, 0x00,
   0x40, 0x00, 0x20, 0x00, 0x40, 0x00, 0x20, 0x00, 0x40, 0x00, 0x20, 0x00,
   0x40, 0x00, 0x20, 0x00, 0x40, 0x00, 0x20, 0x00, 0x40, 0x00, 0x20, 0x00,
   0xc0, 0xff, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

#define ellipse_width 28
#define ellipse_height 24
static unsigned char ellipse_bits[] = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0xf8, 0x03, 0x00, 0x00, 0x0e, 0x0e, 0x00, 0x00, 0x03, 0x18, 0x00,
   0x80, 0x01, 0x30, 0x00, 0xc0, 0x00, 0x60, 0x00, 0x60, 0x00, 0xc0, 0x00,
   0x20, 0x00, 0x80, 0x00, 0x60, 0x00, 0xc0, 0x00, 0xc0, 0x00, 0x60, 0x00,
   0x80, 0x01, 0x30, 0x00, 0x00, 0x03, 0x18, 0x00, 0x00, 0x0c, 0x06, 0x00,
   0x00, 0xf8, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

#define circle_width 28
#define circle_height 24
static unsigned char circle_bits[] = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0xf0, 0x01, 0x00, 0x00, 0x4c, 0x06, 0x00, 0x00, 0x43, 0x18, 0x00,
   0x80, 0x40, 0x20, 0x00, 0x80, 0x40, 0x20, 0x00, 0x40, 0x40, 0x40, 0x00,
   0x40, 0x40, 0x40, 0x00, 0x20, 0x40, 0x80, 0x00, 0x20, 0x40, 0x80, 0x00,
   0x20, 0x40, 0x80, 0x00, 0x20, 0x00, 0x80, 0x00, 0x20, 0x00, 0x80, 0x00,
   0x40, 0x00, 0x40, 0x00, 0x40, 0x00, 0x40, 0x00, 0x80, 0x00, 0x20, 0x00,
   0x80, 0x00, 0x20, 0x00, 0x00, 0x03, 0x18, 0x00, 0x00, 0x0c, 0x06, 0x00,
   0x00, 0xf0, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

#define polyline_width 28
#define polyline_height 24
static unsigned char polyline_bits[] = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x80, 0x00, 0x00, 0x00, 0x40, 0x01, 0x00, 0x00, 0x20, 0x02, 0x00,
   0x00, 0x10, 0x0c, 0x00, 0x00, 0x10, 0x10, 0x00, 0x00, 0x08, 0x20, 0x00,
   0x00, 0x04, 0x20, 0x00, 0x00, 0x02, 0x10, 0x00, 0x00, 0x00, 0x10, 0x00,
   0x00, 0x00, 0x10, 0x00, 0x40, 0x00, 0x10, 0x00, 0x80, 0x20, 0x08, 0x00,
   0x80, 0x58, 0x08, 0x00, 0x00, 0x87, 0x08, 0x00, 0x00, 0x01, 0x09, 0x00,
   0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

#define abc_width 28
#define abc_height 24
static unsigned char abc_bits[] = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0xf1, 0x38, 0x00, 0x80, 0x22, 0x45, 0x00,
   0x40, 0x24, 0x05, 0x00, 0x40, 0x24, 0x05, 0x00, 0x40, 0xe4, 0x04, 0x00,
   0xc0, 0x27, 0x05, 0x00, 0x40, 0x24, 0x05, 0x00, 0x40, 0x24, 0x45, 0x00,
   0x40, 0xf4, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

#define ggl_width 28
#define ggl_height 24
static unsigned char ggl_bits[] = {
   0x00, 0x00, 0x00, 0x00, 0x80, 0x03, 0x00, 0x00, 0xc0, 0x07, 0x00, 0x00,
   0xc0, 0x07, 0x00, 0x00, 0xe0, 0x07, 0x00, 0x00, 0xf0, 0x0f, 0x00, 0x00,
   0xf8, 0x1f, 0x00, 0x00, 0x70, 0x3f, 0x00, 0x00, 0x20, 0x7f, 0x00, 0x00,
   0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x01, 0x00, 0x00, 0xff, 0x03, 0x00,
   0x00, 0xff, 0x07, 0x00, 0x00, 0xfe, 0x0f, 0x00, 0x00, 0xfe, 0x1f, 0x00,
   0x00, 0xfc, 0x3f, 0x00, 0x00, 0xfe, 0x7b, 0x00, 0x00, 0xde, 0xf3, 0x00,
   0x00, 0xde, 0xe7, 0x01, 0x00, 0x9e, 0xc7, 0x03, 0x00, 0x9e, 0x07, 0x03,
   0x00, 0xcf, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

#define ggr_width 28
#define ggr_height 24
static unsigned char ggr_bits[] = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x3e, 0x00,
   0x00, 0x00, 0x3e, 0x00, 0x00, 0x00, 0x7e, 0x00, 0x00, 0x00, 0xff, 0x00,
   0x00, 0x80, 0xff, 0x01, 0x00, 0xc0, 0xef, 0x00, 0x00, 0xe0, 0x4f, 0x00,
   0x00, 0xf0, 0x0f, 0x00, 0x00, 0xf8, 0x0f, 0x00, 0x00, 0xfc, 0x0f, 0x00,
   0x00, 0xfe, 0x0f, 0x00, 0x00, 0xff, 0x07, 0x00, 0x80, 0xff, 0x07, 0x00,
   0xc0, 0xff, 0x03, 0x00, 0xe0, 0xfd, 0x07, 0x00, 0xf0, 0xbc, 0x07, 0x00,
   0x78, 0xbe, 0x07, 0x00, 0x3c, 0x9e, 0x07, 0x00, 0x0c, 0x9e, 0x07, 0x00,
   0x00, 0x3c, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

#define bgl_width 28
#define bgl_height 24
static unsigned char bgl_bits[] = {
   0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x00,
   0xf8, 0x01, 0x00, 0x00, 0xfc, 0x01, 0x00, 0x00, 0xfc, 0x03, 0x00, 0x00,
   0xfe, 0x03, 0x00, 0x00, 0xfe, 0x07, 0x00, 0x00, 0xcc, 0x07, 0x00, 0x00,
   0xc0, 0x0f, 0x00, 0x00, 0xc0, 0x1f, 0x00, 0x00, 0x80, 0xff, 0x1f, 0x00,
   0x80, 0xff, 0x3f, 0x00, 0x80, 0xff, 0x7f, 0x00, 0x00, 0xff, 0xff, 0x00,
   0x00, 0xff, 0xff, 0x01, 0x00, 0xff, 0xdf, 0x03, 0x00, 0xfe, 0x1f, 0x07,
   0x00, 0x1c, 0x1c, 0x0c, 0x00, 0x1c, 0x1c, 0x00, 0x00, 0x1c, 0x1c, 0x00,
   0x00, 0x1e, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

#define bgr_width 28
#define bgr_height 24
static unsigned char bgr_bits[] = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0xf0, 0x00,
   0x00, 0x00, 0xf8, 0x01, 0x00, 0x00, 0xf8, 0x03, 0x00, 0x00, 0xfc, 0x03,
   0x00, 0x00, 0xfc, 0x07, 0x00, 0x00, 0xfe, 0x07, 0x00, 0x00, 0x3e, 0x03,
   0x00, 0x00, 0x3f, 0x00, 0x00, 0x80, 0x3f, 0x00, 0x80, 0xff, 0x1f, 0x00,
   0xc0, 0xff, 0x1f, 0x00, 0xe0, 0xff, 0x1f, 0x00, 0xf0, 0xff, 0x0f, 0x00,
   0xf8, 0xff, 0x0f, 0x00, 0xbc, 0xff, 0x0f, 0x00, 0x8e, 0xff, 0x07, 0x00,
   0x83, 0x83, 0x03, 0x00, 0x80, 0x83, 0x03, 0x00, 0x80, 0x83, 0x03, 0x00,
   0x80, 0x87, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

#define rgl_width 28
#define rgl_height 24
static unsigned char rgl_bits[] = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x80, 0x01, 0x00, 0x00, 0xc0, 0x07, 0x00, 0x00, 0xe0, 0x07, 0x00, 0x00,
   0xf0, 0x1f, 0x00, 0x00, 0xf0, 0x1f, 0x00, 0x00, 0xf0, 0x7e, 0x00, 0x00,
   0x60, 0x7e, 0x00, 0x00, 0x00, 0xff, 0x01, 0x00, 0x80, 0xff, 0x11, 0x00,
   0x80, 0xff, 0x37, 0x00, 0x00, 0xfe, 0x77, 0x00, 0x40, 0xfe, 0xe7, 0x00,
   0xc0, 0xff, 0xef, 0x00, 0xc0, 0xff, 0xff, 0x00, 0xc0, 0xff, 0xff, 0x00,
   0x80, 0xff, 0x7f, 0x00, 0x00, 0xff, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

#define rgr_width 28
#define rgr_height 24
static unsigned char rgr_bits[] = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x3e, 0x00, 0x00, 0x00, 0x7e, 0x00,
   0x00, 0x80, 0xff, 0x00, 0x00, 0x80, 0xff, 0x00, 0x00, 0xe0, 0xf7, 0x00,
   0x00, 0xe0, 0x67, 0x00, 0x00, 0xf8, 0x0f, 0x00, 0x80, 0xf8, 0x1f, 0x00,
   0xc0, 0xfe, 0x1f, 0x00, 0xe0, 0xfe, 0x07, 0x00, 0x70, 0xfe, 0x27, 0x00,
   0x70, 0xff, 0x3f, 0x00, 0xf0, 0xff, 0x3f, 0x00, 0xf0, 0xff, 0x3f, 0x00,
   0xe0, 0xff, 0x1f, 0x00, 0xc0, 0xff, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

#define ygl_width 28
#define ygl_height 24
static unsigned char ygl_bits[] = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x80, 0x04, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x82, 0xf7, 0x07, 0x00,
   0xe4, 0xff, 0x0f, 0x00, 0xf8, 0xff, 0x1f, 0x00, 0xfc, 0xff, 0x3f, 0x00,
   0xfc, 0xff, 0x7f, 0x00, 0xf8, 0xfd, 0xf7, 0x00, 0xe0, 0x34, 0xc6, 0x01,
   0x00, 0x30, 0x06, 0x03, 0x00, 0x38, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

#define ygr_width 28
#define ygr_height 24
static unsigned char ygr_bits[] = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x12, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x00, 0xfe, 0x1e, 0x04,
   0x00, 0xff, 0x7f, 0x02, 0x80, 0xff, 0xff, 0x01, 0xc0, 0xff, 0xff, 0x03,
   0xe0, 0xff, 0xff, 0x03, 0xf0, 0xfe, 0xfb, 0x01, 0x38, 0xc6, 0x72, 0x00,
   0x0c, 0xc6, 0x00, 0x00, 0x00, 0xce, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

#define pgl_width 28
#define pgl_height 24
static unsigned char pgl_bits[] = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x80, 0x04, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x82, 0xf7, 0x07, 0x00,
   0xe4, 0xff, 0x0f, 0x00, 0xf8, 0xff, 0x1f, 0x00, 0xfc, 0xff, 0x3f, 0x00,
   0xfc, 0xff, 0x7f, 0x00, 0xf8, 0xfd, 0xf7, 0x00, 0xe0, 0x34, 0xc6, 0x01,
   0x00, 0x30, 0x06, 0x03, 0x00, 0x38, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

#define pgr_width 28
#define pgr_height 24
static unsigned char pgr_bits[] = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x12, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x00, 0xfe, 0x1e, 0x04,
   0x00, 0xff, 0x7f, 0x02, 0x80, 0xff, 0xff, 0x01, 0xc0, 0xff, 0xff, 0x03,
   0xe0, 0xff, 0xff, 0x03, 0xf0, 0xfe, 0xfb, 0x01, 0x38, 0xc6, 0x72, 0x00,
   0x0c, 0xc6, 0x00, 0x00, 0x00, 0xce, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

#define reverse_width 28
#define reverse_height 24
static unsigned char reverse_bits[] = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00,
   0x00, 0x20, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00,
   0x04, 0x20, 0x00, 0x01, 0x1c, 0x20, 0xc0, 0x01, 0x64, 0x20, 0x30, 0x01,
   0x84, 0x21, 0x0c, 0x01, 0x04, 0x22, 0x02, 0x01, 0x04, 0x22, 0x02, 0x01,
   0x84, 0x21, 0x0c, 0x01, 0x64, 0x20, 0x30, 0x01, 0x1c, 0x20, 0xc0, 0x01,
   0x04, 0x20, 0x00, 0x01, 0x00, 0x20, 0x00, 0x00, 0xc0, 0x21, 0x1c, 0x00,
   0xc0, 0x20, 0x18, 0x00, 0x40, 0x23, 0x16, 0x00, 0x00, 0xfc, 0x01, 0x00,
   0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

#define space_width 28
#define space_height 6
static unsigned char space_bits[] = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

#endif /* __BITMAP_H__ */
