#include "pch.h"
#include "VulkanShader.h"

namespace ynm
{

	Shader* Shader::Create(const std::string& filename, ShaderType type)
	{
		//Switch statement, just calls constructor with correct Vulkan shader type provided.
		switch (type)
		{
		case VRTX:
			return new VulkanShader(filename, VkShaderStageFlagBits::VK_SHADER_STAGE_VERTEX_BIT, type, shaderc_shader_kind::shaderc_glsl_vertex_shader);
		case TESSCTRL:
			return new VulkanShader(filename, VkShaderStageFlagBits::VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT, type, shaderc_shader_kind::shaderc_glsl_tess_control_shader);
		case TESSEVAL:
			return new VulkanShader(filename, VkShaderStageFlagBits::VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT, type, shaderc_shader_kind::shaderc_glsl_tess_evaluation_shader);
		case GMTRY:
			return new VulkanShader(filename, VkShaderStageFlagBits::VK_SHADER_STAGE_GEOMETRY_BIT, type, shaderc_shader_kind::shaderc_glsl_geometry_shader);
		case FRAG:
			return new VulkanShader(filename, VkShaderStageFlagBits::VK_SHADER_STAGE_FRAGMENT_BIT, type, shaderc_shader_kind::shaderc_glsl_fragment_shader);
		case COMP:
			return new VulkanShader(filename, VkShaderStageFlagBits::VK_SHADER_STAGE_COMPUTE_BIT, type, shaderc_shader_kind::shaderc_glsl_compute_shader);
		default:
			YNM_CORE_ERROR("Shader: Valid Shader type not provided!");
			return nullptr;
		}
	}

	VulkanShader::VulkanShader(const std::string& filename, VkShaderStageFlagBits type, ShaderType ynmType, shaderc_shader_kind shadercType)
	{
		this->ynmType = ynmType;
		this->type = type;
		this->shadercType = shadercType;

		//Read in file, pass file contents to CompileShader, done.
		std::vector<char> contents = readFile(filename);

		const std::string consttents(contents.begin(), contents.end());

		this->spirv = CompileShader(shadercType, consttents, filename);

	}

	VulkanShader::~VulkanShader()
	{
		
	}

	std::vector<uint32_t> VulkanShader::CompileShader(shaderc_shader_kind type, const std::string& pshader, const std::string& filename)
	{
		//set up compiler and options
		shaderc::Compiler compiler;
		shaderc::CompileOptions options;

		options.SetOptimizationLevel(shaderc_optimization_level_size);

		shaderc::SpvCompilationResult module =
			compiler.CompileGlslToSpv(pshader, type, filename.c_str(), options);

		if (module.GetCompilationStatus() != shaderc_compilation_status_success) {
			YNM_CORE_ERROR("Vulkan Shader: {0}", module.GetErrorMessage());
			return std::vector<uint32_t>();
		}

		YNM_CORE_INFO("Vulkan Shader: Shader successfully compiled!");
		return { module.cbegin(), module.cend() };
	}
}