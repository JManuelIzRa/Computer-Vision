// ----------------------------------------
// train.cpp
// (c) FSIV, University of Cordoba
// ----------------------------------------

#include <iostream>
#include <exception>

//Includes para OpenCV, Descomentar según los módulo utilizados.
#include <opencv2/core/core.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/ml/ml.hpp>

#include "common_code.hpp"
// #include "lbp.hpp"
#include "metrics.hpp"

#ifndef NDEBUG
int __Debug_Level = 0;
#endif

const cv::String keys =
    "{help h usage ? |      | print this message   }"        
    "{img_norm       |0    | Image normalization: 0: none, 1:minmax, 2:mean_stdDev.}"
    "{v validate     |      | Use the training/validation partitions to train "
    "and validate, else use both partitions to train again an already trained model.}"
    "{t test         |      | Only test a trained model.}"
    "{class        |0     | Classifier to train/test. 0: K-NN, 1:SVM, 2:RTREES.}"
    "{d desc       |0     | Descriptor type. 0: gray, 1: ???}"
    "{knn_K        |1     | Parameter K for K-NN class.}"
    "{svm_C        |1.0   | Parámeter C for SVM class.}"
    "{svm_K        |0     | Kernel to use with SVM class. 0:Linear, 1:Polinomial. "
    "2:RBF, 3:SIGMOID, 4:CHI2, 5:INTER}"
    "{svm_D        |3.0   | Degree of svm polinomial kernel.}"
    "{svm_G        |1.0   | Gamma for svm RBF kernel.}"
    "{rtrees_V     |0     | Num of features random sampled per node. "
    "Default 0 meas sqrt(num. of total features).}"
    "{rtrees_T     |50    | Max num. of rtrees in the forest.}"
    "{rtrees_E     |0.1   | OOB error to stop adding more rtrees.}"
    "{@dataset_path  |<none>| Dataset pathname.}"
    "{@model         |<none>| Model filename.}"
#ifndef NDEBUG
    "{verbose        |0     | Set the verbose level.}"
#endif
    ;

int
main (int argc, char* const* argv)
{
  int retCode=EXIT_SUCCESS;

  try {

      cv::CommandLineParser parser(argc, argv, keys);
      parser.about("Train a traffic sign classifier using the GTSRB dataset.");
      if (parser.has("help"))
      {
          parser.printMessage();
          return 0;
      }

#ifndef NDEBUG
      __Debug_Level = parser.get<int>("verbose");
#endif      
      bool validate = parser.has("v");
      bool only_testing = parser.has("t");
      if (validate && only_testing)
      {
          std::cerr << "Error: Validating and only testing are not "
                       "compatible options." << std::endl;
          return EXIT_FAILURE;
      }
      int img_norm = parser.get<int>("img_norm");

      std::string dataset_path = parser.get<std::string>("@dataset_path");
      std::string model_fname = parser.get<std::string>("@model");
      int classifier = parser.get<int>("class");
      int desctype = parser.get<int>("desc");
      int knn_K = parser.get<int>("knn_K");
      float svm_C = parser.get<float>("svm_C");
      int svm_K = parser.get<int>("svm_K");
      float svm_D = parser.get<float>("svm_D");
      float svm_G = parser.get<float>("svm_G");
      int rtrees_V = parser.get<int>("rtrees_V");
      int rtrees_T = parser.get<int>("rtrees_T");
      double rtrees_E = parser.get<double>("rtrees_T");
      if (!parser.check())
      {
          parser.printErrors();
          return 0;
      }

      cv::Mat train_descs, train_labels;
      cv::Mat val_descs, val_labels;      
      bool hist_norm = true;      
      cv::Size canonical_size(64, 64);

      std::vector<int> categories;
      std::vector<std::string> category_descs;
      if (!load_gtsrb_categories(dataset_path, categories, category_descs))
      {
          std::cerr << "Error: could not load the categories dataset file."
                    << std::endl;
          return EXIT_FAILURE;
      }
      std::cout << "Working with [" << categories.size() <<  "] categories: ";
      for (size_t c=0;c<categories.size();++c)
          std::cout << '\'' << category_descs[c] << "' ";
      std::cout << std::endl;


      if (validate)
      {
          std::vector<std::string> train_images;
          std::vector<cv::Rect> train_rois;

          if (!load_gtsrb_train_metadata(dataset_path+"/train/train_train.csv",
                                         train_images, train_rois, train_labels))
          {
              std::cerr << "Error: could not load training data." << std::endl;
              return EXIT_FAILURE;
          }

          if(!fsiv_compute_desc_from_list(train_images,
                                    train_rois,
                                    canonical_size,
                                    train_descs,
                                    desctype,
                                    img_norm,
                                    hist_norm))
          {
              std::cerr << "Error: could not compute training features."
                        << std::endl;
              return EXIT_FAILURE;
          }
          else
              std::cerr << "Computed training features." << std::endl;



          std::vector<std::string> val_images;
          std::vector<cv::Rect> val_rois;

          if (!load_gtsrb_train_metadata(dataset_path+"/train/train_valid.csv",
                                         val_images, val_rois, val_labels))
          {
              std::cerr << "Error: could not load training data." << std::endl;
              return EXIT_FAILURE;
          }

          if(!fsiv_compute_desc_from_list(val_images,
                                     val_rois,
                                    canonical_size,
                                     val_descs,
                                     desctype,
                                     img_norm,
                                     hist_norm))    /* To be COMPLETED by the student ...*/

          {
              std::cerr << "Error: could not compute validation features."
                        << std::endl;
              return EXIT_FAILURE;
          }
          else
              std::cerr << "Computed validation features." << std::endl;
      }
      else
      {
          std::vector<std::string> train_images;
          std::vector<cv::Rect> train_rois;
          if (!only_testing)
          {
              if (!load_gtsrb_train_metadata(dataset_path+"/train/train_all.csv",
                                             train_images, train_rois, train_labels))
              {
                  std::cerr << "Error: could not load training data." << std::endl;
                  return EXIT_FAILURE;
              }
              if(!fsiv_compute_desc_from_list(train_images,
                                        train_rois,
                                        canonical_size,
                                        train_descs,
                                        desctype,
                                        img_norm,
                                        hist_norm))
              {
                  std::cerr << "Error: could not compute training features."
                            << std::endl;
                  return EXIT_FAILURE;
              }
              else
                  std::cerr << "Computed training features." << std::endl;
          }
          std::vector<std::string> test_images;
          std::vector<cv::Rect> test_rois;

          if (!load_gtsrb_test_metadata(dataset_path, test_images,
                                        test_rois, val_labels))
          {
              std::cerr << "Error: could not load testing data." << std::endl;
              return EXIT_FAILURE;
          }

          if(!fsiv_compute_desc_from_list(test_images,
                                     test_rois,
                                    canonical_size,
                                     val_descs,
                                     desctype,
                                     img_norm,
                                     hist_norm))

          {
              std::cerr << "Error: could not compute testing features."
                        << std::endl;
              return EXIT_FAILURE;
          }
          else
              std::cerr << "Computed testing features." << std::endl;
      }

      cv::Ptr<cv::ml::StatModel> clsf;
      int train_flags = 0;

      if (validate)
      {
          if (classifier == 0)
          {
              cv::Ptr<cv::ml::KNearest> knn;

              //TODO: Create an KNN classifier.
              //Set algorithm type to BRUTE_FORCE.
              //Set it as a classifier (setIsClassifier)
              //Set hyperparameter K.


              //
              assert(knn != nullptr);
              clsf = knn;
          } else if (classifier == 1)
          {
              cv::Ptr<cv::ml::SVM> svm;

              //TODO: Create an SVM classifier.
              //Set algorithm type to C_SVC.
              //Set it as a classifier (setIsClassifier)
              //Set hyperparameters: C, kernel, Gamma, Degree.



              //
              assert(svm!=nullptr);
              clsf = svm;
          }
          else if (classifier == 2)
          {
              cv::Ptr<cv::ml::RTrees> rtrees;
              //TODO: Create an RTrees classifier.
              //Set hyperparameters: Number of features used per node (ActiveVarCount).
              //Remenber that rtrees_V=0 means to use the default value.
              //Set the max num of trees rtrees_T, and required OOB error rtrees_E
              //using a cv::TermCriteria object.


              //
              assert(rtrees!=nullptr);
              clsf = rtrees;
          }
          else
          {
              std::cerr << "Error: unknown classifier." << std::endl;
              return EXIT_FAILURE;
          }
      }
      else
      {
          train_flags = cv::ml::StatModel::UPDATE_MODEL;
          if (classifier==0)
              //TODO: load a KNN classifier.
              ;
              //
          else if (classifier==1)
              //TODO: load a SVM classifier.
              ;
              //
          else if (classifier==2)
              //TODO: load a RTrees classifier.
              ;
              //
          else
          {
              std::cerr << "Error: unknown classifier." << std::endl;
              return EXIT_FAILURE;
          }
          if (clsf==nullptr || !clsf->isTrained())
          {
              std::cerr << "Error: Not validate mode activated. I need a trained model!" << std::endl;
              return EXIT_FAILURE;
          }
      }
      if (!only_testing)
      {
          std::cout << "Training with "<< train_labels.rows << " samples ... ";

          cv::Ptr<cv::ml::TrainData> train_data =
                  cv::ml::TrainData::create(train_descs, cv::ml::ROW_SAMPLE,
                                            train_labels);

          //TODO: train the classifer using training data.        
          

          //
          assert(clsf->isTrained());
          std::cout << "done." << std::endl;
      }

      if (validate)
          std::cerr << "Validating with ";
      else
          std::cerr << "Testing with ";
      std::cout << val_labels.rows << " samples ... ";

      cv::Mat predict_labels;
      std::cout << "done." << std::endl;

      //TODO: make the predictions over the validation/test data.


      //
      assert(!predict_labels.empty() && predict_labels.rows == val_descs.rows);

      //Compute the confusion matrix.
      cv::Mat cmat = compute_confusion_matrix(cv::Mat_<float>(val_labels),
                                              predict_labels);

      float mrr = compute_mean_recognition_rate(cmat, categories);
      float acc = compute_accuracy(cmat);

      std::cout << "##############################" << std::endl;
      std::cout << "# Model metrics: ";
      if (validate)
          std::cout << " VALIDATION" << std::endl;
      else
          std::cout << " TESTING" << std::endl;
      std::cout << "##############################" << std::endl;
      std::cout << std::endl;
      std::cout << "Accuracy : " << acc << std::endl;
      std::cout << "Mean RR  : " << mrr << std::endl;
      std::cout << "RR per class: " << std::endl;
      for (size_t c=0;c<categories.size();c++)
          std::cout << "\t[" << category_descs[c] << "]: "
                    << compute_recognition_rate(cmat, categories[c])
                    << std::endl;
      std::cout << std::endl;

      if (!only_testing)
      {
          std::cout << "Saving the model to '" << model_fname << "'." << std::endl;
          clsf->save(model_fname);
      }

      std::cout << "Model size: " << compute_file_size(model_fname, 1024*1024)
                << " MB." << std::endl;
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception caught: " << e.what() << std::endl;
    retCode = EXIT_FAILURE;
  }
  return retCode;
}
