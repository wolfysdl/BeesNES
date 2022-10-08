/**
 * Copyright L. Spiro 2022
 *
 * Written by: Shawn (L. Spiro) Wilcoxen
 *
 * Description: The base class for working with files.
 */


#include "LSNFileBase.h"
#include "../OS/LSNOs.h"
#include "../Utilities/LSNUtilities.h"

namespace lsn {

	CFileBase::~CFileBase() {}

	// == Functions.
	/**
	 * Opens a file.  The path is given in UTF-8.
	 *
	 * \param _pcPath Path to the file to open.
	 * \return Returns true if the file was opened, false otherwise.
	 */
	bool CFileBase::Open( const char8_t * _pcFile ) {
		bool bErrored;
		std::u16string swTmp = CUtilities::Utf8ToUtf16( _pcFile, &bErrored );
		if ( bErrored ) { return false; }
		return Open( swTmp.c_str() );
	}

	/**
	 * Opens a file.  The path is given in UTF-16.
	 *
	 * \param _pcPath Path to the file to open.
	 * \return Returns true if the file was opened, false otherwise.
	 */
	bool CFileBase::Open( const char16_t * _pcFile ) {
		bool bErrored;
		std::string sTmp = CUtilities::Utf16ToUtf8( _pcFile, &bErrored );
		if ( bErrored ) { return false; }
		return Open( reinterpret_cast<const char8_t *>(sTmp.c_str()) );
	}

	/**
	 * Closes the opened file.
	 */
	void CFileBase::Close() {}

	/**
	 * If true, the file is an archive containing more files.
	 *
	 * \return Returns true if the file is an archive, false otherwise.
	 */
	bool CFileBase::IsArchive() const { return false; }

	/**
	 * Loads the opened file to memory, storing the result in _vResult.
	 *
	 * \param _vResult The location where to store the file in memory.
	 * \return Returns true if the file was successfully loaded into memory.
	 */
	bool CFileBase::LoadToMemory( std::vector<uint8_t> &/*_vResult*/ ) const { return false; }

}	// namespace lsn
