  
  #include "stereo.h"
  #include <iostream>
  
  bool Context::setup(
    const int sampleRate,
    const int frameSize,
    const int numFrames
  ) {

    _sampleRate = sampleRate;
    _frameSize = frameSize;
    _numFrames = numFrames;
    _numBytes = frameSize*numFrames;

    return true;
  }

  void Context::write(float amplitude, int frameIdx)
  {
    _outputBuffer[frameIdx] = amplitude;
  }

  void Context::write(float amplitude, int frameIdx, int channel)
  {
    if (channel >= _numChannels) {
      std::cout << "Input channel " << std::to_string(channel) << "Doesn't Exist" << std::endl; 
    }
    _outputBuffer[frameIdx] = amplitude;
  }

  float Context::read(int frameIdx) 
  {
    return 0.0+_inputBuffer[frameIdx];
  }

  float Context::read(int frameIdx, int channel) 
  {
    return 0.0+_inputBuffer[frameIdx];
  }

  void Context::updateBuffers(float * inBufferPtr, float * outBufferPtr) 
  {
    _inputBuffer = inBufferPtr;
    _outputBuffer = outBufferPtr;
  }
