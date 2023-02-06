#ifndef DISCOMFORT_H
#define DISCOMFORT_H

class Discomfort {
  public:
    Discomfort();
    float process(
      float input,
      float foldGain,
      float shaperGain,
      float tilt,
      float dryWet
    );
};

#endif