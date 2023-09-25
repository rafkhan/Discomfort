#ifndef DISCOMFORT_PAGEBANK_H
#define DISCOMFORT_PAGEBANK_H

class PageBankValue {
  public:
    bool locked = false;
    float lastValue = 0;
    float value = 0;
    float getValue();
    void setValue(float value);
    void lock();
};

class PageBank {
  public:
    PageBank();
    // float pot1 = 0;
    // float pot2 = 0;
    // float pot3 = 0;
    // float pot4 = 0;
    float cv1 = 0;
    float cv2 = 0;
    float cv3 = 0;
    float cv4 = 0;

    PageBankValue pot1;
    PageBankValue pot2;
    PageBankValue pot3;
    PageBankValue pot4;

    void updatePage(
      float p1 = 0,
      float p2 = 0,
      float p3 = 0,
      float p4 = 0,
      float c1 = 0,
      float c2 = 0,
      float c3 = 0,
      float c4 = 0
    );

    void lockPage();
};

#endif
