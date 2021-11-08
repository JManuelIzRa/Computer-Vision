// ----------------------------------------
// seglib.cpp
// (c) FSIV, University of Cordoba
// ----------------------------------------

#include "seglib.hpp"

void fsiv_segm_by_dif(const cv::Mat & prevFrame, const cv::Mat & curFrame, cv::Mat & difimg, int thr, int r)
{
   // WRITE ME
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
