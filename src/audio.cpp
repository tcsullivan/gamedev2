/**
 * @file audio.cpp
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

#include "audio.hpp"
#include "components/Player.hpp"

#include <AL/alut.h>
#include <iostream>

AudioSystem::AudioSystem(void) :
    device(nullptr, [](ALCdevice *d) { alcCloseDevice(d); }),
    context(nullptr, [](ALCcontext *c) { alcDestroyContext(c); }) {}

AudioSystem::~AudioSystem(void)
{
    alutExit();

    // Delete context before device
    alcMakeContextCurrent(nullptr);
    context.reset();
    device.reset();
}

void AudioSystem::configure([[maybe_unused]] entityx::EntityManager& entities,
                            entityx::EventManager& events)
{
    events.subscribe<entityx::ComponentAddedEvent<Audio>>(*this);
    events.subscribe<entityx::ComponentRemovedEvent<Audio>>(*this);

    // Access device
    device.reset(alcOpenDevice(nullptr));
    if (!device)
        return; // TODO Uh oh

    // Create context
    context.reset(alcCreateContext(device.get(), nullptr));
    if (!alcMakeContextCurrent(context.get()))
        return; // TODO Another uh oh

    if (alutInitWithoutContext(nullptr, nullptr) != AL_TRUE)
        return; // TODO Third uh oh

    // Set up listener
    ALfloat listenerOri[] = { 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f };
    alListener3f(AL_POSITION, 0, 0, 0.0f);
    alListener3f(AL_VELOCITY, 0, 0, 0);
    alListenerfv(AL_ORIENTATION, listenerOri);

    alDistanceModel(AL_LINEAR_DISTANCE); 
}

void AudioSystem::update(entityx::EntityManager& entities,
                         [[maybe_unused]] entityx::EventManager& events,
                         [[maybe_unused]] entityx::TimeDelta dt)
{
    entities.each<Player, Position>(
        []([[maybe_unused]] entityx::Entity e,
                   [[maybe_unused]] Player& p,
                   Position &pos)
        {
            alListener3f(AL_POSITION, pos.x, pos.y, 0.0f);
        });
}

void AudioSystem::receive(const entityx::ComponentAddedEvent<Audio>& cae)
{
    alGenSources(1, const_cast<ALuint*>(&cae.component->source));
    //alGenBuffers(1, const_cast<ALuint*>(&cae.component->buffer));
    
    if (auto buf = alutCreateBufferFromFile(cae.component->fileName.c_str());
        buf != AL_NONE) {
        const_cast<Audio*>(cae.component.get())->buffer = buf;
        alSourcei(cae.component->source, AL_BUFFER, buf);
        // TODO Do these values need to be scaled to the world or window?
        alSourcef(cae.component->source, AL_MAX_DISTANCE, 25);
        alSourcef(cae.component->source, AL_REFERENCE_DISTANCE, 2);

        std::cout << "Loaded audio: " << cae.component->fileName << std::endl;
    }
}

void AudioSystem::receive(const entityx::ComponentRemovedEvent<Audio>& cae)
{
    alDeleteBuffers(1, &cae.component->buffer);
    alDeleteSources(1, &cae.component->source);
}

void AudioSystem::playSound(const Position& pos, const Audio& audio)
{
    alSource3f(audio.source, AL_POSITION, pos.x, pos.y, 0);
    alSourcePlay(audio.source);
}

