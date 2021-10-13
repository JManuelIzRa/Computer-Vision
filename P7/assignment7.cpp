#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <string>

#include "CMDLineParser.h"

using namespace std;

void threshold(cv::Mat in, int threshold_value, cv::Mat &out);

void threshold(cv::Mat in, int threshold_value, cv::Mat &out)
{
    out.create(in.size(), in.type());

    for( int y = 0; y<in.rows-2; y++ )
        for( int x = 0; x<in.cols-2; x++ )
        {
            if(in.at<uchar>(y,x) < threshold_value)
                out.at<uchar>(y,x) = 255;
            else
                out.at<uchar>(y,x) = 0;
        }
}

int main ( int argc, char** argv)
{
    
    try
    {
        if(argc!=7)
        {
            cerr << "Usage: /prog in_image out_image -thres <val> -op <(erode|dilate|open|close)> ";
            return 0;
        }
    
        cv::Mat image = cv::imread(argv[1], cv::IMREAD_GRAYSCALE);
        cv::Mat threshold_image;

        if ( image.rows == 0 )
        {
            cerr << "Error reading the image.";
            return 0;
        }

        CmdLineParser cml(argc,argv);

        //check if a command is present
        if (cml["-thres"] && cml["-op"])
        {
            //THRESHOLDING THE IMAGE
            cout<<"-thres option is in the command line\n";

            int i= stoi( cml("-thres"));//convert the value of the command
            cout << "i: " << i << endl;

            threshold(image, i, threshold_image);
            //END OF

            string option = cml("-op");
            cout<<"-op option is in the command line\n";

            if ( option == "erode" )
            {
                cout<<"erode option is in the command line\n";
            }
            else if ( option == "dilate" )
            {
                cout<<"dilate option is in the command line\n";
            }
            else if ( option == "open" )
            {
                cout<<"open option is in the command line\n";
            }
            else if ( option == "close" )
            {
                cout<<"close option is in the command line\n";
            }
            else
            {
                cerr << "The given option is not correct <" << option << ">" << endl;
                return 0;
            }
                
        }

        cv::namedWindow("Threshold");
        cv::imshow("Threshold", threshold_image);
        
        cv::waitKey(0);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}