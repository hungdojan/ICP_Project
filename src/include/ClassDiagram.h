//
// Created by rebulien on 4/2/22.
//

#ifndef DIAGRAMEDITOR_CLASSDIAGRAM_H
#define DIAGRAMEDITOR_CLASSDIAGRAM_H
#include <vector>
#include "Element.h"
#include "UMLClassifier.h"
#include "UMLClass.h"
#include "SequenceDiagram.h"

class ClassDiagram : public Element {
protected:
    std::vector<UMLClassifier *> classElements_;
    std::vector<SequenceDiagram *> sequenceDiagrams_;
public:
    enum ClassElementType {
        CLASSIFIER, CLASS, INTERFACE
    };

    ClassDiagram() =delete;
    // Class constructor
    explicit ClassDiagram(const std::string& name) : Element{name}, classElements_{}, sequenceDiagrams_{} { };
    // Updates class diagram's name
    void setName(const std::string& newName) override;
    // Returns collection of class elements
    const std::vector<UMLClassifier *>& classElements() const;
    // Returns collection of sequence diagrams
    const std::vector<SequenceDiagram *>& sequenceDiagrams() const;

    // Loads content stored in the JSON file
    static void initClassDiagramFromFile(ClassDiagram &classDiagram, std::string path);
    // Save class diagram into the file
    void saveClassDiagramToFile(std::string path);

    // classifier methods
    // Factory method that creates new classifier
    static UMLClassifier *createClassifier(std::string name, ClassElementType classElementType);
    // Adds new classifier to the class diagram
    bool addClassifier(UMLClassifier *classifier);
    // Adds new classifier to the class diagram
    UMLClassifier *addOrReplaceClassifier(UMLClassifier *classifier);
    // Adds multiple classifiers into the class diagram
    void addClassifiers(std::vector<UMLClassifier *> classifiers);
    // Updates classifier's name
    bool changeClassifierName(UMLClassifier *classifier, const std::string& name);
    // Updates classifier's name
    bool changeClassifierName(std::string oldName, std::string newName);
    // Search for classifier in the class diagram
    UMLClassifier *getClassifier(std::string name);
    // Search for the instance of UMLClass in the class diagram
    UMLClass *getClass(std::string name);
    // Checks if given classifier is in the class diagram
    bool isInClassDiagram(const UMLClassifier *classifier);
    // Removes classifier from the class diagram
    UMLClassifier *removeClassElement(std::string name);
    // Removes classifier from the class diagram
    bool removeClassElement(UMLClassifier *classElement);

    // sequence diagram methods
    // TODO: add, search, remove
    // Class destructor
    ~ClassDiagram();
};

#endif //DIAGRAMEDITOR_CLASSDIAGRAM_H