#include <opencv.hpp>
#include <vector>

class PathDetect
{
public:
    PathDetect();
    ~PathDetect();
    cv::Mat GetPath(const cv::Mat& input) const;

private:
    cv::Mat WatershedImage(const cv::Mat& input) const;
    void SetArea(cv::Mat& areaImage, unsigned char areaId, unsigned char areaValue) const;
    void FindLargest(cv::Mat& watershedImage) const;
    int FloodArea(cv::Mat& watershedImage, cv::Point& startPoint, unsigned char id) const;
    bool ValidForFloodFill(const cv::Point& newPoint, const cv::Point& maxPoint) const;
    typedef struct {int row; int col;} Loc;

    static const unsigned char PATH = 255;
    static const unsigned char NOT_PATH = 128;

};
