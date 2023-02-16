
class Context
{
public:

  int _sampleRate;
  int _frameSize;
  int _numFrames;
  int _numBytes;
  int _numChannels;


  bool setup(
    const int sampleRate,
    const int frameSize,
    const int numFrames
  );

  void write(float amplitude, int frameIdx);

  void write(float amplitude, int frameIdx, int channel);

  float read(int frameIdx);

  float read(int frameIdx, int channel);

  void updateBuffers(float * inBufferPtr, float * outBufferPtr);

  float * _inputBuffer;
  float * _outputBuffer;
};




//// Loop script functions

bool setup(Context * ctx);
void loop(Context * ctx);
void pd_call();