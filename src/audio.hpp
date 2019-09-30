/**
 * @file audio.hpp
 * Handles audio loading and playback
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
#ifndef SYSTEM_AUDIO_HPP_
#define SYSTEM_AUDIO_HPP_

#include <AL/alut.h>
#include <entityx/entityx.h>

#include <components/Audio.hpp>

class AudioSystem : public entityx::System<AudioSystem>,
                    public entityx::Receiver<AudioSystem>
{
private:
    std::unique_ptr<ALCdevice, void (*)(ALCdevice *)> device;
    std::unique_ptr<ALCcontext, void (*)(ALCcontext *)> context;

public:
    AudioSystem(void);
    ~AudioSystem(void);

    /**
     * Prepares the system for running.
     */
    void configure(entityx::EntityManager& entities,
                   entityx::EventManager& events) final;
    
    /**
     * Updates the render system.
     */
    void update(entityx::EntityManager& entities,
                entityx::EventManager& events,
                entityx::TimeDelta dt) final;

    void receive(const entityx::ComponentAddedEvent<Audio>& cae);
    void receive(const entityx::ComponentRemovedEvent<Audio>& cae);
};

#endif // SYSTEM_AUDIO_HPP_

