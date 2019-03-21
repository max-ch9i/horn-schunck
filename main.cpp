#include <iostream>
#include <iomanip>
#include <string>
#include <opencv2/opencv.hpp>
#include "jacobi.hpp"
#include <vector>

// ffmpeg -framerate 24 -i derivative/%03d.jpg out.mp4

using namespace cv;

template <class T>
struct neighbourhood;

template <class T, class U>
U dee_I_dee_x(const neighbourhood<T>& hood)
{
  // First 
  U ret = (hood.I_xd_y_t + hood.I_xd_y_td + hood.I_xd_yd_t + hood.I_xd_yd_td)/4
            - (hood.I_x_y_t + hood.I_x_y_td + hood.I_x_yd_t + hood.I_x_yd_td)/4;

  return ret;
}

template <class T, class U>
U dee_I_dee_y(const neighbourhood<T>& hood)
{
  U ret = (hood.I_x_yd_t + hood.I_x_yd_td + hood.I_xd_yd_t + hood.I_xd_yd_td)/4
            - (hood.I_x_y_t + hood.I_x_y_td + hood.I_xd_y_t + hood.I_xd_y_td)/4;

  return ret;
}

template <class T, class U>
U dee_I_dee_t(const neighbourhood<T>& hood)
{
  U ret = (hood.I_x_y_td + hood.I_x_yd_td + hood.I_xd_y_td + hood.I_xd_yd_td)/4
            - (hood.I_x_y_t + hood.I_x_yd_t + hood.I_xd_y_t + hood.I_xd_yd_t)/4;

  return ret;
}

template <class T>
struct neighbourhood
{
  T I_x_y_t;
  T I_xd_y_t;
  T I_xd_yd_t;
  T I_x_yd_t;

  T I_x_y_td;
  T I_xd_y_td;
  T I_xd_yd_td;
  T I_x_yd_td;
};

template <class T>
neighbourhood<T> fetch_hood(__uint16_t cols, MatConstIterator_<T> t0, MatConstIterator_<T> t1)
{
  T I_x_y_t = *t0;
  T I_xd_y_t = *(++t0);
  T I_xd_yd_t = *(t0+=cols);
  T I_x_yd_t = *(--t0);
  t0-=cols;

  T I_x_y_td = *t1;
  T I_xd_y_td = *(++t1);
  T I_xd_yd_td = *(t1+=cols);
  T I_x_yd_td = *(--t1);
  t1-=cols;

  neighbourhood<T> ret = {
    I_x_y_t,
    I_xd_y_t,
    I_xd_yd_t,
    I_x_yd_t,

    I_x_y_td,
    I_xd_y_td,
    I_xd_yd_td,
    I_x_yd_td
  };

  return ret;
}

int main()
{
  std::vector<float> vec{1,2,3,4};
  jacobi_iterate<float, 4>(cv::Mat_<float>(2,2, vec.data()));
  return 0;
  std::cout << "Differentiating..." << std::endl;

  Mat image, image2;

  for (int i = 1; i < 10000; ++i)
  {
    std::cout << "Differentiating " << i << " ..." << std::endl;

    std::stringstream path_im;
    if (image2.rows == 0)
    {
      path_im << "video/" << std::setfill('0') << std::setw(3) << i << ".jpg";
      image = imread( path_im.str(), IMREAD_COLOR );
    }
    else
    {
      image = image2;
    }

    path_im.str(std::string());
    path_im << "video/" << std::setfill('0') << std::setw(3) << (i + 1) << ".jpg";
    image2 = imread( path_im.str(), IMREAD_COLOR );

    Mat res(image);

    Mat grey, grey2;
    cvtColor(image, grey, COLOR_BGR2GRAY);
    cvtColor(image2, grey2, COLOR_BGR2GRAY);

    typedef unsigned char VT;
    typedef Vec<unsigned char, 3> V3T;
    typedef int B;

    MatConstIterator_<VT> ptr1 = grey.begin<VT>();
    MatConstIterator_<VT> end1 = grey.end<VT>();
    MatConstIterator_<VT> ptr2 = grey2.begin<VT>();
    MatConstIterator_<VT> end2 = grey2.end<VT>();

    MatIterator_<V3T> ptr_res = res.begin<V3T>();

    ++ptr1;
    ++ptr_res;
    while (ptr1 != end1-1) 
    {
      neighbourhood<VT> hood = fetch_hood(res.cols, ptr1, ptr2);
      B I_x = dee_I_dee_x<VT, B>(hood);
      B I_y = dee_I_dee_y<VT, B>(hood);
      B I_t = dee_I_dee_t<VT, B>(hood);
      *ptr_res = V3T(abs(I_x), abs(I_y), abs(I_t));

      ++ptr_res;
      ++ptr1;
      ++ptr2;
    }

    path_im.str(std::string());
    path_im << "derivative/" << std::setfill('0') << std::setw(3) << i << ".jpg";

    imwrite(path_im.str(), res);

  }

  std::cout << "Done..." << std::endl;
}
