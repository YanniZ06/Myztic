#pragma once

#include <SDL.h>
#include <glm.hpp>

namespace Myztic {
	class Scene; //fwd declare
	class Drawable;

	struct PositionChangeEvent {
		Scene* renderingScene;
		glm::vec3 newPosition;

		PositionChangeEvent(Scene* scene, glm::vec3 newPosition) : renderingScene(scene), newPosition(newPosition) {}
	};

	struct ScaleChangeEvent {
		Scene* renderingScene;
		glm::vec3 newScale;

		ScaleChangeEvent(Scene* scene, glm::vec3 newScale) : renderingScene(scene), newScale(newScale) {}
	};

	struct RotationChangeEvent {
		Scene* renderingScene;
		glm::vec3 newRotation;
		
		RotationChangeEvent(Scene* scene, glm::vec3 newRotation) : renderingScene(scene), newRotation(newRotation) {};
	};
}