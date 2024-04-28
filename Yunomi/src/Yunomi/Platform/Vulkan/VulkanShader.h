#pragma once
#include "Yunomi/Render/Shader.h"
#include "shaderc/shaderc.hpp"

namespace ynm
{

	class VulkanShader : public Shader
	{
	public:

		Shader* Create(const std::string& filename, ShaderType type);
		VulkanShader(const std::string& filename, VkShaderStageFlagBits type, ShaderType ynmType, shaderc_shader_kind shadercType);
		~VulkanShader();

		//Used to return compiled shader code
		inline std::vector<uint32_t> getSpirv() const override { return spirv; }

		inline ShaderType getYNMType() const override { return ynmType; }

	private:
		//Data members
		ShaderType ynmType;
		VkShaderStageFlagBits type;
		shaderc_shader_kind shadercType;
		std::vector<uint32_t> spirv;

		//Vulkan shader module
		VkShaderModule shader;

		//Compiles the shader
		std::vector<uint32_t> CompileShader(shaderc_shader_kind type, const std::string& pshader, const std::string& filename);

	};
}

