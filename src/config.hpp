/**
 * @file config.hpp
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

#ifndef CONFIG_HPP_
#define CONFIG_HPP_

#include <map>
#include <optional>
#include <string>
#include <variant>

class Config
{
private:
    static constexpr const char *fileName = "settings.json";
    std::map<std::string, std::variant<int, double, std::string>> values;

public:
    template<typename T>
    std::optional<T> get(const std::string& name)
    {
        if (values.count(name) != 0) {
            if (auto value = std::get_if<T>(&values[name]); value != nullptr)
                return *value;
        }
    
        return {};
    }

    template<typename T>
    void set(const std::string& name, T val)
    {
        values[name] = val;
    }

    template<typename T>
    void add(const std::string& name)
    {
        values[name] = T();
    }

    void save(void) const;
    void load(void);
};

#endif // CONFIG_HPP_

