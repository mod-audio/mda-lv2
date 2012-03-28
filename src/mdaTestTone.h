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

#ifndef __mdaTestTone_H
#define __mdaTestTone_H

#include "audioeffectx.h"

class mdaTestTone : public AudioEffectX
{
public:
	mdaTestTone(audioMasterCallback audioMaster);
	~mdaTestTone();

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
  virtual void iso2string(float b, char *text);
  virtual void suspend();
  void midi2string(float n, char *text);

	virtual bool getEffectName(char *name);
	virtual bool getVendorString(char *text);
	virtual bool getProductString(char *text);
	virtual LvzInt32 getVendorVersion() { return 1000; }

protected:
  int updateTx, updateRx;
  void update();

	float fParam0, fParam1, fParam2;
	float fParam3, fParam4, fParam5;
	float fParam6, fParam7;

  float thru, left, right, len;
  float zz0, zz1, zz2, zz3, zz4, zz5, phi, dphi;
  float sw, swd, swx, fscale, cal, calx;
  LvzInt32 swt;
  int mode;

	char programName[32], disp1[16], disp2[16];
};

#endif
