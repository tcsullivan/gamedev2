/**
 * @file Player.h
 * Component for designating player-controlled entities.
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

#ifndef PLAYER_HPP_
#define PLAYER_HPP_

#include "Component.hpp"

struct Player : Component<Player>, entityx::Component<Player>
{
        Player FromLua([[maybe_unused]] sol::object ref)
        {
		return *this;
        }
};

#endif // PLAYER_HPP_
