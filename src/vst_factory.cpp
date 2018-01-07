
#include <public.sdk/source/main/pluginfactoryvst3.h>

#include "../include/vst_fuid.hpp"
#include "../include/vst_processor.hpp"


#define MG_VST_VENDOR   "mogesystem"

#define MG_VST_URL      "http://mg32.github.io/"

#define MG_VST_EMAIL    "mailto:mogesystem@gmail.com"

#define MG_VST_VSTNAME  "mgSinusoid"

#define MG_VST_VERSION  "0"

#define MG_VST_SUBCATEGORIES Vst::PlugType::kFx


bool InitModule() { return true; }
bool DeinitModule() { return true; }


BEGIN_FACTORY_DEF(MG_VST_VENDOR, MG_VST_URL, MG_VST_EMAIL)

	// VstProcessor
	DEF_CLASS2(
		INLINE_UID_FROM_FUID(Steinberg::Vst::ProcessorUID),
		PClassInfo::kManyInstances,
		kVstAudioEffectClass,
		MG_VST_VSTNAME,
		Vst::kDistributable,
		MG_VST_SUBCATEGORIES,
		MG_VST_VERSION,
		kVstVersionString,
		Steinberg::Vst::VstProcessor::createInstance
	)

END_FACTORY
