//
//  EffectsBag.h
//  bRenderer_ios
//
//  Created by Elias on 19.06.17.
//
//

#ifndef EffectsBag_h
#define EffectsBag_h

typedef std::vector<std::pair<double, EFFECT>> EffectList;

class EffectsBag {
public:
    void addEffect(EFFECT effect) {
        effects.push_back(std::make_pair(0.0, effect));
    }
    
    void testEffects(const double &deltaTime) {
        effects.erase(std::remove_if(effects.begin(), effects.end(), [&deltaTime](std::pair<double, EFFECT> &effect) {
            effect.first += deltaTime;
            
            return effect.first > EFFECT_DURATION;
        }), effects.end());
    }
    
    EffectList getEffects() const {
        return effects;
    }
    
private:
    EffectList effects;
};

#endif /* EffectsBag_h */
