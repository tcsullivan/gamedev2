/**
 * @file text.hpp
 * Manages entity text.
 *
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

#ifndef SYSTEM_TEXT_HPP_
#define SYSTEM_TEXT_HPP_

#include <entityx/entityx.h>
#include <ft2build.h>
#include <freetype/freetype.h>

#include <map>
#include <string>
#include <vector>

struct FT_Info;

/**
 * @class PhysicsSystem
 * Handles the position and velocity updating of all entities
 */
class TextSystem : public entityx::System<TextSystem>
{
public:
    /**
     * Prepares the system for running.
     */
    void configure(entityx::EntityManager& entities,
                   entityx::EventManager& events) final;
    
    /**
     * Draws the text for all entities.
     */
    void update(entityx::EntityManager& entites,
                entityx::EventManager& events,
                entityx::TimeDelta dt) final;

    void loadFont(const std::string& name, const std::string& file, int size);

private:
    FT_Library freetype;
    std::map<std::string, FT_Face> fonts;
    std::map<std::string, std::vector<FT_Info>> fontData;
};

#endif // SYSTEM_TEXT_HPP_

