#include "creatureFile.h"

int creatureFile::nLines = 0;
int creatureFile::nFaultyLines = 0;

std::vector<creaturePrototype> creatureFile::parseFile(std::ifstream& fileStream) {
    std::vector<creaturePrototype> tempCreatureList;
    std::string currentLine;
    std::vector<std::string> vecLines;

    // Read every line of the file into a vector
    while (std::getline(fileStream, currentLine)) {
        vecLines.push_back(currentLine);
    }

    nLines = vecLines.size();
    char delimiter = ',';
    std::string lineElement;

    // Loop through every line in the vector
    for (int i = 0; i < vecLines.size(); i++) {
        std::vector<std::string> vecLineElements;
        std::stringstream lineStream(vecLines[i]);
        int nErrPerLine = 0;

        /*
         * Loop through every line itself and split it into its respective parts
         * [0]: Name            [1]: Strength
         * [2]: Speed           [3]: Lifespan
         * [4]: Attributes      [5]: Image Path
         */
        while (getline(lineStream, lineElement, delimiter)) {
            vecLineElements.push_back(lineElement);
        }

        /*
         * Check if the current line has enough descriptive elements
         * If the amount of elements is 6, continue parsing
         * Else, increase the local error counter and jump to the next line
         */
        if (vecLineElements.size() != 6) {
            std::cerr << "Error in Line |" << std::setw(2) << i << "|: Line in list has too few or too many entries ..." << std::endl;
            std::string temp = "Error in Line |";
            temp += i;
            temp += "|: Line in list has too few or too many entries ...";
            continue;
        }
        /*
            Check each attribute for its validity
            If faulty ones exist, throw an error related to the element and skip the line
        */
        try {
            std::string tempName = checkName(vecLineElements[0]);
            int tempStrength = checkStrength(vecLineElements[1]);
            int tempSpeed = checkSpeed(vecLineElements[2]);
            int tempLifetime = checkLifetime(vecLineElements[3]);
            std::vector<std::string> loopAttributes = checkAttribute(vecLineElements[4]);
            creaturePrototype::habitate tempHabitate = checkHabitate(vecLineElements[4]);
            creaturePrototype::sustentation tempSustentation = checkSustentation(vecLineElements[4]);
            std::string tempPath = checkPath(vecLineElements[5]);
            std::unique_ptr<image> img = image::load(tempPath);
            
            tempCreatureList.push_back(
                creaturePrototype(tempName, tempStrength, tempSpeed, tempLifetime, 
                                  loopAttributes, *img, tempHabitate, tempSustentation));
        }
        catch (const std::exception& e) {
            //std::cerr << "Error in line |" << std::setw(2) << i << "|: " << e.what() << " ..." << std::endl;
            nFaultyLines++;
            continue;
        }
    }
    log();
    return tempCreatureList;
}

std::string creatureFile::checkName(const std::string& pName) {
    if (pName.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ ") != std::string::npos) {
        throw parameterError("Name");
    }
    return pName;
}

int creatureFile::checkSpeed(const std::string& pSpeed) {
    if (pSpeed.find_first_not_of("1234567890") != std::string::npos) {
        throw parameterError("Speed");
    }
    return stoi(pSpeed);
}

int creatureFile::checkStrength(const std::string& pStrength) {
    if (pStrength.find_first_not_of("1234567890") != std::string::npos) {
        throw parameterError("Strength");
    }
    return stoi(pStrength);
}

int creatureFile::checkLifetime(const std::string& pLifetime) {
    if (pLifetime.find_first_not_of("1234567890") != std::string::npos) {
        throw parameterError("Lifetime");
    }
    return stoi(pLifetime);
}

std::string creatureFile::checkPath(const std::string& pPath)
{
    if (pPath.find_first_not_of("1234567890abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ/.-_") != std::string::npos) {
        throw parameterError("Path");
    }
    for (int i = 0; i < pPath.size(); i++) {
        if (pPath[i] == '/') {
            if (pPath[i + 1] == '/') {
                throw parameterError("Path");
            }
        }
    }
    return pPath;
}

std::vector<std::string> creatureFile::checkAttribute(const std::string& pAttribute) {
    std::vector<std::string> temp;
    std::stringstream loopAttributeStream(pAttribute);
    std::string attributesElement;
    char attributeDelimiter = ' ';

    while (getline(loopAttributeStream, attributesElement, attributeDelimiter)) {
        temp.push_back(attributesElement);
    }

    if (!temp.empty()) {
        for (int k = 0; k < temp.size(); k++) {
            if (temp[k].find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890_") != std::string::npos) {
                throw parameterError("Attribute");
            }
        }
    }

    return temp;
}

creaturePrototype::habitate creatureFile::checkHabitate(const std::string& pHabitate)
{
    // transform string to lower case to eliminate case sensitivity
    std::string temp = pHabitate;
    std::transform(temp.begin(), temp.end(), temp.begin(), std::tolower);

    if (temp.find("wasserbewohner") != std::string::npos) {
        return creaturePrototype::habitate::water;
    } else if (temp.find("landbewohner") != std::string::npos) {
        return creaturePrototype::habitate::land;
    } else {
        throw parameterError("bruh");
    }
}

creaturePrototype::sustentation creatureFile::checkSustentation(const std::string& pSustentation)
{
    // transform string to lower case to eliminate case sensitivity
    std::string temp = pSustentation;
    std::transform(temp.begin(), temp.end(), temp.begin(), std::tolower);

    if (temp.find("fleischfresser") != std::string::npos) {
        return creaturePrototype::sustentation::carnivore;
    } else if (temp.find("pflanzenfresser") != std::string::npos) {
        return creaturePrototype::sustentation::herbivore;
    } else if (temp.find("pflanze") != std::string::npos) {
        return creaturePrototype::sustentation::plant;
    } else {
        throw parameterError("");
    }
}

void creatureFile::log() {
    std::cout << "\nFILE INFORMATION:" << std::endl
        << std::setfill('=') << std::setw(98) << "=" << std::setfill(' ') << std::endl
        << "Lines read" << "\t:|" << std::setw(2) << nLines << "|" << std::endl
        << "Faulty lines" << "\t:|" << std::setw(2) << nFaultyLines << "|" << std::endl;
}