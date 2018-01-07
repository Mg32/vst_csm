
#include "../include/vst_processor.hpp"

namespace Steinberg {
	namespace Vst {

		tresult PLUGIN_API VstProcessor::initialize(FUnknown * context)
		{
			tresult result = AudioEffect::initialize(context);
			if (result == kResultTrue)
			{
				addAudioInput(STR16("AudioInput"), SpeakerArr::kStereo);
				addAudioOutput(STR16("AudioOutput"), SpeakerArr::kStereo);
			}

			return result;
		}


		tresult PLUGIN_API VstProcessor::setBusArrangements(
			SpeakerArrangement * inputs, int32 numIns,
			SpeakerArrangement * outputs, int32 numOuts)
		{
			if (numIns == 1 && inputs[0] == SpeakerArr::kStereo &&
				numOuts == 1 && outputs[0] == SpeakerArr::kStereo)
			{
				return AudioEffect::setBusArrangements(inputs, numIns, outputs, numOuts);
			}

			return kResultFalse;
		}


		tresult PLUGIN_API VstProcessor::process(ProcessData & data)
		{
			Sample32 * inL = data.inputs[0].channelBuffers32[0];
			Sample32 * inR = data.inputs[0].channelBuffers32[1];
			Sample32 * outL = data.outputs[0].channelBuffers32[0];
			Sample32 * outR = data.outputs[0].channelBuffers32[1];

			for (int32 i = 0; i < data.numSamples; i++)
			{
				outL[i] = inL[i];
				outR[i] = inR[i];
			}

			return kResultTrue;
		}

	}
}
