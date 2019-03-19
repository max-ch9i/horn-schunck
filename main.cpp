#include <iostream>

#include <opencv2/opencv.hpp>
using namespace cv;

int main()
{
  std::cout << "hi there" << std::endl;
  Mat image, image2;
  image = imread( "1.jpg", IMREAD_COLOR );
  image2 = imread( "2.jpg", IMREAD_COLOR );
  Mat grey, grey2;
  cvtColor(image, grey, COLOR_BGR2GRAY);
  cvtColor(image2, grey2, COLOR_BGR2GRAY);
  Mat res(grey2);

  typedef unsigned char VT;
  typedef int B;
  MatConstIterator_<VT> ptr1 = grey.begin<VT>();
  MatConstIterator_<VT> end1 = grey.end<VT>();
  MatConstIterator_<VT> ptr2 = grey2.begin<VT>();
  MatConstIterator_<VT> end2 = grey2.end<VT>();

  MatIterator_<VT> ptr_res = res.begin<VT>();

  ++ptr1;
  while (ptr1 != end1-1) 
  {
    VT prev = *(--ptr1);
    VT next = *(2 + ptr1);
    B diff = next-prev;
    *ptr_res = diff;

    // VT a = *ptr1;
    // VT b = *ptr2;

    // int c = a+b;

    // *ptr_res = c;
    // ++ptr1;
    // ++ptr2;
    // ++ptr_res;
  }

  imwrite("alpha.jpg", res);

  std::cout << "done" << std::endl;
}
