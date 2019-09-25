/**
 * @file config.cpp
 * Manages name-value pairs for configuration options.
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

#include "config.hpp"

#include <cereal/cereal.hpp>
#include <cereal/archives/json.hpp>
#include <fstream>

void Config::save(void) const
{
    if (std::ofstream file (fileName); file.good()) {
        cereal::JSONOutputArchive archive (file);

        for (auto& nvp : values) {
            if (std::holds_alternative<int>(nvp.second)) {
                archive(cereal::make_nvp(nvp.first, std::get<int>(nvp.second)));
            } else if (std::holds_alternative<double>(nvp.second)) {
                archive(cereal::make_nvp(nvp.first, std::get<double>(nvp.second)));
            } else {
                archive(cereal::make_nvp(nvp.first, std::get<std::string>(nvp.second)));
            }
        }
    }
}

void Config::load(void)
{
    if (std::ifstream file (fileName); file.good()) {
        cereal::JSONInputArchive archive (file);

        for (auto& nvp : values) {
            if (std::holds_alternative<int>(nvp.second)) {
                archive(cereal::make_nvp(nvp.first, std::get<int>(nvp.second)));
            } else if (std::holds_alternative<double>(nvp.second)) {
                archive(cereal::make_nvp(nvp.first, std::get<double>(nvp.second)));
            } else {
                archive(cereal::make_nvp(nvp.first, std::get<std::string>(nvp.second)));
            }
        }
    }
}

