#include <opencv2/core/core.hpp> //core routines
#include <opencv2/highgui/highgui.hpp>//imread,imshow,namedWindow,waitKey
#include <opencv2/imgproc.hpp>//equalize function
#include <iostream>

using namespace std;

void applyKernel(cv::Mat image, cv::Mat kernel, cv::Mat &result, float boost_factor);

void makeHighBoostFilter(cv::Mat &kernel, float boost_factor)
{
    kernel = cv::Mat::zeros(cv::Size(3, 3), CV_32FC1);

    for(int j = 0; j<=2; j++)
    {
        for(int i = 0; i<=2; i++)
        {
            kernel.at<float>(j, i) = -boost_factor;
        }
    }

    kernel.at<float>(1,1) = 8*boost_factor + 1;
}

void applyKernel(cv::Mat img, cv::Mat kernel, cv::Mat &result)
{
    result.create(img.size(), CV_32FC1);

    int r = kernel.rows/2;

    float value = 0.0;

    img.convertTo(img, CV_32FC1);

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
                        value += kernel.at<float>( (j+r), (i+r) ) * img.at<float>( (y-j-1), (x-i-1) );                   
                    }
                }
            }

            result.at<float>(y, x) = value;
            value = 0.0;
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

        cv::Mat kernel = cv::Mat::zeros(3, 3, CV_32FC1);

        float boost_factor = 0;

        cv::Mat result;

        if(image.rows == 0)
        {
            cerr<<"Error reading image"<<endl;
            return 0;
        }
        
        std::cout << "Introduce the boost factor: ";
        std::cin >> boost_factor;

        makeHighBoostFilter(kernel, boost_factor);

        applyKernel(image, kernel, result);

        cv::normalize(result, result, 1.0, 0.0, cv::NORM_MINMAX, CV_32FC1);

        result.convertTo(result, image.type());//Para que se muestre adecuadamente se debe usar uchar

        cv::namedWindow("Original Image");
        cv::imshow("Original Image", image);

        cv::namedWindow("Result");
        cv::imshow("Result", result);

        cv::imwrite("result.jpg", result);

        cv::waitKey(0);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
}