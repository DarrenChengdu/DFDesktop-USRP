// SignalGenerator.cpp : implementation file
//

#include "signal_generator.h"
#include <string.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSignalGenerator

CSignalGenerator::CSignalGenerator()
{
    freq = 100.0e6;
    amplitude = -70;
    RFOn = false;
    ModOn = false;
    connected = false;

    SetSignalModOn(ModOn);
    SetSignalRFOn(RFOn);

    updated(this);
}


CSignalGenerator::~CSignalGenerator()
{
    DisconnectSignalGen();
}

void CSignalGenerator::DisconnectSignalGen()
{
//    if (connected) {
//        SetSignalModOn(false);
//        SetSignalRFOn(false);
//        status = viClose(instr); // Close down the system
//        status = viClose(defaultRM);
//        connected = false;

//        updated(this);
//    }
}

bool CSignalGenerator::SetSignalGenFreq(Frequency f)
{    
//	char mesg[100];
//    sprintf(mesg, "FREQ %.3fMHz\n", f.Val()*1.0e-6);

//	status = viWrite(instr, (ViBuf)mesg, strlen(mesg), &retCount);
//	if (status != VI_SUCCESS) { return false; }

//    freq = f; updated(this);

	return true;
}

Frequency CSignalGenerator::QueryFreq()
{
//	ViChar buffer[MAX_COUNT];
//	status = viWrite(instr, (ViBuf)"FREQ?\n", 6, &retCount);
//	status = viRead(instr, (ViBuf)buffer, MAX_COUNT, &retCount);
//	buffer[retCount] = '\0';
//    freq = atof(buffer);
    return freq;
}


bool CSignalGenerator::SetSignalGenPow(double PowdBm)
{    
//	char mesg[100];
//	sprintf(mesg, "POW:AMPL %.3fDBM\n", PowdBm);
//	status = viWrite(instr, (ViBuf)mesg, strlen(mesg), &retCount);

//	if (status != VI_SUCCESS) { return false; }

//    amplitude = PowdBm; updated(this);

	return true;
}


bool CSignalGenerator::SetSignalRFOn(bool _RFOn)
{
//    if (_RFOn)
//	{
//		status = viWrite(instr, (ViBuf)"OUTP ON\n", 8, &retCount);
//	}
//	else
//	{
//		status = viWrite(instr, (ViBuf)"OUTP OFF\n", 9, &retCount);
//	}

//	if (status != VI_SUCCESS) { return false; }

//    RFOn = _RFOn; updated(this);

	return true;
}


bool CSignalGenerator::SetSignalModOn(bool _ModOn)
{
//    if (_ModOn)
//	{
//		status = viWrite(instr, (ViBuf)"OUTP:MOD:STAT ON\n", 17, &retCount);
//	}
//	else
//	{
//		status = viWrite(instr, (ViBuf)"OUTP:MOD:STAT OFF\n", 18, &retCount);
//	}

//	if (status != VI_SUCCESS) { return false; }

//    ModOn = _ModOn; updated(this);

	return true;
}

bool CSignalGenerator::ConnectSignalGen(char *ipStr)
{
//    if (connected)
//        return true;

//    viClear(defaultRM);
//    viClear(instr);

//	status = viOpenDefaultRM(&defaultRM); // Initialize the	system
//	if (status != VI_SUCCESS){ // If problems then prompt	user
//		printf("Open VISA Resource Manager Error!\n");
//		printf("\n");
//		return false;
//	}

//	// Open communication with TCP/IP
//    status = viOpen(defaultRM, ipStr, VI_NULL, 5000, &instr);

//    if (status != VI_SUCCESS){ // If problems then prompt	user
//        printf("Could not open ViSession!\n");
//        printf("Check instruments and connections\n");
//        printf("\n");
//        return false;
//    }

//	// Set timeout for 5 seconds
////	viSetAttribute(instr, VI_ATTR_TMO_VALUE, 5000);
//    connected = true; updated(this);
	return true;
}

