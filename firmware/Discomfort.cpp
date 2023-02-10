#include "Discomfort.h"
#include "Folder.h"
#include "Shaper.h"
#include "DryWet.h"

Discomfort::Discomfort() {}

float Discomfort::process(
      float input,
      float foldGain,
      float shaperGain,
      float tilt,
      float dryWet
) {
  float foldOut = Folder::fold(input, foldGain);
  float shapeOut = Shaper::shape(foldOut, shaperGain);
  // return shapeOut;

  return DryWet::blend(input, shapeOut, dryWet);
}
