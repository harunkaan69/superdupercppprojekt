#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <list>
#include <memory>
#include <fstream>
#include "image.h"
#include "exception.h"

/*
    Class for handling creatures types themselves
*/
class creaturePrototype
{
public:
    enum habitate { water, land };
    enum sustentation { plant, herbivore, carnivore };

    creaturePrototype(const std::string& pName, int pStrength, int pSpeed, int pLifetime,
        const std::vector<std::string>& pAttributes, const image& pImage, habitate pHabitate, sustentation pSustentation)
        :
        name(pName), strength(pStrength), speed(pSpeed), lifetime(pLifetime), attributes(pAttributes), 
        texture(pImage), habitateType(pHabitate), sustentationType(pSustentation) {}

    // GET strength/speed/lifetime/name/attributes
    const int getStrength() const;
    const int getSpeed() const;
    const int getLifetime() const;
    const std::string& getName() const;
    const habitate getHabitate() const;
    const sustentation getSustentation() const;
    const std::vector<std::string>& getAttributes() const;
    const image& getTexture() const;

private:
    std::string name;
    int strength;
    int speed;
    int lifetime;
    habitate habitateType;
    sustentation sustentationType;
    std::vector<std::string> attributes;
    image texture;
};

/*
    Class to manage individual creatures
*/
class creature
{
public:
    const creaturePrototype& prototype;

    creature(const creaturePrototype& pCreaturePrototype, int pX, int pY)
        :
        prototype(pCreaturePrototype), lifetime(pCreaturePrototype.getLifetime()), posX(pX), posY(pY) 
    {
    }

    // UPDATE lifetime/pos
    void updateLifetime(int pLifeTime);
    void updatePosX(int x);
    void updatePosY(int y);
    // GET position on map & lifetime
    int getX() const;
    int getY() const;
    int getLifetime() const;
    void decayStep();
    bool isDead() const;
    bool isDecayed() const;
    const creaturePrototype& getCreaturePrototype() const;

private:
    int posX;
    int posY;
    int lifetime;
    int decay = 10;
};

