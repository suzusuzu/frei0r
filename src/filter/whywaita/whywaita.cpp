#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui.hpp>
#include "frei0r.hpp"
#include <cstdlib>


class Whywaita: public frei0r::filter
{

public:

    Whywaita(unsigned int width, unsigned int height){
    }

    ~Whywaita(){
    }

    virtual void update(double time,
                        uint32_t* out,
                        const uint32_t* in){
            if(cascade.empty()){
              cascade.load(std::getenv("CASCADE_FILE"));
            }
            if(img.empty()){
              img = cv::imread(std::getenv("WHYWAITA_IMG"));
            }

            cv::Mat inMat(height, width, CV_8UC3);
            convertUint32_t2Mat(in, &inMat);

            cascade.detectMultiScale(inMat, object, 1.1, 3);

            std::vector<cv::Rect>::const_iterator i = object.begin();
            while (i != object.end()) {
              cv::Mat mat = (cv::Mat_<double>(2, 3) << i->width/double(img.size().width), 0.0, i->x, 0.0, i->height/double(img.size().height), i->y);
              cv::warpAffine(img, inMat, mat, inMat.size(), CV_INTER_LINEAR, cv::BORDER_TRANSPARENT);
              i++;
            }

            convertMat2Uint32_t(inMat, out);
        }

private:

        cv::CascadeClassifier cascade;
        std::vector<cv::Rect> object;
        cv::Mat img;

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



    frei0r::construct<Whywaita> plugin("whywaita",
                                     "change whywaita face",
                                     "Sotetsu Suzugamine",
                                     0,1,
                                     F0R_COLOR_MODEL_RGBA8888);
