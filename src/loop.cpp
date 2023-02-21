


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

std::vector<int> midiSequence; 
int sequencePtr;
int sequenceCounter;
int sequenceCounterMax;


class MidiReciever : public pd::PdMidiReceiver  {

  // pd midi receiver callbacks
	void receiveNoteOn(const int channel, const int pitch, const int velocity) {
    printf("CPP Host Recieved Midi Message: \n");
    printf(" - Note:     %d\n", pitch);
    printf(" - Velocity: %d\n", velocity);
    printf(" - Channel:  %d\n", channel);
  }
};

MidiReciever midiReciever;

bool setup(Context * ctx) {


  if (!lpd.init(2,2, ctx->_sampleRate, true)) {
    std::cerr << "Failed to Initilize PureData" << std::endl;
    exit(1);
  }

  lpd.setReceiver(&pdObject);
  lpd.setMidiReceiver(&midiReciever);
  lpd.subscribe("cpp_instance");
  lpd.subscribe("bang_out");

  // turn on audio
  lpd.computeAudio(true);

  // load the pd patch
  char patchName[] = "test.pd";
  char patchDir[] = "../pd";
  pd::Patch patch = lpd.openPatch(patchName, patchDir);
  std::cout << patch << std::endl;

  amplitude = 0.5;


  // Init midi sequence to send to Pd
  midiSequence.resize(64);
  int midiMin = 64;
  int midiRange = 24;
  for (int i = 0; i < midiSequence.size(); i++) {
    midiSequence[i] = midiMin + rand() % midiRange;
  }

  sequenceCounter = 0;
  sequencePtr = 0;
  sequenceCounterMax = 5000;


  return true;
}



void loop(Context * ctx)
{

  int pdBlocks = ctx->_numFrames / libpd_blocksize();

  // Using non-interleaved bufferrs
  // for (int block = 0; block < pdBlocks; block++) {
  //   for (unsigned int channel = 0; n < ctx->_numChannels; i++) {
  //     //audio input
  //     memcpy(
  //       inBuffer + n * libpd_pdBlocksize(), 
  //       ctx->_inputBuffer + tick * libpd_pdBlocksize() + channel * ctx->_numFrames,
  //       sizeof(ctx->_inputBuffer[0] * libpd_pdBlocksize())
  //     )
  // }




  // Process multiple interleaved blocks from pd
  lpd.processFloat(pdBlocks, ctx->_inputBuffer, ctx->_outputBuffer);



  lpd.receiveMessages();

  sequenceCounter += ctx->_numFrames;
  if (sequenceCounter >= sequenceCounterMax) {

    lpd.sendNoteOn(
      0,    // channel
      midiSequence[sequencePtr],   // pitch
      64    // velocity
    );    
    sequenceCounter = 0;  
    sequencePtr=(sequencePtr+1)%midiSequence.size();
  }
  lpd.receiveMidi();
  // lpd.sendFloat("FromCpp", 578);
  // lpd.sendBang("midi_bang");

}






