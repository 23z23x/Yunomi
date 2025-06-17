#pragma once

#include "pch.h"

namespace ynm
{
	class YunomiError : public std::exception
	{
	protected:
		const char* message;
		const char* line;

	public:
		YunomiError(const char* message, const char* line)
			: message(message), line(line) {}
		const char* what() const noexcept override;
	};

	class VulkanError : public YunomiError
	{
	private:

	public:
		VulkanError(const char* message, const char* line) : YunomiError(message, line) {}
		const char* what() const noexcept override;
	};

	class GLFWError : public YunomiError
	{
	private:

	public:
		GLFWError(const char* message, const char* line) : YunomiError(message, line) {}
		const char* what() const noexcept override;
	};

	class ApplicationError : public YunomiError
	{
	private:

	public:
		ApplicationError(const char* message, const char* line) : YunomiError(message, line) {}
		const char* what() const noexcept override;
	};
}