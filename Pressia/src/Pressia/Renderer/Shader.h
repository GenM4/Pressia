#pragma once

namespace Pressia {

	class Shader {
	public:
		virtual ~Shader() = default;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		static Shader* Create(const std::string& vertexSrc, const std::string& fragmentSrc);
	};
}