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
	}

	DEFINE_ID(SequencerTrack)
	namespace SequencerTrackProps
	{
		// DEFINE_ID(sample_source)
	}

#undef DEFINE_ID
}