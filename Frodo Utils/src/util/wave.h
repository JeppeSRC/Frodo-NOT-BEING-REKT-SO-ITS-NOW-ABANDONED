#pragma once

#include <fdu.h>
#include <util/string.h>

namespace FD {

struct WAVE_RIFF {
	uint32 ChunkID;
	uint32 ChunkSize;
	uint32 Format;
};

struct WAVE_FORMAT {
	uint32 SubChunkID;
	uint32 SubChunkSize;
	uint16 AudioFormat;
	uint16 NumChannels;
	uint32 SampleRate;
	uint32 ByteRate;
	uint16 BlockAlign;
	uint16 BitsPerSample;
};

struct WAVE_DATA {
	uint32 SubChunkID;
	uint32 SubChunkSize;
};

struct WAVE {
	WAVE_RIFF riff;
	WAVE_FORMAT format;
	WAVE_DATA data;
	byte* audioData;

	~WAVE() { delete[] audioData; }
};


FDAPI WAVE* FDReadWaveFile(const String& filename);




}
