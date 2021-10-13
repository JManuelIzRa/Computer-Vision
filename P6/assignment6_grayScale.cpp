#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <vector>

using namespace std;

void medianFilter(const cv::Mat &in,cv::Mat &out, int size);
bool sortFunction (int i,int j) { return (i<j); }

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

    //cv::normalize(out, out, 255, cv::NORM_MINMAX);

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

        cv::Mat image = cv::imread(argv[1], cv::IMREAD_GRAYSCALE);

        if(image.rows == 0)
        {
            cerr << "Error reading image" << endl;
            return 0;
        }

        cv::Mat result;
        
        int size;

        std::cout << "Introduce the filter size: ";
        std::cin >> size;

        medianFilter( image, result, size );

        cv::namedWindow("Original Image");
        cv::imshow("Original Image", image);

        cv::namedWindow("Result");
        cv::imshow("Result", result);

        cv::waitKey(0);

    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
}