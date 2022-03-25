#include <opencv2/core/core.hpp> //core routines
#include <opencv2/highgui/highgui.hpp>//imread,imshow,namedWindow,waitKey
#include <opencv2/imgproc.hpp>//rectangle
#include <iostream>

using namespace std;

cv::Point point1;
cv::Point point2;
int counter=0;

cv::Mat image;
cv::Mat original;
cv::Mat outImg;

//callback function. Whenever a button pressed or the mouse is moved, this function is called
void on_mouse(int event, int x, int y, int flags, void* param)
{    
    //It store the first axisn of the rectangle when the client press right button
    if(event==cv::EVENT_RBUTTONDOWN && counter==0)
    {        
        //Store the first point
        point1 =  cv::Point(x, y);
        

        cout<<"x, y="<<point1.x<<", "<<point1.y<<endl;

        counter++;
    }
    else if(event==cv::EVENT_RBUTTONUP)//When the client stops pressing the right button
    {
        point2 = cv::Point(x, y);
        cv::Mat outImg = original.clone();

        /*Doing that we reset the Result window showing the last selection
        outImg = original.clone();*/

        //Use min and max function to know the limits of the rectangle
        int xleft = std::min(point1.x, point2.x);
        int yleft = std::min(point1.y, point2.y);

        int xright = std::max(point1.x, point2.x);
        int yright = std::max(point1.y, point2.y);
            
        int gray;
        
        /*   
        //We draw the rectangle on grayscale pixel by pixel
        for(int x = xleft; x<=xright; x++)
            for(int y = yleft; y<=yright; y++)
            {
                uchar *ptr =  outImg.ptr<uchar>(y)+3*x;
                gray = 0;
                gray = (ptr[0] + ptr[1] + ptr[2])/3;

                ptr[0] = gray;
                ptr[1] = gray;
                ptr[2] = gray;
            }
        */

        int height = image.rows;//represents the rows
        int width = image.cols;//represents the columns

        //Error: double free or corruption (!prev)
        //Solution: overstepping the array. Either change <= to < or alloc SIZE + 1 elements

        for(int x = 0; x<outImg.cols; x++)
            for(int y = 0; y<outImg.rows; y++)
            {
                if( (x < xleft) || (x > xright) || (y < yleft) || (y > yright) )
                {
                    //uchar *ptr = outImg.ptr<uchar>(y)+3*x;
                    gray = 0;
                    //gray = (ptr[0] + ptr[1] + ptr[2])/3;
                    
                    gray = (outImg.at<cv::Vec3b>(y,x)[0] + outImg.at<cv::Vec3b>(y,x)[1] + outImg.at<cv::Vec3b>(y,x)[2])/3;

                    outImg.at<cv::Vec3b>(y,x)[0] = gray;
                    outImg.at<cv::Vec3b>(y,x)[1] = gray;
                    outImg.at<cv::Vec3b>(y,x)[2] = gray;
                }
            }

        //Update and show the Result window
        cv::imshow("Result", outImg);

        counter=0;
    }
    else
    {
        //We only want to detect the movement of the mouse when we already have the first point stored
        if(counter>0)
        {
            //If we dont clone the original image stored on outImg, all the rectangles we draw will be 
            //displayed at same time and it will display all the previous conversions on the window "OriginalImage".
            image = original.clone();

            point2 = cv::Point(x, y);
            
            cv::rectangle(image, point1, point2, cv::Scalar(0,0,255), 1);

            cv::imshow("OriginalImage", image);

        }
    }
        
}

int main(int argc, char **argv)
{
    try
    {
        //We check that the command line argument is correct
        if(argc!=2)
        {
            cerr<<"Usage: image"<<endl;
            return 0;
        }

        //loads the image from file
        image = cv::imread(argv[1]);

        //Clone the image to store the original
        original = image.clone();

        //We clon the image here instead of on the function because if we do it here we can display all the changes on the result window.
        outImg = image.clone();

        //Checks if the image have been loaded correctly
        if(image.rows==0)
        {
            cerr<<"Error reading image. Make sure the image path is correct"<<endl;
            return 0;
        }

        //creates a window 
        cv::namedWindow("OriginalImage");

        //Indicates that the on_mouse function should be called in case of mouse moved or pressed over window

        cv::setMouseCallback("OriginalImage", on_mouse, 0/*(void*)&image*/);

        //displays the image in the window
        cv::imshow("OriginalImage", image);

        //wait for a key to be pressed
        cv::waitKey(0);

    }
    catch(std::exception &ex)
    {
        cout<<ex.what()<<endl;
    }

}