/**
 * @file gamestate.hpp
 * Provides functionality to load and save entity data to or from JSON files.
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

#ifndef GAMELOAD_HPP_
#define GAMELOAD_HPP_

#include <cereal/cereal.hpp>
#include <cereal/archives/json.hpp>
#include <entityx/entityx.h>

#include <fstream>

/**
 * @class GameState
 * Manages save files that contain entity data.
 */
class GameState
{
public:
    /**
     * Loads entity data from a JSON file into entities contained in the
     * entity manager.
     *
     * The order of entities and the components they have must match between
     * those in the manager and those in the file.
     *
     * @param file The file to load from
     * @param entities The entity manager to load into
     */
    static void load(const std::string& file, entityx::EntityManager &entities)
    {
        if (std::ifstream saveFile (file); saveFile.good()) {
            cereal::JSONInputArchive archive (saveFile);
	    serializeEntities(archive, false, entities);
        }
    }

    /**
     * Saves entity data from the entities in the provided manager into a JSON
     * file,
     *
     * @param file The file to load from
     * @param entities The entity manager to get entity data from
     */
    static void save(const std::string& file, entityx::EntityManager &entities)
    {
        if (std::ofstream saveFile (file); saveFile.good()) {
            cereal::JSONOutputArchive archive (saveFile);
	    serializeEntities(archive, true, entities);
        }
    }

private:
    /**
     * Calls the serialization funciton for each entity in the given entity
     * manager.
     * @param archive The archive to save/load with (cereal::JSON...)
     * @param save True to save data, false to load
     * @param entities The entity manager to iterate through
     */
    template<class Archive>
    static void serializeEntities(Archive& archive,
                                  bool save,
                                  entityx::EntityManager& entities)
    {
        std::string name ("entity");
        int i = 0;

        for (auto entity : entities.entities_for_debugging()) {
            archive.setNextName((name + std::to_string(i++)).c_str());
            archive.startNode();
            entities.entity_serialize(entity, save, archive);
            archive.finishNode();
        }
    }
};

#endif // GAMELOAD_HPP_

