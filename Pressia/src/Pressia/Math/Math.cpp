#include "pspch.h"
#include "Math.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>

namespace Pressia {
	namespace Math {

		bool DecomposeTransform(const glm::mat4& transform, glm::vec3& outTranslation, glm::quat& outRotation, glm::vec3& outScale) {
			//	From glm::decompose

			outTranslation = transform[3];
			for (int i = 0; i < 3; i++)
				outScale[i] = glm::length(glm::vec3(transform[i]));
			const glm::mat3 rotMtx(
				glm::vec3(transform[0]) / outScale[0],
				glm::vec3(transform[1]) / outScale[1],
				glm::vec3(transform[2]) / outScale[2]);
			outRotation = glm::quat_cast(rotMtx);

			return true;
		}
	}
}