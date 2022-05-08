/**
 * @brief Declaration of JsonParser class.
 * JsonParser contains two static functions that loads class diagram from JSON file
 * and store content of class diagram to JSON file.
 *
 * This source code serves as submission for semester assignment of class ICP at FIT, BUT 2021/22.
 *
 * @file JsonParser.h
 * @date 22/04/2022
 * @authors Hung Do     (xdohun00)
 *          David Kedra (xkedra00)
 */

#ifndef DIAGRAMEDITOR_JSONPARSER_H
#define DIAGRAMEDITOR_JSONPARSER_H
#include "ClassDiagram.h"

/**
 * @brief Class that parse class diagram to and from JSON file.
 */
class JsonParser {
public:
    /**
     * Loads class diagram from file and creates new instance of ClassDiagram.
     * @brief Loads class diagram from the file.
     * @param classDiagram Reference to class diagram object.
     * @param filePath Path to file.
     */
    static void initFromFile(ClassDiagram &classDiagram, const std::string &filePath);

    /**
     * @brief Saves class diagram into the file.
     * @param classDiagram Reference to class diagram object.
     * @param filePath Path to file.
     */
    static void saveToFile(ClassDiagram &classDiagram, const std::string &filePath);
};

#endif //DIAGRAMEDITOR_JSONPARSER_H
