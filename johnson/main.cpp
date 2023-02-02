#include "RtAudio.h"
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include "johnson.h"

#include "Delay.h"

#define SAMPLE float
#define AUDIO_FORMAT RTAUDIO_FLOAT32
#define AUDIO_CHANNELS 1

RtAudio adac;

Delay delayInstance;




Context ctx;



// Audio Callback
int callback( void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
           double streamTime, RtAudioStreamStatus status, void *data )
{

  if ( status ) std::cout << "Stream over/underflow detected." << std::endl;
  
  // unsigned int *bytes = (unsigned int *) data;
  // ctx._numBytes = *bytes;

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
  RtAudio::StreamParameters iParams, oParams;
  iParams.deviceId = adac.getDefaultInputDevice(); // first available device
  iParams.nChannels = 2;
  oParams.deviceId = adac.getDefaultOutputDevice(); // first available device
  oParams.nChannels = 2;


  RtAudio::DeviceInfo inInfo = adac.getDeviceInfo(iParams.deviceId);
  RtAudio::DeviceInfo outInfo =  adac.getDeviceInfo(oParams.deviceId);

  int sampleRate = inInfo.preferredSampleRate;

  std::cout << inInfo.name << std::endl;
  std::cout << "Sample Rate: " << std::to_string(sampleRate) << std::endl;
  std::cout << outInfo.name << std::endl;
  std::cout << inInfo.nativeFormats << std::endl;
  // std::cout << out.nativeFormats << std::endl;


  if (!delayInstance.setup(sampleRate)) {
    std::cerr << "Could not create delay instance" << std::endl;
    return 1;
  }

  // int options = RTAUDIO_NONINTERLEAVED;

  ctx.setup(
    sampleRate,
    bufferFrames,
    sizeof(SAMPLE)
  );

  setup(&ctx);



  try {
      adac.openStream( &oParams, &iParams, AUDIO_FORMAT, sampleRate, &bufferFrames, &callback, (void *)&bufferBytes);
  }
  catch (RtAudioErrorType& e) {
      std::cerr << e << std::endl;
      goto cleanup; 
  }



  try {
    adac.startStream();
    char input;
    std::cout << "\nRunning ... press <enter> to quit.\n";
    std::cin.get(input);
    // Stop the stream.
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

