/*
  LVZ - An ugly C++ interface for writing LV2 plugins.
  Copyright 2008-2012 David Robillard <http://drobilla.net>

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

#ifndef PLUGIN_CLASS
#error "This file requires PLUGIN_CLASS to be defined"
#endif
#ifndef URI_PREFIX
#error "This file requires URI_PREFIX to be defined"
#endif
#ifndef PLUGIN_URI_SUFFIX
#error "This file requires PLUGIN_URI_SUFFIX to be defined"
#endif
#ifndef PLUGIN_HEADER
#error "This file requires PLUGIN_HEADER to be defined"
#endif

#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "audioeffectx.h"
#include "lv2.h"
#include "lv2_programs.h"
#include "lv2/lv2plug.in/ns/ext/atom/atom.h"
#include "lv2/lv2plug.in/ns/ext/midi/midi.h"
#include "lv2/lv2plug.in/ns/ext/urid/urid.h"
#include PLUGIN_HEADER


// Essa função trabalha como uma regra de 3 para adaptar os valores recebidos na instância do plugin
// (que encontram-se determinados no ttl)
// para valores de 0 a 1, pois o código do mda foi projetado para trabalhar com esses valores


float translateParameter(PLUGIN_CLASS* effect,int port,float value,bool inverted) {    
    if(strcmp(effect->getUniqueID(), "mdaAmb") == 0) {
        switch(port) {
            case 0:
            return inverted ? value*10.f : value/10.f;
            case 1:
            return inverted ? value*100.f : value/100.f;
            case 2:
            return inverted ? value*100.f : value/100.f;
            case 3:
            return inverted ? value*40.f-20.f : (value+20.f)/(40.f);
        }
    }
    else if(strcmp(effect->getUniqueID(), "mdaBand") == 0) {
        switch(port) {
            case(0):
            return inverted ? value*(3) : value/(3);
            case(1):
            return inverted ? value*(1020 - 88) + 88 : (value - 88)/(1020 - 88);
            case(2):
            return inverted ? value*(19606 - 112) + 112 : (value - 112)/(19606 - 112);
            case(3):
            return inverted ? value*(60) : value/(60);
            case(4):
            return inverted ? value*(60) : value/(60);
            case(5):
            return inverted ? value*(60) : value/(60);
            case(6):
            return inverted ? value*40 + 20 : (value - 20)/(40);
            case(7):
            return inverted ? value*40 + 20 : (value - 20)/(40);
            case(8):
            return inverted ? value*40 + 20 : (value - 20)/(40);
            case(9):
            return value;
        }
    }
    else if(strcmp(effect->getUniqueID(), "mdaBBox") == 0) {
        switch(port) {
            case(0):
            return inverted ? value*40 - 40 : (value + 40)/(40);
            case(1):
            return inverted ? value*200 + 40 : (value - 40)/(200);
            case(2):
            return inverted ? value*36 - 24 : (value + 24)/(36);
            case(3):
            return inverted ? value*40 - 40 : (value + 40)/(40);
            case(4):
            return inverted ? value*3472 + 22 : (value - 22)/(3472);
            case(5):
            return inverted ? value*36 - 24 : (value + 24)/(36);
            case(6):
            return inverted ? value*40 - 40 : (value + 40)/(40);
            case(7):
            return inverted ? value*3472 + 22 : (value - 22)/(3472);
            case(8):
            return inverted ? value*36 - 24 : (value + 24)/(36);
            case(9):
            return inverted ? value*99 + 1 : (value - 1)/(99);
            case(10):
            return inverted ? value*4 : value/4;
            case(11):
            return inverted ? value*45 - 45 : (value + 45)/(45);
        }
    }
    else if(strcmp(effect->getUniqueID(), "mdaCombo") == 0) {
        switch(port) {
            case(0):
            return inverted ? value*6 : value/6;
            case(1):
            return inverted ? value*200 - 100 : (value + 100)/(200);
            case(2):
            return inverted ? value*200 - 100 : (value + 100)/(200);
            case(3):
            return inverted ? value*40 - 20 : (value + 20)/(40);
            case(4):
            return value;
            case(5):
            return inverted ? value*100 : value/(100);
            case(6):
            return inverted ? value*100 : value/(100);
        }

    }
    else if(strcmp(effect->getUniqueID(), "mdaDeEss") == 0) {
        switch(port) {
            case(0):
            return inverted ? value*60 - 60 : (value + 60)/(60);
            case(1):
            return inverted ? value*11000 + 1000 : (value - 1000)/(11000);
            case(2):
            return inverted ? value*40 - 20 : (value + 20)/(40);
        }
    }
    else if(strcmp(effect->getUniqueID(), "mdaDegrade") == 0) {
        switch(port) {
            case(0):
            return inverted ? value*30-30 : (30+value)/30;
            case(1):
            return inverted ? value*12+4 : (value-4)/12;
            case(2):
            return inverted ? value*2.0*43200+4800 : (value-4800)/43200*0.5;
            case(3):
            return inverted ? value : value;
            case(4):
            return inverted ? value*19800+200 : (value-200)/19800;
            case(5):
            return inverted ? value : value;
            case(6):
            return inverted ? value : value;
            case(7):
            return inverted ? value*40-20 : (value+20)/40;
        }
    }
    else if(strcmp(effect->getUniqueID(), "mdaDelay") == 0) {
        switch(port) {
            case(0):
            return inverted ? value*330.f : value/330.f;
            case(1):
            return inverted ? value*2.f : value/2.f;
            case(2):
            return inverted ? value : value;
            case(3):
            return inverted ? value*100.f : value/100.f;
            case(4):
            return inverted ? value*2.f-1.f : (value+1.f)/2.f;
            case(5):
            return inverted ? value*100.f : value/100.f;
            case(6):
            return inverted ? value*18.f-12.f : (value+12.f)/18.f;
        }
    }
    else if(strcmp(effect->getUniqueID(), "mdaDetune") == 0) {
        switch(port) {
            case(0):
            return inverted ? value*300 : value/(300);
            case(1):
            return inverted ? value*99 : value/(99);
            case(2):
            return inverted ? value*40 - 20 : (value + 20)/(40);
            case(3):
            return inverted ? value*87.1 + 5.8 : (value - 5.8)/(87.1);
        }
    }
    else if(strcmp(effect->getUniqueID(), "mdaDither") == 0) {
        switch(port) {
            case(0):
            return inverted ? value*16 + 8 : (value - 8)/(16);
            case(1):
            return inverted ? value*3 : value/(3);
            case(2):
            return inverted ? value*4 : value/(4);
            case(3):
            return inverted ? value*4 - 2 : (value + 2)/(4);
            case(4):
            return inverted ? (40.0*log(value)/log(10)) + 1 : 1 - pow(10.0, (value/40.0));
        }
    }
    else if(strcmp(effect->getUniqueID(), "mdaDubDelay") == 0) {
        switch(port) {
            case(0):
            return inverted ? value*7500 : value/7500;
            case(1):
            return inverted ? value*220 - 110 : (value + 110)/(220);
            case(2):
            return inverted ? value*200 - 100 : (value + 100)/(200);
            case(3):
            return inverted ? value*100 : value/(100);
            case(4):
            return inverted ? (value*99.9 + 0.1) : ((value - 0.10)/99.9);
            case(5):
            return inverted ? value*100 : value/(100);
            case(6):
            return inverted ? value*18 - 12 : (value + 12)/(18);
        }
    }
    else if(strcmp(effect->getUniqueID(), "mdaDX10") == 0) {
        /* 
        Scaling process is convoluted but here is an explanation, 
        but just plugging through a bunch of values would make the presets not work:
        
        All envelope time based parameters are implemented as a recursive equation within the synth.
        This means that at each sample it calculated something along the lines of: env_volume = env_volume * 0.999...
        Here the 0.999... parameter is a decayparameter which will be explained later, for now let's call it dec.
        The volume over time can then be found as: amp = dec^[n_samples].
        Thus the decay time can be found when subsituting a "silence ratio" for amp, in this case -60db -> 0.001.
        This gives n_samples = log_{dec}(amp) or in c++ terms: log(0.001)/log(dec). (as log_a(b) = log(b)/log(a)) 
        
        Now the host provides the value as a linear interpolation from min to max, no matter if you specify it as logarithmic.
        F.Y.I. the scaling they use for logarithmic is min*e^(x * ln(max/min)) where x = [0,1].
        The different time parameters have different equations for how they calculate the parameters:
        - Carrier   Attack  = exp( -(1/samplerate) * exp(8-8x) )
        - Carrier   Decay   = exp( -(1/samplerate) * exp(5-8x) )
        - Carrier   Release = exp( -(1/samplerate) * exp(5-5x) )
        - Modulator Decay   = exp( -(1/samplerate) * exp(6-7x) )
        - Modulator Release = exp( -(1/samplerate) * exp(5-8x) )
        However these map exactly to the way the logarithmic parameter mapping in the host works.
        Given that the output needs to be scaled the same way we can solve the equation for x given a value between min and max:
        value = min*e^(x * ln(max/min)) -> x = ln(value/min)/ln(max/min). 
        Put this value in the equation and it should return the correct value for x for the time parameter.
        However some parameters have insanely long times so there is some additional stretching going on for the last ten percent of the range.
        */
        switch(port) {
            case(0):
            return inverted ? 2.5*exp(value * log(4000.f/2.5)) : log(value/2.5)/log(4000.f/2.5);
            //return inverted ? value*100 : value/(100);
            case(1):
            /* 
            carrier decay times range from ~45ms to ~135 seconds 
            so in order to make it useful some stretching is applied to the value input for 6s to 7s to make it stretch
            stretching should happen from 6000ms to 7000ms value input
            log(6000/46.5)/log(7000/46.5) = ~0.97
            inputting 0.97 * 0.625 in the decay function gives release times of ~6000ms
            from there scale value up to the extreme for [0.97, 1.0]
            */
            if (inverted) {
                // inverting the stretching done
                if (value < 0.60625){ // = 0.625*0.97
                    value = value*1.6;
                } else {
                    value = (value+12.125)/13.125;
                }
                return 46.5*exp(value * log(7000.f/46.5));
            } else {
                // stretching the end of the range
                value = log(value/46.5)/log(7000.f/46.5);
                if (value < 0.97) {
                    return 0.625 * value;
                } else {
                    return 0.625 * value + (1.f-0.625)/0.03 * (value - 0.97);
                }
            }
            //return inverted ? value*100 : value/(100);
            case(2):
            return inverted ? 46.5*exp(value * log(7000.f/46.5)) : log(value/46.5)/log(7000.f/46.5);
            //return inverted ? value*100 : value/(100);
            case(3):
            // compensate for course [0,1] squaring in the update function without breaking the presets
            if (inverted) {
                value = value * value * 40;
            } else {
                value = sqrt(value/40.f);
            }
            return value;
            case(4):
            return inverted ? value*0.750 : value/(0.750);
            case(5):
            return inverted ? value*100 : value/(100);
            case(6):
            // same stretching as the carrier decay except the range is a bit different as it runs from ~17ms to ~18 seconds
            // this gives the following equations:
            // log(6000/17)/log(7000/17) = ~0.975
            // inputting 0.975 * 0.86 in the decay function gives release times of ~6000ms
            if (inverted) {
                // inverting the stretching done
                if (value < 0.8385){ // = 0.86*0.975
                    value = value/0.86;
                } else {
                    value = (value+5.46)/6.46;
                }
                return 17.f*exp(value * log(7000.f/17.f));
            } else {
                // stretching the end of the range
                value = log(value/17.f)/log(7000.f/17.f);
                if (value < 0.975) {
                    return 0.86 * value;
                } else {
                    return 0.86 * value + (1.f-0.86)/0.025 * (value - 0.975);
                }
            }
            //return inverted ? 17.f*exp(value * log(7000.f/17.f)) : log(value/17.f)/log(7000.f/17.f);
            //return inverted ? value*100 : value/(100);
            case(7):
            return inverted ? value*100 : value/(100);
            case(8):
            // same stretching as carrier decay (case 1)
            if (inverted) {
                // inverting the stretching done
                if (value < 0.60625){ // = 0.625*0.97
                    value = value*1.6;
                } else {
                    value = (value+12.125)/13.125;
                }
                return 46.5*exp(value * log(7000.f/46.5));
            } else {
                // stretching the end of the range
                value = log(value/46.5)/log(7000.f/46.5);
                if (value < 0.97) {
                    return 0.625 * value;
                } else {
                    return 0.625 * value + (1.f-0.625)/0.03 * (value - 0.97);
                }
            }
            //return inverted ? value*100 : value/(100);
            case(9):
            return inverted ? value*100 : value/(100);
            case(10):
            return inverted ? value*100 : value/(100);
            case(11):
            return inverted ? value*6 - 3 : (value + 3)/(6);
            case(12):
            return inverted ? value*200 - 100 : (value + 100)/(200);
            case(13):
            return inverted ? value*100 : value/(100);
            case(14):
            return inverted ? value*100 : value/(100);
            case(15):
            return inverted ? value*25 : value/(25);
        }
    }
    else if(strcmp(effect->getUniqueID(), "mdaDynamics") == 0) {
        switch(port) {
            case(0):
            return inverted ? value*40 - 40 : (value + 40)/(40);
            case(1):
            return inverted ? value*17.5 - 17 + 1 : 1 - ((value + 17)/(17.5));
            case(2):
            return inverted ? value*40 : value/(40);
            case(3):
            return inverted ? value*1569 + 2 : (value - 2)/(1569);
            case(4):
            return inverted ? value*1570 + 1 : (value - 1)/(1570);
            case(5):
            return inverted ? value*30 - 20 : (value + 20)/(30);
            case(6):
            return inverted ? value*60 - 60 : (value + 60)/60;
            case(7):
            return inverted ? value*15777 + 5 : (value - 5)/(15777);
            case(8):
            return inverted ? value*17375 + 9 : (value - 9)/(17375);
            case(9):
            return inverted ? value*100 : value/(100);
        }
    }
    else if(strcmp(effect->getUniqueID(), "mdaEPiano") == 0) {
        /*
        decay and release times (in seconds) are found by solving their recursive equations:
        dec = exp(-exp(-1.0 + 0.03 * note - 2.0 * param[0]))
        rel = exp(-exp(6.0  + 0.01 * note - 5.0 * param[1]))
        given that they decay as env = env * dec where dec is an amp ratio
        here note 60 is chosen as the calibration and silence for -60dB -> 0.001
        solving for amp = 0.001:
        ( exp(-exp(-1+1.8-2*param[0]) ) ^ x = 0.001
        then solving for x (= decay/release time):
        log(0.001)/log( exp( -exp( 0.8-2*value ) ) )
        or solving for param given a time (=non-inverted):
        0.4 - 0.5*log(-log(exp(log(0.001)/y))) = value
        */
        switch(port) {
            case(0):
            return inverted ? log(0.001)/-exp(0.8-2.0*value) : 0.4 - 0.5*log(-log(0.001)/value);
            //return inverted ? log(0.001)/-exp(0.8-2.0*((value-3.f)/20.f)) : 0.4 - 0.5*log(-log(0.001)/value);
            //return inverted ? value*100 : value/(100);
            case(1):
            return inverted ? log(0.001)/-exp(6.6-5.0*value) : 1.32 - 0.2*log(-log(0.001)/value);
            //return inverted ? log(0.001)/-exp(6.6-5.0*((value-0.01)/1.39)) : 1.32 - 0.2*log(-log(0.001)/value);
            //return inverted ? value*100 : value/(100);
            case(2):
            return inverted ? value*100 - 50 : (value + 50)/(100);
            case(3):
            return inverted ? value*100 - 50 : (value + 50)/(100);
            case(4):
            return inverted ? value*200 - 100 : (value + 100)/(200);
            case(5):
            return inverted ? value*39.95 + 0.05 : (value - 0.05)/(39.95);
            case(6):
            return inverted ? value*100 : value/(100);
            case(7):
            return inverted ? value*200 : value/(200);
            case(8):
            return value;
            case(9):
            return inverted ? value*100 - 50 : (value + 50)/(100);
            case(10):
            return inverted ? value*50 : value/(50);
            case(11):
            return inverted ? value*100 : value/(100);
        }
    }
    else if(strcmp(effect->getUniqueID(), "mdaImage") == 0) {
        switch(port) {
            case(0):
            return inverted ? value : value;
            case(1):
            return inverted ? value*400 - 200 : (value + 200)/(400);
            case(2):
            return inverted ? value*200 - 100 : (value + 100)/(200);
            case(3):
            return inverted ? value*400 - 200 : (value + 200)/(400);
            case(4):
            return inverted ? value*200 - 100 : (value + 100)/(200);
            case(5):
            return inverted ? value*40 - 20 : (value + 20)/(40);
        }
    }
    else if(strcmp(effect->getUniqueID(), "mdaJX10") == 0) {
        switch(port) {
            case(0):
            return value;
            case(1):
            return value;
            case(2):
            return value;
            case(3):
            return value;
            case(4):
            return value;
            case(5):
            return value;
            case(6):
            return value;
            case(7):
            return value;
            case(8):
            return value;
            case(9):
            return value;
            case(10):
            return value;
            case(11):
            return value;
            case(12):
            return value;
            case(13):
            return value;
            case(14):
            return value;
            case(15):
            return value;
            case(16):
            return value;
            case(17):
            return value;
            case(18):
            return value;
            case(19):
            return value;
            case(20):
            return value;
            case(21):
            return value;
            case(22):
            return value;
            case(23):
            return value;
        }
    }
    else if(strcmp(effect->getUniqueID(), "mdaLeslie") == 0) {
        switch(port) {
            case(0):
            return value;
            case(1):
            return inverted ? value*100 : value/(100);
            case(2):
            return inverted ? value*100 : value/(100);
            case(3):
            return inverted ? value*100 : value/(100);
            case(4):
            return inverted ? value*100 : value/(100);
            case(5):
            return inverted ? value*100 : value/(100);
            case(6):
            // making it logarithmic:
            if (inverted) {
                return 150.f*exp(2.30923*value);
            } else {
                return log(value/150.f)/2.30923;
            }
            //return inverted ? value*1360 + 150 : (value - 150)/(1360);
            case(7):
            return inverted ? value*40 - 20 : (value + 20)/(40);
            case(8):
            return inverted ? value*200 : value/(200);
        }
    }
    else if(strcmp(effect->getUniqueID(), "mdaLimiter") == 0) {
        switch(port) {
            case(0):
            return inverted ? value*40-40 : (value + 40)/(40);
            case(1):
            return inverted ? value*40-20 : (value + 20)/(40);
            case(2):
            return inverted ? value*1570+1 : (value - 1)/(1570);
            case(3):
            return inverted ? value*1563 : value/(1563);
            case(4):
            return inverted ? value : value;
        }
    }
    //else if(strcmp(effect->getUniqueID(), "mdaLoopLex") == 0) {

    //}
    else if(strcmp(effect->getUniqueID(), "mdaLoudness") == 0) {

    }
    else if(strcmp(effect->getUniqueID(), "mdaMultiBand") == 0) {
        switch(port) {
            case(0):
            return inverted ? value : value;
            case(1):
            return inverted ? value*933+87 : (value - 87)/(933);
            case(2):
            return inverted ? value*19495 + 111 : (value - 111)/(19495);
            case(3):
            return inverted ? value*30 : value/(30);
            case(4):
            return inverted ? value*30 : value/(30);
            case(5):
            return inverted ? value*30 : value/(30);
            case(6):
            return inverted ? value*40-20 : (value + 20)/(40);
            case(7):
            return inverted ? value*40-20 : (value + 20)/(40);
            case(8):
            return inverted ? value*40-20 : (value + 20)/(40);
            case(9):
            return inverted ? value*1748+7 : (value - 7)/(1748);
            case(10):
            return inverted ? value*1570+1 : (value - 1)/(1570);
            case(11):
            return inverted ? value*200 : value/(200);
            case(12):
            return inverted ? value : value ;
        }

    }
    else if(strcmp(effect->getUniqueID(), "mdaOverdrive") == 0) {
        switch(port) {
            case(0):
            return inverted ? value*100 : value/(100);
            case(1):
            return inverted ? value*100 : value/(100);
            case(2):
            return inverted ? value*40-20 : (value + 20)/(40);
        }
    }
    else if(strcmp(effect->getUniqueID(), "mdaPiano") == 0) {
        switch(port) {
            case(0):
            return inverted ? value*100 : value/(100);
            case(1):
            return inverted ? value*100 : value/(100);
            case(2):
            return inverted ? value*100-50 : (value + 50)/(100);
            case(3):
            return inverted ? value*100 : value/(100);
            case(4):
            return inverted ? value*100 : value/(100);
            case(5):
            return inverted ? value*100 : value/(100);
            case(6):
            return inverted ? value*100 : value/(100);
            case(7):
            return inverted ? value*200 : value/(200);
            case(8):
            return inverted ? value : value;
            case(9):
            return inverted ? value*100-50 : (value + 50)/(100);
            case(10):
            return inverted ? value*50 : value/(50);
            case(11):
            return inverted ? value*100-50 : (value + 50)/(100);
        }
    }
    else if(strcmp(effect->getUniqueID(), "mdaRePsycho") == 0) {
        switch(port) {
            case(0):
            return inverted ? value*24-24 : (value + 24)/(24);
            case(1):
            return inverted ? value*99-99 : (value + 99)/(99);
            case(2):
            return inverted ? value*100-50 : (value + 50)/(100);
            case(3):
            return inverted ? value*30-30 : (value + 30)/(30);
            case(4):
            return inverted ? value*250+10 : (value - 10)/(250);
            case(5):
            return inverted ? value*100 : value/(100);
            case(6):
            return inverted ? value : value;
        }
    }
    else if(strcmp(effect->getUniqueID(), "mdaRezFilter") == 0) {
        switch(port) {
            case(0):
            return inverted ? value*100 : value/(100);
            case(1):
            return inverted ? value*100 : value/(100);
            case(2):
            return inverted ? value*40-20 : (value + 20)/(40);
            case(3):
            return inverted ? value*200-100 : (value + 100)/(200);
            case(4):
            return inverted ? value*160.83 : value/(160.83);
            case(5):
            return inverted ? value*15510.08+1.56 : (value - 1.56)/(15510.08);
            case(6):
            return inverted ? value*200-100 : (value + 100)/(200);
            case(7):
            return 2 + log10(value)/4;
            case(8):
            return inverted ? value*40-37 : (value + 37)/(40);
            case(9):
            return inverted ? value*100 : value/(100);
        }
    }
    else if(strcmp(effect->getUniqueID(), "mdaRingMod") == 0) {
         switch(port) {
            case(0):
            return inverted ? value*16000 : value/(16000);
            case(1):
            return inverted ? value*100 : value/(100);
            case(2):
            return inverted ? value*100 : value/(100);
        }
    }
    else if(strcmp(effect->getUniqueID(), "mdaRoundPan") == 0) {
        switch(port) {
            case(0):
            return inverted ? value*360-180 :(value + 180)/(360);
            case(1):
            return inverted ? value*1030-515 : (value + 515)/(1030);
        }
    }
    else if(strcmp(effect->getUniqueID(), "mdaShepard") == 0) {
        switch(port) {
            case(0):
            return inverted ? value : value;
            case(1):
            return inverted ? value*200-100 : (value + 100)/(200);
            case(2):
            return inverted ? value*40-20 : (value + 20)/(40);
        }
    }
    else if(strcmp(effect->getUniqueID(), "mdaSplitter") == 0) {
        switch(port) {
            case(0):
            return inverted ? value : value;
            case(1):
            return inverted ? value*9900+100 : (value - 100)/(9900);
            case(2):
            return inverted ? value : value;
            case(3):
            return inverted ? value*40 - 40 : (value + 40)/(40);
            case(4):
            return inverted ? value : value;
            case(5):
            return inverted ? value*990 + 10 : (value - 10)/(990);
            case(6):
            return inverted ? value*40 - 20 : (value + 20)/(40);
        }
    }
    else if(strcmp(effect->getUniqueID(), "mdaStereo") == 0) {
        switch(port) {
            case(0):
            return inverted ? value*200 - 100 : (value + 100)/(200);
            case(1):
            return inverted ? value*47.17 + 0.45 : (value - 0.45)/(47.17);
            case(2):
            return inverted ? value*200 - 100 : (value + 100)/(200);
            case(3):
            return inverted ? value*47.62 : value/(47.62);
            case(4):
            return inverted ? value*99.9 + 0.1 : (value - 0.1)/(99.9);
        }
    }
    else if(strcmp(effect->getUniqueID(), "mdaSubSynth") == 0) {
        switch(port) {
            case(0):
            return inverted ? value : value;
            case(1):
            return inverted ? value*100 : value/(100);
            case(2):
            return inverted ? value*310+10 : (value - 10)/(310);
            case(3):
            return inverted ? value*100 : value/(100);
            case(4):
            return inverted ? value*60-60 : (value + 60)/60;
            case(5):
            if (inverted) {
                return 1.64*exp(6.89*value);
            } else {
                return log(value/1.64)/6.89;
            }
        }
    }
    else if(strcmp(effect->getUniqueID(), "mdaTalkBox") == 0) {
        switch(port) {
            case(0):
            return inverted ? value*200 : value/(200);
            case(1):
            return inverted ? value*200 : value/(200);
            case(2):
            return inverted ? value : value;
            case(3):
            return inverted ? (value*95+5)/2.5 : pow((value - 5)/(95), 0.5);
        }
    }
    else if(strcmp(effect->getUniqueID(), "mdaTestTone") == 0) {

    }
    else if(strcmp(effect->getUniqueID(), "mdaThruZero") == 0) {
        switch(port) {
            case(0):
            return inverted ? (value+0.1*93.24)+1 : 1 - ((value - 0.1)/(93.24));
            case(1):
            return inverted ? value*42.f : value/(42.f);
            case(2):
            return inverted ? value*100 : value/(100);
            case(3):
            return inverted ? value*200-100 : (value + 100)/(200);
            case(4):
            return inverted ? value*100 : value/(100);
        }
    }
    else if(strcmp(effect->getUniqueID(), "mdaTracker") == 0) {
        switch(port) {
            case(0):
            return inverted ? value : value;
            case(1):
            return inverted ? value*100 : value/(100);
            case(2):
            return inverted ? value*100 : value/(100);
            case(3):
            return inverted ? value*100 : value/(100);
            case(4):
            return inverted ? value*72-36 : (value + 36)/(72);
            case(5):
            return inverted ? value*7311+39 : (value - 39)/(7311);
            case(6):
            return inverted ? value*60 - 60 :(value + 60)/(60);
            case(7):
            return inverted ? value*40-20 : (value + 20)/(40);
        }
    }
    else if(strcmp(effect->getUniqueID(), "mdaTransient") == 0) {
        switch(port) {
            case(0):
            return inverted ? value*200-100 : (value + 100)/(200);
            case(1):
            return inverted ? value*200-100 : (value + 100)/(200);
            case(2):
            return inverted ? value*40-20 : (value + 20)/(40);
            case(3):
            return inverted ? value*20-10 : (value + 10)/(20);
            case(4):
            return inverted ? value*100 : value/(100);
            case(5):
            return inverted ? value*100 : value/(100);

        }
    }
    else if(strcmp(effect->getUniqueID(), "mdaVocInput") == 0) {

    }
    else if(strcmp(effect->getUniqueID(), "mdaVocoder") == 0) {
        switch(port) {
            case(0):
            return inverted ? value : value;
            case(1):
            return inverted ? value*20-20 : (value + 20)/(20);
            case(2):
            return inverted ? value*100 : value/(100);
            case(3):
            return inverted ? value*100 : value/(100);
            case(4):
            if (inverted) {
                return 10.9*exp(6.85*value);
            } else {
                return log(value/10.9)/6.85;
            }
            case(5):
            return inverted ? value*100 : value/(100);
            case(6):
            if (inverted) {
                return 201.f*exp(2.09*value);
            } else {
                return log(value/201.f)/2.09;
            }
            case(7):
            return inverted ? value : value;
        }
    }
    return value;
}






extern "C" {

/* Plugin */

typedef struct {
    PLUGIN_CLASS* effect;
    float*        controls;
    float**       control_buffers;
    float**       inputs;
    float**       outputs;
} LVZPlugin;

static void
lvz_cleanup(LV2_Handle instance)
{
    LVZPlugin* plugin = (LVZPlugin*)instance;
    free(plugin->controls);
    free(plugin->control_buffers);
    free(plugin->inputs);
    free(plugin->outputs);
    delete plugin->effect;
    free(plugin);
}

static void
lvz_connect_port(LV2_Handle instance, uint32_t port, void* data)
{
    LVZPlugin*     plugin      = (LVZPlugin*)instance;
    const uint32_t num_params  = plugin->effect->getNumParameters();
    const uint32_t num_inputs  = plugin->effect->getNumInputs();
    const uint32_t num_outputs = plugin->effect->getNumOutputs();

    if (port < num_params) {
        plugin->control_buffers[port] = (float*)data;
    } else if (port < num_params + num_inputs) {
        plugin->inputs[port - num_params] = (float*)data;
    } else if (port < num_params + num_inputs + num_outputs) {
        plugin->outputs[port - num_params - num_inputs] = (float*)data;
    } else if (port == num_params + num_inputs + num_outputs) {
        plugin->effect->setEventInput((LV2_Atom_Sequence*)data);
    }
}

static int
master_callback(int, int ver, int, int, int, int)
{
    return 0;
}

static LV2_Handle
lvz_instantiate(const LV2_Descriptor*    descriptor,
                double                   rate,
                const char*              bundle_path,
                const LV2_Feature*const* features)
{
    PLUGIN_CLASS* effect = new PLUGIN_CLASS(master_callback);
    effect->setURI(URI_PREFIX PLUGIN_URI_SUFFIX);
    effect->setSampleRate(rate);

    uint32_t num_params  = effect->getNumParameters();
    uint32_t num_inputs  = effect->getNumInputs();
    uint32_t num_outputs = effect->getNumOutputs();

    LVZPlugin* plugin = (LVZPlugin*)malloc(sizeof(LVZPlugin));
    plugin->effect = effect;

    for (int i = 0; features[i]; ++i) {
        if (!strcmp(features[i]->URI, LV2_URID__map)) {
            LV2_URID_Map* map = (LV2_URID_Map*)features[i]->data;
            plugin->effect->setMidiEventType(
                map->map(map->handle, LV2_MIDI__MidiEvent));
            break;
        }
    }

    if (num_params > 0) {
        plugin->controls        = (float*)malloc(sizeof(float) * num_params);
        plugin->control_buffers = (float**)malloc(sizeof(float*) * num_params);
        for (uint32_t i = 0; i < num_params; ++i) {
            plugin->controls[i] = translateParameter(effect, i, effect->getParameter(i), true);
            plugin->control_buffers[i] = NULL;
        }
    } else {
        plugin->controls        = NULL;
        plugin->control_buffers = NULL;
    }

    if (num_inputs > 0) {
        plugin->inputs = (float**)malloc(sizeof(float*) * num_inputs);
        for (uint32_t i = 0; i < num_inputs; ++i) {
            plugin->inputs[i] = NULL;
        }
    } else {
        plugin->inputs = NULL;
    }

    if (num_outputs > 0) {
        plugin->outputs = (float**)malloc(sizeof(float*) * num_outputs);
        for (uint32_t i = 0; i < num_outputs; ++i) {
            plugin->outputs[i] = NULL;
        }
    } else {
        plugin->outputs = NULL;
    }

    return (LV2_Handle)plugin;
}

static void
lvz_run(LV2_Handle instance, uint32_t sample_count)
{
    LVZPlugin* plugin = (LVZPlugin*)instance;

    for (int32_t i = 0; i < plugin->effect->getNumParameters(); ++i) {
        const float val = *plugin->control_buffers[i];
        if (val != plugin->controls[i]) {
            plugin->effect->setParameter(i, translateParameter(plugin->effect, i, val, false));
            plugin->controls[i] = val;
        }
    }

    plugin->effect->processReplacing(plugin->inputs, plugin->outputs, sample_count);
}

static const LV2_Program_Descriptor*
lv2_get_program(LV2_Handle handle, uint32_t index)
{
	static LV2_Program_Descriptor desc = { 0, 0, NULL };
	static char name[256] = { 0 };

	LVZPlugin* plugin = (LVZPlugin*)handle;

	if ((int)index < plugin->effect->getNumPrograms() && plugin->effect->getProgramNameIndexed(0, index, name))
	{
		desc.bank    = index / 128;
		desc.program = index % 128;
		desc.name    = name;
		return &desc;
	}

	return NULL;
}

static void
lv2_select_program(LV2_Handle handle, uint32_t bank, uint32_t program)
{
	LVZPlugin* plugin = (LVZPlugin*)handle;

	int realProgram = bank * 128 + program;

	if (realProgram < plugin->effect->getNumPrograms())
	{
		plugin->effect->setProgram(realProgram);

		for (int32_t i = 0; i < plugin->effect->getNumParameters(); ++i)
			plugin->controls[i] = *plugin->control_buffers[i] = translateParameter(plugin->effect, i, plugin->effect->getParameter(i), true);
	}
}

static const void*
lvz_extension_data(const char* uri)
{
	static const LV2_Programs_Interface programs = { lv2_get_program, lv2_select_program };
	if (strcmp(uri, LV2_PROGRAMS__Interface) == 0)
		return &programs;
    return NULL;
}

static void
lvz_activate(LV2_Handle instance)
{
    LVZPlugin* plugin = (LVZPlugin*)instance;
    plugin->effect->resume();
}

static void
lvz_deactivate(LV2_Handle instance)
{
    LVZPlugin* plugin = (LVZPlugin*)instance;
    plugin->effect->suspend();
}

/* Library */

static LV2_Descriptor descriptor;
static bool           initialised = false;

LV2_SYMBOL_EXPORT
const LV2_Descriptor*
lv2_descriptor(uint32_t index)
{
    if (!initialised) {
        descriptor.URI            = URI_PREFIX PLUGIN_URI_SUFFIX;
        descriptor.instantiate    = lvz_instantiate;
        descriptor.connect_port   = lvz_connect_port;
        descriptor.activate       = lvz_activate;
        descriptor.run            = lvz_run;
        descriptor.deactivate     = lvz_deactivate;
        descriptor.cleanup        = lvz_cleanup;
        descriptor.extension_data = lvz_extension_data;
        initialised = true;
    }

    switch (index) {
    case 0:
        return &descriptor;
    default:
        return NULL;
    }
}

/** Entry point for LVZ gendata */
LV2_SYMBOL_EXPORT
AudioEffectX*
lvz_new_audioeffectx()
{
    PLUGIN_CLASS* effect = new PLUGIN_CLASS(master_callback);
    effect->setURI(URI_PREFIX PLUGIN_URI_SUFFIX);
    return effect;
}


LV2_SYMBOL_EXPORT
float
lvz_translate_parameter(void* effect,int port,float value)
{
    return translateParameter((PLUGIN_CLASS*)effect, port, value, true);
}

} // extern "C"
