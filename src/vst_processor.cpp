
#include "../include/vst_processor.hpp"

#include <public.sdk/source/vst/vstaudioprocessoralgo.h>

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


		tresult PLUGIN_API VstProcessor::canProcessSampleSize(int32 symbolicSampleSize)
		{
			if (symbolicSampleSize == kSample32)
				return kResultTrue;
		
			else if (symbolicSampleSize == kSample64)
				return kResultTrue;
		
			return kResultFalse;
		}

		tresult PLUGIN_API VstProcessor::setState(IBStream * state)
		{
			tresult result;

			int32 stateBypass = 0;
			result = state->read(&stateBypass, sizeof(stateBypass));
			if (result != kResultOk)
				return kResultFalse;

#if BYTEORDER == kBigEndian
			SWAP_32(stateBypass);
#endif
			m_bypass = (stateBypass > 0);

			return kResultOk;
		}

		tresult PLUGIN_API VstProcessor::getState(IBStream * state)
		{
			int32 stateBypass = m_bypass ? 1 : 0;
			state->write(&stateBypass, sizeof(stateBypass));

			return kResultOk;
		}


		tresult PLUGIN_API VstProcessor::process(ProcessData & data)
		{
			// parameters
			if (data.inputParameterChanges != NULL)
			{
				IParameterChanges * paramChanges = data.inputParameterChanges;

				int32 paramChangeCount = paramChanges->getParameterCount();
				for (int32 i = 0; i < paramChangeCount; i++)
				{
					IParamValueQueue * queue = paramChanges->getParameterData(i);
					if (queue == NULL) continue;

					int32 sampleOffset;
					ParamValue value;

					int32 valueChangeCount = queue->getPointCount();
					tresult result = queue->getPoint(valueChangeCount - 1, sampleOffset, value);
					if (result != kResultTrue) continue;

					int32 tag = queue->getParameterId();
					switch (tag)
					{
					case kParamBypass:
						m_bypass = (value > 0.5);
						break;

					default:
						break;
					}
				}
			}

			if (data.numInputs == 0 || data.numOutputs == 0)
				return kResultTrue;

			AudioBusBuffers & input = data.inputs[0];
			AudioBusBuffers & output = data.outputs[0];

			if (input.numChannels != output.numChannels)
				return kResultFalse;

			int32 numChannels = input.numChannels;
			int32 numSamples = data.numSamples;
			void ** buf_in = getChannelBuffersPointer(this->processSetup, input);
			void ** buf_out = getChannelBuffersPointer(this->processSetup, output);

			if (!buf_in || !buf_out) return kResultTrue;

			// bypass
			if (m_bypass)
			{
				if (data.symbolicSampleSize == kSample32)
					return processBypass<Sample32>(
						numChannels, numSamples,
						(Sample32 **)buf_in, (Sample32 **)buf_out
					);

				else if (data.symbolicSampleSize == kSample64)
					return processBypass<Sample64>(
						numChannels, numSamples,
						(Sample64 **)buf_in, (Sample64 **)buf_out
					);

				return kResultTrue;
			}

			// normal process
			if (data.symbolicSampleSize == kSample32)
				return processAudioBuffer<Sample32>(
					numChannels, numSamples,
					(Sample32 **)buf_in, (Sample32 **)buf_out
				);

			else if (data.symbolicSampleSize == kSample64)
				return processAudioBuffer<Sample64>(
					numChannels, numSamples,
					(Sample64 **)buf_in, (Sample64 **)buf_out
				);

			return kResultTrue;
		}

	}
}
