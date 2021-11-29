// ----------------------------------------
// common_code.cpp
// (c) FSIV, University of Cordoba
// ----------------------------------------

#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

#include "common_code.hpp"
#include <opencv2/imgproc.hpp>

#ifndef NDEBUG
#include<opencv2/highgui.hpp>

const std::string __Debug_Padding =
"                                                                              "
"                                                                              "
"                                                                              "
"                                                                              "
"                                                                              ";
size_t __Debug_PPL = 3;
#endif

cv::Mat
fsiv_normalize_mean_var(cv::Mat const& src)
{
    assert(!src.empty() && src.channels()==1);
    cv::Mat dst;

    //TODO: normalize source image so its mean will be equal to 0.0 and
    // its var equal to 1.0.
    // Hint: use cv::meanStdDev() to get the source mean and stdev.

    cv::normalize ( src, dst, 1, 0, cv::NORM_L2, -1, cv::noArray() ); 		

    //
#ifndef NDEBUG
    assert(!dst.empty());
    {
        std::vector<double> mean, stdev;
        cv::meanStdDev(dst, mean, stdev);
        assert(std::abs(mean[0])<=1.0e-5 && std::abs(stdev[0]-1.0)<=1.0e-5);
    }
#endif
    return dst;
}

cv::Mat
fsiv_normalize_minmax(cv::Mat const& src)
{
    assert(!src.empty() && src.channels()==1);
    cv::Mat dst;

    //TODO: normalize the source image so its mininum value will be 0.0 and its
    // maximun value be 1.0
    // Hint: use cv::normalize()

    cv::normalize ( src, dst, 1.0, 0.0, cv::NORM_L2, -1, cv::noArray() ); 		

    //

#ifndef NDEBUG
    assert(!dst.empty());
    {
        assert(dst.type()==CV_32FC1);
        double min,max;
        cv::minMaxLoc(dst, &min, &max);
        assert(std::abs(min)<=1.0e-6 && std::abs(max-1.0)<=1.0e-6);
    }
#endif
    return dst;
}

bool
string_to_ncells(const std::string& str, int n_cells[])
{
    bool ret_val = true;
    std::istringstream input(str);
    char sep;
    input >> n_cells[0] >> sep >> n_cells[1];
    if (!input)
        ret_val = false;
    else
        ret_val = ((n_cells[0]*n_cells[1]) > 0);
    return ret_val;
}

bool
load_gtsrb_categories(const std::string& dataset_pathname,
                      std::vector<int>& cats,
                      std::vector<std::string>& descs)
{
    bool ret_val = false;
    std::ifstream in(dataset_pathname+"/train/CLASSES.TXT");
    if (in)
    {
        int class_label = 0;
        char sep = ' ';
        std::string class_description;
        while(in)
        {
            in >> class_label >> sep >> sep >> sep >> class_description;
            if (in)
            {
                cats.push_back(class_label);
                descs.push_back(class_description);
            }
        }
        ret_val = true;
    }
    return ret_val;
}

bool
load_gtsrb_train_metadata(const std::string& metadata_file,
                          std::vector<std::string> & lfiles,
                          std::vector<cv::Rect>& rois,
                          cv::Mat & labels)
{
    bool ret_val = true;
    DEBUG(1, "Loading metadata from file '"<< metadata_file << "'." << std::endl);
    std::ifstream input (metadata_file);
    if (input)
    {
        std::vector<int> labels_v;
        std::istringstream line;
        std::string dataset_path;

        size_t pos = metadata_file.rfind("/");
        if (pos != std::string::npos)
            dataset_path=metadata_file.substr(0, pos);
        else
            dataset_path=".";
        DEBUG(2, "\tDataset path is: "<< dataset_path << "'." << std::endl);
        while(input && ret_val)
        {
            std::string buffer;
            //get a line.
            input >> buffer;
            if (input)
            {
                DEBUG(3,"\tDecoding line: '"<< buffer << "'."<< std::endl);
                //replace ; by ' '
                for(size_t i=0;i<buffer.size(); ++i)
                    if (buffer[i]==';')
                        buffer[i]=' ';
                //get the line's metadata.
                std::istringstream line (buffer);
                std::string filename;
                int w, h, x1, y1, x2, y2, label;
                line >> filename >> w >> h >> x1 >> y1 >> x2 >> y2 >> label;
                if (line)
                {
                    lfiles.push_back(dataset_path+'/'+filename);
                    rois.push_back(cv::Rect(x1, y1, x2-x1, y2-y1));
                    labels_v.push_back(label);
                    DEBUG(3,"\tDecoded fname: '" << lfiles.back() << "'." << std::endl);
                    DEBUG(3,"\tDecoded roi  :  " << rois.back() << std::endl);
                    DEBUG(3,"\tDecoded label:  " << labels_v.back() << std::endl);
                }
                else
                    ret_val = false;
            }
        }
        if (ret_val)
        {
            //Transform vector to cv::Mat.
            labels = cv::Mat(labels_v.size(), 1, CV_32SC1);
            std::copy(labels_v.begin(), labels_v.end(), labels.begin<int>());
        }
    }
    else
        ret_val = false;
    return ret_val;
}

bool
load_gtsrb_test_metadata(const std::string& dataset_path,
                          std::vector<std::string> & lfiles,
                          std::vector<cv::Rect>& rois,
                          cv::Mat & labels)
{
    bool ret_val = true;
    std::string metadata_file = dataset_path + "/test/metadada.csv";
    DEBUG(1, "Loading metadata from file '"<< metadata_file << "'."
          << std::endl);
    std::ifstream input (metadata_file);
    if (input)
    {
        std::vector<int> labels_v;
        std::istringstream line;
        while(input && ret_val)
        {
            std::string buffer;
            //get a line.
            input >> buffer;
            if (input)
            {
                DEBUG(3,"\tDecoding line: '"<< buffer << "'."<< std::endl);
                //replace ; by ' '
                for(size_t i=0;i<buffer.size(); ++i)
                    if (buffer[i]==';')
                        buffer[i]=' ';
                //get the line's metadata.
                std::istringstream line (buffer);
                std::string filename;
                int w, h, x1, y1, x2, y2, label;
                line >> filename >> w >> h >> x1 >> y1 >> x2 >> y2 >> label;
                if (line)
                {
                    lfiles.push_back(dataset_path+"/test/"+filename);
                    rois.push_back(cv::Rect(x1, y1, x2-x1, y2-y1));
                    labels_v.push_back(label);
                    DEBUG(3,"\tDecoded fname: '" << lfiles.back() << "'." << std::endl);
                    DEBUG(3,"\tDecoded roi  :  " << rois.back() << std::endl);
                    DEBUG(3,"\tDecoded label:  " << labels_v.back() << std::endl);
                }
                else
                    ret_val = false;
            }
        }
        if (ret_val)
        {
            //Transform vector to cv::Mat.
            labels = cv::Mat(labels_v.size(), 1, CV_32SC1);
            std::copy(labels_v.begin(), labels_v.end(), labels.begin<int>());
        }
    }
    else
        ret_val = false;
    return ret_val;
}

bool fsiv_desc_simple_gray(const cv::Mat & image, cv::Mat & desc)
{
    //TODO: compute the descriptor and save into desc

    return true;
}


bool
fsiv_compute_desc_from_list(const std::vector<std::string> & lfiles,
                      const std::vector<cv::Rect>& rois,
                      const cv::Size& canonical_size,
                      cv::Mat & l_descs, const int desctype,
                      const int img_norm, bool hist_norm)
{
    DEBUG(1, "Computing lbp descriptors from files" << std::endl);
    bool ret_val = true;
    for (size_t i =0; i < lfiles.size() && ret_val; i++)
    {
        DEBUG(2, "\t Processing image: '" << lfiles[i] << "'." << std::endl);
        cv::Mat image = cv::imread(lfiles[i], cv::IMREAD_GRAYSCALE);       
        if (!image.empty())
        {
            image.convertTo(image, CV_32F, 1.0/255.0, 0.0);
#ifndef NDEBUG
            if (__Debug_Level>=3)
            {
                cv::imshow("IMAGE", image);
                cv::imshow("ROI", image(rois[i]));
            }
#endif
            if (img_norm==1)
                image = fsiv_normalize_minmax(image);
            else if (img_norm==2)
                image = fsiv_normalize_mean_var(image);

            cv::Mat canonical_img;
            cv::resize(image(rois[i]), canonical_img, canonical_size);            

            // TODO: compute the selected descriptor
            cv::Mat vimg_mat;
            if (desctype == 0)
                fsiv_desc_simple_gray(canonical_img, vimg_mat);
            // else ....    

            if (i==0)
            {
                l_descs = cv::Mat(lfiles.size(), vimg_mat.cols, CV_32FC1);
                vimg_mat.copyTo(l_descs.row(0));
            }
            else
                vimg_mat.copyTo(l_descs.row(i));
                
#ifndef NDEBUG
            if (__Debug_Level>=3)
            {
                if ((cv::waitKey(0)&0xff)==27)
                    return false;
            }
#endif
        }
        else
            ret_val = false;
    }
#ifndef NDEBUG
            if (__Debug_Level>=3)
                cv::destroyAllWindows();
#endif
    return ret_val;
}


float
compute_file_size(std::string const& fname, const long units)
{
    float size = -1.0;
    std::ifstream file (fname);
    if (file)
    {
        file.seekg (0, file.end);
        long length = file.tellg();
        size = static_cast<float>(length) / static_cast<float>(units);
    }
    return size;
}


