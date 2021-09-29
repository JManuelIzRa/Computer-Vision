#include <opencv2/core/core.hpp> //core routines
#include <opencv2/highgui/highgui.hpp>//imread,imshow,namedWindow,waitKey
#include <opencv2/imgproc.hpp>//equalize function
#include <iostream>

using namespace std;

/*void makeBoxFilterKernel(cv::Mat image, cv::Mat &kernel, int size);*/
void applyKernel(cv::Mat image, cv::Mat kernel, cv::Mat &result);

void makeBoxFilterKernel(cv::Mat image, cv::Mat &kernel, int size)
{
    kernel = cv::Mat::zeros(cv::Size(size, size), CV_32F);

    for(int y = 0; y<kernel.rows ; y++)
    {
        for(int x = 0; x<kernel.cols ; x++)
        {
            kernel.at<float>(y, x)= 1.0/(kernel.rows*kernel.cols);;
        }
    }
}

void applyKernel(cv::Mat image, cv::Mat kernel, cv::Mat &result)
{
    result.create(image.size(),image.type());
    float sum = 0.0;

    for(int y = 0; y<image.rows; y++)
    {
        for(int x = 0; x<image.cols; x++)
        {
            for(int j = 0; j< kernel.rows; j++)
            {
                for(int i = -1; i<kernel.cols-1; i++)
                {
                    if( ( (x-1+i)>0 ) && ( (y-1+j)>0 ) /*&&  ( (x-i)<image.cols ) && ( (y-j)>image.rows )*/ )
                    {
                        sum += kernel.at<float>(cv::Point(j, i)) * image.at<uchar>(cv::Point(y-1+j, x-1+i));
                    }
                }
            }

            result.at<float>(y, x) = sum;
            sum = 0.0;
        }
    }
     cv::namedWindow("Result");
        cv::imshow("Result", result);
}


int main(int argc, char **argv)
{
    try
    {
        if(argc!=2)
        {
            cerr << "Usage: image" <<endl;
            return 0;
        }

        cv::Mat image = cv::imread(argv[1]/*, cv::IMREAD_GRAYSCALE*/);

        cv::Mat kernel = cv::Mat::zeros(3, 3, CV_32F);
        kernel = 1./9.;

        cv::Mat result;

        if(image.rows == 0)
        {
            cerr<<"Error reading image"<<endl;
            return 0;
        }


       //makeBoxFilterKernel(image, kernel, 3);

        applyKernel(image, kernel, result);

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