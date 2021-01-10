#pragma once
#include <stdio.h>
#include <stdarg.h>

namespace piRadio {

	class vlc {
	public:
		enum printType { PRINT_CLEAR, PRINT_VOLUME, PRINT_ADD };

		vlc() {
			char a[]{ 'w', '\0' },
				c[]{ 'c', 'v', 'l', 'c', ' ', '-', 'I', ' ', 'r', 'c', ' ', '-', '-', 'n', 'o', '-', 'v', 'i', 'd', 'e', 'o', ' ', 'n', 'e', 't', 'w', 'o', 'r', 'k', '-', 'c', 'a', 'c', 'h', 'i', 'n', 'g', ' ', '1', '0', '0', '0', ' ', '-', '-', 'l', 'o', 'o', 'p', ' ', '-', '-', 's', 'o', 'u', 't', '-', 'k', 'e', 'e', 'p', '\0' };
			popen(c, a);
			printF(PRINT_VOLUME, 256);
		}

	protected:
		void printF(printType type, ...) {
			va_list argptr;
			va_start(argptr, type);
			vfprintf(pipe, printArg(type), argptr);
			va_end(argptr);
		}

	private:
		FILE* pipe;

		const char* printArg(printType type) {
			switch (type) {
			case PRINT_CLEAR: return "clear\n";
			case PRINT_VOLUME: return "volume %d\n";
			case PRINT_ADD: return "add %s --sout-keep\n";
			} return NULL;
		}
	};

}