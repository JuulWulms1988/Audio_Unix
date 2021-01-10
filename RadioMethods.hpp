#pragma once
#include <stdio.h>
#include "RadioTypes.hpp"

namespace piRadio {

	template <class stdinType>
	inline stdinType reqFunc() {
		stdinType req;
		std::cin >> req;
		return req;
	}
}
