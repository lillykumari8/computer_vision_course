#include <cmath>
#include "image.h"

using namespace std;

// HW1 #1
// float x,y: inexact coordinates
// int c: channel
// returns the nearest neibor to pixel (x,y,c)
float Image::pixel_nearest(float x, float y, int c) const
  {
  // Since you are inside class Image you can
  // use the member function pixel(a,b,c)
  
  // TODO: Your code here
  return clamped_pixel(roundf(x), roundf(y), c);
  }

// HW1 #1
// float x,y: inexact coordinates
// int c: channel
// returns the bilinearly interpolated pixel (x,y,c)
float Image::pixel_bilinear(float x, float y, int c) const
  {
  // Since you are inside class Image you can
  // use the member function pixel(a,b,c)
  // TODO: Your code here
  float x_left = floorf(x);
  float x_right = ceilf(x);
  float y_top = floorf(y);
  float y_bottom = ceilf(y);

  float v1_a1 = clamped_pixel(x_left, y_top, c)*(x_right - x)*(y_bottom - y); // v1 -> (x_left, y_top) 
  float v2_a2 = clamped_pixel(x_right, y_top, c)*(x - x_left)*(y_bottom - y); // v2 ->  (x_right, y_top)
  float v3_a3 = clamped_pixel(x_left, y_bottom, c)*(x_right - x)*(y - y_top); // v3 -> (x_left, y_bottom)
  float v4_a4 = clamped_pixel(x_right, y_bottom, c)*(x - x_left)*(y - y_top);
  return v1_a1 + v2_a2 + v3_a3 + v4_a4;

  }

// HW1 #1
// int w,h: size of new image
// const Image& im: input image
// return new Image of size (w,h,im.c)
Image nearest_resize(const Image& im, int w, int h)
  {
  Image ret(w,h,im.c);
  
  // TODO: Your code here
  float x_ratio = im.w / float(w);
  float y_ratio = im.h / float(h);

  for (int i=0; i<w; i++) {
    for (int j=0; j<h; j++) {
      for (int k=0; k<im.c; k++) {
        float temp_x = (i + 0.5)*x_ratio - 0.5;
        float temp_y = (j + 0.5)*y_ratio - 0.5;
        float pixel_value = im.pixel_nearest(temp_x, temp_y, k);
        ret(i,j,k) = pixel_value;
      }
    }
  }
  return ret;
  }


// HW1 #1
// int w,h: size of new image
// const Image& im: input image
// return new Image of size (w,h,im.c)
Image bilinear_resize(const Image& im, int w, int h)
  {
  
  // TODO: Your code here
  Image ret(w, h, im.c);

  float x_ratio = im.w / float(w);
  float y_ratio = im.h / float(h);

  for (int i=0; i<w; i++) {
    for (int j=0; j<h; j++) {
      for (int k=0; k<im.c; k++) {
        float temp_x = (i + 0.5)*x_ratio - 0.5;
        float temp_y = (j + 0.5)*y_ratio - 0.5;
        float pixel_value = im.pixel_bilinear(temp_x, temp_y, k);
        ret(i,j,k) = pixel_value;
      }
    }
  }
  return ret;
  }
