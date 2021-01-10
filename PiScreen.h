#pragma once
#include <stdio.h>

namespace piRadio {

	class piScreenC {
	protected:
		piScreenC();
		volatile bool ready;
		void set();
		void clear();
		
	private:
		virtual void piScreenF() = 0;
		void* timer;
	};

}