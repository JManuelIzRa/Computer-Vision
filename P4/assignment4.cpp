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

void combine(cv::Mat matrixX, cv::Mat matrixY, cv::Mat &result)
{
    float value = 0.0;
    result.create(matrixX.size(),CV_32FC1);

    for(int y = 0; y<matrixX.rows-2; y++)
    {
        for(int x = 0; x<matrixY.cols-2; x++)
        {
            value = sqrt( pow( matrixX.at<float>(y,x) , 2 ) + pow( matrixY.at<float>(y,x) , 2 ) );

            if (value > 255)
            {
                value = 255; //for best performance
            }

            result.at<float>(y,x) = value;
            value = 0.0;
        }
    }

}

void applyKernel(cv::Mat img, cv::Mat kernel, cv::Mat &result)
{
    result.create(img.size(),CV_32FC1);//Si la declaracion no es del mismo tipo que lo que luego
    //usamos para asignar da una violacion de segmento en ejecucion
    
    float sum = 0.0;
    
    img.convertTo(img, CV_32FC1);
    
    for( int y = 0; y < img.rows-2; y++)
    {
        for( int x = 0; x < img.cols-2; x++ )
        {
            for( int j = 0; j < kernel.rows; j++)
            {
                for(int i = 0; i < kernel.rows; i++)
                {
                    sum += kernel.at<float>(j,i) * img.at<float>(y+i, x+j);
                }
            }

            result.at<float>(y,x) = sum;
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

        cv::Mat image = cv::imread(argv[1], cv::IMREAD_GRAYSCALE);

        cv::Mat kernelX = cv::Mat::zeros(3, 3, CV_32FC1);
        cv::Mat kernelY = cv::Mat::zeros(3, 3, CV_32FC1);

        makeSobelFilterKernel(kernelX, kernelY, 3);

        cv::Mat resultX;
        cv::Mat resultY;

        cv::Mat result;

        if(image.rows == 0)
        {
            cerr<<"Error reading image"<<endl;
            return 0;
        }

        applyKernel(image, kernelX, resultX);
        applyKernel(image, kernelY, resultY);


        combine(resultX, resultY, result);

        result.convertTo(result, image.type());//Para que se muestre adecuadamente se debe usar uchar

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