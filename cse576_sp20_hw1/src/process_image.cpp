#include <cstdio>
#include <cstring>
#include <cassert>
#include <cmath>

#include "image.h"
// #define M_PI 3.141592653589793238462643383279502884L

using namespace std;
// HW0 #3
// const Image& im: input image
// return the corresponding grayscale image
Image rgb_to_grayscale(const Image& im)
  {
  assert(im.c == 3); // only accept RGB images
  Image gray(im.w,im.h,1); // create a new grayscale image (note: 1 channel)
  
  // TODO: calculate the pixels of 'gray'
  for (int i=0; i<im.w; i++)
    {
      for (int j=0; j<im.h; j++)
        {
          gray(i,j,0) = 0.299*im(i,j,0) + 0.587*im(i,j,1) + 0.114*im(i,j,2);
        }
    }
  return gray;
  }



// Example function that changes the color of a grayscale image
Image grayscale_to_rgb(const Image& im, float r, float g, float b)
  {
  assert(im.c == 1);
  Image rgb(im.w,im.h,3);
  
  for(int q2=0;q2<im.h;q2++)for(int q1=0;q1<im.w;q1++)
    {
    rgb(q1,q2,0)=r*im(q1,q2);
    rgb(q1,q2,1)=g*im(q1,q2);
    rgb(q1,q2,2)=b*im(q1,q2);
    }
  
  return rgb;
  }


// HW0 #4
// Image& im: input image to be modified in-place
// int c: which channel to shift
// float v: how much to shift
void shift_image(Image& im, int c, float v)
  {
  assert(c>=0 && c<im.c); // needs to be a valid channel
  
  // TODO: shift all the pixels at the specified channel
  for (int i=0; i<im.w; i++)
    {
      for (int j=0; j<im.h; j++)
        {
          im(i,j,c) += v;
        }
    }
  }

// HW0 #8
// Image& im: input image to be modified in-place
// int c: which channel to scale
// float v: how much to scale
void scale_image(Image& im, int c, float v)
  {
  assert(c>=0 && c<im.c); // needs to be a valid channel
  
  // TODO: scale all the pixels at the specified channel
  for (int i=0; i<im.w; i++)
    {
      for (int j=0; j<im.h; j++)
        {
          im(i,j,c) *= v;
        }
    }
  }


// HW0 #5
// Image& im: input image to be modified in-place
void clamp_image(Image& im)
  {
  // TODO: clamp all the pixels in all channel to be between 0 and 1
  for (int k=0; k<im.c; k++)
    {
      for (int i=0; i<im.w; i++)
        {
          for (int j=0; j<im.h; j++)
            {
              if (im(i,j,k) < 0) im(i,j,k) = 0;
              else if (im(i,j,k) > 1) im(i,j,k) = 1;
            }
        }
    }  
  }

// These might be handy
float max(float a, float b, float c)
  {
  return max({a,b,c});
  }

float min(float a, float b, float c)
  {
  return min({a,b,c});
  }


// HW0 #6
// Image& im: input image to be modified in-place
void rgb_to_hsv(Image& im)
  {
  assert(im.c==3 && "only works for 3-channels images");
  
  // TODO: Convert all pixels from RGB format to HSV format
  float R, G, B, V, m, C, S, H;
  for (int i=0; i<im.w; i++)
    {
      for (int j=0; j<im.h; j++)
        {
          R = im(i,j,0);
          G = im(i,j,1);
          B = im(i,j,2);

          V = max(R, G, B);
          m = min(R, G, B);
          C = V - m;

          S = (V == 0) ? 0 : C/V;

          if (C == 0) H = 0;
          else {
            if (V == R) H = (G - B) / C;
            else if (V == G) H = (B - R) / C + 2;
            else H = (R - G) / C + 4;
          }
          H = (H < 0) ? (H/6) + 1 : H/6;

          im(i,j,0) = H;
          im(i,j,1) = S;
          im(i,j,2) = V;
        }
    }
  }

// HW0 #7
// Image& im: input image to be modified in-place
void hsv_to_rgb(Image& im)
  {
  assert(im.c==3 && "only works for 3-channels images");
  
  // TODO: Convert all pixels from HSV format to RGB format  
  float H, S, V, C, X, m, R_1, G_1, B_1;
  float val = 1.0/6.0;
  for (int i=0; i<im.w; i++)
    {
      for (int j=0; j<im.h; j++)
        {
          H = im(i,j,0);
          S = im(i,j,1);
          V = im(i,j,2);
          C = V*S;
          X = C * (1 - abs(fmod(H*6,2.0) - 1));
          m = V - C;

          if (H >= 0 && H < val) {
            R_1 = C;
            G_1 = X;
            B_1 = 0;
          }
          else if (H >= val && H < 2*val) {
            R_1 = X;
            G_1 = C;
            B_1 = 0;
          }
          else if (H >= 2*val && H < 3*val) {
            R_1 = 0;
            G_1 = C;
            B_1 = X;
          }
          else if (H >= 3*val && H < 4*val) {
            R_1 = 0;
            G_1 = X;
            B_1 = C;
          }
          else if (H >= 4*val && H < 5*val) {
            R_1 = X;
            G_1 = 0;
            B_1 = C;
          }
          else {
            R_1 = C;
            G_1 = 0;
            B_1 = X;
          }
          im(i,j,0) = R_1 + m;
          im(i,j,1) = G_1 + m;
          im(i,j,2) = B_1 + m;
        }
    }
  }

void rgb_to_ciexyz(Image& im){
  // https://www.image-engineering.de/library/technotes/958-how-to-convert-between-srgb-and-ciexyz
  // cout << "rgb2xyz" << im(4,4,0) << im(4,4,1) << im(4,4,2) << "\n";
  float sR, sG, sB;
  for (int i=0; i<im.w; i++)
  {
    for (int j=0; j<im.h; j++)
    {
      sR = (im(i,j,0) <= 0.04045) ? im(i,j,0)/12.92 : powf((im(i,j,0)+0.055)/1.055, 2.4);

      sG = (im(i,j,1) <= 0.04045) ? im(i,j,1)/12.92 : powf((im(i,j,1)+0.055)/1.055, 2.4);

      sB = (im(i,j,2) <= 0.04045) ? im(i,j,2)/12.92 : powf((im(i,j,2)+0.055)/1.055, 2.4);

      im(i,j,0) = 0.4124564*sR + 0.3575761*sG + 0.1804375*sB;
      im(i,j,1) = 0.2126729*sR + 0.7151522*sG + 0.0721750*sB;
      im(i,j,2) = 0.0193339*sR + 0.1191920*sG + 0.9503041*sB;
    }
  }
  // cout << "rgb2xyz" << im(4,4,0) << im(4,4,1) << im(4,4,2) << "\n";
}

void ciexyz_to_cieluv(Image& im){
  // http://www.brucelindbloom.com/index.html?Eqn_XYZ_to_Luv.html
  // https://en.wikipedia.org/wiki/CIELUV
  // cout << "xyz2luv" << im(4,4,0) << im(4,4,1) << im(4,4,2) << "\n";
  float Xr = 95.047; // https://en.wikipedia.org/wiki/Illuminant_D65
  float Yr = 100.00;
  float Zr = 108.883;

  float ur_ = 4*Xr / (Xr + 15*Yr + 3*Zr);
  float vr_ = 9*Yr / (Xr + 15*Yr + 3*Zr);
  float X, Y, Z, u_, v_, yr, base, L;

  for (int i=0; i<im.w; i++)
  {
    for (int j=0; j<im.h; j++)
    {
      X = im(i,j,0); Y = im(i,j,1); Z = im(i,j,2);
      base = X + 15*Y + 3*Z;
      u_ = (base == 0) ? 0 : (4*X) / base;
      v_ = (base == 0) ? 0 : (9*Y) / base;
      yr = Y/Yr;

      L = (yr <= powf(6.0/29, 3)) ? powf(29.0/3, 3)*yr : 116*powf(yr, 1.0/3) - 16;

      im(i,j,0) = L;
      im(i,j,1) = 13*L*(u_ - ur_);
      im(i,j,2) = 13*L*(v_ - vr_);
    }
  }
  // cout << "xyz2luv" << im(4,4,0) << im(4,4,1) << im(4,4,2) << "\n";
}

void cieluv_to_lch(Image& im){
  // http://www.brucelindbloom.com/index.html?Math.html
  // cout << "luv2lch" << im(4,4,0) << im(4,4,1) << im(4,4,2) << "\n";
  float L, U, V, H;
  for (int i=0; i<im.w; i++)
  {
    for (int j=0; j<im.h; j++)
    {
      L = im(i,j,0); U = im(i,j,1); V = im(i,j,2);
      
      im(i,j,0) = L;
      im(i,j,1) = sqrtf(U*U + V*V);
      H = atan2f(V, U);
      H = (H/M_PI) * 180.0;
      
      if (H < 0) im(i,j,2) = H + 360.0;
      else if (H >= 360) im(i,j,2) = H - 360.0;
      else im(i,j,2) = H;
    }
  }  
  // cout << "luv2lch" << im(4,4,0) << "\t" <<  im(4,4,1) <<  "\t" << im(4,4,2) << "\n";
}

// ----------------------------------------------------//
void lch_to_cieluv(Image& im){
  // http://www.brucelindbloom.com/index.html?Math.html
  float L, C, H;
  for (int i=0; i<im.w; i++)
  {
    for (int j=0; j<im.h; j++)
    {
      L = im(i,j,0); C = im(i,j,1); H = im(i,j,2);
      im(i,j,0) = L;
      
      H = (H*M_PI) / 180;
      im(i,j,1) = C * cosf(H);
      im(i,j,2) = C * sinf(H);
    }
  }  
}

void cieluv_to_ciexyz(Image& im){
  // https://en.wikipedia.org/wiki/CIELUV

  float Xr = 95.047; // https://en.wikipedia.org/wiki/Illuminant_D65
  float Yr = 100.00;
  float Zr = 108.883;

  float ur_ = 4*Xr / (Xr + 15*Yr + 3*Zr);
  float vr_ = 9*Yr / (Xr + 15*Yr + 3*Zr);
  float L, U, V, u_, v_, Y;

  for (int i=0; i<im.w; i++)
  {
    for (int j=0; j<im.h; j++)
    {
      L = im(i,j,0); U = im(i,j,1); V = im(i,j,2);
      if (L != 0) {
        u_ = (U / (13*L)) + ur_;
        v_ = (V / (13*L)) + vr_;
        Y = (L <= 8) ? Yr*L*powf(3.0/29, 3.0) : Yr*powf((L+16)/116.0, 3.0);
        
        im(i,j,0) = Y*9*u_ / (4*v_);
        im(i,j,1) = Y;
        im(i,j,2) = Y*(12 - 3*u_ - 20*v_) / (4*v_);
      }
      else {
        im(i,j,0) = 0;
        im(i,j,1) = 0;
        im(i,j,2) = 0;
      }
    }
  }  
}

void ciexyz_to_rgb(Image& im){
  // http://www.brucelindbloom.com/index.html?Eqn_XYZ_to_RGB.html
  float X, Y, Z, sR, sG, sB;
  for (int i=0; i<im.w; i++)
  {
    for (int j=0; j<im.h; j++)
    {
      X = im(i,j,0); Y = im(i,j,1); Z = im(i,j,2);
      sR = 3.2404542*X + (-1.5371385)*Y + (-0.4985314)*Z;
      sG = (-0.9692660)*X + 1.8760108*Y + 0.0415560*Z;
      sB = 0.0556434*X + (-0.2040259)*Y + 1.0572252*Z;

      im(i,j,0) = (sR <= 0.0031308) ? 12.92*sR : 1.055*powf(sR, 1.0/2.4) - 0.055;
      im(i,j,1) = (sG <= 0.0031308) ? 12.92*sG : 1.055*powf(sG, 1.0/2.4) - 0.055;
      im(i,j,2) = (sB <= 0.0031308) ? 12.92*sB : 1.055*powf(sB, 1.0/2.4) - 0.055;
    }
  }  
}

// HW0 #9
// Image& im: input image to be modified in-place
void rgb_to_lch(Image& im)
  {
  assert(im.c==3 && "only works for 3-channels images");
  
  // TODO: Convert all pixels from RGB format to LCH format
  rgb_to_ciexyz(im);
  ciexyz_to_cieluv(im);
  cieluv_to_lch(im);
  }

// HW0 #9
// Image& im: input image to be modified in-place
void lch_to_rgb(Image& im)
  {
  assert(im.c==3 && "only works for 3-channels images");
  
  // TODO: Convert all pixels from LCH format to RGB format
  lch_to_cieluv(im);
  cieluv_to_ciexyz(im);
  ciexyz_to_rgb(im);
  }

// Implementation of member functions
void Image::clamp(void) { clamp_image(*this); }
void Image::shift(int c, float v) { shift_image(*this,c,v); }
void Image::scale(int c, float v) { scale_image(*this,c,v); }

void Image::HSVtoRGB(void) { hsv_to_rgb(*this); }
void Image::RGBtoHSV(void) { rgb_to_hsv(*this); }
void Image::LCHtoRGB(void) { lch_to_rgb(*this); }
void Image::RGBtoLCH(void) { rgb_to_lch(*this); }
