#include <opencv2/core/core.hpp> //core routines
#include <opencv2/highgui/highgui.hpp>//imread,imshow,namedWindow,waitKey
#include <opencv2/imgproc.hpp>//rectangle
#include <iostream>
#include <vector>

using namespace std;

void equalizedHist(cv::Mat image, cv::Mat result);
void makeHist(cv::Mat image, int* hist);
void comulativeHist(int* hist);
void normalizedHist(int* hist,double* cmpt);
void transform(cv::Mat image, double* cmpt);

void equalizedHist(cv::Mat image, cv::Mat result)
{
   
   int hist[256] {};//Inicializacion por lista vacia
   double cmpt[256]{};

   makeHist(image, hist);
   comulativeHist(hist);
   normalizedHist(hist, cmpt);
   transform(image, cmpt);
    
   cv::namedWindow("OriginalImage");
   cv::imshow("OriginalImage", image);
}

void makeHist(cv::Mat image, int* hist)
{
   
    for(int y = 0; y<image.rows; y++)
    {
        uchar *ptr=image.ptr<uchar>(y);

        for(int x = 0; x<image.cols; x++)
        { 
                hist[ ptr[x]] ++;
        }
    }

}

void comulativeHist(int* hist)
{
    //Compute the integral of the histogram
    int compute = 0;

    for(int i=0; i<256; i++)
    {
        compute += hist[i];
        
        hist[i] = compute;
            
    }

}

void normalizedHist(int* hist,double* cmpt)
{
    float max = hist[255];

    for(int i=0; i<256; i++)
    {
        cmpt[i] = round(hist[i] * (255.0/max));
    }

}


void transform(cv::Mat image, double* cmpt)
{
    cv::Mat result = image.clone();

    for(int y = 0; y<result.rows; y++)
        for(int x = 0; x<result.cols; x++)
        {
                result.at<uchar>(y,x)=cmpt[result.at<uchar>(y,x)];
        }

    cv::namedWindow("Result");
    cv::imshow("Result", result);
}

/*bool buscarElemento(std::vector < pair <int, int> > hist, int color)
{
    for(int i=0; i<hist.size(); i++)
    {
        if(color == hist[i].first)
        {
            &posicion = i;
            return true;
        }
    }

    return false;
}*/

int main(int argc, char **argv)
{
    try
    {
        //loads the image from file
        if(argc!=2)
        {
            cerr<<"Usage: image"<<endl;
            return 0;
        }

        cv::Mat image = cv::imread(argv[1], cv::IMREAD_GRAYSCALE);
        cv::Mat result;

        if(image.rows==0)
        {
            cerr<<"Error reading image. Make sure the image path is correct"<<endl;
            return 0;
        }

        //creates a window 
        //cv::namedWindow("OriginalImage");

        //Indicates that the on_mouse function should be called in case of mouse moved or pressed over window

        equalizedHist(image, result);

        //displays the image in the window
        //cv::imshow("OriginalImage", image);

        //wait for a key to be pressed
        cv::waitKey(0);

    }
    catch(std::exception &ex)
    {
        cout<<ex.what()<<endl;
    }

}