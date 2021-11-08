// ----------------------------------------
// seglib.cpp
// (c) FSIV, University of Cordoba
// ----------------------------------------

#include "seglib.hpp"

void fsiv_segm_by_dif(const cv::Mat & prevFrame, const cv::Mat & curFrame, cv::Mat & difimg, int thr, int r)
{
   // WRITE ME
   difimg.create(curFrame.size(), curFrame.type());

   for( int y = 0; y<curFrame.rows-2; y++ )
      for( int x = 0; x<curFrame.cols-2; x++ )
      {
         difimg.at<float>(y,x) = prevFrame.at<float>(y,x) - curFrame.at<float>(y,x);
      }


   threshold(difimg, thr, difimg);

   cv::namedWindow("Threshold");
   cv::imshow("Threshold", difimg);

}

//Threshold functcion
void threshold(cv::Mat in, int threshold_value, cv::Mat &out)
{
   out.create(in.size(), in.type());

   for( int y = 0; y<in.rows-2; y++ )
         for( int x = 0; x<in.cols-2; x++ )
         {
            if(in.at<float>(y,x) < threshold_value)
               out.at<float>(y,x) = 255;
            else
               out.at<float>(y,x) = 0;
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
