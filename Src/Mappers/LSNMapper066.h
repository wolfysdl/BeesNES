/**
 * Copyright L. Spiro 2022
 *
 * Written by: Shawn (L. Spiro) Wilcoxen
 *
 * Description: Mapper 066 implementation.
 */


#pragma once

#include "../LSNLSpiroNes.h"
#include "LSNMapperBase.h"

namespace lsn {

	/**
	 * Class CMapper066
	 * \brief Mapper 066 implementation.
	 *
	 * Description: Mapper 066 implementation.
	 */
	class CMapper066 : public CMapperBase {
	public :
		CMapper066() {
		}
		virtual ~CMapper066() {
		}


		// == Functions.
		/**
		 * Gets the PGM bank size.
		 *
		 * \return Returns the size of the PGM banks.
		 */
		static constexpr uint16_t						PgmBankSize() { return 32 * 1024; }

		/**
		 * Gets the CHR bank size.
		 *
		 * \return Returns the size of the CHR banks.
		 */
		static constexpr uint16_t						ChrBankSize() { return 8 * 1024; }

		/**
		 * Initializes the mapper with the ROM data.  This is usually to allow the mapper to extract information such as the number of banks it has, as well as make copies of any data it needs to run.
		 *
		 * \param _rRom The ROM data.
		 */
		virtual void									InitWithRom( LSN_ROM &_rRom ) {
			CMapperBase::InitWithRom( _rRom );
			SanitizeRegs<PgmBankSize(), ChrBankSize()>();
			/*m_ui8PgmBank = uint8_t( m_prRom->vPrgRom.size() / (PgmBankSize()) - 1 );
			m_ui8ChrBank = uint8_t( m_prRom->vChrRom.size() / (ChrBankSize()) - 1 );*/
		}

		/**
		 * Applies mapping to the CPU and PPU busses.
		 *
		 * \param _pbCpuBus A pointer to the CPU bus.
		 * \param _pbPpuBus A pointer to the PPU bus.
		 */
		virtual void									ApplyMap( CCpuBus * _pbCpuBus, CPpuBus * _pbPpuBus ) {
			CMapperBase::ApplyMap( _pbCpuBus, _pbPpuBus );

			// ================
			// SWAPPABLE BANKS
			// ================
			// CPU.
			for ( uint32_t I = 0x8000; I < 0x10000; ++I ) {
				_pbCpuBus->SetReadFunc( uint16_t( I ), &CMapperBase::PgmBankRead_8000, this, uint16_t( I - 0x8000 ) );
			}
			// PPU.
			for ( uint32_t I = 0x0000; I < 0x2000; ++I ) {
				_pbPpuBus->SetReadFunc( uint16_t( I ), &CMapperBase::ChrBankRead_2000, this, uint16_t( I - 0x0000 ) );
			}

			// ================
			// BANK-SELECT
			// ================
			// Writes to the whole area are used to select a bank.
			for ( uint32_t I = 0x8000; I < 0x10000; ++I ) {
				_pbCpuBus->SetWriteFunc( uint16_t( I ), &CMapper066::SelectBank8000_FFFF, this, 0 );	// Treated as ROM.
			}
		}


	protected :
		// == Members.


		// == Functions.
		/**
		 * Bank select ($8000-$FFFF).
		 *
		 * \param _pvParm0 A data value assigned to this address.
		 * \param _ui16Parm1 A 16-bit parameter assigned to this address.  Typically this will be the address to write to _pui8Data.
		 * \param _pui8Data The buffer to which to write.
		 * \param _ui8Ret The value to write.
		 */
		static void LSN_FASTCALL						SelectBank8000_FFFF( void * _pvParm0, uint16_t /*_ui16Parm1*/, uint8_t * /*_pui8Data*/, uint8_t _ui8Val ) {
			CMapper066 * pmThis = reinterpret_cast<CMapper066 *>(_pvParm0);
			/*
			 *	7  bit  0
			 *	---- ----
			 *	xxPP xxCC
			 *	  ||   ||
			 *	  ||   ++- Select 8 KB CHR ROM bank for PPU $0000-$1FFF
			 *	  ++------ Select 32 KB PRG ROM bank for CPU $8000-$FFFF
			 */
			//pmThis->m_ui8PgmBank = ((_ui8Val & 0b00110000) >> 4) % (pmThis->m_prRom->vPrgRom.size() / (PgmBankSize()));
			pmThis->SetPgmBank<0, PgmBankSize()>( (_ui8Val & 0b00110000) >> 4 );
			pmThis->m_ui8ChrBank = (_ui8Val & 0b00000011) % (pmThis->m_prRom->vChrRom.size() / (ChrBankSize()));
			pmThis->SetChrBank<0, ChrBankSize()>( _ui8Val & 0b00000011 );
		}
	};

}	// namespace lsn
