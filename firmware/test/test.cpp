#include <iostream>
#include <assert.h>
#include <cmath>

#include "../src/Folder.h"

bool checkFloat(float x, float y, float epsilon = 0.000001f){
   if(fabs(x - y) < epsilon)
      return true; //they are same
      return false; //they are not same
}

void testFolder() {
  assert(checkFloat(Folder::fold(1, 1.1, 0), 0.9f));
  assert(checkFloat(Folder::fold(1, 2, 0), 0));
  assert(checkFloat(Folder::fold(1, 2.1, 0), -0.1));
  assert(checkFloat(Folder::fold(1, 3, 0), -1));
  assert(checkFloat(Folder::fold(1, 3.1f, 0), -0.9f));
  assert(checkFloat(Folder::fold(1, 4.1f, 0), 0.1f));
  assert(checkFloat(Folder::fold(1, 5.1f, 0), 0.9f));
  assert(checkFloat(Folder::fold(1, 6.0f, 0), 0.0f));
}

void testCrusher(void) {

}

int main() {
  testFolder();
  testCrusher();
  return 0;
}