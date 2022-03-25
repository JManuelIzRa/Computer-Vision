#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>
#include <opencv2/calib3d.hpp>
#include <iostream>

using namespace std;

int main(int argc,char **argv){

    int rows = atoi(argv[1]);
    int cols = atoi(argv[2]);
    int size = atoi(argv[3]);
    string pathToIntrinsics = argv[4];
    string pathToVideo = argv[5];

    cv::Size patternSize = cv::Size( cols-1, rows-1);

    cv::VideoCapture video(pathToVideo);

    cv::Mat_<float> CameraMatrix(3,3);
    CameraMatrix << 1.2713210608312768e+03, 0., 1.0067769621159175e+03, 0., 1.2713210608312768e+03, 5.0032365934190113e+02, 0., 0., 1. ;
    cv::Mat_<float> DistCoefficients(5,1);
    DistCoefficients << -8.6461309780959159e-02, 4.4702519511230371e-01, 5.6516747157794433e-04, 4.1919659681176145e-03, -1.4285968018576394e+00;
    cv::Mat rvec = cv::Mat::zeros(3, 1, CV_32FC1); 
    cv::Mat tvec = cv::Mat::zeros(3, 1, CV_32FC1);

    CameraMatrix.convertTo(CameraMatrix, CV_8UC1);
    DistCoefficients.convertTo(DistCoefficients, CV_8UC1);

    cv::Mat image;
    cv::Mat imagePoints;

    vector<cv::Point3f> objectPoints;

    for(size_t i=0; i<rows; i++){
        for(size_t j=0; j<cols; j++){
            objectPoints.push_back(cv::Point3f(j*size, i*size, 0.0));
        }
    }

    while(video.grab()){

        video.retrieve(image);
        

        cv::findChessboardCorners( image, patternSize, imagePoints);

        cv::cornerSubPix( image, imagePoints, CameraMatrix, DistCoefficients, rvec, tvec);

        cv::solvePnP( objectPoints, imagePoints, CameraMatrix, DistCoefficients, rvec, tvec);

        vector<cv::Point3d> reproject3d;

        reproject3d.push_back(cv::Point3d(0.0,0.0,0.0));
        reproject3d.push_back(cv::Point3d(size, 0.0, 0.0));
        reproject3d.push_back(cv::Point3d(0.0, size, 0.0));
        reproject3d.push_back(cv::Point3d(0.0, 0.0, -size));

        cv::Mat RodriguesM;

        cv::Rodrigues(rvec, RodriguesM);

        vector<cv::Point2d> reproject2d;

        reproject2d.resize(4);

        projectPoints(reproject3d, RodriguesM, tvec, CameraMatrix, DistCoefficients, reproject2d);

        cv::line(image, reproject2d[0], reproject2d[1], cv::Scalar(0,0,255), 2, CV_AA);
        cv::line(image, reproject2d[0], reproject2d[2], cv::Scalar(0,255,0), 2, CV_AA);
        cv::line(image, reproject2d[0], reproject2d[3], cv::Scalar(255,0,0), 2, CV_AA);


        cv::imshow("image",image);
        cv::waitKey(10);
    }

}


//exe row col size .yml video