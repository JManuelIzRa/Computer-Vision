#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <vector>

using namespace std;

void medianFilter(const cv::Mat &in,cv::Mat &out, int size);
bool sortFunction (int i,int j) { return (i<j); }
void applyKernel(cv::Mat image, cv::Mat kernel, cv::Mat &result);

void applyKernel(cv::Mat image, cv::Mat kernel, cv::Mat &result)
{
    result.create(image.size(),image.type());
    float sum = 0.0;

    int r = kernel.rows/2;

    for(int y = 1; y<result.rows-1; y++)
    {
        for(int x = 1; x<result.cols-1; x++)
        {
            for(int j = -r; j< kernel.rows-r; j++)
            {
                for(int i = -r; i<kernel.cols-r; i++)
                {
                    if( ( (x-i)>0 ) && ( (y-j)>0 ) && ( (x-i)<result.cols ) && ( (y-j)<result.rows ) )
                    {
                        sum += kernel.at<float>( (j+r), (i+r) ) * image.at<uchar>( (y-j-1), (x-i-1) );
                    }
                }
            }

            result.at<uchar>(y, x) = sum;
            sum = 0.0;
        }
    }
}

//Using split function and merge one
void medianFilter(const cv::Mat &in,cv::Mat &out, int size)
{
    out.create(in.size(), in.type());

    int r = size/2;

    int filter_size = size*size;

    std::vector<uchar> v(0, filter_size);

    for(int y = 1; y<out.rows-1; y++)
    {
        for(int x = 1; x<out.cols-1; x++)
        {
            for(int j = -r; j< size-r; j++)
            {
                for(int i = -r; i<size-r; i++)
                {
                    if( ( (x-i)>0 ) && ( (y-j)>0 ) && ( (x-i)<out.cols ) && ( (y-j)<out.rows ) )
                    {
                        v.push_back( in.at<uchar>( (y-j-1), (x-i-1) ) );                   
                    }
                }
            }
            
            std::sort (v.begin(), v.end(), sortFunction);
            //std::cout << "Pixel" << "(" << y << "," <<  x << "): " << v[size/2];
            out.at<uchar>(y,x) = v[filter_size/2];

            v.clear();

        }
    }

    /*cv::namedWindow("MedianFilter");
        cv::imshow("MedianFilter", out);*///cv::normalize(out, out, 255, cv::NORM_MINMAX);

}


int main (int argc, char** argv)
{
    try
    {
        if(argc!=2)
        {
           cerr << "Usage: image" << endl;
           return 0; 
        }

        cv::Mat image = cv::imread(argv[1]);

        if(image.rows == 0)
        {
            cerr << "Error reading image" << endl;
            return 0;
        }

        int size;

        std::cout << "Introduce the filter size: ";
        std::cin >> size;

        //CREATION OF BOX FILTER KERNEL
        cv::Mat kernel = cv::Mat::zeros(size, size, CV_32FC1);
        
        float k_size = size * size;
        
        kernel = 1./k_size;
        //END OF

        cv::Mat result_boxfilter;
        cv::Mat result_medianblur;
        
        applyKernel( image, kernel, result_boxfilter );
        
        cv::Mat bgrchannel[3];
        
        cv::Mat result_channel_mFilter[3];
        cv::Mat result_channel_bFilter[3];

        cv::split(image, bgrchannel);//We split the channels and store them on different Mats

        medianFilter( bgrchannel[0], result_channel_mFilter[0], size );
        medianFilter( bgrchannel[1], result_channel_mFilter[1], size );
        medianFilter( bgrchannel[2], result_channel_mFilter[2], size );

        applyKernel( bgrchannel[0] , kernel, result_channel_bFilter[0] );
        applyKernel( bgrchannel[1] , kernel, result_channel_bFilter[1] );
        applyKernel( bgrchannel[2] , kernel, result_channel_bFilter[2] );

        cv::merge(result_channel_mFilter, 3, result_medianblur);
        cv::merge(result_channel_bFilter, 3, result_boxfilter);

        cv::namedWindow("Original Image");
        cv::imshow("Original Image", image);

        cv::namedWindow("Result Box Filter");
        cv::imshow("Result Box Filter", result_boxfilter);

        cv::namedWindow("Result Median Blur");
        cv::imshow("Result Median Blur", result_medianblur);

        cv::waitKey(0);

    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
}