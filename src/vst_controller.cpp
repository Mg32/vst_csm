
#include "../include/vst_controller.hpp"

namespace Steinberg {
	namespace Vst {

		tresult PLUGIN_API VstController::initialize(FUnknown * context)
		{
			tresult result = EditController::initialize(context);
			if (result == kResultTrue)
			{
				parameters.addParameter(
					STR16("Bypass"), 0,
					1, 0,
					ParameterInfo::kIsBypass,
					TAG_PARAM_BYPASS
				);
			}

			return result;
		}

		tresult PLUGIN_API VstController::setComponentState(IBStream * state)
		{
			tresult result;

			int32 stateBypass;
			result = state->read(&stateBypass, sizeof(stateBypass));
			if (result != kResultOk)
				return kResultFalse;

			setParamNormalized(TAG_PARAM_BYPASS, (stateBypass > 0) ? 1 : 0);

			return kResultOk;
		}

	}
}
