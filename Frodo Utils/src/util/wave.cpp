#include "wave.h"
#include "fileutils.h"
#include "fdutils.h"
#include "vfs/vfs.h"
#include <core/log.h>

#define CHUNK_RIFF_ID ((uint32)('R' | (uint32)'I' << 8 | (uint32)'F' << 16 | (uint32)'F' << 24))
#define CHUNK_RIFF_FORMAT ((uint32)('W' | (uint32)'A' << 8 | (uint32)'V' << 16 | (uint32)'E' << 24))
#define CHUNK_FORMAT_ID ((uint32)('f' | (uint32)'m' << 8 | (uint32)'t' << 16 | (uint32)' ' << 24))
#define CHUNK_DATA_ID ((uint32)('d' | (uint32)'a' << 8 | (uint32)'t' << 16 | (uint32)'a' << 24))

namespace FD {

inline bool VerifyWave(WAVE* w) {
	if (w->riff.ChunkID != CHUNK_RIFF_ID || w->riff.Format != CHUNK_RIFF_FORMAT) { //RIFF && WAVE
		FD_FATAL("Invalid RIFF Chunk");
		return false;
	}

	if (w->format.SubChunkID != CHUNK_FORMAT_ID) { //fmt
		FD_FATAL("Invalid FORMAT Chunk");
		return false;
	}

	if (w->format.AudioFormat != 1) { //is not pcm
		FD_FATAL("Compression not supported");
		return false;
	}

	if (w->data.SubChunkID != CHUNK_DATA_ID) { //data
		FD_FATAL("Invalid DATA Chunk");
		return false;
	}

	return true;
}

WAVE* FDReadWaveFile(const String& filename) {

	uint_t waveSize;

	byte* waveFile = VFS::Get()->ReadFile(filename, &waveSize);

	WAVE* wave = new WAVE;

	memcpy(wave, waveFile, (uint_t)OFFSETOF(WAVE, audioData));

	if (!VerifyWave(wave)) {
		FD_FATAL("Couldn't verify WAVE header: %s", *filename);
		delete[] waveFile;
		delete wave;
		return nullptr;
	}
	
	wave->audioData = new byte[wave->data.SubChunkSize];

	memcpy(wave->audioData, waveFile + (uint_t)OFFSETOF(WAVE, audioData), wave->data.SubChunkSize);

	delete[] waveFile;

	return wave;
}
	

}