#ifndef DISCOMFORT_PAGEBANK_H
#define DISCOMFORT_PAGEBANK_H

class PageBank {
  public:
    float pot1 = 0;
    float pot2 = 0;
    float pot3 = 0;
    float pot4 = 0;
    float cv1 = 0;
    float cv2 = 0;
    float cv3 = 0;
    float cv4 = 0;

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
};

#endif
