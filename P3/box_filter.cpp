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
    
    //std::cout << image.rows << std::endl;
    //std::cout << image.cols << std::endl;

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
                        //std::cout <<"SUM: " << sum << std::endl;
                    }
                }
            }

            result.at<uchar>(y, x) = sum;
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

        int size = 0;

        std::cout << "Introduce the filter size: ";
        std::cin >> size;


        cv::Mat image = cv::imread(argv[1], cv::IMREAD_GRAYSCALE);

        cv::Mat kernel = cv::Mat::zeros(size, size, CV_32FC1);
        
        float k_size = size * size;
        
        kernel = 1./k_size;

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