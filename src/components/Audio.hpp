/**
 * Copyright (C) 2019 Clyne Sullivan
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef COMPONENT_AUDIO_HPP_
#define COMPONENT_AUDIO_HPP_

#include <AL/al.h>

#include "Component.hpp"

struct Audio : Component<Audio>
{
public:
    std::string fileName;
    ALuint source;
    ALuint buffer;

    Audio(std::string _fileName = "") :
        fileName(_fileName), source(0), buffer(0) {}

    Audio FromLua(sol::object ref)
    {
        if (ref.get_type() == sol::type::table) {
            sol::table tab = ref;
            if (tab["file"] != nullptr)
                this->fileName = tab["file"];
        } else {
            throw std::string("Audio table not formatted properly");
        }

        return *this;
    }

    void serialize([[maybe_unused]] cereal::JSONOutputArchive& ar) final {}
    void serialize([[maybe_unused]] cereal::JSONInputArchive& ar) final {}

    virtual std::string serializeName(void) const final {
        return "Audio";
    }
};

#endif // COMPONENT_AUDIO_HPP_

