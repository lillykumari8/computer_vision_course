#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "image.h"

#define M_PI 3.14159265358979323846

// HW1 #2.1
// Image& im: image to L1-normalize
void l1_normalize(Image& im)
  {
  
  // TODO: Normalize each channel
  float norm_denom = 0;
  for (int i=0; i<im.w; i++) {
    for (int j=0; j<im.h; j++) {
      for (int k=0; k<im.c; k++) {
        norm_denom += im(i,j,k);
      }
    }
  }

  if (norm_denom == 0) {
    float denom = im.w*im.h*im.c;
    for (int i=0; i<im.w; i++) {
      for (int j=0; j<im.h; j++) {
        for (int k=0; k<im.c; k++) {
          float value = (float)1/denom;
          im.set_pixel(i,j,k,value);
        }
      }
    }
  }
  else {
    for (int i=0; i<im.w; i++) {
      for (int j=0; j<im.h; j++) {
        for (int k=0; k<im.c; k++) {
          float value = im(i,j,k) / norm_denom;
          im.set_pixel(i,j,k,value);
        }
      }
    }
  }
  }

// HW1 #2.1
// int w: size of filter
// returns the filter Image of size WxW
Image make_box_filter(int w)
  {
  assert(w%2); // w needs to be odd
  
  // TODO: Implement the filter
  Image filter(w, w, 1);
  for (int i=0; i<w; i++) {
    for (int j=0; j<w; j++) {
      filter(i,j,0) = 1.0;
    }
  }

  l1_normalize(filter);
  return filter;
  // return Image(1,1,1);
  }

// HW1 #2.2
// const Image&im: input image
// const Image& filter: filter to convolve with
// bool preserve: whether to preserve number of channels
// returns the convolved image
Image convolve_image(const Image& im, const Image& filter, bool preserve)
  {
  assert(filter.c==1);
  // Image ret;
  // This is the case when we need to use the function clamped_pixel(x,y,c).
  // Otherwise you'll have to manually check whether the filter goes out of bounds
  
  // TODO: Make sure you set the sizes of ret properly. Use ret=Image(w,c,c) to reset ret
  // TODO: Do the convolution operator
  int out_channel;
  float mod_value;
  if (preserve) out_channel = im.c;
  else out_channel = 1;

  Image ret(im.w, im.h, out_channel);

  for (int k=0; k<im.c; k++) {
    for (int i=0; i<im.w; i++) {
      for (int j=0; j<im.h; j++) {
        if (preserve) mod_value = 0;
        else mod_value = ret.clamped_pixel(i,j,0);
        for (int m=0; m<filter.w; m++) {
          for (int n=0; n<filter.h; n++) {
            mod_value += filter.clamped_pixel(m,n,0)*im.clamped_pixel((i - (filter.w/2) + m), (j - (filter.h/2) + n), k);
          }
        }
        if (preserve) ret.set_pixel(i,j,k,mod_value);
        else ret.set_pixel(i,j,0,mod_value);
      }
    }
  }
  // Make sure to return ret and not im. This is just a placeholder
  return ret;
  }

// HW1 #2.3
// returns basic 3x3 high-pass filter
Image make_highpass_filter()
  {
  // TODO: Implement the filter
  Image filter(3,3,1);

  float filter_weights[9] = {0, -1, 0, -1, 4, -1, 0, -1, 0};
  memcpy(filter.data, filter_weights, sizeof(filter_weights));
  
  return filter;  
  }

// HW1 #2.3
// returns basic 3x3 sharpen filter
Image make_sharpen_filter()
  {
  // TODO: Implement the filter
  Image filter(3,3,1);

  float filter_weights[9] = {0, -1, 0, -1, 5, -1, 0, -1, 0};
  memcpy(filter.data, filter_weights, sizeof(filter_weights));
  
  return filter;
  }

// HW1 #2.3
// returns basic 3x3 emboss filter
Image make_emboss_filter()
  {
  // TODO: Implement the filter
  Image filter(3,3,1);

  float filter_weights[9] = {-2, -1, 0, -1, 1, 1, 0, 1, 2};
  memcpy(filter.data, filter_weights, sizeof(filter_weights));
  
  return filter;
  }

// HW1 #2.4
// float sigma: sigma for the gaussian filter
// returns basic gaussian filter
Image make_gaussian_filter(float sigma)
  {
  // TODO: Implement the filter
  int width = (int)roundf(6*sigma);
  if (width > 0) {
    if (width%2 == 0) width += 1;
  }
  else {
    width = 1;
  }
  
  Image filter(width, width, 1);

  int offset = width/2;

  for (int i=0; i<width; i++) {
    for (int j=0; j<width; j++) {
      int x_ = i - offset;
      int y_ = j - offset;
      filter(i,j,0) = (1/(2*M_PI*powf(sigma, 2)))*expf(-(powf(x_, 2) + powf(y_, 2)) / (2*powf(sigma, 2)));
    }
  }

  l1_normalize(filter);
  return filter;
  }


// HW1 #3
// const Image& a: input image
// const Image& b: input image
// returns their sum
Image add_image(const Image& a, const Image& b)
  {
  assert(a.w==b.w && a.h==b.h && a.c==b.c); // assure images are the same size
  
  // TODO: Implement addition
  Image added_image(a.w, a.h, a.c);

  for (int i=0; i<a.w; i++) {
    for (int j=0; j<a.h; j++) {
      for (int k=0; k<a.c; k++) {
        added_image(i,j,k) = a(i,j,k) + b(i,j,k);
      }
    }
  }
  return added_image;
  // return a;
  }

// HW1 #3
// const Image& a: input image
// const Image& b: input image
// returns their difference res=a-b
Image sub_image(const Image& a, const Image& b)
  {
  assert(a.w==b.w && a.h==b.h && a.c==b.c); // assure images are the same size
  
  // TODO: Implement subtraction
  Image res_image(a.w, a.h, a.c);

  for (int i=0; i<a.w; i++) {
    for (int j=0; j<a.h; j++) {
      for (int k=0; k<a.c; k++) {
        res_image(i,j,k) = a(i,j,k) - b(i,j,k);
      }
    }
  }
  return res_image;
  // return a;  
  }

// HW1 #4.1
// returns basic GX filter
Image make_gx_filter()
  {
  // TODO: Implement the filter
  Image filter(3,3,1);

  float filter_weights[9] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
  memcpy(filter.data, filter_weights, sizeof(filter_weights));
  
  return filter;
  }

// HW1 #4.1
// returns basic GY filter
Image make_gy_filter()
  {
  // TODO: Implement the filter
  Image filter(3,3,1);

  float filter_weights[9] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};
  memcpy(filter.data, filter_weights, sizeof(filter_weights));

  return filter;
  }

// HW1 #4.2
// Image& im: input image
// void feature_normalize(Image& im)
//   {
//   assert(im.w*im.h); // assure we have non-empty image
  
//   // TODO: Normalize the features for each channel
//   float max = 0;
//   float min = 1;
//   for (int i=0; i<im.w; i++) {
//     for (int j =0; j<im.h; j++) {
//       if (im(i,j,0) < min) min = im(i,j,0);
//       if (im(i,j,0) > max) max = im(i,j,0);
//     }
//   }

//   float scaling_factor = max - min;
//   if (scaling_factor == 0) {
//     for (int i=0; i<im.w; i++) {
//       for (int j=0; j<im.h; j++) {
//         im(i,j,0) = 0;
//       }
//     }    
//   }
//   else {
//     for (int i=0; i<im.w; i++) {
//       for (int j=0; j<im.h; j++) {
//         im(i,j,0) = (im(i,j,0) - min)/scaling_factor;
//       }
//     }
//   }
//   }

void feature_normalize(Image& im)
  {
  assert(im.w*im.h); // assure we have non-empty image
  
  // TODO: Normalize the features for each channel
  for (int k=0; k<im.c; k++) {
    float max = 0;
    float min = 1;
    for (int i=0; i<im.w; i++) {
      for (int j =0; j<im.h; j++) {
        if (im(i,j,k) < min) min = im(i,j,k);
        if (im(i,j,k) > max) max = im(i,j,k);
      }
    }
    float scaling_factor = max - min;
    if (scaling_factor == 0) {
      for (int i=0; i<im.w; i++) {
        for (int j=0; j<im.h; j++) {
          im(i,j,k) = 0;
        }
      }    
    }
    else {
      for (int i=0; i<im.w; i++) {
        for (int j=0; j<im.h; j++) {
          im(i,j,k) = (im(i,j,k) - min)/scaling_factor;
        }
      }
    }
  }
  }


// Normalizes features across all channels
void feature_normalize_total(Image& im)
  {
  assert(im.w*im.h*im.c); // assure we have non-empty image
  
  int nc=im.c;
  im.c=1;im.w*=nc;
  
  feature_normalize(im);
  
  im.w/=nc;im.c=nc;
  
  }


// HW1 #4.3
// Image& im: input image
// return a pair of images of the same size
pair<Image,Image> sobel_image(const Image& im)
  {
  // TODO: Your code here
  Image grad_mag(im.w, im.h, 1);
  Image grad_direc(im.w, im.h, 1);
  Image gx_filter = make_gx_filter();
  Image gy_filter = make_gy_filter();
  Image gx = convolve_image(im, gx_filter, false);
  Image gy = convolve_image(im, gy_filter, false);

  for (int i=0; i<im.w; i++) {
    for (int j=0; j<im.h; j++) {
      grad_mag(i,j,0) = sqrtf(powf(gx(i,j,0), 2.0) + powf(gy(i,j,0), 2.0));
      grad_direc(i,j,0) = atan2(gy(i,j,0), gx(i,j,0));
    }
  }

  return {grad_mag,grad_direc};
  }


// HW1 #4.4
// const Image& im: input image
// returns the colorized Sobel image of the same size
Image colorize_sobel(const Image& im)
  {
  
  // TODO: Your code here
  Image f = make_gaussian_filter(4);
  Image blur = convolve_image(im, f, true);
  blur.clamp();

  pair <Image, Image> sobel = sobel_image(blur);
  Image sobel_mag = sobel.first;
  Image sobel_direc = sobel.second;
  feature_normalize(sobel_mag);
  // feature_normalize(sobel_direc);

  for (int i=0; i<sobel_direc.w; i++) {
    for (int j=0; j<sobel_direc.h; j++) {
      sobel_direc(i,j,0) = (sobel_direc(i,j,0)/(2*M_PI)) + 0.5;
    }
  }
  Image colorized_sobel(im.w, im.h, 3);
  for (int i=0; i<colorized_sobel.w; i++) {
    for (int j=0; j<colorized_sobel.h; j++) {
      colorized_sobel(i,j,0) = sobel_direc(i,j,0);
      colorized_sobel(i,j,1) = sobel_mag(i,j,0);
      colorized_sobel(i,j,2) = sobel_mag(i,j,0);
    }
  }
  hsv_to_rgb(colorized_sobel);

  return colorized_sobel;
  }


// HW1 #4.5
// const Image& im: input image
// float sigma1,sigma2: the two sigmas for bilateral filter
// returns the result of applying bilateral filtering to im
Image bilateral_filter(const Image& im, float sigma1, float sigma2)
  {

  // Image bf=im;
  
  // // TODO: Your bilateral code
  // NOT_IMPLEMENTED();
  
  // return bf;    
  Image bf(im.w, im.h, im.c);
  
  // TODO: Your bilateral code
  int width = (int)roundf(6*sigma1);
  if (width > 0) {
    if (width%2 == 0) width += 1;  
  }
  else {
    width = 1;
  }
  
  Image spatial_filter = make_gaussian_filter(sigma1);
  int offset = width/2;
  
  for (int c=0; c<im.c; c++) {
    for (int x=0; x<im.w; x++) {
      for (int y=0; y<im.h; y++) {
        float N_x_y = 0;
        float pixel_val = 0;
        // Image color_filter(width, width, 1);
        for (int i=0; i<width; i++) {
          for (int j=0; j<width; j++) {
            int x_ = x - offset + i;
            int y_ = y - offset + j;
            float pixel_val_diff = im.clamped_pixel(x, y, c) - im.clamped_pixel(x_, y_, c);
            float g_color = (1/(2*M_PI*powf(sigma2, 2)))*expf(-(powf(pixel_val_diff, 2)) / (2*powf(sigma2, 2)));
            N_x_y += g_color * spatial_filter(i,j,0);
            // color_filter(i,j,0) = g_color;
            pixel_val += im.clamped_pixel(x_, y_, c)*spatial_filter(i,j,0)*g_color;
          }
        }
        bf(x,y,c) = (float)pixel_val/N_x_y;
      }
    }
  }
  return bf;
  }



// HELPER MEMBER FXNS

void Image::feature_normalize(void) { ::feature_normalize(*this); }
void Image::feature_normalize_total(void) { ::feature_normalize_total(*this); }
void Image::l1_normalize(void) { ::l1_normalize(*this); }

Image operator-(const Image& a, const Image& b) { return sub_image(a,b); }
Image operator+(const Image& a, const Image& b) { return add_image(a,b); }
