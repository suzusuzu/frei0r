#include "frei0r.hpp"
#include "frei0r_math.h"
#include <stdlib.h>

class Mat33 : public frei0r::filter
{
public:
  Mat33(unsigned int width, unsigned int height)
  {
    register_param(m11, "m11", "matrix element m11");
    register_param(m12, "m12", "matrix element m12");
    register_param(m13, "m13", "matrix element m13");
    register_param(m21, "m21", "matrix element m21");
    register_param(m22, "m22", "matrix element m22");
    register_param(m23, "m23", "matrix element m23");
    register_param(m31, "m31", "matrix element m31");
    register_param(m32, "m32", "matrix element m32");
    register_param(m33, "m33", "matrix element m33");
  }

  virtual void update(double time,
                      uint32_t* out,
                      const uint32_t* in)
  {
    std::copy(in, in + width*height, out);
    for (unsigned int y=1; y<height-1; y++){
      for (unsigned int x=1; x<width-1; x++){
        unsigned char *p1 = (unsigned char *)&in[(y-1)*width+(x-1)];
        unsigned char *p2 = (unsigned char *)&in[(y-1)*width+x];
        unsigned char *p3 = (unsigned char *)&in[(y-1)*width+(x+1)];
        unsigned char *p4 = (unsigned char *)&in[y*width+(x-1)];
        unsigned char *p5 = (unsigned char *)&in[y*width+x];
        unsigned char *p6 = (unsigned char *)&in[y*width+(x+1)];
        unsigned char *p7 = (unsigned char *)&in[(y+1)*width+(x-1)];
        unsigned char *p8 = (unsigned char *)&in[(y+1)*width+x];
        unsigned char *p9 = (unsigned char *)&in[(y+1)*width+(x+1)];

        unsigned char *o = (unsigned char *)&out[y*width+x];
        for (int i=0; i<3; ++i){
          o[i] = CLAMP0255(abs(p1[i]*m11 + p2[i]*m12 + p3[i]*m13 + p4[i]*m21 + p5[i]*m22 + p6[i]*m23 + p7[i]*m31 + p8[i]*m32 + p9[i]*m33));
        }
        o[3] = ((unsigned char*)&in[y*width+x])[3];
      }
    }
  }
private:
  f0r_param_double m11;
  f0r_param_double m12;
  f0r_param_double m13;
  f0r_param_double m21;
  f0r_param_double m22;
  f0r_param_double m23;
  f0r_param_double m31;
  f0r_param_double m32;
  f0r_param_double m33;
};


frei0r::construct<Mat33> plugin("Mat33",
                                "3*3 Matrix filter",
                                "Sotetsu Suzgamine",
                                0,1,
                                F0R_COLOR_MODEL_RGBA8888);
