


#include "johnson.h"




float amplitude = 0.0;

bool setup(Context * ctx) {

  amplitude = 0.5;
  return true;
}


void loop(Context * ctx)
{
  for (int i = 0; i < ctx->_numBytes; i++) {  

    float in = ctx->read(i) * amplitude;
    ctx->write(in, i);

  }
}



