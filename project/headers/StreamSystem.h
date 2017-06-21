//
//  StreamSystem.h
//  bRenderer_ios
//
//  Created by Elias on 15.05.17.
//
//

#ifndef StreamSystem_h
#define StreamSystem_h

class StreamSystem: public virtual System {
public:
    StreamSystem(World& world): System(world) {
        requirements_mask = (COMPONENT_STREAM);
    }
    
protected:
	void apply(EntityPtr entity, const double &deltaTime) {
		auto stream = entity->getComponent<Stream>(COMPONENT_STREAM);

		if (deltaTime > 0.0f) {
			stream->offset += 5 * deltaTime;
			std::cout << stream->offset;
		}

		if (entity->modelName() == "Fall") {
			ShaderPtr fallShader = entity->renderer().getObjects()->getShader("fallShader");
			TexturePtr texture1 = entity->renderer().getObjects()->loadTexture("main_falls_01.png");
			TexturePtr texture2 = entity->renderer().getObjects()->loadTexture("main_falls_02.png");
			TexturePtr texture3 = entity->renderer().getObjects()->loadTexture("main_falls_03.png");
			fallShader->setUniform("offset", (GLfloat)stream->offset);
			fallShader->setUniform("fall1", texture1);
			fallShader->setUniform("fall2", texture2);
			fallShader->setUniform("fall3", texture3);
			
		}

    }
};

#endif /* StreamSystem_h */
