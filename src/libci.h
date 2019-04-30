#ifndef LIBCI_H
#define LIBCI_H

#if defined(_WIN32) || defined(_WIN64)
    #ifdef DLL_EXPORTS
        #define LIB_CI_API __declspec(dllexport)
    #else
        #define LIB_CI_API __declspec(dllimport)
    #endif
#else // Linux
    #define LIB_CI_API
#endif

#include <stddef.h>

enum ARRAY_TYPE {
    ARRAY_CIRCLE = 0,
    ARRAY_LINE = 1
};

typedef enum ciStatus {
    // Configuration Errors
//    bbInvalidModeErr             = -112,
//    bbReferenceLevelErr          = -111,
//    bbInvalidVideoUnitsErr       = -110,
//    bbInvalidWindowErr           = -109,
//    bbInvalidBandwidthTypeErr    = -108,
//    bbInvalidSweepTimeErr        = -107,
//    bbBandwidthErr               = -106,
//    bbInvalidGainErr             = -105,
    ciLessThanThreshold          = -104,
    ciFrequencyRangeErr          = -103,
//    bbInvalidSpanErr             = -102,
//    bbInvalidScaleErr            = -101,
//    bbInvalidDetectorErr         = -100,

    // General Errors
    ciInterpErr                  = -1,

    // No Error
    ciNoError                    = 0,

    // Warnings/Messages
//    bbAdjustedParameter          = 1,
//    bbADCOverflow                = 2,
//    bbNoTriggerFound             = 3,
//    bbClampedToUpperLimit        = 4,
//    bbClampedToLowerLimit        = 5,
//    bbUncalibratedDevice         = 6,
//    bbDataBreak                  = 7,
//    bbUncalSweep                 = 8
} ciStatus;

class IAlgorithmCI
{
public:
    virtual bool Config(ARRAY_TYPE _type, size_t Na, size_t _nArrays,
                        const float *_x, const float *_y,
                        const double *_freqs_start, const double *_freqs_step, const double *_freqs_stop,
                        float _theta_start, float _theta_step, float _theta_stop) = 0;

    virtual bool ConfigCircularArray(size_t _Na, size_t _nArrays, const float *_radius,
                                     const double *_freqs_start, const double *_freqs_step, const double *_freqs_stop,
                                     float _theta_start, float _theta_step, float _theta_stop) = 0;

    virtual bool ConfigLinearArray(size_t _Na, size_t _nArrays, const float *_Dn1,
                                   const double *_freqs_start, const double *_freqs_step, const double *_freqs_stop,
                                   float _theta_start, float _theta_step, float _theta_stop) = 0;

    virtual const float * GetTheorPHATabl(size_t nfreqs, const double *_freqs, size_t nthetas, const float *_thetas) const = 0;
    virtual const float * GetTheorPHATabl(double freqs_start, double freqs_step, double freqs_stop,
                                    float theta_start, float theta_step, float theta_stop) const = 0;

    virtual ciStatus ConfigLookupTable(const float *_samples_P, const float *_samples_A = 0, float _threshold = 0.0) = 0;

    virtual ciStatus GetDOA(float &bearing, float &energy, double freq, const float *phases, const float *amplitudes = 0, float *curve = 0) = 0;

    virtual size_t AntennaCount() const = 0;

    virtual const float *PhaseSamples() const = 0;
    virtual const float *AmpSamples() const = 0;

    virtual size_t ThetaCount() const = 0;
    virtual const float *Thetas() const = 0;

    virtual size_t FreqCount() const = 0;
    virtual const double *Freqs() const = 0;
    virtual double FreqMin() const = 0;
    virtual ciStatus GetFrequencyIndex(double f, size_t &index) = 0;
};

extern "C" LIB_CI_API IAlgorithmCI* getInstance();

#endif // LIBCI_H
