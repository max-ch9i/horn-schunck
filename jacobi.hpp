#ifndef JACOBI_H

#define JACOBI_H
#include <iostream>
#include <opencv2/opencv.hpp>

template <class T, int size=2, int type = CV_32F>
cv::Vec<T, size> jacobi_iterate(cv::Mat_<T> A)
{
  cv::Mat_<T> A_diagonal = A.diag();
  cv::Mat_<T> ones = cv::Mat::ones(A_diagonal.rows, 1, type) / A_diagonal;
  cv::Mat_<T> D_inv = cv::Mat::diag(ones);

  std::cout << D_inv << std::endl;
  
  return cv::Vec<T, size>(1,1);
}




#endif /* end of include guard: JACOBI_H */
