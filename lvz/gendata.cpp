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

#include <cassert>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <string>

#include <dlfcn.h>

#include "audioeffectx.h"

using namespace std;

// VST is so incredibly awful.  Just.. wow.
#define MAX_NAME_LENGTH 1024
char name_buf[MAX_NAME_LENGTH];

struct Record {
	Record(const string& n) : base_name(n) {}
	string base_name;
	typedef list<string> UIs;
	UIs uis;
};

typedef std::map<string, Record> Manifest;
Manifest manifest;

typedef float (*lvz_translate_parameter_func)(void* effect,int port,float value);
static lvz_translate_parameter_func lvz_translate_parameter;

string
symbolify(const char* name, char space_char='_')
{
	string str(name);

	// Like This -> Like_This
	for (size_t i=0; i < str.length(); ++i)
		if (str[i] == ' ')
			str[i] = space_char;

	str[0] = std::tolower(str[0]);

	// LikeThis -> like_this
	for (size_t i=1; i < str.length(); ++i)
		if (str[i] >= 'A' && str[i] <= 'Z'
				&& str[i-1] >= 'a' && str[i-1] <= 'z'
				&& ((i == str.length() - 1) || (str[i+1] <= 'a' && str[i+1] >= 'Z'))
				&& (!(str[i-1] == 'd' && str[i] == 'B'))
				&& (!(str[i-1] == 'F' && str[i] == 'X'))
				&& (!(str[i-1] == 'D' && str[i] == 'C')))
			str = str.substr(0, i) + space_char + str.substr(i);

	// To lowercase, and skip invalids
	for (size_t i=1; i < str.length(); ) {
		if (std::isalpha(str[i]) || std::isdigit(str[i])) {
			str[i] = std::tolower(str[i]);
			++i;
		} else if (str[i-1] != space_char) {
			str[i] = space_char;
			++i;
		} else {
			str = str.substr(0, i) + str.substr(i+1);
		}
	}

	// Compat
	/**/ if (str == "envelope_decay")
	    str = "env_decay";
	else if (str == "envelope_release")
	    str = "env_release";
// 	else if (str == "polyphony")
// 	    str = "polyphonic";
	else if (str == "velocity_sense")
	    str = "vel_sense";
	else if (str == "velocity_to_hardness")
	    str = "vel_to_hardness";
	else if (str == "velocity_to_muffling")
	    str = "vel_to_muffling";
	else if (str == "velocity_sensitivity")
	    str = "vel_sensitivity";
	else if (str == "depthmod")
	    str = "depth_mod";

	return str;
}

void
write_plugin(AudioEffectX* effect, const string& lib_file_name)
{
	const string base_name = lib_file_name.substr(0, lib_file_name.find_last_of("."));
	const string data_file_name = base_name + ".ttl";

	fstream os(data_file_name.c_str(), ios::out);
	effect->getProductString(name_buf);

	const bool     has_midi_in    = effect->flagWantEvents;
	const uint32_t num_params     = effect->getNumParameters();
	const uint32_t num_audio_ins  = effect->getNumInputs();
	const uint32_t num_audio_outs = effect->getNumOutputs();
	const uint32_t num_ports      = num_params + num_audio_ins + num_audio_outs + (has_midi_in ? 1 : 0);

	os << "@prefix atom: <http://lv2plug.in/ns/ext/atom#> ." << endl;
	os << "@prefix doap: <http://usefulinc.com/ns/doap#> ." << endl;
	os << "@prefix lv2: <http://lv2plug.in/ns/lv2core#> ." << endl;
	os << "@prefix mda: <http://moddevices.com/plugins/mda/> ." << endl;
	os << "@prefix mod: <http://moddevices.com/ns/mod#> ." << endl;
	os << "@prefix pg: <http://lv2plug.in/ns/ext/port-groups#> ." << endl;
	os << "@prefix epp: <http://lv2plug.in/ns/ext/port-props#> ." << endl;
	os << "@prefix units: <http://lv2plug.in/ns/extensions/units#> ." << endl;
	os << "@prefix pprops: <http://lv2plug.in/ns/ext/port-props#> ." << endl;
	os << "@prefix rdfs: <http://www.w3.org/2000/01/rdf-schema#> ." << endl;
	os << "@prefix rdf: <http://www.w3.org/1999/02/22-rdf-syntax-ns#> ." << endl;
	os << "@prefix foaf: <http://xmlns.com/foaf/0.1/> ." << endl;
	os << endl;
	if (num_audio_ins == 2) {
		os << "mda:mainIn" << endl;
		os << "    a pg:StereoGroup ," << endl;
		os << "        pg:InputGroup ;" << endl;
		os << "    lv2:name \"Input\" ;" << endl;
		os << "    lv2:symbol \"in\" ." << endl;
		os << endl;
	}
	if (num_audio_outs == 2) {
		os << "mda:mainOut" << endl;
		os << "    a pg:StereoGroup ," << endl;
		os << "        pg:OutputGroup ;" << endl;
		if (num_audio_ins == 2) {
			os << "    pg:source mda:mainIn ;" << endl;
		}
		os << "    lv2:name \"Output\" ;" << endl;
		os << "    lv2:symbol \"out\" ." << endl;
		os << endl;
	}
	os << "mda:" << base_name << endl;
	if (effect->flagIsSynth) {
		os << "    a lv2:Plugin ," << endl;
		os << "        lv2:InstrumentPlugin ;" << endl;
	} else {
		os << "    a lv2:Plugin ;" << endl;
	}
	os << "    lv2:project mda: ;" << endl;
	os << "    lv2:symbol \"" << base_name << "\" ;" << endl;
	os << "    doap:name \"" << name_buf << "\" ;" << endl;
	os << "    doap:license <http://usefulinc.com/doap/licenses/gpl> ;" << endl;
	os << "    lv2:optionalFeature lv2:hardRTCapable ;" << endl;
	if (has_midi_in) {
		os << "    lv2:requiredFeature <http://lv2plug.in/ns/ext/urid#map> ;" << endl;
	}
	if (effect->getNumPrograms() > 1) {
		os << "    lv2:extensionData <http://kxstudio.sf.net/ns/lv2ext/programs#Interface> ;" << endl;
	}
	if (num_audio_ins == 2) {
		os << "    pg:mainInput mda:mainIn ;" << endl;
	}
	if (num_audio_outs == 2) {
		os << "    pg:mainOutput mda:mainOut ;" << endl;
	}
	os << "    lv2:minorVersion 0;" << endl;
	os << "    lv2:microVersion 4;" << endl;
	os << "    mod:brand \"MDA\"";

	if (num_ports > 0)
		os << " ;" << endl << "    lv2:port [" << endl;
	else
		os << " ." << endl;

	uint32_t idx = 0;

	for (uint32_t i = idx; i < num_params; ++i, ++idx) {
		effect->getParameterName(i, name_buf);
		os << "        a lv2:InputPort ,\n";
		os << "            lv2:ControlPort ;" << endl;
		os << "        lv2:index" << " " << idx << " ;" << endl;
		os << "        lv2:name \"" << name_buf << "\" ;" << endl;
		os << "        lv2:symbol \"" << symbolify(name_buf) << "\" ;" << endl;
		os << "        lv2:default " << lvz_translate_parameter(effect, i, effect->getParameter(i)) << " ;" << endl;
		os << "        lv2:minimum " << lvz_translate_parameter(effect, i, 0.0f) << " ;" << endl;
		os << "        lv2:maximum " << lvz_translate_parameter(effect, i, 1.0f) << " ;" << endl;
		os << ((idx == num_ports - 1) ? "    ] ." : "    ] , [") << endl;
	}

	if (num_audio_ins == 2) {
		os << "        a lv2:InputPort ,\n";
		os << "            lv2:AudioPort ;" << endl;
		os << "        lv2:index " << idx++ << " ;" << endl;
		os << "        lv2:symbol \"left_in\" ;" << endl;
		os << "        lv2:name \"Left In\" ;" << endl;
		os << "        lv2:designation pg:left ;" << endl;
		os << "        pg:group mda:mainIn" << endl;
		os << "    ] , [" << endl;
		os << "        a lv2:InputPort ,\n";
		os << "            lv2:AudioPort ;" << endl;
		os << "        lv2:index " << idx++ << " ;" << endl;
		os << "        lv2:symbol \"right_in\" ;" << endl;
		os << "        lv2:name \"Right In\" ;" << endl;
		os << "        lv2:designation pg:right ;" << endl;
		os << "        pg:group mda:mainIn" << endl;
		os << (idx == num_ports ? "    ] ." : "    ] , [") << endl;
	} else {
		for (uint32_t i = 0; i < num_audio_ins; ++i, ++idx) {
			os << "        a lv2:InputPort ,\n";
			os << "            lv2:AudioPort ;" << endl;
			os << "        lv2:index" << " " << idx << " ;" << endl;
			os << "        lv2:symbol \"in" << i+1 << "\" ;" << endl;
			os << "        lv2:name \"Input " << i+1 << "\" ;" << endl;
			os << ((idx == num_ports - 1) ? "    ] ." : "    ] , [") << endl;
		}
	}

	if (num_audio_outs == 2) {
		os << "        a lv2:OutputPort ,\n";
		os << "            lv2:AudioPort ;" << endl;
		os << "        lv2:index " << idx++ << " ;" << endl;
		os << "        lv2:symbol \"left_out\" ;" << endl;
		os << "        lv2:name \"Left Out\" ;" << endl;
		os << "        lv2:designation pg:left ;" << endl;
		os << "        pg:group mda:mainOut" << endl;
		os << "    ] , [" << endl;
		os << "        a lv2:OutputPort ,\n";
		os << "            lv2:AudioPort ;" << endl;
		os << "        lv2:index " << idx++ << " ;" << endl;
		os << "        lv2:symbol \"right_out\" ;" << endl;
		os << "        lv2:name \"Right Out\" ;" << endl;
		os << "        lv2:designation pg:right ;" << endl;
		os << "        pg:group mda:mainOut" << endl;
		os << (idx == num_ports ? "    ] ." : "    ] , [") << endl;
	} else {
		for (uint32_t i = 0; i < num_audio_outs; ++i, ++idx) {
			os << "        a lv2:OutputPort ,\n";
			os << "            lv2:AudioPort ;" << endl;
			os << "        lv2:index " << idx << " ;" << endl;
			os << "        lv2:symbol \"out" << i+1 << "\" ;" << endl;
			os << "        lv2:name \"Output " << i+1 << "\" ;" << endl;
			os << ((idx == num_ports - 1) ? "    ] ." : "    ] , [") << endl;
		}
	}

	if (has_midi_in) {
		os << "        a lv2:InputPort ," << endl;
		os << "            atom:AtomPort ;" << endl;
		os << "        atom:bufferType atom:Sequence ;" << endl;
		os << "        atom:supports <http://lv2plug.in/ns/ext/midi#MidiEvent> ;" << endl;
		os << "        lv2:index " << idx++ << " ;" << endl;
		os << "        lv2:symbol \"event_in\" ;" << endl;
		os << "        lv2:name \"Event In\"" << endl;
		os << "    ] ." << endl;
	}

	os.close();

	Manifest::iterator i = manifest.find(effect->getURI());
	if (i != manifest.end()) {
		i->second.base_name = base_name;
	} else {
		manifest.insert(std::make_pair(effect->getURI(), Record(base_name)));
	}

	if (effect->getNumPrograms() > 1) {
		std::string preset_file = "presets.ttl";

		fstream pos(preset_file.c_str(), ios::out);
		pos << "@prefix lv2: <http://lv2plug.in/ns/lv2core#> ." << endl;
		pos << "@prefix pset: <http://lv2plug.in/ns/ext/presets#> ." << endl;
		pos << "@prefix rdfs: <http://www.w3.org/2000/01/rdf-schema#> ." << endl << endl;
		for (int32_t i = 0; i < effect->getNumPrograms(); ++i) {
			effect->setProgram(i);
			effect->getProgramName(name_buf);

			if (!strcmp(name_buf, "Empty Patch"))
				continue;

			std::string preset_uri = string("http://moddevices.com/plugins/mda/presets#") + base_name + "-" + symbolify(name_buf, '-');

			// Write manifest entry
			std::cout << "<" << preset_uri << ">"
			          << "\n    a pset:Preset ;\n    lv2:appliesTo <"
			          << effect->getURI() << "> ;\n    "
			          << "rdfs:label \"" << name_buf << "\" ;\n    "
			          << "rdfs:seeAlso <" << preset_file << "> .\n" << std::endl;

			// Write preset file
			pos << "<" << preset_uri << ">\n"
			    << "\ta pset:Preset ;\n"
			    << "\tlv2:appliesTo <" << effect->getURI() << ">";
			for (uint32_t i = 0; i < num_params; ++i) {
				effect->getParameterName(i, name_buf);
				pos << " ;\n\tlv2:port [" << endl;
				pos << "\t\tlv2:symbol \"" << symbolify(name_buf) << "\" ;" << endl;
				pos << "\t\tpset:value " << lvz_translate_parameter(effect, i, effect->getParameter(i)) << " ;" << endl;
				pos << "\t]";
			}
			pos << " .\n" << endl;
		}
		pos.close();
	}
}

void
write_manifest(ostream& os)
{
	os << "@prefix lv2: <http://lv2plug.in/ns/lv2core#> ." << endl;
	os << "@prefix rdfs: <http://www.w3.org/2000/01/rdf-schema#> ." << endl;
	os << "@prefix uiext: <http://lv2plug.in/ns/extensions/ui#> ." << endl << endl;
	for (Manifest::iterator i = manifest.begin(); i != manifest.end(); ++i) {
		Record& r = i->second;
		os << "<" << i->first << ">\n    a lv2:Plugin ;" << endl;
		os << "    rdfs:seeAlso <" << r.base_name << ".ttl> ;" << endl;
		os << "    lv2:binary <" << r.base_name << ".so> ";
		for (Record::UIs::iterator j = r.uis.begin(); j != r.uis.end(); ++j)
			os << ";" << endl << "    uiext:ui <" << *j << "> ";
		os << "." << endl << endl;
	}
}

int
main(int argc, char** argv)
{
	if (argc == 0) {
		cout << "Usage: gendata [PLUGINLIB1] [PLUGINLIB2]..." << endl;
		cout << "Each argument is a path to a LVZ plugin library." << endl;
		cout << "For each library an LV2 data file with the same name" << endl;
		cout << "will be output containing the data for that plugin." << endl;
		cout << "A manifest of the plugins found is written to stdout" << endl;
		return 1;
	}

	typedef AudioEffectX* (*new_effect_func)();
	typedef AudioEffectX* (*plugin_uri_func)();

	new_effect_func constructor = NULL;
	AudioEffectX*   effect      = NULL;

	for (int i = 1; i < argc; ++i) {
		void* handle = dlopen(argv[i], RTLD_LAZY);
		if (handle == NULL) {
			cerr << "ERROR: " << argv[i] << ": " << dlerror() << " (ignoring)" << endl;
			continue;
		}

		string lib_path = argv[i];
		size_t last_slash = lib_path.find_last_of("/");
		if (last_slash != string::npos)
			lib_path = lib_path.substr(last_slash + 1);

		constructor = (new_effect_func)dlsym(handle, "lvz_new_audioeffectx");
		if (constructor != NULL) {
			lvz_translate_parameter = (lvz_translate_parameter_func)dlsym(handle, "lvz_translate_parameter");
			effect = constructor();
			effect->resume();
			write_plugin(effect, lib_path);
		}

		if (constructor == NULL) {
			cerr << "ERROR: " << argv[i] << ": not an LVZ plugin library, ignoring" << endl;
		}

		dlclose(handle);
	}

	write_manifest(cout);

	return 0;
}

