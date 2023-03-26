#ifndef DISCOMFORT_FOLDER_H
#define DISCOMFORT_FOLDER_H

#define FOLDER_MIN_GAIN 1
#define FOLDER_MAX_GAIN 50

class Folder {
  public:
    static float fold(float input, float gain, float offset);
};

#endif
