#include <opencv2/core/core.hpp> //core routines
#include <opencv2/highgui/highgui.hpp>//imread,imshow,namedWindow,waitKey
#include <opencv2/imgproc.hpp>//equalize function
#include <iostream>

using namespace std;

void applyKernel(cv::Mat image, cv::Mat kernel, cv::Mat &result);

void makeSobelFilterKernel(cv::Mat &kernel_x, cv::Mat &kernel_y, int size)
{
    kernel_x = cv::Mat::zeros(cv::Size(size, size), CV_32F);
    kernel_y = cv::Mat::zeros(cv::Size(size, size), CV_32F);


    float v1[] = {1, 2 , 1};
    float v2[] = {-1, 0, 1};

    for(int i = 0; i<=3; i++)
    {
        for(int j = 0; j<=2; j++)
        {
                kernel_x.at<float>(i, j) = v1[i] * v2[j] ;
        }
    }

    for(int i = 0; i<=2; i++)
    {
        for(int j = 0; j<=2; j++)
        {
            kernel_y.at<float>(i, j) = v2[i] * v1[j] ;
        }
    }
}

void applyKernel(cv::Mat img, cv::Mat kernel_x, cv::Mat kernel_y, cv::Mat &result)
{
    result.create(img.size(),img.type());
    float sum = 0.0;

    int value_x = 0;
    int value_y = 0;

    for( int y = 0; y < img.rows-2; y++)
    {
        for( int x = 0; x < img.cols-2; x++ )
        {
            for( int j = 0; j < 3; j++)
            {
                for(int i = 0; i < 3; i++)
                {
                    value_x += kernel_x.at<float>(j,i) * img.at<uchar>(y+i, x+j);
                    value_y += kernel_y.at<float>(j,i) * img.at<uchar>(y+i, x+j);
                }
            }

            sum = sqrt( pow( value_x , 2 ) + pow( value_y , 2 ) )/*abs(pixval_x) + abs(pixval_y)*/;
        std::cout << "sum= " << sum << std::endl;
            if (sum > 255)
            {
                sum = 255; //for best performance
            }

            result.at<uchar>(y,x) = sum;

            value_x = 0;
            value_y = 0;
            sum = 0.0;

        }

    }
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

        int size = 0;

        /*std::cout << "Introduce the filter size: ";
        std::cin >> size;*/


        cv::Mat image = cv::imread(argv[1], cv::IMREAD_GRAYSCALE);
        /*cv::Mat transformed = image.clone();

        image.convertTo(transformed, CV_32FC1, 255, 0);*/

        cv::Mat kernelX = cv::Mat::zeros(3, 3, CV_32FC1);
        cv::Mat kernelY = cv::Mat::zeros(3, 3, CV_32FC1);

        makeSobelFilterKernel(kernelX, kernelY, 3);
        
        cv::Mat result;

        if(image.rows == 0)
        {
            cerr<<"Error reading image"<<endl;
            return 0;
        }

        applyKernel(image, kernelX, kernelY, result);

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