#pragma once

#include "pch.h"

namespace ynm
{
	class YunomiError : public std::exception
	{
	protected:
		char* message;
		char* line;

	public:
		YunomiError(char* message, char* line)
			: message(message), line(line) {}
		const char* what() const noexcept override;
	};

	class VulkanError : public YunomiError
	{
	private:

	public:
		VulkanError(char* message, char* line) : YunomiError(message, line) {}
		const char* what() const noexcept override;
	};

	class GLFWError : public YunomiError
	{
	private:

	public:
		GLFWError(char* message, char* line) : YunomiError(message, line) {}
		const char* what() const noexcept override;
	};

	class ApplicationError : public YunomiError
	{
	private:

	public:
		ApplicationError(char* message, char* line) : YunomiError(message, line) {}
		const char* what() const noexcept override;
	};
}