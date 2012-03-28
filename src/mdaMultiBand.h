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

#ifndef __mdaMultiBand_H
#define __mdaMultiBand_H

#include "audioeffectx.h"

class mdaMultiBand : public AudioEffectX
{
public:
	mdaMultiBand(audioMasterCallback audioMaster);
	~mdaMultiBand();

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

	virtual bool getEffectName(char *name);
	virtual bool getVendorString(char *text);
	virtual bool getProductString(char *text);
	virtual LvzInt32 getVendorVersion() { return 1000; }

protected:
	float fParam1, fParam2, fParam3, fParam4;
	float fParam5, fParam6, fParam7, fParam8;
  float fParam9, fParam10, fParam11, fParam12, fParam13;
  float gain1, driv1, att1, rel1, trim1;
  float gain2, driv2, att2, rel2, trim2;
  float gain3, driv3, att3, rel3, trim3;
  float fi1, fb1, fo1, fi2, fb2, fo2, fb3, slev;
  int mswap;

	char programName[32];
};

#endif
