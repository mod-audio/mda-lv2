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

#ifndef __mdaBeatBox_H
#define __mdaBeatBox_H

#include "audioeffectx.h"

class mdaBeatBox : public AudioEffectX
{
public:
	mdaBeatBox(audioMasterCallback audioMaster);
	~mdaBeatBox();

	virtual void process(float **inputs, float **outputs, LvzInt32 sampleFrames);
	virtual void processReplacing(float **inputs, float **outputs, LvzInt32 sampleFrames);
	virtual void setProgramName(char *name);
	virtual void getProgramName(char *name);
	virtual bool getProgramNameIndexed (LvzInt32 category, LvzInt32 index, char* name);
	virtual void setParameter(LvzInt32 index, float value);
	virtual float getParameter(LvzInt32 index);
	virtual void getParameterLabel(LvzInt32 index, char *label);
	virtual void getParameterDisplay(LvzInt32 index, char *text);
	virtual void getParameterName(LvzInt32 index, char *text);
	virtual void suspend();
  virtual void synth();

	virtual bool getEffectName(char *name);
	virtual bool getVendorString(char *text);
	virtual bool getProductString(char *text);
	virtual LvzInt32 getVendorVersion() { return 1000; }

protected:
	float fParam1;
  float fParam2;
  float fParam3;
  float fParam4;
  float fParam5;
  float fParam6;
  float fParam7;
	float fParam8;
  float fParam9;
  float fParam10;
  float fParam11;
  float fParam12;
  float hthr, hfil, sthr, kthr, kfil1, kfil2, mix;
  float klev, hlev, slev;
  float  ww,  wwx,  sb1,  sb2,  sf1,  sf2, sf3;
  float kww, kwwx, ksb1, ksb2, ksf1, ksf2;
  float dyne, dyna, dynr, dynm;

  float *hbuf;
  float *kbuf;
  float *sbuf, *sbuf2;
	LvzInt32 hbuflen, hbufpos, hdel;
	LvzInt32 sbuflen, sbufpos, sdel, sfx;
  LvzInt32 kbuflen, kbufpos, kdel, ksfx;
  LvzInt32 rec, recx, recpos;

	char programName[32];
};

#endif
