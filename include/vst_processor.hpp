
#pragma once

#include "vst_tags.hpp"
#include "vst_fuid.hpp"

#include <public.sdk/source/vst/vstaudioeffect.h>

namespace Steinberg {
	namespace Vst {

		class VstProcessor : public AudioEffect
		{
		public:
			VstProcessor()
				: m_bypass(false)
			{
				setControllerClass(ControllerUID);
			}

			tresult PLUGIN_API initialize(FUnknown * context);
			
			tresult PLUGIN_API setBusArrangements(
				SpeakerArrangement * inputs, int32 numIns,
				SpeakerArrangement * outputs, int32 numOuts);

			tresult PLUGIN_API canProcessSampleSize(int32 symbolicSampleSize);
			
			tresult PLUGIN_API process(ProcessData & data);
			
			static FUnknown * createInstance(void *)
			{
				return (IAudioProcessor *)new VstProcessor();
			}

		protected:
			bool m_bypass;

		private:
			template<typename T>
			tresult processBypass(int32 numChannels, int32 numSamples, T ** buf_in, T ** buf_out)
			{
				for (int32 ch = 0; ch < numChannels; ch++)
				{
					T * in = buf_in[ch];
					T * out = buf_out[ch];

					// don't need to copy if the buffers are the same address
					if (in == out) continue;

					for (int32 i = 0; i < numSamples; i++)
					{
						out[i] = in[i];
					}
				}
				return kResultTrue;
			}

			template<typename T>
			tresult processAudioBuffer(int32 numChannels, int32 numSamples, T ** buf_in, T ** buf_out)
			{
				for (int32 ch = 0; ch < numChannels; ch++)
				{
					T * in = buf_in[ch];
					T * out = buf_out[ch];

					for (int32 i = 0; i < numSamples; i++)
					{
						// TODO: implement our effector here
						out[i] = in[i];
					}
				}

				return kResultTrue;
			}
		};

	}
}
