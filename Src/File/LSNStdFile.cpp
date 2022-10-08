/**
 * Copyright L. Spiro 2022
 *
 * Written by: Shawn (L. Spiro) Wilcoxen
 *
 * Description: A class for opening files using the standard C library (FILE).
 */


#include "LSNStdFile.h"

namespace lsn {

	CStdFile::CStdFile() :
		m_pfFile( nullptr ),
		m_ui64Size( 0 ) {
	}
	CStdFile::~CStdFile() {
		Close();
	}

	// == Functions.
#ifdef LSN_WINDOWS
	/**
	 * Opens a file.  The path is given in UTF-16.
	 *
	 * \param _pcPath Path to the file to open.
	 * \return Returns true if the file was opened, false otherwise.
	 */
	bool CStdFile::Open( const char16_t * _pcFile ) {
		Close();

		FILE * pfFile = nullptr;
		errno_t enOpenResult = ::_wfopen_s( &pfFile, reinterpret_cast<const wchar_t *>(_pcFile), L"rb" );
		if ( nullptr == pfFile || enOpenResult != 0 ) { return false; }

		::_fseeki64( pfFile, 0, SEEK_END );
		m_ui64Size = ::_ftelli64( pfFile );
		std::rewind( pfFile );

		m_pfFile = pfFile;
		PostLoad();
		return true;
	}
#else
	/**
	 * Opens a file.  The path is given in UTF-8.
	 *
	 * \param _pcPath Path to the file to open.
	 * \return Returns true if the file was opened, false otherwise.
	 */
	bool CStdFile::Open( const char8_t * _pcFile ) {
		FILE * pfFile = std::fopen( reinterpret_cast<const char *>(_pcFile), "rb" );
		if ( nullptr == pfFile ) { return false; }

		std::fseek( pfFile, 0, SEEK_END );
		m_ui64Size = std::ftell( pfFile );
		std::rewind( pfFile );

		m_pfFile = pfFile;
		PostLoad();
		return true;
	}
#endif	// #ifdef LSN_WINDOWS

	/**
	 * Closes the opened file.
	 */
	void CStdFile::Close() {
		if ( m_pfFile != nullptr ) {
			::fclose( m_pfFile );
			m_pfFile = nullptr;
			m_ui64Size = 0;
		}
	}

	/**
	 * Loads the opened file to memory, storing the result in _vResult.
	 *
	 * \param _vResult The location where to store the file in memory.
	 * \return Returns true if the file was successfully loaded into memory.
	 */
	bool CStdFile::LoadToMemory( std::vector<uint8_t> &_vResult ) const {
		if ( m_pfFile != nullptr ) {
#ifdef LSN_WINDOWS
			__int64 i64Pos = ::_ftelli64( m_pfFile );
			::_fseeki64( m_pfFile, 0, SEEK_END );
			__int64 i64Len = ::_ftelli64( m_pfFile );
			std::rewind( m_pfFile );
			try {
				_vResult.resize( size_t( i64Len ) );
			}
			catch ( ... ) {
				::_fseeki64( m_pfFile, i64Pos, SEEK_SET );
				return false;
			}
			if ( __int64( _vResult.size() ) != i64Len ) {
				::_fseeki64( m_pfFile, i64Pos, SEEK_SET );
				return false;
			}
			if ( std::fread( _vResult.data(), _vResult.size(), 1, m_pfFile ) != 1 ) {
				::_fseeki64( m_pfFile, i64Pos, SEEK_SET );
				return false;
			}
			::_fseeki64( m_pfFile, i64Pos, SEEK_SET );
#else
			long lPos = std::ftell( m_pfFile );
			std::fseek( m_pfFile, 0, SEEK_END );
			long lLen = std::ftell( m_pfFile );
			std::rewind( m_pfFile );
			try {
				_vResult.resize( size_t( lLen ) );
			}
			catch ( ... ) {
				std::fseek( m_pfFile, lPos, SEEK_SET );
				return false;
			}
			if ( _vResult.size() != lLen ) {
				std::fseek( m_pfFile, lPos, SEEK_SET );
				return false;
			}
			if ( std::fread( _vResult.data(), _vResult.size(), 1, m_pfFile ) != 1 ) {
				std::fseek( m_pfFile, lPos, SEEK_SET );
				return false;
			}
			std::fseek( m_pfFile, lPos, SEEK_SET );
#endif	// #ifdef LSN_WINDOWS
			return true;
		}
		return false;
	}

	/**
	 * Performs post-loading operations after a successful loading of the file.  m_pfFile will be valid when this is called.  Override to perform additional loading operations on m_pfFile.
	 */
	void CStdFile::PostLoad() {}

}	// namespace lsn
