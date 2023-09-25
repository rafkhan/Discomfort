#include "PageBank.h"

PageBank::PageBank(void) {
  this->pot1 = 0;
  this->pot2 = 0;
  this->pot3 = 0;
  this->pot4 = 0;
  this->cv1 = 0;
  this->cv2 = 0;
  this->cv3 = 0;
  this->cv4 = 0;
}

void PageBank::updatePage(
  float p1,
  float p2,
  float p3,
  float p4,
  float c1,
  float c2,
  float c3,
  float c4
) {
  this->pot1 = p1;
  this->pot2 = p2;
  this->pot3 = p3;
  this->pot4 = p4;
  this->cv1 = c1;
  this->cv2 = c2;
  this->cv3 = c3;
  this->cv4 = c4;
}
