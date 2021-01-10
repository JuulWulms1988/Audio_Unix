#pragma once
#include <stdio.h>
#include <cstdint>

namespace piRadio {
	class radChanCls {
	protected:
		typedef uint16_t radioChannel;
		radioChannel channel;
		static const char* path[];
	};
#ifdef _PIRAD_SRC_SPACE
#undef _PIRAD_SRC_SPACE
	const char* radChanCls::path[]{ 
		"https://stream.slam.nl/web10_mp3", 
		"https://stream.slam.nl/slam_mp3" 
	};
#endif
}