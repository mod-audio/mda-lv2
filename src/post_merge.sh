#!/bin/sh

rm -f mdaDe-ess.h
rm -f mdaDe-ess.cpp
rm -f mdaRePsycho!.h
rm -f mdaRePsycho!.cpp
rm -f mdaspecmeter.cpp

sed -i 's/VstInt32/LvzInt32/g' *.h *.cpp
sed -i 's/VstInt16/LvzInt16/g' *.h *.cpp
sed -i 's/DECLARE_VST_DEPRECATED/DECLARE_LVZ_DEPRECATED/g' *.h *.cpp
