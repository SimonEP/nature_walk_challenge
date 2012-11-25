#include <opencv.hpp>
#include <string>
#include <program_options.hpp>
#include "../PathDetect.h"
#include "../ImageAdmin.h"


void ProcessVideo(std::string videoName)
{
    cv::VideoCapture inputVideo(videoName);
    cv::Size size = cv::Size((int) inputVideo.get(CV_CAP_PROP_FRAME_WIDTH),    //Acquire input size
                      (int) inputVideo.get(CV_CAP_PROP_FRAME_HEIGHT)*2);
    cv::VideoWriter outputVideo("output_video.avi", 0, inputVideo.get(CV_CAP_PROP_FPS),size, true);
    cv::Mat src, res, colourRes, stacked;
    PathDetect pd;
    while( true ) //Show the image captured in the window and repeat
    {
        inputVideo >> src;              // read
        if( src.empty()) break;         // check if at end
        res = pd.DetectPath(src);
        cvtColor(res, colourRes, CV_GRAY2RGB);
        stacked = StackImages(src, colourRes);
        outputVideo << stacked;
    }
}

int main(int argc, char* argv[])
{
    // Declare the supboost::program_optionsrted options.
    boost::program_options::options_description desc("Allowed options");
    desc.add_options()
        ("help", "produce help message")
        ("video", boost::program_options::value<std::string>(), "video file to be processed")
        ("image", boost::program_options::value<std::string>(), "image file to be processed")
    ;

    boost::program_options::variables_map vm;
    boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
    boost::program_options::notify(vm);

    if (vm.count("help"))
    {
        std::cout << desc << "\n";
        return 1;
    }

    if (vm.count("image"))
    {
        PathDetect pd;
        cv::Mat image = cv::imread(vm["image"].as<std::string>());
        imwrite("originalimage.jpg", image);
        cv::Mat result = pd.DetectPath(image);
        imwrite("finalimage.jpg", result);
    }

    if (vm.count("video"))
    {
        ProcessVideo(vm["video"].as<std::string>());

    }

    return 0;

}
