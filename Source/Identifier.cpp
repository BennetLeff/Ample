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

	DEFINE_ID(SampleEditorScene)
	namespace SampleEditorSceneProps
	{
		DEFINE_ID(sample_file_path)
		DEFINE_ID(start_position)
		DEFINE_ID(end_position)
	}

	DEFINE_ID(SampleSource)
	namespace SampleSourceProps
	{
		DEFINE_ID(file_path)
		DEFINE_ID(start_position)
		DEFINE_ID(end_position)
		DEFINE_ID(resources_directory)
	}


#undef DEFINE_ID
}