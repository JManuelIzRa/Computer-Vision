// ----------------------------------------
// seglib.cpp
// (c) FSIV, University of Cordoba
// ----------------------------------------

#include "seglib.hpp"

bool sortFunction (int i,int j) { return (i<j); }
bool sortFunctionMaxMin (int i, int j) { return (i>j); }

void fsiv_segm_by_dif(const cv::Mat & prevFrame, const cv::Mat & curFrame, cv::Mat & difimg, int thr, int r)
{
   // WRITE ME
   difimg.create(curFrame.size(), curFrame.type());

   difimg = prevFrame - curFrame;
/*   for( int y = 0; y<curFrame.rows-2; y++ )
      for( int x = 0; x<curFrame.cols-2; x++ )
      {
         difimg.at<uchar>(y,x) = prevFrame.at<uchar>(y,x) - curFrame.at<uchar>(y,x);
      }
*/
   cv::namedWindow("Dif");
   cv::imshow("Dif", difimg);

   threshold(difimg, thr, difimg);

   cv::Mat dilate_img;
   cv::Mat erode_img;

   erode(difimg, r, erode_img);
   dilate(erode_img, r, difimg);

   dilate(difimg, r, dilate_img);
   erode(dilate_img, r, difimg);

   cv::namedWindow("Open image");
   cv::imshow("Open image", difimg);

}

//Threshold functcion
void threshold(cv::Mat in, int threshold_value, cv::Mat &out)
{
   out.create(in.size(), in.type());

   for( int y = 0; y<in.rows-2; y++ )
         for( int x = 0; x<in.cols-2; x++ )
         {
            if(in.at<uchar>(y,x) < threshold_value)
               out.at<uchar>(y,x) = 255;
            else
               out.at<uchar>(y,x) = 0;
         }
}

void erode(cv::Mat in, int k, cv::Mat &out)
{
    out.create(in.size(), in.type());

    int r = k/2;

    int neighborhood = k*k;

    std::vector<uchar> v(0, k*k);

    for(int y = 1; y<in.rows-1; y++)
    {
        for(int x = 1; x<in.cols-1; x++)
        {
            for(int j = -r; j< k-r; j++)
            {
                for(int i = -r; i< k-r; i++)
                {
                    if( ( (x-i)>0 ) && ( (y-j)>0 ) && ( (x-i)<out.cols ) && ( (y-j)<out.rows ) )
                    {
                        v.push_back( in.at<uchar>( (y-j-1), (x-i-1) ) );
                    }
                }
            }

            std::sort (v.begin(), v.end(), sortFunction);
            out.at<uchar>(y,x) = v[0];

            v.clear();
        }
    }
}

void dilate(cv::Mat in, int k, cv::Mat &out)
{
    out.create(in.size(), in.type());

    int r = k/2;

    int neighborhood = k*k;

    std::vector<uchar> v(0, k*k);

    for(int y = 1; y<in.rows-1; y++)
    {
        for(int x = 1; x<in.cols-1; x++)
        {
            for(int j = -r; j< k-r; j++)
            {
                for(int i = -r; i< k-r; i++)
                {
                    if( ( (x-i)>0 ) && ( (y-j)>0 ) && ( (x-i)<out.cols ) && ( (y-j)<out.rows ) )
                    {
                        v.push_back( in.at<uchar>( (y-j-1), (x-i-1) ) );
                    }
                }
            }

            std::sort (v.begin(), v.end(), sortFunctionMaxMin);
            out.at<uchar>(y,x) = v[0];

            v.clear();
        }
    }
}

void open(cv::Mat erode, cv::Mat dilate, cv::Mat &morph_ops)
{
    morph_ops.create(erode.size(), erode.type());

    for(int y = 1; y<morph_ops.rows-1; y++)
    {
        for(int x = 1; x<morph_ops.cols-1; x++)
        {
            morph_ops.at<uchar>(y,x) = erode.at<uchar>(y,x)+dilate.at<uchar>(y,x);
        }
    }
}

void close(cv::Mat dilate, cv::Mat erode, cv::Mat &morph_ops)
{
    morph_ops.create(dilate.size(), dilate.type());

    for(int y = 1; y<morph_ops.rows-1; y++)
    {
        for(int x = 1; x<morph_ops.cols-1; x++)
        {
            morph_ops.at<uchar>(y,x) = dilate.at<uchar>(y,x)+erode.at<uchar>(y,x);
        }
    }
}


void fsiv_apply_mask(const cv::Mat & frame, const cv::Mat & mask, cv::Mat & outframe)
{
   // WRITE ME
}

// ================= OPTIONAL PART STARTS HERE =======================

void fsiv_learn_model(cv::VideoCapture & input, int maxframes, cv::Mat & MeanI, cv::Mat & I2, cv::Mat & VarI, cv::Mat & StdI)
{
   // WRITE ME

}

void fsiv_acc_model(const cv::Mat & frame, cv::Mat & MeanI, cv::Mat & I2)
{
   // WRITE ME
}

void fsiv_segm_by_model(const cv::Mat & frame, cv::Mat & theMask, const cv::Mat & mean, const cv::Mat & std, float t, int r)
{
   // WRITE ME
}

void fsiv_update_model(const cv::Mat & frame, cv::Mat & mean, cv::Mat & I2, cv::Mat &std,  float alpha, const cv::Mat & theMask)
{
   // WRITE ME
}
