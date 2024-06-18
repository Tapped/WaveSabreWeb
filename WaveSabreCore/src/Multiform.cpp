#include <WaveSabreCore/Multiform.h>
#include <WaveSabreCore/Helpers.h>

#include <math.h>

namespace WaveSabreCore
{
	Multiform::Multiform()
		: SynthDevice((int)ParamIndices::NumParams)
		, sample(nullptr)
	{
		for (int i = 0; i < maxVoices; i++) voices[i] = new MultiformVoice(this);

		chunkData = nullptr;

		ampAttack = 1.0f;
		ampDecay = 1.0f;
		ampSustain = 1.0f;
		ampRelease = 1.0f;

		sampleStart = 0.0f;
		reverse = false;
		loopMode = LoopMode::Disabled;
		loopBoundaryMode = LoopBoundaryMode::Manual;
		loopStart = 0.0f;
		loopLength = 1.0f;
		sampleLoopStart = 0;
		sampleLoopLength = 0;

		interpolationMode = InterpolationMode::Linear;

		coarseTune = 0.5f;
		fineTune = 0.5f;

		filterType = StateVariableFilterType::Lowpass;
		filterFreq = 20000.0f - 20.0f;
		filterResonance = 1.0f;
		filterModAmt = .5f;

		modAttack = 1.0f;
		modDecay = 5.0f;
		modSustain = 1.0f;
		modRelease = 1.5f;

		masterLevel = 0.5f;
	}

	Multiform::~Multiform()
	{
		if (chunkData) delete [] chunkData;
		if (sample) delete sample;
	}

	void Multiform::SetParam(int index, float value)
	{
		switch ((ParamIndices)index)
		{
		case ParamIndices::AmpAttack: ampAttack = Helpers::ScalarToEnvValue(value); break;
		case ParamIndices::AmpDecay: ampDecay = Helpers::ScalarToEnvValue(value); break;
		case ParamIndices::AmpSustain: ampSustain = value; break;
		case ParamIndices::AmpRelease: ampRelease = Helpers::ScalarToEnvValue(value); break;

		case ParamIndices::SampleStart: sampleStart = value; break;
		case ParamIndices::Reverse: reverse = Helpers::ParamToBoolean(value); break;
		case ParamIndices::LoopMode: loopMode = (LoopMode)(int)(value * (float)((int)LoopMode::NumLoopModes - 1)); break;
		case ParamIndices::LoopStart: loopStart = value; break;
		case ParamIndices::LoopLength: loopLength = value; break;

		case ParamIndices::InterpolationMode: interpolationMode = (InterpolationMode)(int)(value * (float)((int)InterpolationMode::NumInterpolationModes - 1)); break;

		case ParamIndices::CoarseTune: coarseTune = value; break;
		case ParamIndices::FineTune: fineTune = value; break;

		case ParamIndices::FilterType: filterType = Helpers::ParamToStateVariableFilterType(value); break;
		case ParamIndices::FilterFreq: filterFreq = Helpers::ParamToFrequency(value); break;
		case ParamIndices::FilterResonance: filterResonance = 1.0f - value; break;
		case ParamIndices::FilterModAmt: filterModAmt = value; break;

		case ParamIndices::ModAttack: modAttack = Helpers::ScalarToEnvValue(value); break;
		case ParamIndices::ModDecay: modDecay = Helpers::ScalarToEnvValue(value); break;
		case ParamIndices::ModSustain: modSustain = value; break;
		case ParamIndices::ModRelease: modRelease = Helpers::ScalarToEnvValue(value); break;

		case ParamIndices::VoicesUnisono: VoicesUnisono = Helpers::ParamToUnisono(value); break;
		case ParamIndices::VoicesDetune: VoicesDetune = value; break;
		case ParamIndices::VoicesPan: VoicesPan = value; break;

		case ParamIndices::Master: masterLevel = value; break;

		case ParamIndices::VoiceMode: SetVoiceMode(Helpers::ParamToVoiceMode(value)); break;
		case ParamIndices::SlideTime: Slide = value; break;
		}
	}

	float Multiform::GetParam(int index) const
	{
		switch ((ParamIndices)index)
		{
		case ParamIndices::AmpAttack: default: return Helpers::EnvValueToScalar(ampAttack);
		case ParamIndices::AmpDecay: return Helpers::EnvValueToScalar(ampDecay);
		case ParamIndices::AmpSustain: return ampSustain;
		case ParamIndices::AmpRelease: return Helpers::EnvValueToScalar(ampRelease);

		case ParamIndices::SampleStart: return sampleStart;
		case ParamIndices::Reverse: return Helpers::BooleanToParam(reverse);
		case ParamIndices::LoopMode: return (float)loopMode / (float)((int)LoopMode::NumLoopModes - 1);
		case ParamIndices::LoopStart: return loopStart;
		case ParamIndices::LoopLength: return loopLength;
		case ParamIndices::InterpolationMode: return (float)interpolationMode / (float)((int)InterpolationMode::NumInterpolationModes - 1);

		case ParamIndices::CoarseTune: return coarseTune;
		case ParamIndices::FineTune: return fineTune;

		case ParamIndices::FilterType: return Helpers::StateVariableFilterTypeToParam(filterType);
		case ParamIndices::FilterFreq: return Helpers::FrequencyToParam(filterFreq);
		case ParamIndices::FilterResonance: return 1.0f - filterResonance;
		case ParamIndices::FilterModAmt: return filterModAmt;

		case ParamIndices::ModAttack: return Helpers::EnvValueToScalar(modAttack);
		case ParamIndices::ModDecay: return Helpers::EnvValueToScalar(modDecay);
		case ParamIndices::ModSustain: return modSustain;
		case ParamIndices::ModRelease: return Helpers::EnvValueToScalar(modRelease);

		case ParamIndices::VoicesUnisono: return Helpers::UnisonoToParam(VoicesUnisono);
		case ParamIndices::VoicesDetune: return VoicesDetune;
		case ParamIndices::VoicesPan: return VoicesPan;

		case ParamIndices::Master: return masterLevel;

		case ParamIndices::VoiceMode: return Helpers::VoiceModeToParam(GetVoiceMode());
		case ParamIndices::SlideTime: return Slide;
		}
	}

	typedef struct
	{
		int CompressedSize;
		int UncompressedSize;
	} ChunkHeader;

	void Multiform::SetChunk(void *data, int size)
	{
		if (!size) return;

		// Read header
		auto headerPtr = (ChunkHeader *)data;
		auto headerSize = sizeof(ChunkHeader);

		// Read wave format
		auto waveFormatPtr = (WAVEFORMATEX *)((char *)data + headerSize);
		auto waveFormatSize = sizeof(WAVEFORMATEX) + waveFormatPtr->cbSize;

		// Read compressed data and load sample
		auto compressedDataPtr = (char *)waveFormatPtr + waveFormatSize;
		auto compressedDataSize = headerPtr->CompressedSize;
		LoadSample(compressedDataPtr, headerPtr->CompressedSize, headerPtr->UncompressedSize, waveFormatPtr);

		// Read params
		//  The rest of the data from the start of the params until the end of the chunk
		//  should be interpreted as a bunch of float params, minus a single int at the
		//  end, which should match the size of the chunk.
		auto paramDataPtr = compressedDataPtr + compressedDataSize;
		// This may be different than our internal numParams value if this chunk was
		//  saved with an earlier version of the plug for example. It's important we
		//  don't read past the chunk data, so we set as many parameters as the
		//  chunk contains, not the amount of parameters we have available. The
		//  remaining parameters will retain their default values in that case, which
		//  if we've done our job right, shouldn't change the sound with respect to
		//  the parameters we read here.
		auto numChunkParams = (int)((size - sizeof(int) - (paramDataPtr - (char *)data)) / sizeof(float));
		for (int i = 0; i < numChunkParams; i++)
			SetParam(i, ((float *)paramDataPtr)[i]);
	}

	int Multiform::GetChunk(void **data)
	{
		if (!sample) return 0;

		// Figure out size of chunk
		//  The names here are meant to be symmetric with those in SetChunk for clarity
		auto headerSize = sizeof(ChunkHeader);
		auto waveFormatSize = sizeof(WAVEFORMATEX) + ((WAVEFORMATEX *)sample->WaveFormatData)->cbSize;
		auto compressedDataSize = sample->CompressedSize;
		auto paramSize = (int)ParamIndices::NumParams * sizeof(float);
		auto chunkSizeSize = sizeof(int);
		int size = (int)(headerSize + waveFormatSize + sample->CompressedSize + paramSize + chunkSizeSize);

		// (Re)allocate chunk data
		if (chunkData) delete [] chunkData;
		chunkData = new char[size];

		// Write header
		ChunkHeader header;
		header.CompressedSize = sample->CompressedSize;
		header.UncompressedSize = sample->UncompressedSize;
		memcpy(chunkData, &header, sizeof(ChunkHeader));

		// Write wave format
		auto waveFormatPtr = (char *)chunkData + headerSize;
		memcpy(waveFormatPtr, sample->WaveFormatData, waveFormatSize);

		// Write compressed data
		auto compressedDataPtr = waveFormatPtr + waveFormatSize;
		memcpy(compressedDataPtr, sample->CompressedData, compressedDataSize);

		// Write params
		auto paramDataPtr = (float *)(compressedDataPtr + compressedDataSize);
		for (int i = 0; i < (int)ParamIndices::NumParams; i++)
			paramDataPtr[i] = GetParam(i);

		// Write final chunk size
		auto chunkSizePtr = (int *)((char *)paramDataPtr + paramSize);
		*chunkSizePtr = size;

		*data = chunkData;
		return size;
	}

	void Multiform::LoadSample(char *compressedDataPtr, int compressedSize, int uncompressedSize, WAVEFORMATEX *waveFormatPtr)
	{
		if (sample) delete sample;

		sample = new GsmSample(compressedDataPtr, compressedSize, uncompressedSize, waveFormatPtr);

		sampleLoopStart = 0;
		sampleLoopLength = sample->SampleLength;
	}

	Multiform::MultiformVoice::MultiformVoice(Multiform *multiform)
	{
		this->multiform = multiform;
	}

	SynthDevice *Multiform::MultiformVoice::GetSynthDevice() const
	{
		return multiform;
	}

	void Multiform::MultiformVoice::Run(double songPosition, float **outputs, int numSamples)
	{
		filter.SetType(multiform->filterType);
		filter.SetQ(multiform->filterResonance);

		samplePlayer.SampleStart = multiform->sampleStart;
		samplePlayer.LoopStart = multiform->loopStart;
		samplePlayer.LoopLength = multiform->loopLength;
		samplePlayer.LoopMode = multiform->loopMode;
		samplePlayer.LoopBoundaryMode = multiform->loopBoundaryMode;
		samplePlayer.InterpolationMode = multiform->interpolationMode;
		samplePlayer.Reverse = multiform->reverse;

		samplePlayer.RunPrep();

		float amp = Helpers::VolumeToScalar(multiform->masterLevel);
		float panLeft = Helpers::PanToScalarLeft(Pan);
		float panRight = Helpers::PanToScalarRight(Pan);

		for (int i = 0; i < numSamples; i++)
		{
			calcPitch();

			filter.SetFreq(Helpers::Clamp(multiform->filterFreq + modEnv.GetValue() * (20000.0f - 20.0f) * (multiform->filterModAmt * 2.0f - 1.0f), 0.0f, 20000.0f - 20.0f));

			float sample = samplePlayer.Next();
			if (!samplePlayer.IsActive)
			{
				IsOn = false;
				break;
			}

			sample = filter.Next(sample) * ampEnv.GetValue() * velocity * amp;
			outputs[0][i] += sample * panLeft;
			outputs[1][i] += sample * panRight;

			modEnv.Next();
			ampEnv.Next();
			if (ampEnv.State == EnvelopeState::Finished)
			{
				IsOn = false;
				break;
			}
		}
	}

	void Multiform::MultiformVoice::NoteOn(int note, int velocity, float detune, float pan)
	{
		Voice::NoteOn(note, velocity, detune, pan);

		ampEnv.Attack = multiform->ampAttack;
		ampEnv.Decay = multiform->ampDecay;
		ampEnv.Sustain = multiform->ampSustain;
		ampEnv.Release = multiform->ampRelease;
		ampEnv.Trigger();

		modEnv.Attack = multiform->modAttack;
		modEnv.Decay = multiform->modDecay;
		modEnv.Sustain = multiform->modSustain;
		modEnv.Release = multiform->modRelease;
		modEnv.Trigger();

		samplePlayer.SampleData = multiform->sample->SampleData;
		samplePlayer.SampleLength = multiform->sample->SampleLength;
		samplePlayer.SampleLoopStart = multiform->sampleLoopStart;
		samplePlayer.SampleLoopLength = multiform->sampleLoopLength;

		samplePlayer.SampleStart = multiform->sampleStart;
		samplePlayer.LoopStart = multiform->loopStart;
		samplePlayer.LoopLength = multiform->loopLength;
		samplePlayer.LoopMode = multiform->loopMode;
		samplePlayer.LoopBoundaryMode = multiform->loopBoundaryMode;
		samplePlayer.InterpolationMode = multiform->interpolationMode;
		samplePlayer.Reverse = multiform->reverse;

		calcPitch();
		samplePlayer.InitPos();

		this->velocity = (float)velocity / 128.0f;
	}
	
	void Multiform::MultiformVoice::NoteOff()
	{
		ampEnv.Off();
		modEnv.Off();
	}

	double Multiform::MultiformVoice::coarseDetune(float detune)
	{
		return floor((detune * 2.0f - 1.0f) * 12.0f);
	}

	void Multiform::MultiformVoice::calcPitch()
	{
		samplePlayer.CalcPitch(GetNote() - 60 + Detune + multiform->fineTune * 2.0f - 1.0f + MultiformVoice::coarseDetune(multiform->coarseTune));
	}
}
