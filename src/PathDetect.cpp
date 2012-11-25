#include "PathDetect.h"
#include <opencv.hpp>

class WatershedSegmenter
{
private:
    cv::Mat markers;
public:
    void setMarkers(cv::Mat& markerImage)
    {
        markerImage.convertTo(markers, CV_32S);
    }

    cv::Mat process(cv::Mat &image)
    {
        cv::watershed(image, markers);
        markers.convertTo(markers,CV_8U);
        return markers;
    }
};

PathDetect::PathDetect()
{
}

PathDetect::~PathDetect()
{
}

cv::Mat PathDetect::DetectPath(cv::Mat& input)
{
    // Create markers image
    cv::Mat markers(input.size(),CV_8U,cv::Scalar(0));
    for (int i = 0; i < input.rows; ++i)
    {
        for (int j = 0; j < input.cols; ++j)
        {
            unsigned char value = 0;
            double r = input.at<cv::Vec3b>(i,j)[2];
            double g = input.at<cv::Vec3b>(i,j)[1];
            double b = input.at<cv::Vec3b>(i,j)[0];
            if (r > g && r > b) value = 255;
            if (g > r && g > b) value = 128;

            markers.at<unsigned char>(i,j) = value;
        }
    }
    // Create watershed segmentation object
    WatershedSegmenter segmenter;
    segmenter.setMarkers(markers);

    return   segmenter.process(input);
}

