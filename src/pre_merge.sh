#!/bin/sh

ln -fs mdaDeEss.h mdaDe-ess.h
ln -fs mdaDeEss.cpp mdaDe-ess.cpp
ln -fs mdaRePsycho.h mdaRePsycho!.h
ln -fs mdaRePsycho.cpp mdaRePsycho!.cpp

sed -i 's/LvzInt32/VstInt32/g' *.h *.cpp
sed -i 's/LvzInt16/VstInt16/g' *.h *.cpp
sed -i 's/DECLARE_LVZ_DEPRECATED/DECLARE_VST_DEPRECATED/g' *.h *.cpp
