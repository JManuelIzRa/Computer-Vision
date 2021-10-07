#include <opencv2/core/core.hpp> //core routines
#include <opencv2/highgui/highgui.hpp>//imread,imshow,namedWindow,waitKey
#include <opencv2/imgproc.hpp>//equalize function
#include <iostream>

using namespace std;

void applyKernel(cv::Mat image, cv::Mat kernel, cv::Mat &result);


void applyKernel(cv::Mat img, cv::Mat kernel, cv::Mat &result)
{
    result.create(img.size(),CV_32FC1);//Si la declaracion no es del mismo tipo que lo que luego
    //usamos para asignar da una violacion de segmento en ejecucion
    cv::Mat blur_factor, mask;
    blur_factor.create(img.size(), CV_32FC1);
    mask.create(img.size(), CV_32FC1);



    float sum = 0.0;
    
    img.convertTo(img, CV_32FC1);
    
    for i in range(1,image.shape[0]-1):
        for j in range(1,image.shape[1]-1):
            
            resultant_image[i, j] = image[i, j] + mask

    for( int y = 0; y < img.rows-2; y++)
    {
        for( int x = 0; x < img.cols-2; x++ )
        {
            blur_factor = (image[y-1, x-1] + image[y-1, x] + image[y-1, x+1] + image[y, x-1] + image[y, x] + image[y, x+1] + image[y+1, x+1] + image[y+1, x] + image[y+1, x+1])/9;
            mask = boost_factor*image[i, j] - blur_factor;


            result.at<float>(y,x) = sum;            sum = 0.0;

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


        convolve(resultX, resultY, result);

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