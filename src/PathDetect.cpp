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

    cv::Mat process(const cv::Mat& image)
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

cv::Mat PathDetect::GetPath(const cv::Mat& input) const
{
    cv::Mat watershedImage = WatershedImage(input);
    FindLargest(watershedImage);
    return watershedImage;
}

cv::Mat PathDetect::WatershedImage(const cv::Mat& input) const
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
            if (r > g && r > b) value = PATH;
            if (g > r && g > b) value = NOT_PATH;

            markers.at<unsigned char>(i,j) = value;
        }
    }
    // Create watershed segmentation object
    WatershedSegmenter segmenter;
    segmenter.setMarkers(markers);

    return   segmenter.process(input);
}


void PathDetect::SetArea(cv::Mat& areaImage, unsigned char areaId, unsigned char areaValue) const
{
    int rows = areaImage.rows;
    int cols = areaImage.cols;

    for (int r = 0; r < rows; ++r)
    {
        for (int c = 0; c < cols; ++c)
        {
            unsigned char value = 0;
            cv::Point currentPoint(c, r);
            if (areaImage.at<unsigned char>(currentPoint) == areaId )
            {
                value = areaValue;
            }
            areaImage.at<unsigned char>(currentPoint) = value;
        }
    }
}

void PathDetect::FindLargest(cv::Mat& watershedImage) const
{
    int rows = watershedImage.rows;
    int cols = watershedImage.cols;

    cv::Mat image = watershedImage.clone();
    cv::Point bestPoint;
    int bestPixelCount = 0;
    int areaCount = 0;
    for (int r = 0; r < rows; ++r)
    {
        for (int c = 0; c < cols; ++c)
        {
            cv::Point currentPoint(c, r);
            if (image.at<unsigned char>(currentPoint) == PATH)
            {
                areaCount++;
                int pixelCount = FloodArea(image, currentPoint, 1);
                if (pixelCount > bestPixelCount)
                {
                    bestPixelCount = pixelCount;
                    bestPoint = currentPoint;
                }
            }
        }
    }

    FloodArea(watershedImage, bestPoint, 1);
    SetArea(watershedImage, 1, PATH);
}

int PathDetect::FloodArea(cv::Mat& watershedImage, cv::Point& startPoint, unsigned char id) const
{
    const int floodX[4]={1,-1,0,0};
    const int floodY[4]={0,0,1,-1};
    cv::Point maxLoc(watershedImage.cols, watershedImage.rows);

    std::vector<cv::Point> stack;
    int pixelCount = 1;
    stack.push_back(startPoint);
    watershedImage.at<unsigned char>(startPoint) = id;
    while (stack.size() > 0)
    {
        cv::Point currentLocation = stack.back();
        stack.pop_back();
        int x = currentLocation.x;
        int y = currentLocation.y;
        for (int i=0; i<4; ++i)
        {
            int possibleX = x+floodX[i];
            int possibleY = y+floodY[i];
            cv::Point newLoc(possibleX, possibleY);
            if (ValidForFloodFill(newLoc, maxLoc) && watershedImage.at<unsigned char>(newLoc) == PATH)
            {
                stack.push_back(newLoc);
                watershedImage.at<unsigned char>(newLoc) = id;
                pixelCount += 1;
            }
        }
    }
    return pixelCount;
}

bool PathDetect::ValidForFloodFill(const cv::Point& newPoint, const cv::Point& maxPoint) const
{
    if (newPoint.x < 0) return false;
    if (newPoint.y < 0) return false;
    if (newPoint.x >= maxPoint.x) return false;
    if (newPoint.y >= maxPoint.y) return false;
    return true;
}
