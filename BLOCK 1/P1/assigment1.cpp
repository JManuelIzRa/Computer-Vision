#include <opencv2/core/core.hpp> //core routines
#include <opencv2/highgui/highgui.hpp>//imread,imshow,namedWindow,waitKey
#include <opencv2/imgproc.hpp>//rectangle
#include <iostream>

using namespace std;

cv::Point point1;
cv::Point point2;
int counter=0;

cv::Mat image;


//callback function. Whenever a button pressed, this function is called
void on_mouse(int event, int x, int y, int flags, void* param)
{    
    
    if(event==cv::EVENT_RBUTTONDOWN)
    {        
        if(counter==0)
        {
            point1 =  cv::Point(x, y);
            cout<<"x, y="<<point1.x<<", "<<point1.y<<endl;
            counter++;
        }
        else if(counter==1)
        {
            point2 = cv::Point(x, y);
            cv::Mat outImg = image.clone();
            
            cv::namedWindow("Result");

            //Use min and max function to know the limits of the rectangle
            int xleft = std::min(point1.x, point2.x);
            int yleft = std::min(point1.y, point2.y);

            int xright = std::max(point1.x, point2.x);
            int yright = std::max(point1.y, point2.y);

            //cv::rectangle(outImg, point1, point2, cv::Scalar(255,255,255), cv::FILLED);
            
            int gray;
            
            //This loops transforms the inner rectangle to grayscale
            /*for(int x = xleft; x<=xright; x++)
                for(int y = yleft; y<=yright; y++)
                {
                    uchar *ptr = outImg.ptr<uchar>(y)+3*x;
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
                        uchar *ptr = outImg.ptr<uchar>(y)+3*x;
                        gray = 0;
                        gray = (ptr[0] + ptr[1] + ptr[2])/3;
                    
                        ptr[0] = gray;
                        ptr[1] = gray;
                        ptr[2] = gray;
                    }
                }


            cv::imshow("Result", outImg);

            counter=0;
        }
        

        
    }
}

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

        image = cv::imread(argv[1]);

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