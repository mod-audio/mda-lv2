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

#include "mdaDegrade.h"

#include <math.h>
#include <float.h>

AudioEffect *createEffectInstance(audioMasterCallback audioMaster)
{
  return new mdaDegrade(audioMaster);
}

mdaDegrade::mdaDegrade(audioMasterCallback audioMaster)	: AudioEffectX(audioMaster, 1, 8)	// programs, parameters
{
  //inits here!
  fParam1 = (float)0.8; //clip
  fParam2 = (float)0.50; //bits
  fParam3 = (float)0.65; //rate
  fParam4 = (float)0.9; //postfilt
  fParam5 = (float)0.58; //non-lin
  fParam6 = (float)0.5; //level
  fParamRateCharacter = 0.0;
  fParamNonLinEvenOdd = 1.0;

  setNumInputs(2);
	setNumOutputs(2);
	setUniqueID("mdaDegrade");    // identify here
	DECLARE_LVZ_DEPRECATED(canMono) ();
	canProcessReplacing();
	strcpy(programName, "Degrade");

  buf0 = buf1 = buf2 = buf3 = buf4 = buf5 = buf6 = buf7 = buf8 = buf9 = 0.0f;

  setParameter(7, 0.5f);
}

bool  mdaDegrade::getProductString(char* text) { strcpy(text, "MDA Degrade"); return true; }
bool  mdaDegrade::getVendorString(char* text)  { strcpy(text, "mda"); return true; }
bool  mdaDegrade::getEffectName(char* name)    { strcpy(name, "Degrade"); return true; }

void mdaDegrade::setParameter(int32_t index, float value)
{
	float f;

  switch(index)
  {
    case 0: fParam1 = value; break;
    case 1: fParam2 = value; break;
    case 2: fParam3 = value; break;
    case 3: fParamRateCharacter = value; break; // the option for samplerate character
    case 4: fParam4 = value; break;
    case 5: fParam5 = value; break;
    case 6: fParamNonLinEvenOdd = value; break; // the option even or odd non-linearity
    case 7: fParam6 = value; break;
  }
  //calcs here
  /*
  if(fParam3>0.5) { 
    f = fParam3 - 0.5f;  mode = 1.0f; 
  } else { 
    f = 0.5f - fParam3;  mode = 0.0f; 
  }
  tn = (int)exp(18.0f * f);
  */
  // due to decoupling the mode from fParam3 the equations are substitueted by he following 6 lines:
  if(fParamRateCharacter == 1.0) {
    mode = 1.0;
  } else {
    mode = 0.0;
  }
  tn = (int)exp(18.0f * (0.5-fParam3) * 0.4);
  tcount = 1;
  clp = (float)(pow(10.0,(-1.5 + 1.5 * fParam1)) );
  fo2 = filterFreq((float)(200.0 + fParam4*19800.0));
  fi2 = (1.f-fo2); fi2=fi2*fi2; fi2=fi2*fi2;
  float _g1 = (float)(pow(2.0,2.0 + int(fParam2*12.0)));
  g2 = (float)(1.0/(2.0 * _g1));
  if(fParamRateCharacter == 1.0) g1 = -_g1/(float)tn; else g1= -_g1;
  g3 = (float)(pow(10.0,2.0*fParam6 - 1.0));
  /*
  if(fParam5>0.5) { 
    lin = (float)(pow(10.0,0.3 * (0.5 - fParam5))); 
    lin2=lin; 
  } else { 
    lin = (float)pow(10.0,0.3 * (fParam5 - 0.5)); 
    lin2=1.0; 
  }
  */
  // due to decoupling odd or even characteristics from fParam5 the equations are substitueted by he following lines:
  lin = (float)(pow(10.0, fParam5 * -0.15));
  if(fParamNonLinEvenOdd == 1.0) {
    lin2 = lin;
  } else {
    lin2 = 1.0;
  }
}

mdaDegrade::~mdaDegrade()
{
}

void mdaDegrade::suspend()
{
}

float mdaDegrade::filterFreq(float hz)
{
  float j, k, r=0.999f;

  j = r * r - 1;
  k = (float)(2.f - 2.f * r * r * cos(0.647f * hz / getSampleRate() ));
  return (float)((sqrt(k*k - 4.f*j*j) - k) / (2.f*j));
}

void mdaDegrade::setProgramName(char *name)
{
	strcpy(programName, name);
}

void mdaDegrade::getProgramName(char *name)
{
	strcpy(name, programName);
}

bool mdaDegrade::getProgramNameIndexed (int32_t category, int32_t index, char* name)
{
	if (index == 0) 
	{
	    strcpy(name, programName);
	    return true;
	}
	return false;
}

float mdaDegrade::getParameter(int32_t index)
{
	float v=0;

  switch(index)
  {
    case 0: v = fParam1; break;
    case 1: v = fParam2; break;
    case 2: v = fParam3; break;
    case 3: v = fParamRateCharacter; break;
    case 4: v = fParam4; break;
    case 5: v = fParam5; break;
    case 6: v = fParamNonLinEvenOdd; break;
    case 7: v = fParam6; break;
  }
  return v;
}

void mdaDegrade::getParameterName(int32_t index, char *label)
{
	switch(index)
  {
    case 0: strcpy(label, "Headroom"); break;
    case 1: strcpy(label, "Quant"); break;
    case 2: strcpy(label, "Rate"); break;
    case 3: strcpy(label, "Rate Char."); break;
    case 4: strcpy(label, "PostFilt"); break;
    case 5: strcpy(label, "Non-Lin"); break;
    case 6: strcpy(label, "Even Odd"); break;
    case 7: strcpy(label, "Output"); break;
  }
}

#include <stdio.h>
static void int2strng(int32_t value, char *string) { sprintf(string, "%d", value); }

void mdaDegrade::getParameterDisplay(int32_t index, char *text)
{
	switch(index)
  {
    case 0: int2strng((int32_t)(-30.0 * (1.0 - fParam1)), text); break;
    case 1: int2strng((int32_t)(4.0 + 12.0 * fParam2), text); break;
    case 2: int2strng((int32_t)(getSampleRate()/tn), text); break;
    case 3: int2strng((int32_t)(fParamRateCharacter), text); break;
    case 4: int2strng((int32_t)pow(10.0f, 2.30104f + 2.f*fParam4), text); break;
    case 5: int2strng((int32_t)(200.0 * fabs(fParam5 - 0.5)), text); break;
    case 6: int2strng((int32_t)(fParamNonLinEvenOdd), text); break;
    case 7: int2strng((int32_t)(40.0 * fParam6 - 20.0), text); break;
  }
}

void mdaDegrade::getParameterLabel(int32_t index, char *label)
{
	switch(index)
  {
    case 0: strcpy(label, "dB"); break;
    case 1: strcpy(label, "bits"); break;
    case 2: strcpy(label, "Rate"); break;
    case 3: strcpy(label, "Rate Char."); break;
    case 4: strcpy(label, "Hz"); break;
    case 5: strcpy(label, "Non lin."); break;
    case 6: strcpy(label, "Odd Even"); break;
  }
}

//--------------------------------------------------------------------------------
// process

void mdaDegrade::process(float **inputs, float **outputs, int32_t sampleFrames)
{
	float *in1 = inputs[0];
	float *in2 = inputs[1];
	float *out1 = outputs[0];
	float *out2 = outputs[1];
	float b0=buf0, c, d, l=lin, l2=lin2;
  float cl=clp, i2=fi2, o2=fo2;
  float b1=buf1, b2=buf2, b3=buf3, b4=buf4, b5=buf5;
  float b6=buf6, b7=buf7, b8=buf8, b9=buf9;
  float gi=g1, go=g2, ga=g3, m=mode;
  int n=tn, t=tcount;

  --in1;
	--in2;
	--out1;
	--out2;
	while(--sampleFrames >= 0)
	{
    c = out1[1];
		d = out2[1]; //process from here...

    b0 = (*++in1 + *++in2) + m * b0;

    if(t==n)
    {
      t=0;
      b5 = (float)(go * int(b0 * gi));
      if(b5>0)
      { b5=(float)pow(b5,l2); if(b5>cl) b5=cl;}
      else
      { b5=-(float)pow(-b5,l); if(b5<-cl) b5=-cl; }
      b0=0;
    }
    t=t+1;

    b1 = i2 * (b5 * ga) + o2 * b1;
    b2 =      b1 + o2 * b2;
    b3 =      b2 + o2 * b3;
    b4 =      b3 + o2 * b4;
    b6 = i2 * b4 + o2 * b6;
    b7 =      b6 + o2 * b7;
    b8 =      b7 + o2 * b8;
    b9 =      b8 + o2 * b9;

    c += b9; // output
		d += b9;

    *++out1 = c;
		*++out2 = d;
	}
  if(fabs(b1)<1.0e-10)
  { buf1=0.f; buf2=0.f; buf3=0.f; buf4=0.f; buf6=0.f; buf7=0.f;
    buf8=0.f; buf9=0.f; buf0=0.f; buf5=0.f; }
  else
  { buf1=b1; buf2=b2; buf3=b3; buf4=b4; buf6=b6; buf7=b7;
    buf8=b8, buf9=b9; buf0=b0; buf5=b5; tcount=t; }
}

void mdaDegrade::processReplacing(float **inputs, float **outputs, int32_t sampleFrames)
{
	float *in1 = inputs[0];
	float *in2 = inputs[1];
	float *out1 = outputs[0];
	float *out2 = outputs[1];
	float b0=buf0, l=lin, l2=lin2;
  float cl=clp, i2=fi2, o2=fo2;
  float b1=buf1, b2=buf2, b3=buf3, b4=buf4, b5=buf5;
  float b6=buf6, b7=buf7, b8=buf8, b9=buf9;
  float gi=g1, go=g2, ga=g3, m=mode;
  int n=tn, t=tcount;

	--in1;
	--in2;
	--out1;
	--out2;
	while(--sampleFrames >= 0)
	{
    b0 = (*++in1 + *++in2) + m * b0;

    if(t==n)
    {
      t=0;
      b5 = (float)(go * int(b0 * gi));
      if(b5>0)
      { b5=(float)pow(b5,l2); if(b5>cl) b5=cl;}
      else
      { b5=-(float)pow(-b5,l); if(b5<-cl) b5=-cl; }
      b0=0;
    }
    t=t+1;

    b1 = i2 * (b5 * ga) + o2 * b1;
    b2 =      b1 + o2 * b2;
    b3 =      b2 + o2 * b3;
    b4 =      b3 + o2 * b4;
    b6 = i2 * b4 + o2 * b6;
    b7 =      b6 + o2 * b7;
    b8 =      b7 + o2 * b8;
    b9 =      b8 + o2 * b9;

		*++out1 = b9;
		*++out2 = b9;
	}
  if(fabs(b1)<1.0e-10)
  { buf1=0.f; buf2=0.f; buf3=0.f; buf4=0.f; buf6=0.f; buf7=0.f;
    buf8=0.f; buf9=0.f; buf0=0.f; buf5=0.f; }
  else
  { buf1=b1; buf2=b2; buf3=b3; buf4=b4; buf6=b6; buf7=b7;
    buf8=b8, buf9=b9; buf0=b0; buf5=b5; tcount=t; }
}
