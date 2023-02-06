#include "Discomfort.h"
#include "Folder.h"

Discomfort::Discomfort() {}

float Discomfort::process(
      float input,
      float foldGain,
      float shaperGain,
      float tilt,
      float dryWet
) {
  return Folder::fold(input, foldGain);
}
