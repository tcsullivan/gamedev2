/*
 * Copyright (C) 2019  Belle-Isle, Andrew <drumsetmonkey@gmail.com>
 * Author: Belle-Isle, Andrew <drumsetmonkey@gmail.com>
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

#ifndef IDLEFUNC_HPP_
#define IDLEFUNC_HPP_

#include <components/Component.hpp>

struct IdleFunc : Component<IdleFunc>, entityx::Component<IdleFunc>
{
    sol::function luafunc;
    public:
        IdleFunc() {}

        IdleFunc FromLua(sol::object ref)
        {
            if (ref.get_type() == sol::type::function) {
                this->luafunc = ref;
            }
            return *this;
        }
};

#endif//IDLEFUNC_HPP_