#include "creature.h"

void creature::updateLifetime(int pLifeTime) {
    if (pLifeTime < 0) {
        lifetime = 0;
    }
    else {
        lifetime = pLifeTime;
    }
}

void creature::updatePosX(int x)
{
    posX = x;
}

void creature::updatePosY(int y)
{
    posY = y;
}

const int creaturePrototype::getStrength() const
{ 
    return strength;
}

const int creaturePrototype::getSpeed() const
{
    return speed;
}

const int creaturePrototype::getLifetime() const
{
    return lifetime;
}

const std::string& creaturePrototype::getName() const
{
    return name;
}

const std::vector<std::string>& creaturePrototype::getAttributes() const
{
    return attributes;
}

const image& creaturePrototype::getTexture() const
{
    return texture;
}

int creature::getX() const
{
    return posX;
}

int creature::getY() const
{
    return posY;
}

int creature::getLifetime() const
{
	return lifetime;
}

void creature::decayStep()
{
    if (isDead() && decay > 0)
        --decay;
}

bool creature::isDead() const
{
    return lifetime == 0;
}

bool creature::isDecayed() const
{
    return decay == 0;
}


const creaturePrototype& creature::getCreaturePrototype() const
{
    return prototype;
}



const creaturePrototype::habitate creaturePrototype::getHabitate() const
{
    return habitateType;
}

const creaturePrototype::sustentation creaturePrototype::getSustentation() const
{
    return sustentationType;
}