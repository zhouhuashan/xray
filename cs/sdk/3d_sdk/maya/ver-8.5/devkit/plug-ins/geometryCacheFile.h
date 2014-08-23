#ifndef _GEOMETRY_CACHE_FILE
#define _GEOMETRY_CACHE_FILE

//-
// ==========================================================================
// Copyright (C) 1995 - 2006 Autodesk, Inc. and/or its licensors.  All 
// rights reserved.
//
// The coded instructions, statements, computer programs, and/or related 
// material (collectively the "Data") in these files contain unpublished 
// information proprietary to Autodesk, Inc. ("Autodesk") and/or its 
// licensors, which is protected by U.S. and Canadian federal copyright 
// law and by international treaties.
//
// The Data is provided for use exclusively by You. You have the right 
// to use, modify, and incorporate this Data into other products for 
// purposes authorized by the Autodesk software license agreement, 
// without fee.
//
// The copyright notices in the Software and this entire statement, 
// including the above license grant, this restriction and the 
// following disclaimer, must be included in all copies of the 
// Software, in whole or in part, and all derivative works of 
// the Software, unless such copies or derivative works are solely 
// in the form of machine-executable object code generated by a 
// source language processor.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND. 
// AUTODESK DOES NOT MAKE AND HEREBY DISCLAIMS ANY EXPRESS OR IMPLIED 
// WARRANTIES INCLUDING, BUT NOT LIMITED TO, THE WARRANTIES OF 
// NON-INFRINGEMENT, MERCHANTABILITY OR FITNESS FOR A PARTICULAR 
// PURPOSE, OR ARISING FROM A COURSE OF DEALING, USAGE, OR 
// TRADE PRACTICE. IN NO EVENT WILL AUTODESK AND/OR ITS LICENSORS 
// BE LIABLE FOR ANY LOST REVENUES, DATA, OR PROFITS, OR SPECIAL, 
// DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES, EVEN IF AUTODESK 
// AND/OR ITS LICENSORS HAS BEEN ADVISED OF THE POSSIBILITY 
// OR PROBABILITY OF SUCH DAMAGES.
//
// ==========================================================================
//+

///////////////////////////////////////////////////////////////////////////////
//
// File Name: geometryCacheFile.h
//
// Description : 
//		An interface used for reading cache file data, storing it
//		and converting it to ASCII.  The geometry cache file data is 
//		currently being stored in the Maya IFF format.  However, note that 
//		the file format is subject to change in future versions of Maya.  
//		The following is the typical tag format for a single file cache.
//
//  |---CACH (Group)	// Header
//  |     |---VRSN		// Version Number (char*)
//  |     |---STIM		// Start Time of the Cache File (int)
//  |     |---ETIM		// End Time of the Cache File (int)
//  |
//  |---MYCH (Group)	// 1st Time 
//  |     |---TIME		// Time (int)
//  |     |---CHNM		// 1st Channel Name (char*)
//  |     |---SIZE		// 1st Channel Size
//  |     |---DVCA		// 1st Channel Data (Double Vector Array)
//  |     |---CHNM		// n-th Channel Name
//  |     |---SIZE		// n-th Channel Size
//  |     |---DVCA		// n-th Channel Data (Double Vector Array)
//  |     |..
//  |
//  |---MYCH (Group)	// 2nd Time 
//  |     |---TIME		// Time
//  |     |---CHNM		// 1st Channel Name
//  |     |---SIZE		// 1st Channel Size
//  |     |---DVCA		// 1st Channel Data (Double Vector Array)
//  |     |---CHNM		// n-th Channel Name
//  |     |---SIZE		// n-th Channel Size
//  |     |---DVCA		// n-th Channel Data (Double Vector Array)
//  |     |..
//  |
//  |---..
//	|
//
//      The above example shows a cache file containing channels of double
//      vector array data.  Maya can also store float vector array data,
//      using the IFF key "FVCA".
//
//		In a multiple file caches, the only difference is that after the 
//		header "CACH" group, there is only one MYCH group and there is no 
//		TIME chunk.	Note that the value of the header's STIM and ETIM chunk 
//		are only relevant with multiple file caches.
//
///////////////////////////////////////////////////////////////////////////////

// Project includes
//
#include <geometryCacheBlockBase.h>

// Maya includes
//
#include <maya/MString.h>
#include <maya/MIffFile.h>

// Other includes
//
#include <list>

// Typedef of STL containers to store our chunks of data
//
typedef std::list<geometryCacheBlockBase*> cacheBlockList;
typedef std::list<geometryCacheBlockBase*>::iterator cacheBlockIterator;

class geometryCacheFile
{
public:
	// Constructor / Destructor methods
	//
				geometryCacheFile( const MString& fileName, MIffFile* iffFile );
	virtual		~geometryCacheFile();

	// Accessor methods
	//
	const MString&	fileName();
	const bool&		isRead();

	// Read cache method
	//
	bool	readCacheFiles();

	// Convert cache method
	//
	bool	convertToAscii();

private:
	// Read header methods
	//
	bool	readHeaderGroup( MStatus& status );
	bool	readHeaderVersion();
	bool	readHeaderTimeRange();

	// Read channel methods
	//    
	bool	readChannelGroup( MStatus& groupStatus );
	
	bool	readChannelTime();
	bool	readChannel( MStatus& channelStatus );
	bool	readChannelName( MStatus& channelStatus );
	bool	readChannelData();

	// These methods are for modifying the data in the 
	// cacheBlockList member called "blockList"
	//
	void	storeCacheBlock( const MString& tag );
	void	storeCacheBlock( const MString& tag, const int& value );
	void	storeCacheBlock( const MString& tag, const MString& value  );
	void	storeCacheBlock( const MString& tag, 
						const double* value, 
						const uint& size );
	void	storeCacheBlock( const MString& tag, 
						const float* value, 
						const uint& size );

	// Data members
	//
	MIffFile*			iffFilePtr;		// Object that reads the cache files
	MString				cacheFileName;	// The cache file name
	bool				readStatus;		// Indicates if the file was read
	cacheBlockList		blockList;		// List of read data blocks from file
};

#endif
