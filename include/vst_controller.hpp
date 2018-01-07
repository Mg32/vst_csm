
#pragma once

#include "vst_tags.hpp"

#include <public.sdk/source/vst/vsteditcontroller.h>

namespace Steinberg {
	namespace Vst {

		class VstController : public EditController
		{
		public:

			VstController()
			{
			}

			tresult PLUGIN_API initialize(FUnknown * context);

			static FUnknown * createInstance(void *)
			{
				return (IEditController *)new VstController();
			}

		private:

		};

	}
}
