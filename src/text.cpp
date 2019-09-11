#include "text.hpp"

void TextSystem::configure([[maybe_unused]] entityx::EntityManager& entities,
                           [[maybe_unused]] entityx::EventManager& events)
{
    // TODO load freetype
}
    
/**
 * Draws the text for all entities.
 */
void TextSystem::update([[maybe_unused]] entityx::EntityManager& entites,
                        [[maybe_unused]] entityx::EventManager& events,
                        [[maybe_unused]] entityx::TimeDelta dt)
{
    // TODO render each Text component's text
}

