//
//  IdManager.hpp
//  bRenderer_ios
//
//  Created by Elias on 14.05.17.
//
//

#ifndef IdManager_hpp
#define IdManager_hpp

#include <vector>

class IdManager {
public:
    int next();
    void removeId(int entityId);
private:
    int highest;
    std::vector<int> freeSlots;
};

#endif /* IdManager_hpp */
