#include <opencv2/core/core.hpp> //core routines
#include <opencv2/highgui/highgui.hpp>//imread,imshow,namedWindow,waitKey
#include <opencv2/imgproc.hpp>//equalize function
#include <iostream>

using namespace std;

void applyKernel(cv::Mat image, cv::Mat kernel, cv::Mat &result);


void applyKernel(cv::Mat img, cv::Mat kernel, cv::Mat &result)
{
    result.create(img.size(), CV_32FC1);
    
    float blur_factor = 0.0;

    int boost_factor = 1;

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
                        blur_factor += kernel.at<float>( (j+r), (i+r) ) * img.at<float>( (y-j-1), (x-i-1) );
                    }
                }
            }
           
            value = boost_factor*img.at<float>(y,x) - blur_factor;

            if ( value > 255 )
                value = 255;

            result.at<float>(y, x) = value;
            blur_factor = 0.0;
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
        
        kernel = 1./9.;

        cv::Mat result;

        if(image.rows == 0)
        {
            cerr<<"Error reading image"<<endl;
            return 0;
        }

        applyKernel(image, kernel, result);

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