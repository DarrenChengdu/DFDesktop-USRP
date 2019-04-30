#ifndef ALGORITHM_CI_H
#define ALGORITHM_CI_H

#include "libci.h"

#include "armadillo/armadillo"
using namespace arma;

class AlgorithmCI : public IAlgorithmCI
{
public:
    AlgorithmCI();

    bool ConfigCircularArray(size_t _Na, size_t _nArrays, const float *_radius,
                             const double *_freqs_start, const double *_freqs_step, const double *_freqs_stop,
                             float _theta_start, float _theta_step, float _theta_stop);

    bool ConfigLinearArray(size_t _Na, size_t _nArrays, const float *_Dn1,
                           const double *_freqs_start, const double *_freqs_step, const double *_freqs_stop,
                           float _theta_start, float _theta_step, float _theta_stop);

    const float * GetTheorPHATabl(size_t nfreqs, const double *_freqs, size_t nthetas, const float *_thetas) const;
    const float * GetTheorPHATabl(double freq_start, double freq_step, double freq_stop,
                            float theta_start, float theta_step, float theta_stop) const;

    ciStatus ConfigLookupTable(const float *_samples_P, const float *_samples_A = 0, float _threshold = 0.0);

    ciStatus GetDOA(float &bearing, float &energy, double freq, const float *phases, const float *amplitudes = 0, float *curve = 0);

    const float *PhaseSamples() const { return phases_sampl.memptr(); }
    const float *AmpSamples() const { return amplitudes_sampl.memptr(); }
    const float *Thetas() const { return thetas.memptr(); }
    size_t ThetaCount() const { return Nt; }
    double FreqMin() const { return freq_min; }
    const double *Freqs() const {return freqs.memptr();}
    size_t FreqCount() const { return freqs.size(); }
    ciStatus GetFrequencyIndex(double f, size_t &index);
    size_t AntennaCount() const { return Naa; }

private:
    bool Config(ARRAY_TYPE _type, size_t Na, size_t _nArrays,
                const float *_x, const float *_y,
                const double *_freqs_start, const double *_freqs_step, const double *_freqs_stop,
                float _theta_start, float _theta_step, float _theta_stop);
    bool Interpolate(double start, double step, double stop);

    fcube phases_sampl;
    fcube amplitudes_sampl;

    cx_fcube complex_sampl;

    size_t nArrays;

    // antenna info
    ARRAY_TYPE array_type;

    size_t Naa;
    size_t Nt;
    size_t Nf;

    fmat coords_x;
    fmat coords_y;

    // frequency and azimuth
    vec freqs_start;
    vec freqs_stop;
    vec freqs_step;
    double freq_min;
    double freq_max;
    vec freqs;

    float theta_start;
    float theta_stop;
    float theta_step;

    fvec thetas;

    float threshold;

    // algorithm
    fvec fcorr;
    fmat33 A;
    fvec3  x, y;
};

#endif // ALGORITHM_CI_H
