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
        
        vmml::AABBf aabb = transformBoundingBox(aabbf, model);
        
        vmml::Vector4f playerSphere(-camera->getPosition(), 3.f);
        
        if(isIn(aabb, playerSphere)) {
            world.removeEntity(entity->id());
            entity->effects().addEffect(collider->effect);
        }
    }
    
    vmml::AABBf transformBoundingBox(vmml::AABBf boundingBox, vmml::Matrix4f m) {
        vmml::Matrix3f dimensions(m);
        
        vmml::Vector3f xa = dimensions.get_row(0) * boundingBox.getMin().x();
        vmml::Vector3f xb = dimensions.get_row(0) * boundingBox.getMax().x();
        
        vmml::Vector3f ya = dimensions.get_row(1) * boundingBox.getMin().y();
        vmml::Vector3f yb = dimensions.get_row(1) * boundingBox.getMax().y();
        
        vmml::Vector3f za = dimensions.get_row(2) * boundingBox.getMin().z();
        vmml::Vector3f zb = dimensions.get_row(2) * boundingBox.getMax().z();
        
        vmml::Vector3f translation(m.get_column(3));
        
        return vmml::AABBf(
                           min(xa, xb) + min(ya, yb) + min(za, zb) + translation,
                           max(xa, xb) + max(ya, yb) + max(za, zb) + translation
                           );
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
