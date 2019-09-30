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

AudioSystem::AudioSystem(void) :
    device(nullptr, [](ALCdevice *d) { alcCloseDevice(d); }),
    context(nullptr, [](ALCcontext *c) { alcDestroyContext(c); }) {}

AudioSystem::~AudioSystem(void)
{
    // Delete context before device
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
}

void AudioSystem::update([[maybe_unused]] entityx::EntityManager& entities,
                         [[maybe_unused]] entityx::EventManager& events,
                         [[maybe_unused]] entityx::TimeDelta dt)
{}

void AudioSystem::receive(const entityx::ComponentAddedEvent<Audio>& cae)
{
    alGenSources(1, const_cast<ALuint*>(&cae.component->source));
}

void AudioSystem::receive(const entityx::ComponentRemovedEvent<Audio>& cae)
{
    alDeleteSources(1, &cae.component->source);
}

