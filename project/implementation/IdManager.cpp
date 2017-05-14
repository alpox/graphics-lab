//
//  IdManager.cpp
//  bRenderer_ios
//
//  Created by Elias on 14.05.17.
//
//

#include "IdManager.h"

int IdManager::next() {
    if(freeSlots.empty())
        return ++highest;
    
    int freeId = freeSlots.back();
    freeSlots.pop_back();
    return freeId;
}

void IdManager::removeId(int entityId) {
    freeSlots.push_back(entityId);
}
