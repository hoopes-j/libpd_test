
#include <vector>


class Delay
{
public:

    Delay() {};
    ~Delay() {};

    bool setup(int sampleRate);

    float process(float x);

    void setDelayTime(int delayTime);

private:

    int _maxDelayTimeF;
    
    int _delayTimeF;    

    int _sampleRate;    

    int _ptr;

    int _rPtr;
    int _wPtr;


    std::vector<float> _bfr;


};