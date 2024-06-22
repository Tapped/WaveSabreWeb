#ifndef __VECTRONVST_H__
#define __VECTRONVST_H__

#include <WaveSabreCore.h>
using namespace WaveSabreCore;

#include <WaveSabreVstLib.h>
using namespace WaveSabreVstLib;

class VectronVst : public VstPlug
{
public:
	VectronVst(audioMasterCallback audioMaster);

	virtual void getParameterName(VstInt32 index, char *text);

	virtual bool getEffectName(char *name);
	virtual bool getProductString(char *text);

	Vectron *Device;
};

#endif