/**
 * Copyright L. Spiro 2022
 *
 * Written by: Shawn (L. Spiro) Wilcoxen
 *
 * Description: ROM data.
 */


#pragma once

#include "../LSNLSpiroNes.h"
#include <map>


namespace lsn {

	/**
	 * Class LSN_ROM
	 * \brief ROM data.
	 *
	 * Description: ROM data.
	 */
	class CDatabase {
	public :
		// == Enumerations.
		/** Chips. */
		enum LSN_CHIP {
			LSN_C_UNKNOWN,

			LSN_C_MMC1							= 100,
			LSN_C_MMC1A,
			LSN_C_MMC1B1,
			LSN_C_MMC1B2,
			LSN_C_MMC1B2F,
			LSN_C_MMC1B3,
			LSN_C_MMC1C,
			LSN_C_MMC1_END,
		};


		// == Types.
		/** A database entry. */
		struct LSN_ENTRY {
			uint32_t							ui32Crc = 0;											/**< Game identifier. */
			uint32_t							ui32PgmRomSize = 0;										/**< The actual ROM size (in the case of over-dumps). */
			LSN_MIRROR_MODE						mmMirrorOverride = LSN_MM_NO_OVERRIDE;					/**< A mirror override, or LSN_MM_NO_OVERRIDE. */
			LSN_PPU_METRICS						pmRegion = LSN_PM_UNKNOWN;								/**< The game region. */
			LSN_CHIP							cChip = LSN_C_UNKNOWN;									/**< The mapper chip. */
			uint16_t							ui16Mapper = uint16_t( -1 );							/**< The mapper. */
		};


		// == Functions.
		/**
		 * Initializes the database with known ROMs.
		 */
		static void								Init();

		/**
		 * Frees all memory associated with this class.
		 */
		static void								Reset();


		// == Members.
		/** The database map.  CRC is the key. */
		static std::map<uint32_t, LSN_ENTRY>	m_mDatabase;
	};

}	// namespace lsn
