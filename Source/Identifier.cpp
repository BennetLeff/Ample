#include "Identifier.h"

namespace IDs
{
#define DEFINE_ID(name) const Identifier name (#name);

	DEFINE_ID(Ample)
	namespace AmpleInfo
	{
		DEFINE_ID(name)
	}

	DEFINE_ID(Sequencer)
	namespace SequencerProps
	{
		DEFINE_ID(tempo)
		DEFINE_ID(step_index)
	}

	DEFINE_ID(SequencerTrack)
	namespace SequencerTrackProps
	{
	}

#undef DEFINE_ID
}