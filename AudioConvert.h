//////////
//
//	File:		AudioConvert.h
//
//	Contains:	Header file for code showing how to use the Sound Manager's SoundConverter routines. 
//
//	Written by:	Jim Reekes
//	Revised by: Tim Monroe
//
//	Copyright:	� 1999 by Apple Computer, Inc., all rights reserved.
//
//	Change History (most recent first):
//
//	   <1>	 	03/09/99	rtm		first file from jr; added some comments, and added Windows-specific code
//	   
//////////

#define TARGET_API_MAC_CARBON						1


//////////
//
// header files
//	   
//////////

#ifndef __AIFF__
#include <AIFF.h>
#endif

#ifndef __COMPONENTS__
#include <Components.h>
#endif

#ifndef __CONTROLDEFINITIONS__
#include <ControlDefinitions.h>
#endif

#ifndef __MACERRORS__
#include <MacErrors.h>
#endif

#ifndef __FILETYPESANDCREATORS__
#include <FileTypesAndCreators.h>
#endif

#ifndef __FIXMATH__
#include <FixMath.h>
#endif

#ifndef __FONTS__
#include <Fonts.h>
#endif

#ifndef __FP__
#include <fp.h>
#endif

#ifndef __MACMEMORY__
#include <MacMemory.h>
#endif

#ifndef __MACTYPES__
#include <MacTypes.h>
#endif

#ifndef __MENUS__
#include <Menus.h>
#endif

#ifndef __MOVIES__
#include <Movies.h>
#endif

#ifndef __MOVIESFORMAT__
#include <MoviesFormat.h>
#endif

#ifndef __NAVIGATION__
#include <Navigation.h>
#endif

#ifndef __QUICKDRAW__
#include <Quickdraw.h>
#endif

#ifndef __QUICKTIMECOMPONENTS__
#include <QuickTimeComponents.h>
#endif

#ifndef __RESOURCES__
#include <Resources.h>
#endif

#ifndef __SOUND__
#include <Sound.h>
#endif

#ifndef __STRINGS__
#include <Strings.h>
#endif

#ifndef _STDDEF_H
#include <stddef.h>
#endif

#ifndef _STDLIB_H
#include <stdlib.h>
#endif

// Windows-specific header files
#if TARGET_OS_WIN32

	#ifndef __QTML__
	#include <QTML.h>
	#endif
	
	#include <windows.h>
	
#endif


//////////
//
// compiler flags and defines
//	   
//////////

// turns on debugging code
#define _DEBUG

#if TARGET_OS_WIN32
#define PASCAL_RTN
#endif

#if TARGET_OS_MAC
#define PASCAL_RTN				pascal
#endif


//////////
//
// constants
//	   
//////////

enum {
	kFileBufferSize				= (200*1024),
	kMaxSndConvtBufferSize		= (20*1024),		// upper limit for sound converter's in or out buffer
	rCustomGetFileDialog		= 1000,				// resource ID for custom Std File Dialog
	kFileNameEditTextBox		= 10,				// item in custom Std File Dialog
	kOutputAIFFButton			= 13,				// item in custom Std File Dialog
	kOutputMovieButton			= 14,				// item in custom Std File Dialog
	
	rStringsResID				= 128,				// resource ID for our application's strings
	rAIFFMenuLabelIndex			= 1,				// index for AIFF menu label
	rAIFFExtensionIndex			= 2,				// index for AIFF filename extension
	rMooVExtensionIndex			= 3,				// index for MooV filename extension
	rAppNameStringIndex			= 4					// index for MooV filename extension
};

#define kMovieFileSaveName		"Sound.mov"					// default name for saving a movie file
#define kAIFFFileSaveName		"Sound.aiff"				// default name for saving an AIFF file
#define kACSavePrompt			"Save Sound File As:"		// prompt for saving a file
#define kACAIFFMenuLabel		"AIFF File"					// menu label for AIFF format

#define kFileExtSeparator		(char)'.'					// the filename separator


//////////
//
// function prototypes
//	   
//////////

OSErr 							AudConv_ConvertToAIFF (ConstFSSpecPtr theInputFSSpec, ConstFSSpecPtr theOutputFSSpec, SoundComponentDataPtr theDestInfo);
OSErr 							AudConv_ConvertToMovie (ConstFSSpecPtr theInputFSSpec, ConstFSSpecPtr theOutputFSSpec, SoundComponentDataPtr theDestInfo);
OSErr							AudConv_GetDataFromAIFF (short theRefNum, SoundComponentDataPtr theSourceInfo, long *theSourceOffset, long *theSourceSize, Handle *theDestCompParams);
OSErr							AudConv_ConvertAudioIntoHandle (	SoundComponentDataPtr theSourceInfo, 
																	Handle theSourceCompParamsHandle,
																	Handle theDestHandle, 
																	SoundComponentDataPtr theDestInfo,
																	Handle *theDestCompParamsHandle, 
																	CompressionInfoPtr theDestCompInfo);
OSErr							AudConv_WriteAudioToHandle (SoundConverter theConverter, SoundComponentDataPtr theSourceInfo, short theSourceBytesPerFrame, SoundComponentDataPtr theDestInfo, Handle theDestHandle);
OSErr							AudConv_GetOutputFormat (SoundComponentDataPtr theDestInfo);
OSErr							AudConv_SetFPosToChunk (short theRefNum, ID theChunkID);
OSErr							AudConv_PrepFileAsAIFF (short theOutputFile, SoundComponentDataPtr theDestInfo, Handle theDestParams);
OSErr							AudConv_FinishFileAsAIFF (short outputFile, unsigned long destFramesMoved, unsigned long destBytesMoved);
OSErr							AudConv_PutAudioIntoTrack (Track theTrack, Handle theDestAudioData, SoundComponentDataPtr destInfo, Handle destCompParams, CompressionInfoPtr destCompInfo);
PASCAL_RTN short				AudConv_SFGetDialogHook (short theItem, DialogPtr theDialog, void *theOutputAIFF);

PASCAL_RTN Boolean				AudConv_NavObjectFilterProc (AEDesc *theItem, void *theInfo, void *theCallBackUD, NavFilterModes theFilterMode);
StringPtr						AudConv_ConvertCToPascalString (char *theString);
PASCAL_RTN void					AudConv_HandleNavEvent (NavEventCallbackMessage theCallBackSelector, NavCBRecPtr theCallBackParms, void *theCallBackUD);


//////////
//
// failure handling macros
//	   
//////////

/*
	Some macros used to check for errors and also to allow for
	handling them by using a goto statement.  This makes the source
	code easier to read.  It will break into the debugger with a
	message showing the condition that caused the failure.  In some
	of the macros the debug message is removed but goto remains.  In
	other macros all of it is removed when doing a non-debug build.

    Note that these macros use the "\p" construct for creating
	Pascal strings at compile time.  Most non-Mac compilers do
	not recognize this, give a warning, and put 'p' as the first
	character of the string.  You can ignore the warning because
	the non-Mac version of DebugStr deals with this just fine.
	For Microsoft's Visual C++, we suppress the warning below.
*/
#if defined(_MSC_VER) && !defined(__MWERKS__)
	// Visual C++ from Microsoft
	#pragma warning(disable:4129) // unrecognized character escape sequence
#endif

// This checks for the exception, and if true then goto handler
#ifdef _DEBUG
#define FailIf(cond, handler)								\
	if (cond) {												\
		DebugStr((ConstStr255Param)"\p"#cond " goto " #handler);	\
		goto handler;										\
	} else 0
#else
#define FailIf(cond, handler)								\
	if (cond) {												\
		goto handler;										\
	} else 0
#endif

// This checks for the exception, and if true do the action and goto handler
#ifdef _DEBUG
#define FailWithAction(cond, action, handler)				\
	if (cond) {												\
		DebugStr((ConstStr255Param)"\p"#cond " goto " #handler);	\
		{ action; }											\
		goto handler;										\
	} else 0
#else
#define FailWithAction(cond, action, handler)				\
	if (cond) {												\
		{ action; }											\
		goto handler;										\
	} else 0
#endif

// This will insert debugging code in the application to check conditions
// and displays the condition in the debugger if true.  This code is
// completely removed in non-debug builds.
#ifdef _DEBUG
#define FailMessage(cond)									\
	if (cond)												\
		DebugStr((ConstStr255Param)"\p"#cond);				\
	else 0
#else
#define FailMessage(cond)
#endif

// This allows you to test for the result of a condition (i.e. CloseComponent)
// and break if it returns a non zero result, otherwise it ignores the result.
// When a non-debug build is done, the result is ignored.
#ifdef _DEBUG
#define ErrorMessage(cond)									\
	if (cond)												\
		DebugStr((ConstStr255Param)"\p"#cond);				\
	else 0
#else
#define ErrorMessage(cond)		cond
#endif

// This will display a given message in the debugger, this code is completely
// removed in non-debug builds.
#ifdef _DEBUG
#define DebugMessage(s)			DebugString((ConstStr255Param)s)
#else
#define DebugMessage(s)
#endif

