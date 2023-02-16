


#include "core.h"
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "PdBase.hpp"
#include "PdObject.h"

float amplitude = 0.0;

pd::PdBase lpd;

PdObject pdObject;

float inbuf[64], outbuf[64];

bool setup(Context * ctx) {


  if (!lpd.init(2,2, ctx->_sampleRate, true)) {
    std::cerr << "Failed to Initilize PureData" << std::endl;
    exit(1);
  }

  lpd.setReceiver(&pdObject);
  lpd.subscribe("cpp_instance");

  // turn on audio
  lpd.computeAudio(true);

  // load the pd patch
  pd::Patch patch = lpd.openPatch("test.pd", "../pd");
  std::cout << patch << std::endl;


  amplitude = 0.5;

  printf("setup");
  return true;
}

void pd_call() {

}


void loop(Context * ctx)
{


  int ticks = ctx->_frameSize / libpd_blocksize();
  // printf("ticks: %d, pd_block: %d, frame: %d\n", ticks, libpd_blocksize(), ctx->_frameSize);
  lpd.processFloat(ticks, ctx->_inputBuffer, ctx->_outputBuffer);

  for (int i = 0; i < ctx->_numBytes; i++) {

    // float in = ctx->read(i);
    // ctx->write(in, i);
  }

  lpd.receiveMessages();
  lpd.sendFloat("FromCpp", 578);
  // lpd.sendBang("jawn");


}






