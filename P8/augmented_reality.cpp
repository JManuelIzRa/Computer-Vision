#include <iostream>
#include <opencv2/calib3d.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <vector>

#include "CMDLineParser.h"

int main(int argc, char**argv)
{
    const cv::String parameters = "{help h usage ? |          | print this message   }"
                            "{rows           |6         | number of rows of the pattern.}"
                            "{cols           |8         | number of columns of the pattern.}"
                            "{size           |8         | size of the axis}"
                            "{intrinsics     |camera.yml| path to the file wit the intrinsincs of the camera}"
                            "{@input         |<none>    | path to a video file or camera index}"
                            ;

    cv::CommandLineParser parser(argc, argv, parameters);

    if ( parser.has("help") )
    {
        parser.printMessage();
        return 0;
    }

    bool 

}