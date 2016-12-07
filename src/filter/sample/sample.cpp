#include <opencv2/core.hpp>
#include "frei0r.hpp"
#include <limits>
#include <climits>
#include <cmath>


class Sample : public frei0r::filter
{

public:

    Sample(unsigned int width, unsigned int height){
    }

    ~Sample(){
    }

    virtual void update(double time,
                        uint32_t* out,
                        const uint32_t* in){
            cv::Mat inMat(height, width, CV_8UC3);
            convertUint32_t2Mat(in, &inMat);
            cv::Mat outMat = ~inMat;
            convertMat2Uint32_t(outMat, out);
        }

private:

        void convertMat2Uint32_t(cv::Mat mat, uint32_t* p){
            for(int w=0;w<width;w++){
                for(int h=0;h<height;h++){
                    uint8_t b = mat.at<cv::Vec3b>(h, w)[0];
                    uint8_t g = mat.at<cv::Vec3b>(h, w)[1];
                    uint8_t r = mat.at<cv::Vec3b>(h, w)[2];
                    p[width*h + w] = 0xFF000000 | (b << 16) | (g << 8) | r;
                }
            }
        }

        void convertUint32_t2Mat(const uint32_t* p, cv::Mat* mat){
            for(int w=0;w<width;w++){
                for(int h=0;h<height;h++){
                    uint8_t b = p[width*h + w] >> 16;
                    uint8_t g = p[width*h + w] >> 8;
                    uint8_t r = p[width*h + w] ;
                    (*mat).at<cv::Vec3b>(h, w)[0] = b;
                    (*mat).at<cv::Vec3b>(h, w)[1] = g;
                    (*mat).at<cv::Vec3b>(h, w)[2] = r;
                }
            }
        }

    };



    frei0r::construct<Sample> plugin("Sample",
                                     "This is an example filter, kind of a quick howto showing how to add a frei0r filter.",
                                     "Your Name",
                                     0,2,
                                     F0R_COLOR_MODEL_RGBA8888);
