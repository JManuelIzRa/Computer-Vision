#include <opencv2/core/core.hpp> //core routines
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>//imread,imshow,namedWindow,waitKey
#include <opencv2/highgui.hpp>
#include <opencv2/calib3d.hpp>

#include <iostream>

using namespace std;
using namespace cv;



int main(int argc,char **argv){
try{
   
   vector<Point2f> corners;
   Size patternSize(6,4);
   cv::Mat image;

   int rows=atoi(argv[1]);
   int cols=atoi(argv[2]);
   int size=atoi(argv[3]);
   string intrinsics = argv[4];
   std::string pathToVideo=argv[5];
   cv::VideoCapture video(pathToVideo);

   float error;
   int height, width;
   cv::Mat matrix;
   cv::Mat dist_coeffs;

   cv::Mat rvec = cv::Mat::zeros(3, 1, CV_32FC1); //Will have the camera's pose: rotation vector
   cv::Mat tvec = cv::Mat::zeros(3, 1, CV_32FC1); //Will have the camera's pose: traslation vector
   const cv::TermCriteria criteria(cv::TermCriteria::MAX_ITER+cv::TermCriteria::EPS, 30, 0.001);

   FileStorage fs;
   fs.open("../intrinsics.yml", FileStorage::READ);


   if(fs.isOpened()==false)
   {;
     return 0;
   }


   fs["image_height"] >> height;
   fs["image_width"] >> width;
   fs["camera_matrix"] >> matrix;
   fs["distortion_coefficients"] >> dist_coeffs;
   fs["avg_reprojection_error"] >> error;


   cout<<matrix<<endl;
   cout<<error<<endl;
   cout<<dist_coeffs<<endl;
   cout<<width<<" "<<height<<endl;

   std::vector<cv::Point3f> obj_points;
   std::vector<cv::Point2f> image_points;

    for(size_t i=0; i<rows; i++)
    {
        for(size_t j=0; j<cols; j++)
        {
            obj_points.push_back(Point3f(j*size, i*size, 0.0));
        }
    }

    Mat image_gray;
   while(video.grab()){
       video.retrieve(image);
       if(findChessboardCorners(image,  Size(cols, rows), corners, CALIB_CB_FAST_CHECK)==false)
       {
         cout<<"Error1\n";
         return 0;
       }

       cvtColor(image,image_gray,COLOR_BGR2GRAY);

       cornerSubPix(image_gray, corners, Size(11,11), Size (-1,-1),criteria);

       if(solvePnP(obj_points, corners,  matrix,  dist_coeffs,  rvec,  tvec)==false)
       {
         cout<<"Error2\n";
         return 0;
       }

       std::vector<Point3d> reproject3d;

       reproject3d.push_back(Point3d(0.0,0.0,0.0));
       reproject3d.push_back(Point3d(size, 0.0, 0.0));
       reproject3d.push_back(Point3d(0.0, size, 0.0));
       reproject3d.push_back(Point3d(0.0, 0.0, -size));




       Mat rmat;

       Rodrigues(rvec, rmat);

       std::vector<Point2d> reproject2d;
       reproject2d.resize(4);
       projectPoints(reproject3d, rmat, tvec, matrix, dist_coeffs, reproject2d);


      // getPerspectiveTransform(image,perspective);

       line(image, reproject2d[0], reproject2d[1], Scalar(0,0,255), 2, LINE_AA);
       line(image, reproject2d[0], reproject2d[2], Scalar(0,255,0), 2, LINE_AA);
       line(image, reproject2d[0], reproject2d[3], Scalar(255,0,0), 2, LINE_AA);


       vector<cv::Point3f> warpPoint3d;
       vector<cv::Point2f> warpPoint2d;
       vector<cv::Point2f> image2dPoints;
       warpPoints2d.resize();

      warpPoint3d.push_back(cv::Point3f(0.0, 0.0, 0.0));
      warpPoint3d.push_back(cv::Point3f(5*size, 0.0, 0.0));
      warpPoint3d.push_back(cv::Point3f(0.0, 3*size, 0.0));
      warpPoint3d.push_back(cv::Point3f(5*size, 3*size, 0.0));

      image2dPoints.push_back(cv::Point2f(0.0, 0.0));
      image2dPoints.push_back(cv::Point2f(width-1, 0.0));
      image2dPoints.push_back(cv::Point2f(0.0, height-1));
      image2dPoints.push_back(cv::Point2f(width-1, height-1));

      projectPoints(warpPoints3d, rmat, tvec, matrix, dist_coeffs, warpPoints2d);

      line(image, warpPoint2d[0], warpPoint2d[1], cv::Scalar(255,255,0), 2, cv::LINE_AA);
      line(image, warpPoint2d[1], warpPoint2d[3], cv::Scalar(255,255,0), 2, cv::LINE_AA);
      line(image, warpPoint2d[0], warpPoint2d[2], cv::Scalar(255,255,0), 2, cv::LINE_AA);
      line(image, warpPoint2d[3}, warpPoint2d[1], cv::Scalar(255,255,0), 2, cv::LINE_AA);

      Mat perspective, result;

      perspective = getPerspectiveTransform(image2dPoints, warpPoints2d);

      warpPerspective(image, result, perspective, resolution);

      for(int i=0; y<=result.rows; y++)
      {
          uchar *ptr = image.ptr<uchar>(y);
          uchar *ptr2 = result.ptr<uchar>(y);

          for(int x=0; x<= result.cols; x++)
          {
            if (ptr2[0]==0 && ptr2[1]==0 && ptr2[2]==0)
            {
              /* code */
              ptr2[0] = ptr[0];
              ptr2[1] = ptr[1];
              ptr2[2] = ptr[2];
            }

            ptr2 += 3;
            ptr += 3;
            
          }
      }

       cv::imshow("image",image);
       cv::waitKey(10);
   }

  }catch(std::exception &ex)
  {
    cout<<ex.what()<<endl;
  }
}
