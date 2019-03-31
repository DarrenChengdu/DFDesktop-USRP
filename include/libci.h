#ifndef LIBCI_H
#define LIBCI_H

#if defined(_WIN32) || defined(_WIN64)
    #ifdef CI_EXPORTS
        #define CI_API __declspec(dllexport)
    #else
        #define CI_API __declspec(dllimport)
    #endif
#else // Linux
    #define CI_API
#endif

typedef unsigned int Hz;

enum ArrayType {
    ArrayType_Circle = 0,
    ArrayType_Line = 1
};

class IAlgorithmCI
{
public:
    virtual bool Config(ArrayType _type, int Na, int _nlayers,
                        const float *_x, const float *_y,
                        const Hz *_freq_start, const Hz *_freq_step, const Hz *_freq_stop,
                        float _theta_start, float _theta_step, float _theta_stop) = 0;

    virtual bool ConfigCircularArray(int Na, int _nlayers, const float *_radius,
                                     const Hz *_freq_start, const Hz *_freq_step, const Hz *_freq_stop,
                                     float _theta_start, float _theta_step, float _theta_stop) = 0;

    virtual bool ConfigLinearArray(int Na, const float *space) = 0;

    virtual const float * GetTheorPHATabl(int nfreqs, const Hz *_freqs, int nthetas, const float *_thetas) const = 0;
    virtual const float * GetTheorPHATabl(Hz freq_start, Hz freq_step, Hz freq_stop,
                                    float theta_start, float theta_step, float theta_stop) const = 0;

    virtual bool ConfigLookupTable(float _threshold, const float *_samples_P, const float *_samples_A = 0) = 0;

    virtual float GetDOA(Hz freq, const float *phases, const float *amplitudes = 0, float *curve = 0) = 0;

    virtual int AntennaCount() const = 0;

    virtual const float *PhaseSamples() const = 0;
    virtual const float *AmpSamples() const = 0;

    virtual int ThetaCount() const = 0;
    virtual const float *Thetas() const = 0;

    virtual int FreqCount() const = 0;
    virtual const Hz *Freqs() const = 0;
    virtual Hz FreqMin() const = 0;
    virtual int FreqIndex(Hz f) = 0;
};

extern "C" CI_API IAlgorithmCI* getInstance();

#endif // LIBCI_H
