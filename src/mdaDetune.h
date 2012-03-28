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

#define NPARAMS  4  ///number of parameters
#define NPROGS   4  ///number of programs
#define BUFMAX   4096

#ifndef __mdaDetune_H
#define __mdaDetune_H

#include "audioeffectx.h"

struct mdaDetuneProgram
{
  friend class mdaDetune;
  float param[NPARAMS];
  char name[32];
};


class mdaDetune : public AudioEffectX
{
public:
  mdaDetune(audioMasterCallback audioMaster);

  virtual void  process(float **inputs, float **outputs, LvzInt32 sampleFrames);
  virtual void  processReplacing(float **inputs, float **outputs, LvzInt32 sampleFrames);
  virtual void  setProgram(LvzInt32 program);
  virtual void  setProgramName(char *name);
  virtual void  getProgramName(char *name);
  virtual bool getProgramNameIndexed (LvzInt32 category, LvzInt32 index, char* name);
  virtual void  setParameter(LvzInt32 index, float value);
  virtual float getParameter(LvzInt32 index);
  virtual void  getParameterLabel(LvzInt32 index, char *label);
  virtual void  getParameterDisplay(LvzInt32 index, char *text);
  virtual void  getParameterName(LvzInt32 index, char *text);
  virtual void  suspend();

	virtual bool getEffectName(char *name);
	virtual bool getVendorString(char *text);
	virtual bool getProductString(char *text);
	virtual LvzInt32 getVendorVersion() { return 1000; }

protected:
	mdaDetuneProgram programs[NPROGS];
	float buf[BUFMAX];
	float win[BUFMAX];

  ///global internal variables
  LvzInt32  buflen;           //buffer length
  float bufres;           //buffer resolution display
  float semi;             //detune display
  LvzInt32  pos0;             //buffer input
  float pos1, dpos1;      //buffer output, rate
  float pos2, dpos2;      //downwards shift
  float wet, dry;         //ouput levels
};

#endif
