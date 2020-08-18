#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <map>
#include <memory>
#include <iomanip>
#include <filesystem>
#include "creature.h"
#include "exception.h"
#include "ai3_bachelor_praktikum.h"
/*
    Class for parsing files with the corresponding creature table
*/
class creatureFile
{
private:
    // Used for statistics/general information about the file
    static int nFaultyLines;
    static int nLines;
    static void log();

public:
    // Main method to parse the creature table
    static std::vector<creaturePrototype> parseFile(std::ifstream& fileStream);
    /* 
        Check creature of validity
        return TRUE if the input is not legal
    */
    static std::string checkName(const std::string& pName);
    static int checkSpeed(const std::string& pSpeed);
    static int checkStrength(const std::string& pStrength);
    static int checkLifetime(const std::string& pLifetime);
    static std::string checkPath(const std::string& pPath);
    static std::vector<std::string> checkAttribute(const std::string& pAttribute);
    static creaturePrototype::habitate checkHabitate(const std::string& pHabitate);
    static creaturePrototype::sustentation checkSustentation(const std::string& pSustentation);
};

