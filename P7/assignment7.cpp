#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <string>
#include <vector>

#include "CMDLineParser.h"

using namespace std;

void threshold(cv::Mat in, int threshold_value, cv::Mat &out);
void erode(cv::Mat in, int k, cv::Mat &out);
bool sortFunction (int i,int j) { return (i<j); }


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

void erode(cv::Mat in, int k, cv::Mat &out)
{
    out.create(in.size(), in.type());

    int r = k/2;

    int neighborhood = k*k;

    std::vector<uchar> v(0, k*k);

    for(int y = 1; y<in.rows-1; y++)
    {
        for(int x = 1; x<in.cols-1; x++)
        {
            for(int j = -r; j< out.rows-r; j++)
            {
                for(int i = -r; i<out.cols-r; i++)
                {
                    if( ( (x-i)>0 ) && ( (y-j)>0 ) && ( (x-i)<result.cols ) && ( (y-j)<result.rows ) )
                    {
                        v.push_back( in.at<uchar>( (y-j-1), (x-i-1) ) );
                    }
                }
            }

            std::sort (v.begin(), v.end(), sortFunction);
            out.at<uchar>(y,x) = v[0];

            v.clear();
        }
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
        cv::Mat morph_ops;

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
                erode(threshold_image, 4, morph_ops);
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

        cv::namedWindow("Morph");
        cv::imshow("Morph", morph_ops);
        
        cv::waitKey(0);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}