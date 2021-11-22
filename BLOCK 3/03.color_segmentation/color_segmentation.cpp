#include <opencv2/core/core.hpp> //core routines
#include <opencv2/highgui/highgui.hpp>//imread,imshow,namedWindow,waitKey
#include <iostream>

using std::cout;
using std::endl;
using std::cerr;

using cv::Mat;

int main(int argc, char **argv)
{
    try
    {
        //loads the image from file
        if(argc!=2)
        {
            cerr<<"Usage: image"<<endl;
            return 0;
        }

        Mat image = cv::imread(argv[1]);

        if(image.rows==0)
        {
            cerr<<"Error reading image. Make sure the image path is correct"<<endl;
            return 0;
        }

        cv::namedWindow("Original");
        cv::imshow("Original", image);

        Mat HSV_image = cv::cvtColor(img, cv::COLOR_BGR2HSV);

        cv::waitKey(0);
    }
    catch(std::exception &ex)
    {
        cout << ex.what() << endl;
    }    
    
    return 0;


}