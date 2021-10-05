#include <opencv2/core/core.hpp> //core routines
#include <opencv2/highgui/highgui.hpp>//imread,imshow,namedWindow,waitKey
#include <opencv2/imgproc.hpp>//equalize function
#include <iostream>

using namespace std;

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

void applyKernel(cv::Mat img, cv::Mat kernel, cv::Mat &result)
{
    result.create(img.size(),img.type());
    float sum = 0.0;

    //int r = kernel.rows/2;

    /*for(int y = 1; y<result.rows-1; y++)
    {
        for(int x = 1; x<result.cols-1; x++)
        {
            for(int j = -1; j< kernel.rows-1; j++)
            {
                for(int i = -1; i<kernel.cols-1; i++)
                {
                    if( ( (x-i)>0 ) && ( (y-j)>0 ) && ( (x-i)<result.cols ) && ( (y-j)<result.rows ) )
                    {
                        sum += sqrt( pow( (kernel.at<float>( (j+1), (i+1) ) * image.at<uchar>( (y-j-1), (x-i-1) )), 2 ) +
                                pow( (kernel.at<float>( (i+1), (j+1) ) * image.at<uchar>( (y-j-1), (x-i-1) )), 2 ) );
                    }
                }
            }

            result.at<uchar>(y, x) = sum;
            sum = 0.0;
        }
    }*/

    int sobel_x[3][3];
    int sobel_y[3][3];

    sobel_x[0][0] = -1; sobel_x[0][1] = 0; sobel_x[0][2] =1;
    sobel_x[1][0] = -2; sobel_x[1][1] = 0; sobel_x[1][2] =2;
    sobel_x[2][0] = -1; sobel_x[2][1] = 0; sobel_x[2][2] =1;

    sobel_y[0][0] = -1; sobel_y[0][1] = -2; sobel_y[0][2] = -1;
    sobel_y[1][0] = 0; sobel_y[1][1] = 0; sobel_y[1][2] = 0;
    sobel_y[2][0] = 1; sobel_y[2][1] = 2; sobel_y[2][2] = 1;

    int value_x;
    int value_y;

    for (int j = 0; j<img.rows-2; j++)
    {
        for (int i = 0; i<img.cols-2; i++)
        {

            int pixval_x =
            (sobel_x[0][0] * (int)img.at<uchar>(j,i)) + (sobel_x[0][1] * (int)img.at<uchar>(j+1,i)) + (sobel_x[0][2] * (int)img.at<uchar>(j+2,i)) +
            (sobel_x[1][0] * (int)img.at<uchar>(j,i+1)) + (sobel_x[1][1] * (int)img.at<uchar>(j+1,i+1)) + (sobel_x[1][2] * (int)img.at<uchar>(j+2,i+1)) +
            (sobel_x[2][0] * (int)img.at<uchar>(j,i+2)) + (sobel_x[2][1] * (int)img.at<uchar>(j+1,i+2)) + (sobel_x[2][2] * (int)img.at<uchar>(j+2,i+2));   
         
            int pixval_y =
            (sobel_y[0][0] * (int)result.at<uchar>(j,i)) + (sobel_y[0][1] * (int)result.at<uchar>(j+1,i)) + (sobel_y[0][2] * (int)result.at<uchar>(j+2,i)) +
            (sobel_y[1][0] * (int)result.at<uchar>(j,i+1)) + (sobel_y[1][1] * (int)result.at<uchar>(j+1,i+1)) + (sobel_y[1][2] * (int)result.at<uchar>(j+2,i+1)) +
            (sobel_y[2][0] * (int)result.at<uchar>(j,i+2)) + (sobel_y[2][1] * (int)result.at<uchar>(j+1,i+2)) + (sobel_y[2][2] * (int)result.at<uchar>(j+2,i+2));
         
            //There are few ways of doing it:
            //sqrt( pow(X,2) + pow(Y, 2) )
            //abs(X) + abs(Y)
            int sum = sqrt( pow( pixval_x , 2 ) + pow( pixval_y , 2 ) )/*abs(pixval_x) + abs(pixval_y)*/;
            
            if (sum > 255)
            {
                sum = 255; //for best performance
            }

            result.at<uchar>(j,i) = sum;
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
        //cv::Mat kernelY = cv::Mat::zeros(3, 3, CV_32FC1);

        float v1[] = {1, 2 , 1};
        float v2[] = {-1, 0, 1};

        for(int i = 0; i<=2; i++)
        {
            for(int j = 0; j<=2; j++)
            {
                kernelX.at<float>(i, j) = v1[i] * v2[j] ;
            }
        }
        
        cv::Mat result;

        if(image.rows == 0)
        {
            cerr<<"Error reading image"<<endl;
            return 0;
        }

        applyKernel(image, kernelX, result);

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