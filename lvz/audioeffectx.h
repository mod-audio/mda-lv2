/*
  LVZ - A C++ interface for writing LV2 plugins.
  Copyright 2008-2011 David Robillard <http://drobilla.net>

  This is free software: you can redistribute it and/or modify it
  under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License,
  or (at your option) any later version.

  This software is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this software. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef LVZ_AUDIOEFFECTX_H
#define LVZ_AUDIOEFFECTX_H

#include <stdint.h>
#include <string.h>

#include "AEffect.h"

typedef int16_t LvzInt16;
typedef int32_t LvzInt32;

typedef int (*audioMasterCallback)(int, int ver, int, int, int, int);

class AEffEditor;

struct LvzFileSelect {
	int reserved;
	char* returnPath;
	size_t sizeReturnPath;
	char** returnMultiplePaths;
	long nbReturnPath;
};

enum LvzPinFlags {
	kLvzPinIsActive = 1<<0,
	kLvzPinIsStereo = 1<<1
};

struct LvzPinProperties {
	LvzPinProperties() : label(NULL), flags(0) {}
	char* label;
	int   flags;
};

enum LvzOpCodes {
	effEditClose,
	effEditOpen
};

enum LvzEventTypes {
	kLvzMidiType = 0
};

struct LvzEvent {
	int type;
};

struct LvzMidiEvent : public LvzEvent {
	char* midiData;
	LvzInt32 deltaFrames;
};

struct LvzEvents {
	LvzInt32 numEvents;
	LvzEvent** events;
};

#define DECLARE_LVZ_DEPRECATED(x) x

class AudioEffect {
public:
	AudioEffect() : editor(NULL) {}
	virtual ~AudioEffect() {}

	virtual void  setParameter(LvzInt32 index, float value) = 0;
	virtual void  setParameterAutomated(LvzInt32 index, float value) {}
	virtual float getParameter(LvzInt32 index)              = 0;

	void setEditor(AEffEditor* e) { editor = e; }
	virtual void masterIdle()                {}
protected:
	AEffEditor* editor;
};


class AudioEffectX : public AudioEffect {
public:
	AudioEffectX(audioMasterCallback audioMaster, LvzInt32 progs, LvzInt32 params)
		: URI("NIL")
		, uniqueID("NIL")
		, sampleRate(44100)
		, curProgram(0)
		, numInputs(0)
		, numOutputs(0)
		, numParams(params)
		, numPrograms(progs)
	{
		cEffect.flags = 0;
	}

	virtual void process         (float **inputs, float **outputs, LvzInt32 nframes) = 0;
	virtual void processReplacing(float **inputs, float **outputs, LvzInt32 nframes) = 0;

	virtual LvzInt32 processEvents(LvzEvents* ev) { return 0; }

	virtual const char*  getURI()           { return URI; }
	virtual const char*  getUniqueID()      { return uniqueID; }
	virtual float        getSampleRate()    { return sampleRate; }
	virtual LvzInt32     getNumInputs()     { return numInputs; }
	virtual LvzInt32     getNumOutputs()    { return numOutputs; }
	virtual LvzInt32     getNumParameters() { return numParams; }

	virtual void  getParameterName(LvzInt32 index, char *label) = 0;
	virtual bool  getProductString(char* text)                  = 0;

	virtual bool canHostDo(const char* act) { return false; }
	virtual void canMono()                  {}
	virtual void canProcessReplacing()      {}
	virtual void isSynth()                  {}
	virtual void wantEvents()               {}

	virtual void setBlockSize(LvzInt32 size) {}
	virtual void setNumInputs(LvzInt32 num)  { numInputs = num; }
	virtual void setNumOutputs(LvzInt32 num) { numOutputs = num; }
	virtual void setSampleRate(float rate)   { sampleRate = rate; }
	virtual void setProgram(LvzInt32 prog)   { curProgram = prog; }
	virtual void setURI(const char* uri)     { URI = uri; }
	virtual void setUniqueID(const char* id) { uniqueID = id; }
	virtual void suspend()                   {}
	virtual void beginEdit(LvzInt32 index)   {}
	virtual void endEdit(LvzInt32 index)     {}

	virtual bool openFileSelector (LvzFileSelect* sel)  { return false; }
	virtual bool closeFileSelector (LvzFileSelect* sel) { return false; }

	virtual long dispatcher(long opCode, long index, long value, void *ptr, float opt) {
		return 0;
	}

protected:
	const char* URI;
	const char* uniqueID;
	float       sampleRate;
	LvzInt32    curProgram;
	LvzInt32    numInputs;
	LvzInt32    numOutputs;
	LvzInt32    numParams;
	LvzInt32    numPrograms;
	AEffect     cEffect;
};

#endif // LVZ_AUDIOEFFECTX_H

