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

#ifndef __mdaLeslie_H
#define __mdaLeslie_H

#include "audioeffectx.h"

#define NPARAMS  9      //number of parameters
#define NPROGS   3      //number of programs

class mdaLeslieProgram
{
public:
	mdaLeslieProgram();
private:
	friend class mdaLeslie;
    float param[NPARAMS];
	char name[24];
};

class mdaLeslie : public AudioEffectX
{
public:
	mdaLeslie(audioMasterCallback audioMaster);
	~mdaLeslie();

	virtual void process(float **inputs, float **outputs, LvzInt32 sampleFrames);
	virtual void processReplacing(float **inputs, float **outputs, LvzInt32 sampleFrames);
	virtual void setProgram(LvzInt32 program);
	virtual void setProgramName(char *name);
	virtual void getProgramName(char *name);
	virtual bool getProgramNameIndexed (LvzInt32 category, LvzInt32 index, char* name);
	virtual void setParameter(LvzInt32 index, float value);
	virtual float getParameter(LvzInt32 index);
	virtual void getParameterLabel(LvzInt32 index, char *label);
	virtual void getParameterDisplay(LvzInt32 index, char *text);
	virtual void getParameterName(LvzInt32 index, char *text);
	virtual void suspend();

	virtual bool getEffectName(char *name);
	virtual bool getVendorString(char *text);
	virtual bool getProductString(char *text);
	virtual LvzInt32 getVendorVersion() { return 1000; }

protected:
    void update();
	mdaLeslieProgram *programs;

  float filo; //crossover filter coeff
  float fbuf1, fbuf2; //filter buffers
  float twopi; //speed, target, momentum, phase, width, ampmod, freqmod...
  float hspd, hset, hmom, hphi, hwid, hlev, hdep;
  float lspd, lset, lmom, lphi, lwid, llev, gain;
  float *hbuf;  //HF delay buffer
	LvzInt32 size, hpos; //buffer length & pointer

  float chp, dchp, clp, dclp, shp, dshp, slp, dslp;
};

#endif
