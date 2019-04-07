#include "factory.h"
#include "df_lib.h"
#include <omp.h>
//#include "precise_timer.h"
//PreciseTimer timer;

float GetAmplitude(unsigned char amp, int gain = 0)
{
    return (((float)amp / 2) - 174) + gain;//+ 160 ;
}

float GetDeltaPhase(short cPha)
{
    return df_lib::angle(float(cPha * 0.010986328125));
}

// CAL_COUNT_MAX must be large enough
const int CAL_COUNT_MAX = 100;

DataFactory::DataFactory(Session *sPtr) : session_ptr(sPtr), initialized(false)
  , DFEnabled(true)
{

}

void DataFactory::Init(int _nchannels, int _npts, Hz _rbw, Hz _start, Hz _stop, Hz _step)
{
    Hz stop = _stop;
    Hz start = _start;
    Hz step = _step;

    int length2 = ceil((stop - start)*1.0/step);
    stop = start + step * length2;
    freqs_center = linspace<Hzvec>(start + step/2, stop - step/2, length2);

    Init(_nchannels, _npts, _rbw, freqs_center);
}

void DataFactory::Init(int _nchannels, int _npts, Hz _rbw, Hzvec centers)
{
    nchannels = _nchannels;
    npts = _npts;
    rbw = _rbw;
    bw = rbw * _npts;
    length = centers.size();

    atten_rf.set_size(length); atten_rf.fill(0);

    amplitudes_cal.set_size(npts*length, nchannels); //amplitudes_cal.fill(0.0);
    phases_cal.set_size(npts*length, nchannels); //phases_cal.fill(0.0);
    calCount.set_size(length); calCount.fill(0);

    phases_cal_last = randu<fmat>(npts*length, nchannels) * 100.0;
    amplitudes.set_size(npts*length, nchannels);
    phases.set_size(npts*length, nchannels);
    bearings.set_size(npts*length);    
//    corrCurve.set_size(session_ptr->algorithm->ThetaCount(), npts*length);
//    corrCurve.fill(0.0);

    freqs_center = centers;
    bands_status.set_size(length); bands_status.fill(0);
    time_stamps.set_size(length);

    initialized = true;
}

void DataFactory::SwitchToCAL(bool sta)
{
    calibrating = sta;
}

void DataFactory::setDFEnabled(bool enabled) {
    DFEnabled = enabled;
}

bool DataFactory::Push(const DataFrame &frame)
{
    if (!initialized)
        return false;

    Hz tuner_freq = frame.center;
    int ind = (int)((tuner_freq - FreqCenterMin())/bw);

    if (ind < 0 || ind >= length)
        return false;

    int offset = ind * npts;
    atten_rf(ind) = frame.gain;

    // time, tempratures, antenna informations and so on
//    time_stamps(ind) = frame

    if (calibrating)
    {
        unsigned int space_cal = 100000; // 100kHz
        uvec lines_cal(npts, fill::zeros);
        fmat amplitudes_loc(npts, nchannels, fill::zeros);
        fmat phases_loc(npts, nchannels, fill::zeros);

        for (int i = 0; i < npts; i++)
        {
            unsigned int dist = abs(i-npts/2) * rbw;

            if (dist % space_cal == 0)
            {
                lines_cal(i) = 1;

                for (int j = 0; j < nchannels; j++)
                {
                    amplitudes_loc(i,j) = GetAmplitude(frame.amplitudes[j][i], frame.gain);
                    phases_loc(i,j) = frame.phase_differences[j][i];
                }
            }
        }

        amplitudes_loc.each_col() -= amplitudes_loc.col(0);
        uvec ind_cal = find(lines_cal == 1);
        fvec ind_calf = conv_to<fvec>::from(ind_cal);
        fvec x = linspace<fvec>(0, npts-1, npts);
        fvec y_ampl, y_phase;

        for (int j = 0; j < nchannels; j++)
        {
            fvec ampl_chan = amplitudes_loc.col(j);
            fvec pha_chan = phases_loc.col(j);

            interp1(ind_calf, ampl_chan(ind_cal), x, y_ampl, "*nearest");
            interp1(ind_calf, pha_chan(ind_cal), x, y_phase, "*nearest");

            for (int i = 0; i < npts; i++)
            {
                if (is_finite(y_ampl(i))) {
                    amplitudes_cal(offset+i,j) = y_ampl(i);
                    phases_cal(offset+i,j) = y_phase(i);
                }
                else {
                    fvec dist = abs(i - ind_calf);
                    uword nearest = dist.index_min();

                    amplitudes_cal(offset+i,j) = amplitudes_loc(nearest,j);
                    phases_cal(offset+i,j) = phases_loc(nearest,j);
                }
            }
        }

        bands_status(ind) = 1;

        fmat delta = abs(phases_cal(span(offset,offset+npts-1), span::all)
                         - phases_cal_last(span(offset,offset+npts-1), span::all));

        if (all(vectorise(delta) < 3)) {
            bands_status(ind) = 1;
            calCount(ind) = 0;
        }
        else {
            calCount(ind) = calCount(ind) + 1;
            phases_cal_last = phases_cal;

            if (calCount(ind) > CAL_COUNT_MAX) {
                bands_status(ind) = 1;
                calCount(ind) = 0;
                MY_WARNING << QString(QObject::tr("Calibration timeout, f0 = %1 Hz")).arg(tuner_freq);
            }
        }
    }
    else
    {
        #pragma omp parallel for
        for (int i = 0; i < npts; i++)
        {
            fvec  phases_in(nchannels);
            fvec  amplitudes_in(nchannels);

            for (int j = 0; j < nchannels; j++)
            {
                amplitudes(offset+i,j) = GetAmplitude(frame.amplitudes[j][i], frame.gain);
                phases(offset+i,j) = frame.phase_differences[j][i];

                phases_in(j) = df_lib::angle(phases(offset+i,j) - phases_cal(offset+i,j));
                amplitudes_in(j) = amplitudes(offset+i,j) - amplitudes_cal(offset+i,j);
            }                      

            if (DFEnabled) {
                // calculate bearings by calling CI algorithm functions
                unsigned int freq = freqs_center(ind)-bw/2+i*rbw;
                bearings(offset+i) = session_ptr->algorithm->GetDOA(freq, phases_in.memptr(), NULL, corrCurve.colptr(offset+i));
            }
        }

        bands_status(ind) = 1;
    }

    return true;
}


bool DataFactory::AllDone()
{
//    cout << "sum(bands_status) = " << sum(bands_status) << " ; Required = " << length << endl;

    // judge whether tasks have been finished
    if (sum(bands_status) == length)
    {
//        timer.stop();
//        qDebug() << "DF sweep velocity: " << session_ptr->settings->Span()*1.0e-9 / timer.elapse() << " GHz/s";
        bands_status.fill(0); // reset status group
//        timer.start();
        return true;
    }

    return false;
}


