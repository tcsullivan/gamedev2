/**
 * @file script.hpp
 * Manages all Lua scripts.
 *
 * Copyright (C) 2019  Belle-Isle, Andrew <drumsetmonkey@gmail.com>
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

#ifndef SYSTEM_SCRIPT_HPP_
#define SYSTEM_SCRIPT_HPP_

#include "world.hpp"

#include <entityx/entityx.h>
#include <sol/sol.hpp>

#include <concepts>

/**
 * Utility for pairing class instances to their member function calls.
 * This is useful for adding functions to the Lua game namespace.
 *
 * TODO I am certain that this can be done better. -clyne
 *
 * @param func The member function to call
 * @param instance The instance to bind to
 * @return A function that calls the member function using the given instance
 */
template<class C, typename R, typename... Args>
auto bindInstance(R (C::* func)(Args...), C *instance)
{
    if constexpr (std::same_as<R, void>)
        return [instance, func](Args... args) { (instance->*func)(args...); };
    else
        return [instance, func](Args... args) { return (instance->*func)(args...); };
}
template<class C, typename R, typename... Args>
auto bindInstance(R (C::* func)(Args...) const, const C *instance)
{
    if constexpr (std::same_as<R, void>)
        return [instance, func](Args... args) { (instance->*func)(args...); };
    else
        return [instance, func](Args... args) { return (instance->*func)(args...); };
}


struct EntitySpawnEvent
{
    sol::object ref;

    EntitySpawnEvent(sol::object _ref) :
        ref(_ref) {}
};

/**
 * @class ScriptSystem
 * Handles all the game's scripting requests
 */
class ScriptSystem : public entityx::System<ScriptSystem>,
                     public entityx::Receiver<ScriptSystem>
{
private:
    /**
     * The script systems internal lua state that handles all
     * interactions between C and Lua
     */
    sol::state lua;
    sol::table game;

    entityx::EntityManager& manager;
    
    // TODO possibly emit events to spawn worlds instead of passing the
    //  world system around like a toy
    WorldSystem &worldSystem;

public:
    ScriptSystem(entityx::EntityManager& _mg, WorldSystem& _ws):
        manager(_mg), worldSystem(_ws) {}

    ~ScriptSystem(void) {}

    /**
     * Prepares the system for running.
     */
    void configure(entityx::EntityManager& entities,
                   entityx::EventManager& events) final;
    
    /**
     * Updates the scripting system.
     */
    void update(entityx::EntityManager& entites,
                entityx::EventManager& events,
                entityx::TimeDelta dt) final;

    /**
     * Receives all entity spawning events and manages the
     * script counterpart.
     */
    void receive(const EntitySpawnEvent &toSpawn);

    /**
     * Initializes the lua states and libraries.
     * @return Zero on success, non-zero on error
     */
    int init(void);

    /**
     * Run the initialization file.
     */
    void doFile(void);

    /**
     * The function called by lua scripts in order to spawn an entity.
     * @param param The table that must be passed in by Lua. This is a
     * sol2 object instead of a sol2 table because this allows us to handle
     * errors easier instead of letting sol2 do the error handling.
     */
    sol::table spawn(sol::object param);

    /**
     * Contains all calls that export components/functions to lua.
     */
    void scriptExport(void);

    template<typename F>
    void addToGameNamespace(const std::string& name, F func) {
        game.set_function(name, func);
    }
};

#endif // SYSTEM_SCRIPT_HPP_

