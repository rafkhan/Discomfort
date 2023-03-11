#ifndef DISCOMFORT_FOLLOWER_H
#define DISCOMFORT_FOLLOWER_H

class Follower {
public:
  Follower(float sampleRate, float a, float b);
  float process(float x, float attackMs, float decayMs);

private:
  float a_, b_, y_, sampleRate;
};

#endif