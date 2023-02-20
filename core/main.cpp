#include "RtAudio.h"
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <functional>
#include <thread>
#include "core.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


#define SAMPLE float
#define AUDIO_FORMAT RTAUDIO_FLOAT32
#define AUDIO_CHANNELS 1

RtAudio adac;



Context ctx;

// Audio Callback
int callback( void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
           double streamTime, RtAudioStreamStatus status, void *data )
{

  if ( status ) std::cout << "Stream over/underflow detected." << std::endl;
  
  // unsigned int *bytes = (unsigned int *) data;

  // printf(std::to_string((float *) inputBuffer[0])); 

  ctx.updateBuffers((SAMPLE *) inputBuffer, (SAMPLE *) outputBuffer);

  loop(&ctx);

  return 0;
}


int main()
{

  RtAudio adac;
  if ( adac.getDeviceCount() < 1 ) {
    std::cout << "\nNo audio devices found!\n";
    exit( 0 );
  }


  // Setup Audio Devices

  // Set the same number of channels for both input and output.
  unsigned int bufferBytes = 512;
  unsigned int bufferFrames = 512;
  RtAudio::StreamParameters inParams, outParams;
  inParams.deviceId = adac.getDefaultInputDevice(); // first available device
  inParams.nChannels = 2;
  outParams.deviceId = adac.getDefaultOutputDevice(); // first available device
  outParams.nChannels = 2;


  RtAudio::DeviceInfo inInfo = adac.getDeviceInfo(inParams.deviceId);
  RtAudio::DeviceInfo outInfo =  adac.getDeviceInfo(outParams.deviceId);

  int sampleRate = inInfo.preferredSampleRate;

  std::cout << inInfo.name << std::endl;
  std::cout << "Sample Rate: " << std::to_string(sampleRate) << std::endl;
  std::cout << outInfo.name << std::endl;
  std::cout << inInfo.nativeFormats << std::endl;
  // std::cout << out.nativeFormats << std::endl;

  RtAudio::StreamOptions options;
  options.flags = RTAUDIO_SCHEDULE_REALTIME;

  // int options = RTAUDIO_NONINTERLEAVED;

  ctx.setup(
    sampleRate,
    sizeof(SAMPLE),
    bufferFrames
  );

  if (!setup(&ctx)) {
    return 0;
  };

  try {
      adac.openStream( &outParams, &inParams, AUDIO_FORMAT, sampleRate, &bufferFrames, &callback, (void *)&bufferBytes, &options);
  }
  catch (RtAudioErrorType& e) {
      std::cerr << e << std::endl;
      goto cleanup; 
  }




  try {
    adac.startStream();
     // keep the program alive until it's killed 
    while(1) {
      usleep(1000);
    }
    // Stop the audio stream.
    adac.stopStream();
  }
  catch ( RtAudioErrorType& e ) {
    std::cerr << e << std::endl;
    goto cleanup;
  }


 cleanup:
  if ( adac.isStreamOpen() ) adac.closeStream();
  return 0;
}

