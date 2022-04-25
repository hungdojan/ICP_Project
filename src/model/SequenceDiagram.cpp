//
// Created by rebulien on 4/19/22.
//

#include "SequenceDiagram.h"
#include <algorithm>

//void SequenceDiagram::addObject(UMLClass *instanceOfClass, const std::string &objName) {
//    auto iter {std::find_if(objects_.begin(), objects_.end(),
//                            [objName](UMLObject *obj) { return objName == obj->name(); })};
//    // TODO: duplicate found
//    if (iter != objects_.end())
//        return;
//    objects_.push_back(new UMLObject(objName, instanceOfClass));
//}