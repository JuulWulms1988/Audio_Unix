#pragma once
#include <stdio.h>
#include <cstdint>
#include <unistd.h>
#include <iostream>

#include "RadioChannels.hpp"
#include "RadioMethods.hpp"
#include "Vlc.hpp"
#include "PiScreen.h"

namespace piRadio {

#define _REQT reqFunc<reqType>()

	reqType typeNow = REQ_STOP;

	class : private piScreenC, private radChanCls, private vlc {
	public:
		void func(reqType request) {
			if (request == REQ_RESET && (request = _REQT) == typeNow) { resetF(request); return; } 
			clearF(request);
			switch (typeNow = request) {
			case REQ_RADIO: radioF(reqFunc<radChanCls::radioChannel>()); break;
			case REQ_PISCREEN: piScreenC::set(); break;
			}
		}

	private:
		void resetF(reqType request) {
			switch (request) {
			case REQ_RADIO: {
				radChanCls::radioChannel c = reqFunc<radChanCls::radioChannel>();
				if (c != radChanCls::channel) vlc::printF(vlc::PRINT_CLEAR), radioF(c);
				break;
			}
			}
		}

		void clearF(reqType request) {
			if (typeNow == REQ_PISCREEN && piScreenC::ready) piScreenC::clear();
			if (typeNow != REQ_STOP) vlc::printF(vlc::PRINT_CLEAR);
			if ((typeNow == REQ_PISCREEN || request == REQ_PISCREEN) && typeNow != request)
				vlc::printF(vlc::PRINT_VOLUME, request == REQ_PISCREEN ? 320 : 256);
		}

		void radioF(reqType request) {
			vlc::printF(vlc::PRINT_ADD, radChanCls::path[radChanCls::channel = request]);
		}

		void piScreenF() {
			vlc::printF(vlc::PRINT_ADD, "udp://:1234");
		}
	} vlc;
}