


#include "core.h"
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "PdBase.hpp"

float amplitude = 0.0;

pd::PdBase lpd;



float inbuf[64], outbuf[64];

float* inBuffer;
float* outBuffer;

std::vector<int> midiSequence; 
int sequencePtr;
int sequenceCounter;
int sequenceCounterMax;



class Reciever : public pd::PdReceiver {
  // Hooks for all non-midi events sent by Pure Data Patch
  void print(const std::string &message) {
    printf("Recieved print msg:\n - %s\n", message.c_str());
  }

  void receiveBang(const std::string &dest) {
    printf("Recieved Bang from %s\n", dest.c_str());
  }

  void receiveFloat(const std::string &dest, float num) {
    printf("Recieved Float from %s:\n - %f\n", dest.c_str(), num);
  }

  void receiveSymbol(const std::string &dest, const std::string &symbol) {
    printf("Recieved symbol from %s:\n - %s\n", dest.c_str(), symbol.c_str());
  }

  void receiveList(const std::string &dest, const pd::List &list) {
    // step through the list
    for(int i = 0; i < list.len(); ++i) {
      if(list.isFloat(i)) {
        std::cout << list.getFloat(i);
      }
      else if(list.isSymbol(i)) {
        std::cout << list.getSymbol(i);
      }
      if(i < list.len()-1) {
        std::cout << " ";
      }
    }
  }
  void receiveMessage(const std::string &dest, const std::string &msg, const pd::List &list) {
    printf(
      "Recieved Message from %s:\n - %s\n - %s\n - %s\n", 
      dest.c_str(), msg.c_str(), list.toString().c_str(), list.types().c_str()
    );
  }
};

Reciever reciever;

class MidiReciever : public pd::PdMidiReceiver  {
  // Object containing hooks for midi events that are sent from Pd

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

  //Bind recievers to pd instance
  lpd.setReceiver(&reciever);
  lpd.setMidiReceiver(&midiReciever);
  // subscribe recievers to everything sent from PD with [send cpp_instance]
  lpd.subscribe("cpp_instance");


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

  // Using non-interleaved buffers
  // https://github.com/BelaPlatform/Bela/blob/master/core/default_libpd_render.cpp#L1352 
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
      0,                           // channel
      midiSequence[sequencePtr],   // pitch
      64                           // velocity
    );    
    sequenceCounter = 0;  
    sequencePtr=(sequencePtr+1)%midiSequence.size();
  }
  lpd.receiveMidi();
  lpd.sendFloat("from_cpp", 64);

}






