/**
 * Copyright L. Spiro 2022
 *
 * Written by: Shawn (L. Spiro) Wilcoxen
 *
 * Description: ROM information.
 */


#pragma once

#include "../LSNLSpiroNes.h"
#include <string>

namespace lsn {

	/**
	 * Class LSN_ROM_INFO
	 * \brief ROM information.
	 *
	 * Description: ROM information.
	 */
	struct LSN_ROM_INFO {
		std::u16string						s16File;										/**< The full file path in UTF-16. */
		std::u16string						s16RomName;										/**< The ROM file name in UTF-16. */
	};

}	// namespace lsn