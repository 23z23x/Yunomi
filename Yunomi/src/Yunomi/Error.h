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
		char* what();
	};

	class VulkanError : public YunomiError
	{
	private:

	public:
		VulkanError(char* message, char* line) : YunomiError(message, line) {}
		char* what();
	};

	class GLFWError : public YunomiError
	{
	private:

	public:
		GLFWError(char* message, char* line) : YunomiError(message, line) {}
		char* what();
	};

	class ApplicationError : public YunomiError
	{
	private:

	public:
		ApplicationError(char* message) : YunomiError(message, line) {}
		char* what();
	};
}