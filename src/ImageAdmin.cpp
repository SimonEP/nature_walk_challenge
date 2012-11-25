#include "ImageAdmin.h"

cv::Mat StackImages(cv::Mat& one, cv::Mat& two)
{
    cv::Size size = one.size();
    int height = size.height;
    int width = size.width;
    size.height += size.height;
    cv::Mat image(size, one.type());

    cv::Mat oneLoc =image(cv::Rect(0, 0, width, height));
    one.copyTo(oneLoc);
    cv::Mat twoLoc =image(cv::Rect(0, height, width, height));
    two.copyTo(twoLoc);

    return image;
}
