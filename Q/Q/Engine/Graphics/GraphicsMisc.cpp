#include "GraphicsMisc.h"
namespace q_engine
{
	ZDepth::ZDepth(float value)
		:value((static_cast<float>(value) + static_cast<float>(MAX_VALUE)) / static_cast<float>(2 * MAX_VALUE))
	{
        if (value > MAX_VALUE || value < -MAX_VALUE)
        {
            throw std::runtime_error("zDepth's value should be between " + std::to_string(-MAX_VALUE) +
                " and " + std::to_string(MAX_VALUE) + " (inclusive), got " + std::to_string(value));
        }
	}

	void RegisterZDepthType(sol::state& state)
	{
        auto zdepth = state.new_usertype<ZDepth>("ZDepth", sol::constructors<ZDepth(float)>()); 
		zdepth.set("value", &ZDepth::value);
	}

	void RegisterEFrameOfReferenceEnum(sol::state& state)
	{
        std::initializer_list<std::pair<sol::string_view, EFrameOfReference>> FrameOfReferences =
        {
            {"LEFT_BOTTOM", EFrameOfReference::LEFT_BOTTOM},
            {"BOTTOM_LEFT", EFrameOfReference::BOTTOM_LEFT},
            {"LEFT_TOP", EFrameOfReference::LEFT_TOP},
            {"TOP_LEFT", EFrameOfReference::TOP_LEFT},
            {"CENTER_BOTTOM", EFrameOfReference::CENTER_BOTTOM},
            {"BOTTOM_CENTER", EFrameOfReference::BOTTOM_CENTER},
            {"CENTER", EFrameOfReference::CENTER},
            {"MIDDLE_CENTER", EFrameOfReference::MIDDLE_CENTER},
            {"CENTER_MIDDLE", EFrameOfReference::CENTER_MIDDLE},
            {"UNCHANGED", EFrameOfReference::UNCHANGED},
        };

        state.new_enum<EFrameOfReference, false>("EFrameOfReference", FrameOfReferences);
	}
}
