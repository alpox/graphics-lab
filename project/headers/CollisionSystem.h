//
//  CollisionDetection.h
//  bRenderer_ios
//
//  Created by Elias on 19.06.17.
//
//

#ifndef CollisionSystem_h
#define CollisionSystem_h

#include "Collider.h"

class CollisionSystem: public virtual System {
public:
    CollisionSystem(World& world): System(world) {
        requirements_mask = (COMPONENT_COLLIDER | COMPONENT_TRANSFORM | COMPONENT_RENDERER);
    }
    
protected:
    void apply(EntityPtr entity, const double &deltaTime) {
        auto camera = entity->renderer().getObjects()->getCamera("camera");
        auto transform = entity->getComponent<Transform>(COMPONENT_TRANSFORM);
        auto collider = entity->getComponent<Collider>(COMPONENT_COLLIDER);
        auto render = entity->getComponent<Render>(COMPONENT_RENDERER);
        
        vmml::Matrix4f model = transform->modelMatrix;
        
        vmml::AABBf &aabbf = entity->model()->getBoundingBoxObjectSpace();
        
        vmml::Vector3f min = vmml::Vector3f(model * vmml::Vector4f(aabbf.getMin(), 1.0));
        vmml::Vector3f max = vmml::Vector3f(model * vmml::Vector4f(aabbf.getMax(), 1.0));
        
        vmml::AABBf aabb(min, max);
        
        vmml::Vector4f playerSphere(-camera->getPosition(), 3.f);
        
        if(isIn(aabb, playerSphere)) {
            world.removeEntity(entity->id());
            entity->effects().addEffect(collider->effect);
        }
    }
    
    template< typename T >
    bool isIn(const vmml::AABB< T > &aabb, const vmml::Vector< 4, T >& sphere ) const
    {
        vmml::Vector< 3, T > sv ( sphere[0], sphere[1], sphere[2] );
        sv += sphere[3];
        
        vmml::Vector< 3, T > min = aabb.getMin();
        vmml::Vector< 3, T > max = aabb.getMax();
        
        if ( sv.x() < min.x() || sv.y() < min.y() || sv.z() < min.z() )
            return false;
        sv -= sphere[3] * 2.0f;
        if ( sv.x() > max.x() || sv.y() > max.y() || sv.z() > max.z() )
            return false;
        
        return true;
    }
};

#endif /* CollisionSystem_h */
