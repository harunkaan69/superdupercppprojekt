#include "creature.h"

void creature::updateLifetime(int pLifeTime) {
    if (pLifeTime < 0) {
        throw parameterError("Lifetime < 0");
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