


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

float* inBuffer;
float* outBuffer;

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
  char patchName[] = "test.pd";
  char patchDir[] = "../pd";
  pd::Patch patch = lpd.openPatch(patchName, patchDir);
  std::cout << patch << std::endl;

  amplitude = 0.5;

  return true;
}



void loop(Context * ctx)
{

  int pdBlocks = ctx->_numFrames / libpd_blocksize();
  	// 	//audio input
		// for(unsigned int n = 0; n < context->audioInChannels; ++n)
		// {
		// 	memcpy(
		// 		gInBuf + n * gLibpdpdBlocksize,
		// 		context->audioIn + tick * gLibpdpdBlocksize + n * context->audioFrames, 
		// 		sizeof(context->audioIn[0]) * gLibpdpdBlocksize
		// 	);
		// }

  // for (int block = 0; block < pdBlocks; block++) {

  //   for (unsigned int channel = 0; n < ctx->_numChannels; i++) {

  //     //opearting on non-interrleaved buffers
  //     memcpy(
  //       inbuf + n * libpd_pdBlocksize(), 
  //       ctx->_inputBuffer + tick * libpd_pdBlocksize() + channel * ctx->_numFrames,
  //       sizeof(ctx->_inputBuffer[0] * gLibpdpdBlocksize)
  //     )


  //     //opearting on interleaved buffers
  //     memcpy(
  //       inbuf + n * pdpdBlocksize, 
  //       ctx->_inputBuffer + tick * gLibpdpdBlocksize + n * context->audioFrames,
  //       sizeof(ctx->_inputBuffer[0] * gLibpdpdBlocksize)
  //     )
  //   }

  // }



  // printf("ticks: %d, pd_block: %d, frame: %d\n", ticks, libpd_pdBlocksize(), ctx->_frameSize);


  // Process a single interleaved block from pd
  lpd.processFloat(pdBlocks, ctx->_inputBuffer, ctx->_outputBuffer);

  lpd.receiveMessages();
  lpd.sendFloat("FromCpp", 578);

}






