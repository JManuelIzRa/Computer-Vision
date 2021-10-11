#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <vector>

using namespace std;

void medianFilter(const cv::Mat &in,cv::Mat &out, int size);
bool sortFunction (int i,int j) { return (i<j); }

//Using split function and merge one
void medianFilter(const cv::Mat &in,cv::Mat &out, int size)
{
    out = in.clone();

    int r = size/2;

    int filter_size = size*size;

    float value = 0.0;

    std::vector<uchar> vB(0, filter_size);
    std::vector<uchar> vG(0, filter_size);
    std::vector<uchar> vR(0, filter_size);

    for(int y = 1; y<out.rows-1; y++)
    {
        for(int x = 1; x<out.cols-1; x++)
        {
            uchar *ptr_in = out.ptr<uchar>(y)+3*x;  

            for(int j = -r; j< size-r; j++)
            {
                for(int i = -r; i<size-r; i++)
                {
                    if( ( (x-i)>0 ) && ( (y-j)>0 ) && ( (x-i)<out.cols ) && ( (y-j)<out.rows ) )
                    {
                        uchar *ptr_filter = out.ptr<uchar>(y-j-1)+3*(x-i-1);  

                        vB.push_back(ptr_filter[0]);
                        vG.push_back(ptr_filter[1]);
                        vR.push_back(ptr_filter[2]);
                    }
                }
            }

            std::sort (vB.begin(), vB.end(), sortFunction);
            std::sort (vG.begin(), vG.end(), sortFunction);
            std::sort (vR.begin(), vR.end(), sortFunction);

            ptr_in[0] = vB[filter_size/2];
            ptr_in[1] = vG[filter_size/2];
            ptr_in[2] = vR[filter_size/2];

            vB.clear();
            vG.clear();
            vR.clear();

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

        cv::Mat image = cv::imread(argv[1]);

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