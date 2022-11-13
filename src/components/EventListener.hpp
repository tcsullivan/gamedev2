/**
 * @file EventListener.hpp
 * Allows entities to receive events and handle them in Lua.
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

#ifndef COMPONENT_EVENTLISTENER_HPP_
#define COMPONENT_EVENTLISTENER_HPP_

#include "Component.hpp"

struct EventListener : Component<EventListener>
{
public:
    sol::table listeners;

    EventListener(sol::table _listeners) :
        listeners(_listeners) {}

    ~EventListener(void) {}

    void cleanup(void) {
        listeners = sol::nil;
    }

    EventListener FromLua([[maybe_unused]] sol::object ref)
    {
        return *this;
    }

    template<typename... Args>
    void tryListener(const std::string& name, sol::table& self, Args... args)
    {
        if (listeners[name] == sol::type::function)
            listeners[name](self, args...);
    }

    void serialize([[maybe_unused]] cereal::JSONOutputArchive& ar) final {}
    void serialize([[maybe_unused]] cereal::JSONInputArchive& ar) final {}

    virtual std::string serializeName(void) const final {
        return "EventListener";
    }
};

#endif // COMPONENT_EVENTLISTENER_HPP_

