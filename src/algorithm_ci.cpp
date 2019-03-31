#include "algorithm_ci.h"
#include "df_lib.h"
#include "globals.h"

#define C_Speed 299792458
#define Factor_Deg2Rad 0.0174533

AlgorithmCI::AlgorithmCI()
{
    //    ConfigCircularArray(7, 1.0);
    //    setupTheorLookupTable(30, 1350, 1, 0, 355, 5);

    //    float *table = GetTheorPHATabl(30, 1350, 1, 0, 355, 5);
    //    ConfigLookupTable(30, 1350, 1, 0, 355, 5, 0.8, table);
    //    delete []table;
}

bool AlgorithmCI::Config(ArrayType _type, int Na, int _nlayers,
                         const float *_x, const float *_y,
                         const Hz *_freq_start, const Hz *_freq_step, const Hz *_freq_stop,
                         float _theta_start, float _theta_step, float _theta_stop)
{
    array_type = _type;
    Naa = Na;
    nLayers = _nlayers;
    Nbl = Naa;

    if (array_type == ArrayType_Circle)
        Nbl = df_lib::nchoosek(Naa, 2);

    coords_x.set_size(Naa, nLayers);
    coords_y.set_size(Naa, nLayers);

    memcpy(coords_x.memptr(), _x, nLayers*Naa*sizeof(float));
    memcpy(coords_y.memptr(), _y, nLayers*Naa*sizeof(float));

    freq_start.set_size(nLayers);
    freq_stop.set_size(nLayers);
    freq_step.set_size(nLayers);

    memcpy((char *)freq_start.memptr(), (char *)_freq_start, nLayers*sizeof(Hz));
    memcpy((char *)freq_step.memptr(), (char *)_freq_step, nLayers*sizeof(Hz));
    memcpy((char *)freq_stop.memptr(), (char *)_freq_stop, nLayers*sizeof(Hz));

    freq_min = freq_start(0);
    freq_max = freq_stop(nLayers-1);

    theta_start = _theta_start;
    theta_stop = _theta_stop;
    theta_step = _theta_step;

    thetas = regspace<fvec>(theta_start, theta_step, theta_stop);

    return true;
}

bool AlgorithmCI::ConfigCircularArray(int Na, int _nlayers, const float *_radius,
                                      const Hz *_freq_start, const Hz *_freq_step, const Hz *_freq_stop,
                                      float _theta_start, float _theta_step, float _theta_stop)
{
    fvec _x(Na*_nlayers);
    fvec _y(Na*_nlayers);

    for (int n = 0; n < _nlayers; n++)
    {
        for (int i = 0; i < Na; i++)
        {
            _x(n*Na + i) = _radius[n] * df_lib::sind(360.0 / Na * i);
            _y(n*Na + i) = _radius[n] * df_lib::cosd(360.0 / Na * i);
        }
    }

    return Config(ArrayType_Circle, Na
                  , _nlayers, _x.memptr(), _y.memptr()
                  , _freq_start, _freq_step, _freq_stop
                  , _theta_start, _theta_step, _theta_stop);
}

bool AlgorithmCI::ConfigLinearArray(int Na, const float *space)
{
    fvec coords_x(Na);
    fvec coords_y(Na);

    //    for (int i = 0; i < Na; i++)
    //    {
    //        coords_x(i) = radius * df_lib::sind(360.0 / Na * i);
    //        coords_y(i) = radius * df_lib::cosd(360.0 / Na * i);
    //    }

    //    return ConfigArrayOfAntennas(ArrayType_Line, Na, coords_x.memptr(), coords_y.memptr());
    return true;
}

const float* AlgorithmCI::GetTheorPHATabl(Hz freq_start, Hz freq_step, Hz freq_stop,
                                    float theta_start, float theta_step, float theta_stop) const
{
    Hzvec freqs_loc = regspace<Hzvec>(freq_start, freq_step, freq_stop);
    fvec thetas_loc = regspace<fvec>(theta_start, theta_step, theta_stop);

    return GetTheorPHATabl(freqs_loc.size()
                           , freqs_loc.memptr()
                           , thetas_loc.size()
                           , thetas_loc.memptr());
}

const float* AlgorithmCI::GetTheorPHATabl(int _Nf, const Hz *_freqs, int _Nt, const float *_thetas) const
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

                for (int m = 0; m < nLayers; m++)
                {
                    if (_freqs[k] >= freq_start(m) && _freqs[k] <= freq_stop(m))
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

bool AlgorithmCI::Interpolate(Hz start, Hz step, Hz stop)
{
    if (freq_min > start || freq_max < stop)
    {
        return false;
    }

    int M = (stop - start) / step + 1;
    Hzvec freqs_interp = linspace<Hzvec>(start, stop, M);

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
    phases_sampl = phases_sampl_interp;
    amplitudes_sampl = amplitudes_sampl_interp;

    return true;
}

int AlgorithmCI::FreqIndex(Hz f)
{
    return floor((f - freq_min) / 1000000 + 0.5);
}

bool AlgorithmCI::ConfigLookupTable(float _threshold, const float *_samples_P, const float *_samples_A)
{
    threshold = _threshold;

    freqs.clear();

    for (int n = 0; n < nLayers; n++)
    {
        Hzvec f_layer = regspace<Hzvec>(freq_start(n), freq_step(n), freq_stop(n));
        freqs = join_cols(freqs, f_layer);
    }

    int Nf = freqs.size();
    int Nt = thetas.size();

    phases_sampl = fcube(_samples_P, Nt, Naa, Nf);
    // phases_sampl.save("phases_sampl.txt", arma_ascii);

    if (_samples_A)
    {
        amplitudes_sampl = fcube(_samples_A, Nt, Naa, Nf);
    }
    else
    {
        amplitudes_sampl.set_size(Nt, Naa, Nf);
        amplitudes_sampl.fill(-70.0);
    }

    if (array_type == ArrayType_Circle)
    {
        fvec thetas_c(Nt + 2);
        thetas_c(span(1, Nt)) = thetas;
        thetas_c(0) = thetas(Nt - 1) - 360.0;
        thetas_c(Nt + 1) = thetas(0) + 360.0;

        fcube phases_sampl_tmp(Nt + 2, Naa, Nf);
        fcube amplitudes_sampl_tmp(Nt + 2, Naa, Nf);

        for (int n = 0; n < Nf; n++)
        {
            fmat tmp_mat = phases_sampl.slice(n);
            fmat tmp_mat2 = tmp_mat;
            tmp_mat2 = join_vert(tmp_mat.row(Nt - 1), tmp_mat2);
            tmp_mat2 = join_vert(tmp_mat2, tmp_mat.row(0));
            phases_sampl_tmp.slice(n) = tmp_mat2;

            tmp_mat = amplitudes_sampl.slice(n);
            tmp_mat2 = tmp_mat;
            tmp_mat2 = join_vert(tmp_mat.row(Nt - 1), tmp_mat2);
            tmp_mat2 = join_vert(tmp_mat2, tmp_mat.row(0));
            amplitudes_sampl_tmp.slice(n) = tmp_mat2;
        }

        thetas = thetas_c;
        phases_sampl = phases_sampl_tmp;
        amplitudes_sampl = amplitudes_sampl_tmp;
    }

    if (Interpolate(freq_min, 1000000, freq_max))
    {
        if (array_type == ArrayType_Circle)
        {
            thetas_raw = thetas(span(1, Nt));
            phases_sampl_1MHz.set_size(Nt, Naa, Nf);
            amplitudes_sampl_1MHz.set_size(Nt, Naa, Nf);

            for (int i = 0; i < Nt; i++)
            {
                for (int j = 0; j < Naa; j++)
                {
                    for (int k = 0; k < Nf; k++)
                    {
                        phases_sampl_1MHz(i, j, k) = phases_sampl(i + 1, j, k);
                        amplitudes_sampl_1MHz(i, j, k) = amplitudes_sampl(i + 1, j, k);
                    }
                }
            }
        }
        else
        {
            thetas_raw = thetas;
            phases_sampl_1MHz = phases_sampl;
            amplitudes_sampl_1MHz = amplitudes_sampl;
        }
    }
    else {
        return false;
    }

    //  [1/12/2016 Direction Finding]
    fcube phases_sampl_2(phases_sampl.n_rows, Nbl, phases_sampl.n_slices);

    if (array_type == ArrayType_Circle)
    {
        int count = 0;

        for (int i = 0; i < Naa; i++)
        {
            for (int j = i + 1; j < Naa; j++)
            {
                phases_sampl_2(span::all, span(count), span::all) = phases_sampl(span::all, span(i), span::all)
                        - phases_sampl(span::all, span(j), span::all);

                count++;
            }
        }
    }
    else {
        phases_sampl_2 = phases_sampl;
    }

    complex_sampl.set_size(arma::size(phases_sampl_2));

    for (int i = 0; i < complex_sampl.n_rows; i++)
    {
        for (int j = 0; j < complex_sampl.n_cols; j++)
        {
            for (int k = 0; k < complex_sampl.n_slices; k++)
            {
                complex_sampl(i, j, k) = cx_float(df_lib::cosd(phases_sampl_2(i, j, k)), df_lib::sind(phases_sampl_2(i, j, k)));
            }
        }
    }
    //  [1/12/2016 Direction Finding]

    return true;
}

float AlgorithmCI::GetDOA(Hz freq, const float *phases, const float *amplitudes, float *curve)
{
    df_lib::clamp(freq, freq_min, freq_max);

    int nearest = FreqIndex(freq);
    cx_fvec phaseCol(Nbl);

    if (array_type == ArrayType_Circle)
    {
        int count = 0;

        for (int i = 0; i < Naa; i++)
        {
            for (int j = i + 1; j < Naa; j++)
            {
                float dp = (phases[j] - phases[i]) * Factor_Deg2Rad;
                phaseCol(count) = cx_float(cos(dp), sin(dp));

                count++;
            }
        }
    }
    else {
        for (int i = 0; i < Naa; i++)
        {
            float rad = phases[i] * Factor_Deg2Rad;
            phaseCol(i) = cx_float(cos(rad), sin(rad));
        }
    }

    fvec fcorr = real(complex_sampl.slice(nearest) * phaseCol);

    // export correlation curve
    if (curve)
    {
        fcorr = fcorr / Nbl;
        int thetaOffset = 0;
        if (array_type == ArrayType_Circle)
            thetaOffset = 1;
        memcpy((char *)curve, (char *)(fcorr.memptr()+thetaOffset), sizeof(float)*ThetaCount());
    }

    fvec diffv = arma::diff(fcorr);
    float val_pk = - datum::inf;
    int ind_pk = 0;

    for (int n = 0; n < diffv.size()-1; n++) {
        if (diffv(n)>0 && diffv(n+1)<0) {
            if (fcorr(n+1) > val_pk) {
                val_pk = fcorr(n+1);
                ind_pk = n+1;
            }
        }
    }

    if (ind_pk < 1) {
        return -444.0;
    }

    if (val_pk < threshold) {
        return -555.0;
    }

    float theta_prev = thetas[ind_pk - 1];
    float theta_pks  = thetas[ind_pk];
    float theta_next = thetas[ind_pk + 1];

    mat33 V;
    vec3 y, abc;

    V.at(0, 0) = theta_prev * theta_prev; V.at(0, 1) = theta_prev; V.at(0, 2) = 1.0;
    V.at(1, 0) = theta_pks  * theta_pks;  V.at(1, 1) = theta_pks;  V.at(1, 2) = 1.0;
    V.at(2, 0) = theta_next * theta_next; V.at(2, 1) = theta_next; V.at(2, 2) = 1.0;

    y[0] = fcorr[ind_pk - 1]; y[1] = val_pk; y[2] = fcorr[ind_pk + 1];

    abc = inv(V) * y;
    float bearing = -abc[1] / (2.0*abc(0));

    return df_lib::mod(bearing, (float)360.0); // - b / (2a)
}

