#pragma once

#include "JuceHeader.h"

/*
 * Each Identifier represents a named (identified) property of the ValueTree
 * which represents the application. A ValueTree is described in the JUCE docs,
 * and it's used here the Tree which represents the strucutre of the app.
 * Each Identifier is defined and used so that the ValueTree can reference what
 * properties are changed.
 * Much of my reference is David Rowland and the harmonaize project on Github.
 * This class will grow as my ValueTree implementation grows.
 */

namespace IDs
{
#define DECLARE_ID(name) extern const Identifier name;

	DECLARE_ID(Ample)
	namespace AmpleInfo
	{
		DECLARE_ID(name)
	}

	DECLARE_ID(Sequencer)
	namespace SequencerProps
	{
		DECLARE_ID(tempo)
		DECLARE_ID(step_index)
	}
	
	DECLARE_ID(SequencerTrack)
	namespace SequencerTrackProps
	{
	}
	
	DECLARE_ID(SampleSource)
	namespace SampleSourceProps
	{
		DECLARE_ID(file_path);
		DECLARE_ID(resources_directory)
	}

#undef DECLARE_ID
}