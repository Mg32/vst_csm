
#pragma once

#include <public.sdk/source/vst/vstaudioeffect.h>

namespace Steinberg {
	namespace Vst {

		class VstProcessor : public AudioEffect
		{
		public:
			tresult PLUGIN_API initialize(FUnknown * context);
			
			tresult PLUGIN_API setBusArrangements(
				SpeakerArrangement * inputs, int32 numIns,
				SpeakerArrangement * outputs, int32 numOuts);
			
			tresult PLUGIN_API process(ProcessData & data);
			
			static FUnknown * createInstance(void *)
			{
				return (IAudioProcessor *)new VstProcessor();
			}

		};

	}
}
