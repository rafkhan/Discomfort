#include "PageBank.h"
#include <cmath>

float closeEnoughToLastValue(float oldValue, float newValue) {
  return abs((oldValue - newValue)) < 0.001;
}

PageBank::PageBank(void) {}

void PageBankValue::lock() {
  this->locked = true;
  this->lastValue = this->value;
}

float PageBankValue::getValue() {
  if(this->locked) {
    return this->lastValue;
  }

  return this->value;
}

void PageBankValue::setValue(float value) {
  if (this->locked && closeEnoughToLastValue(lastValue, value)) {
    this->locked = false;
  }

  if(!this->locked) {
    this->value = value;
  }
}

void PageBank::lockPage() {
  this->pot1.lock();
  this->pot2.lock();
  this->pot3.lock();
  this->pot4.lock();
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
  this->pot1.setValue(p1);
  this->pot2.setValue(p2);
  this->pot3.setValue(p3);
  this->pot4.setValue(p4);
  // this->cv1 = c1;
  // this->cv2 = c2;
  // this->cv3 = c3;
  // this->cv4 = c4;
}
