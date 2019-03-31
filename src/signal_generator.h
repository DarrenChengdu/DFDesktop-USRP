#if !defined(AFX_SIGNALGENERATOR_H__EF9018F6_0152_4831_BE27_F9DAD963BB32__INCLUDED_)
#define AFX_SIGNALGENERATOR_H__EF9018F6_0152_4831_BE27_F9DAD963BB32__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SignalGenerator.h : header file
//

//#include "visa/visa.h"
#include "lib/frequency.h"
#include <stdio.h>
#include <stdlib.h>
//#include <conio.h>
#include <QObject>
#define MAX_COUNT 200
/////////////////////////////////////////////////////////////////////////////
// CSignalGenerator command target

class CSignalGenerator : public QObject
{
    Q_OBJECT
public:
	CSignalGenerator();
	virtual ~CSignalGenerator();
    bool isConnected() const {return connected;}
    Frequency getFrequency() const {return freq;}
    double getAmplitude() const {return amplitude;}
    bool isRFOn() const {return RFOn;}
    bool isModOn() const {return ModOn;}

	// Operations
public:
    Frequency QueryFreq();
    bool SetSignalSource(Frequency f, double PowdBm, bool _RFOn, bool _ModOn = false)
	{
        return SetSignalGenFreq(f)
                && SetSignalGenPow(PowdBm)
                && SetSignalRFOn(_RFOn)
                && SetSignalModOn(_ModOn);
	}

	bool ConnectSignalGen(char *ipStr);
    void DisconnectSignalGen();

private:
//	ViStatus status;// Declares a type ViStatus variable
//	ViSession defaultRM, instr;// Declares a type ViSession variable
//	ViUInt32 retCount;// Return count for string I/O
//	ViChar buffer[MAX_COUNT];// Buffer for string I/O

    Frequency freq;
    double amplitude;
    bool RFOn;
    bool ModOn;
    bool connected;

public slots:
    bool SetSignalRFOn(bool _RFOn);
    bool SetSignalModOn(bool _ModOn);
    bool SetSignalGenPow(double PowdBm);
    bool SetSignalGenFreq(Frequency f);

signals:
    // Emit when settings successfully changed
    void updated(const CSignalGenerator *);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SIGNALGENERATOR_H__EF9018F6_0152_4831_BE27_F9DAD963BB32__INCLUDED_)
