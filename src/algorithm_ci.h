#ifndef ALGORITHM_CI_H
#define ALGORITHM_CI_H

#include "df_lib.h"
#include "armadillo/armadillo"
using namespace arma;

typedef unsigned int Hz;
typedef Col<Hz> Hzvec;

enum ArrayType {
    ArrayType_Circle = 0,
    ArrayType_Line = 1
};

class AlgorithmCI
{
public:
    AlgorithmCI();

    bool Config(ArrayType _type, int Na, int _nlayers,
                const float *_x, const float *_y,
                const Hz *_freq_start, const Hz *_freq_step, const Hz *_freq_stop,
                float _theta_start, float _theta_step, float _theta_stop);

    bool ConfigCircularArray(int Na, int _nlayers, const float *_radius,
                             const Hz *_freq_start, const Hz *_freq_step, const Hz *_freq_stop,
                             float _theta_start, float _theta_step, float _theta_stop);

    bool ConfigLinearArray(int Na, const float *space);


    const float * GetTheorPHATabl(int nfreqs, const Hz *_freqs, int nthetas, const float *_thetas) const;
    const float * GetTheorPHATabl(Hz freq_start, Hz freq_step, Hz freq_stop,
                            float theta_start, float theta_step, float theta_stop) const;

    bool ConfigLookupTable(float _threshold, const float *_samples_P, const float *_samples_A = 0);

    float GetDOA(Hz freq, const float *phases, const float *amplitudes = 0, float *curve = 0);

    const float *PhaseSamples() const { return phases_sampl_1MHz.memptr(); }
    const float *AmpSamples() const { return amplitudes_sampl_1MHz.memptr(); }
    const float *Thetas() const { return thetas_raw.memptr(); }
    int ThetaCount() const { return thetas_raw.size(); }
    Hz FreqMin() const { return freq_min; }
    const Hz *Freqs() const {return freqs.memptr();}
    int FreqCount() const { return freqs.size(); }
    int FreqIndex(Hz f);
    int AntennaCount() const { return Naa; }

private:
    bool Interpolate(Hz start, Hz step, Hz stop);

    fcube phases_sampl;
    fcube phases_sampl_1MHz;

    fcube amplitudes_sampl;
    fcube amplitudes_sampl_1MHz;

    cx_fcube complex_sampl;

    short nLayers;

    // antenna info
    ArrayType array_type;
    int Naa;
    int Nbl;
    fmat coords_x;
    fmat coords_y;

    // frequency and azimuth
    Hzvec freq_start;
    Hzvec freq_stop;
    Hzvec freq_step;
    Hz freq_min;
    Hz freq_max;
    Hzvec freqs;

    float theta_start;
    float theta_stop;
    float theta_step;

    fvec thetas_raw;
    fvec thetas;

    float threshold;
};

#endif // ALGORITHM_CI_H
