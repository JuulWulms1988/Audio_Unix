#include "PiScreen.h"
#include "DelayTimer.hpp"

using namespace delayTimer;

#define _C(X) ((dTimObj*)X)

namespace piRadio {

	piScreenC::piScreenC() { 
		_C((timer = (void*) new dTimObj(
			&ready, 6000
		)))->set(
			[](void* oP) { 
				((piScreenC*)oP)->piScreenF(); 
			}, this
		); 
	}

	void piScreenC::set() {
		_C(timer)->setPlannerF();
	}

	void piScreenC::clear() {
		_C(timer)->clearF();
	}
}

#undef _C