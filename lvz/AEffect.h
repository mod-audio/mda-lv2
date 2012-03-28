/*
  LVZ - A C++ interface for writing LV2 plugins.
  Copyright 2008-2011 David Robillard <http://drobilla.net>

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

#ifndef LVZ_AEFFECT_H
#define LVZ_AEFFECT_H

class AudioEffect;

enum AEffectFlags {
	effFlagsHasEditor
};

struct AEffect {
	int flags;
};

#endif // LVZ_AEFFECT_H
