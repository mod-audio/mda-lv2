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

#include "audioeffectx.h"

#define NPARAMS            4  ///number of parameters
#define NPROGS             1  ///number of programs
#define BUF_MAX         1600
#define ORD_MAX           50
#define TWO_PI     6.28318530717958647692528676655901f


class mdaTalkBoxProgram
{
public:
  mdaTalkBoxProgram();
  ~mdaTalkBoxProgram() {}

private:
  friend class mdaTalkBox;
  float param[NPARAMS];
  char name[24];
};


class mdaTalkBox : public AudioEffectX
{
public:
  mdaTalkBox(audioMasterCallback audioMaster);
  ~mdaTalkBox();

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
  virtual void  resume();

	virtual bool getEffectName(char *name);
	virtual bool getVendorString(char *text);
	virtual bool getProductString(char *text);
	virtual LvzInt32 getVendorVersion() { return 1000; }

protected:
  mdaTalkBoxProgram *programs;

  void lpc(float *buf, float *car, LvzInt32 n, LvzInt32 o);
  void lpc_durbin(float *r, int p, float *k, float *g);

  ///global internal variables
  float *car0, *car1;
  float *window;
  float *buf0, *buf1;
  
  float emphasis;
  LvzInt32 K, N, O, pos, swap;
  float wet, dry, FX;

  float d0, d1, d2, d3, d4;
  float u0, u1, u2, u3, u4;
};


