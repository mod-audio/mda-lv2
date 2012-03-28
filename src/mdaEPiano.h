/*
  Copyright 2008-2011 David Robillard <http://drobilla.net>
  Copyright 1999-2000 Paul Kellett (Maxim Digital Audio)

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

#ifndef __mdaEPiano__
#define __mdaEPiano__

#include <string.h>

#include "audioeffectx.h"

#define NPARAMS 12       //number of parameters
#define NPROGS   8       //number of programs
#define NOUTS    2       //number of outputs
#define NVOICES 32       //max polyphony
#define SUSTAIN 128
#define SILENCE 0.0001f  //voice choking
#define WAVELEN 422414   //wave data bytes

class mdaEPianoProgram
{
  friend class mdaEPiano;
private:
  float param[NPARAMS];
  char  name[24];
};


struct VOICE  //voice state
{
  LvzInt32  delta;  //sample playback
  LvzInt32  frac;
  LvzInt32  pos;
  LvzInt32  end;
  LvzInt32  loop;

  float env;  //envelope
  float dec;

  float f0;   //first-order LPF
  float f1;
  float ff;

  float outl;
  float outr;
  LvzInt32  note; //remember what note triggered this
};


struct KGRP  //keygroup
{
  LvzInt32  root;  //MIDI root note
  LvzInt32  high;  //highest note
  LvzInt32  pos;
  LvzInt32  end;
  LvzInt32  loop;
};

class mdaEPiano : public AudioEffectX
{
public:
	mdaEPiano(audioMasterCallback audioMaster);
	~mdaEPiano();

	virtual void process(float **inputs, float **outputs, LvzInt32 sampleframes);
	virtual void processReplacing(float **inputs, float **outputs, LvzInt32 sampleframes);
	virtual LvzInt32 processEvents(LvzEvents* events);

	virtual void setProgram(LvzInt32 program);
	virtual void setProgramName(char *name);
	virtual void getProgramName(char *name);
	virtual void setParameter(LvzInt32 index, float value);
	virtual float getParameter(LvzInt32 index);
	virtual void getParameterLabel(LvzInt32 index, char *label);
	virtual void getParameterDisplay(LvzInt32 index, char *text);
	virtual void getParameterName(LvzInt32 index, char *text);
	virtual void setBlockSize(LvzInt32 blockSize);
  virtual void resume();

	virtual bool getOutputProperties (LvzInt32 index, LvzPinProperties* properties);
	virtual bool getProgramNameIndexed (LvzInt32 category, LvzInt32 index, char* text);
	virtual bool copyProgram (LvzInt32 destination);
	virtual bool getEffectName (char* name);
	virtual bool getVendorString (char* text);
	virtual bool getProductString (char* text);
	virtual LvzInt32 getVendorVersion () {return 1;}
	virtual LvzInt32 canDo (char* text);

	virtual LvzInt32 getNumMidiInputChannels ()  { return 1; }

  LvzInt32 guiUpdate;
  void guiGetDisplay(LvzInt32 index, char *label);

private:
	void update();  //my parameter update
  void noteOn(LvzInt32 note, LvzInt32 velocity);
  void fillpatch(LvzInt32 p, const char *name, float p0, float p1, float p2, float p3, float p4,
                 float p5, float p6, float p7, float p8, float p9, float p10,float p11);

  mdaEPianoProgram* programs;
  float Fs, iFs;

  #define EVENTBUFFER 120
  #define EVENTS_DONE 99999999
  LvzInt32 notes[EVENTBUFFER + 8];  //list of delta|note|velocity for current block

  ///global internal variables
  KGRP  kgrp[34];
  VOICE voice[NVOICES];
  LvzInt32  activevoices, poly;
  short *waves;
  float width;
  LvzInt32  size, sustain;
  float lfo0, lfo1, dlfo, lmod, rmod;
  float treb, tfrq, tl, tr;
  float tune, fine, random, stretch, overdrive;
  float muff, muffvel, sizevel, velsens, volume, modwhl;
};

#endif
