#include "pch.h"
#include "Error.h"

namespace ynm
{
	const char* YunomiError::what() const noexcept
	{
		YNM_CORE_ERROR(message);
		return "";
	}

	const char* VulkanError::what() const noexcept
	{
		std::stringstream ss;
		ss << "Vulkan: " << message << " At line " << line;

		YNM_CORE_ERROR(ss.str());
		return "";
	}

	const char* GLFWError::what() const noexcept
	{
		std::stringstream ss;
		ss << "GLFW: " << message << " At line" << line;

		YNM_CORE_ERROR(ss.str());
		return "";
	}

	const char* ApplicationError::what() const noexcept
	{
		std::stringstream ss;
		ss << "Application: " << message << " At line" << line;

		YNM_ERROR(ss.str());
		return "";
	}
}