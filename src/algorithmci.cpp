#include "algorithmci.h"
#include "df_lib.h"

#define C_Speed 299792458
#define Factor_Deg2Rad 0.0174533

LIB_CI_API IAlgorithmCI* getInstance()
{
    IAlgorithmCI *pInstance = new AlgorithmCI;
    return pInstance;
}

AlgorithmCI::AlgorithmCI()
{
    //    ConfigCircularArray(7, 1.0);
    //    setupTheorLookupTable(30, 1350, 1, 0, 355, 5);

    //    float *table = GetTheorPHATabl(30, 1350, 1, 0, 355, 5);
    //    ConfigLookupTable(30, 1350, 1, 0, 355, 5, 0.8, table);
    //    delete []table;
}

bool AlgorithmCI::Config(ARRAY_TYPE _type, size_t Na, size_t _nArrays,
                         const float *_x, const float *_y,
                         const double *_freqs_start, const double *_freqs_step, const double *_freqs_stop,
                         float _theta_start, float _theta_step, float _theta_stop)
{
    array_type = _type;
    Naa = Na;
    nArrays = _nArrays;

    coords_x.set_size(Naa, nArrays);
    coords_y.set_size(Naa, nArrays);

    memcpy(coords_x.memptr(), _x, nArrays*Naa*sizeof(float));
    memcpy(coords_y.memptr(), _y, nArrays*Naa*sizeof(float));

    freqs_start.set_size(nArrays);
    freqs_stop.set_size(nArrays);
    freqs_step.set_size(nArrays);

    memcpy((char *)freqs_start.memptr(), (char *)_freqs_start, nArrays*sizeof(double));
    memcpy((char *)freqs_step.memptr(),  (char *)_freqs_step,  nArrays*sizeof(double));
    memcpy((char *)freqs_stop.memptr(),  (char *)_freqs_stop,  nArrays*sizeof(double));

    freq_min = freqs_start.min();
    freq_max = freqs_stop.max();

    theta_start = _theta_start;
    theta_stop = _theta_stop;
    theta_step = _theta_step;

    thetas = regspace<fvec>(theta_start, theta_step, theta_stop);
    Nt = thetas.size();

    return true;
}

bool AlgorithmCI::ConfigCircularArray(size_t _Na, size_t _nArrays, const float *_radius,
                                      const double *_freqs_start, const double *_freqs_step, const double *_freqs_stop,
                                      float _theta_start, float _theta_step, float _theta_stop)
{
    fvec _x(_Na*_nArrays);
    fvec _y(_Na*_nArrays);

    for (int n = 0; n < _nArrays; n++)
    {
        for (int i = 0; i < _Na; i++)
        {
            _x(n*_Na + i) = _radius[n] * df_lib::sind(360.0 / _Na * i);
            _y(n*_Na + i) = _radius[n] * df_lib::cosd(360.0 / _Na * i);
        }
    }

    return Config(ARRAY_CIRCLE, _Na
                  , _nArrays, _x.memptr(), _y.memptr()
                  , _freqs_start, _freqs_step, _freqs_stop
                  , _theta_start, _theta_step, _theta_stop);
}

bool AlgorithmCI::ConfigLinearArray(size_t _Na, size_t _nArrays, const float *_Dn1,
                                    const double *_freqs_start, const double *_freqs_step, const double *_freqs_stop,
                                    float _theta_start, float _theta_step, float _theta_stop)
{
    fvec _x(_Na*_nArrays);
    fvec _y(_Na*_nArrays);

    for (int n = 0; n < _nArrays; n++)
    {
        for (int i = 0; i < _Na; i++)
        {
            _x(n*_Na + i) = _Dn1[n*_Na + i];
            _y(n*_Na + i) = 0.0;
        }
    }

    return Config(ARRAY_LINE, _Na
                  , _nArrays, _x.memptr(), _y.memptr()
                  , _freqs_start, _freqs_step, _freqs_stop
                  , _theta_start, _theta_step, _theta_stop);
}

const float* AlgorithmCI::GetTheorPHATabl(double freq_start, double freq_step, double freq_stop,
                                    float theta_start, float theta_step, float theta_stop) const
{
    vec freqs_loc = regspace<vec>(freq_start, freq_step, freq_stop);
    fvec thetas_loc = regspace<fvec>(theta_start, theta_step, theta_stop);

    return GetTheorPHATabl(freqs_loc.size()
                           , freqs_loc.memptr()
                           , thetas_loc.size()
                           , thetas_loc.memptr());
}

const float* AlgorithmCI::GetTheorPHATabl(size_t _Nf, const double *_freqs, size_t _Nt, const float *_thetas) const
{
    fcube PhaTable(_Nt, Naa, _Nf);
    fvec2 incident;
    fvec2 baseline;

    for (int i = 0; i < _Nt; i++)
    {
        incident << -df_lib::sind(_thetas[i]) << -df_lib::cosd(_thetas[i]);

        for (int j = 0; j < Naa; j++)
        {
            for (int k = 0; k < _Nf; k++)
            {
                int layer_id = 0;

                for (int m = 0; m < nArrays; m++)
                {
                    if (_freqs[k] >= freqs_start(m) && _freqs[k] <= freqs_stop(m))
                    {
                        layer_id = m;
                        break;
                    }
                }

                baseline << coords_x(j, layer_id) - coords_x(0, layer_id)
                         << coords_y(j, layer_id) - coords_y(0, layer_id);

                double lambda = C_Speed * 1.0 / _freqs[k];
                double dp = 360 * dot(baseline, incident) / lambda;
                PhaTable(i, j, k) = df_lib::angle(dp);
            }
        }
    }

    // PhaTable.save("theoryPHA.txt", raw_ascii);
    float *samplesPHA = new float[PhaTable.size()];
    memcpy(samplesPHA, PhaTable.memptr(), sizeof(float)*PhaTable.size());

    return samplesPHA;
}

bool AlgorithmCI::Interpolate(double start, double step, double stop)
{
    if (step < 0 || start > stop || freq_min > start || freq_max < stop)
    {
        return false;
    }

    int M = (stop - start) / step + 1;
    vec freqs_interp = linspace<vec>(start, stop, M);

    fcube phases_sampl_interp(phases_sampl.n_rows, phases_sampl.n_cols, M);
    fcube amplitudes_sampl_interp(amplitudes_sampl.n_rows, amplitudes_sampl.n_cols, M);

    // Linear interpolations
    for (int m = 0; m < M; m++)
    {
        float x1, x2;
        int i;
        bool sta = false;

        for (i = 0; i < freqs.size() - 1; i++)
        {
            if (freqs_interp(m) >= freqs(i) && freqs_interp(m) <= freqs(i + 1))
            {
                x1 = freqs(i);
                x2 = freqs(i + 1);
                sta = true;

                break;
            }
        }

        if (!sta)
        {
            return false;
        }

        for (int j = 0; j < thetas.size(); j++)
        {
            for (int k = 0; k < Naa; k++)
            {
                float phase_next = phases_sampl(j, k, i + 1);
                float phase_prev = phases_sampl(j, k, i);

                // unwarp of phase_next
                if (phase_next - phase_prev > 180.0)
                {
                    phase_next = phase_next - 360.0;
                }

                if (phase_next - phase_prev < -180.0)
                {
                    phase_next = phase_next + 360.0;
                }

                float interp = (freqs_interp(m) - x1) / (x2 - x1)*(phase_next - phase_prev)
                        + phase_prev;

                phases_sampl_interp(j, k, m) = interp;
            }
        }

        amplitudes_sampl_interp.slice(m) = (freqs_interp(m) - x1) / (x2 - x1)*(amplitudes_sampl.slice(i + 1) - amplitudes_sampl.slice(i))
                + amplitudes_sampl.slice(i);
    }

    freqs = freqs_interp;
    Nf = freqs.size();
    phases_sampl = phases_sampl_interp;
    amplitudes_sampl = amplitudes_sampl_interp;

    return true;
}

ciStatus AlgorithmCI::GetFrequencyIndex(double f, size_t &index)
{
    if (freq_min > f || freq_max < f)
    {
        return ciFrequencyRangeErr;
    }

    index = floor((f - freq_min) / 1000000 + 0.5);
}

ciStatus AlgorithmCI::ConfigLookupTable(const float *_samples_P, const float *_samples_A, float _threshold)
{
    threshold = _threshold;

    freqs.clear();

    for (int n = 0; n < nArrays; n++)
    {
        vec f = regspace<vec>(freqs_start(n), freqs_step(n), freqs_stop(n));
        freqs = join_cols(freqs, f);
    }

    Nf = freqs.size();
    phases_sampl = fcube(_samples_P, Nt, Naa, Nf);
    // phases_sampl.save("phases_sampl.txt", arma_ascii);

    if (_samples_A)
    {
        amplitudes_sampl = fcube(_samples_A, Nt, Naa, Nf);
    }
    else
    {
        amplitudes_sampl.set_size(Nt, Naa, Nf);
        amplitudes_sampl.fill(0.0);
    }

    if (!Interpolate(freq_min, 1000000, freq_max))
    {
        return ciInterpErr;
    }

    complex_sampl.set_size(arma::size(phases_sampl));

    for (size_t i = 0; i < Nt; i++)
    {
        for (size_t j = 0; j < Naa; j++)
        {
            for (size_t k = 0; k < Nf; k++)
            {
                complex_sampl(i, j, k) = cx_float(df_lib::cosd(phases_sampl(i, j, k)), -df_lib::sind(phases_sampl(i, j, k)));
            }
        }
    }

    return ciNoError;
}

ciStatus AlgorithmCI::GetDOA(float &bearing, float &energy, double freq, const float *phases, const float *amplitudes, float *curve)
{
    if (freq > freq_max || freq < freq_min)
    {
        return ciFrequencyRangeErr;
    }

    int nearest = floor((freq - freq_min) / 1000000 + 0.5);
    cx_fvec xc(Naa, fill::zeros);

    for (int i = 0; i < Naa; i++)
    {
        xc(i) = cx_float(df_lib::cosd(phases[i]), df_lib::sind(phases[i]));
    }

    fcorr = abs(complex_sampl.slice(nearest) * xc);

    // export correlation curve
    if (curve) {
        fcorr = fcorr / max(fcorr); // normalization
        memcpy((char *)curve, (char *)fcorr.memptr(), sizeof(float)*Nt);
    }

    uword peakIdx = fcorr.index_max();
    float peak = fcorr(peakIdx);

    if (peak < threshold)
        return ciLessThanThreshold;

    energy = max(fcorr);

    // solve A * x = y
    if (peakIdx > 0 && peakIdx < Nt-1)
    {
        float xl = thetas(peakIdx-1);
        float xp = thetas(peakIdx);
        float xr = thetas(peakIdx+1);

        A(0, 0) = xl * xl; A(0, 1) = xl; A(0, 2) = 1.0f;
        A(1, 0) = xp * xp; A(1, 1) = xp; A(1, 2) = 1.0f;
        A(2, 0) = xr * xr; A(2, 1) = xr; A(2, 2) = 1.0f;

        y(0) = fcorr(peakIdx-1);
        y(1) = fcorr(peakIdx);
        y(2) = fcorr(peakIdx+1);

        x = arma::inv(A) * y;
        bearing = - x(1)/(2.0f*x(0)); // - b / (2a)
    }
    else {
        if (array_type == ARRAY_LINE)
        {
            bearing = thetas(peakIdx);
        }
        else if (array_type == ARRAY_CIRCLE)
        {
            if (peakIdx == 0) {
                float xl = thetas(Nt-1) - 360.0f;
                float xp = thetas(0);
                float xr = thetas(1);

                A(0, 0) = xl * xl; A(0, 1) = xl; A(0, 2) = 1.0f;
                A(1, 0) = xp * xp; A(1, 1) = xp; A(1, 2) = 1.0f;
                A(2, 0) = xr * xr; A(2, 1) = xr; A(2, 2) = 1.0f;

                y(0) = fcorr(Nt-1);
                y(1) = fcorr(0);
                y(2) = fcorr(1);
            }
            else if (peakIdx == Nt-1) {
                float xl = thetas(Nt-2);
                float xp = thetas(Nt-1);
                float xr = thetas(0) + 360.0f;

                A(0, 0) = xl * xl; A(0, 1) = xl; A(0, 2) = 1.0f;
                A(1, 0) = xp * xp; A(1, 1) = xp; A(1, 2) = 1.0f;
                A(2, 0) = xr * xr; A(2, 1) = xr; A(2, 2) = 1.0f;

                y(0) = fcorr(Nt-2);
                y(1) = fcorr(Nt-1);
                y(2) = fcorr(0);
            }

            x = arma::inv(A) * y;
            bearing = - x(1)/(2.0f*x(0)); // - b / (2a)
            bearing = df_lib::mod(bearing, (float)360.0);
        }
    }

    return ciNoError;
}
