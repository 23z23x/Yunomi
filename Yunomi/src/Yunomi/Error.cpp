#include "pch.h"
#include "Error.h"

namespace ynm
{
	char* YunomiError::what()
	{
		YNM_CORE_ERROR(message);
		return "";
	}

	char* VulkanError::what()
	{
		std::stringstream ss;
		ss << "Vulkan: " << message << " At line " << line;

		YNM_CORE_ERROR(ss.str());
		return "";
	}

	char* GLFWError::what()
	{
		std::stringstream ss;
		ss << "GLFW: " << message << " At line" << line;

		YNM_CORE_ERROR(ss.str());
		return "";
	}

	char* ApplicationError::what()
	{
		std::stringstream ss;
		ss << "Application: " << message << " At line" << line;

		YNM_ERROR(ss.str());
		return "";
	}
}