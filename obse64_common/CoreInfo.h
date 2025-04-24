#pragma once

#include <cstdint>

struct OBSE64CoreVersionData
{
	enum
	{
		kVersion = 1,
	};

	std::uint32_t	dataVersion;

	std::uint32_t	runtimeVersion;
};
