#ifndef DISCOMFORT_CLIPPER_H
#define DISCOMFORT_CLIPPER_H

#define CLIPPER_MAX_GAIN 20

enum ClipperType { CLIPPER_HARD, CLIPPER_SOFT, CLIPPER_ALT };

class Clipper {
  public:
    static float clip(float input, float gain, ClipperType type);
};

#endif
