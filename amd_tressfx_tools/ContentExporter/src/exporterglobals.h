//-------------------------------------------------------------------------------------
// ExporterGlobals.h
//  
// Common includes for the samples content exporter.
// Version number, display name, and vendor are stored here, and each exporter
// front-end project uses these defines in their builds.
//  
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//  
// Advanced Technology Group (ATG)
// Copyright (C) Microsoft Corporation. All rights reserved.
//
// http://go.microsoft.com/fwlink/?LinkId=226208
//-------------------------------------------------------------------------------------
#pragma once

#define CONTENT_EXPORTER_MAJOR_VERSION 3
#define CONTENT_EXPORTER_MINOR_VERSION 0
#define CONTENT_EXPORTER_REVISION 0
#define MAKEVERSION(major, minor, rev) "" #major "." #minor "." #rev
#define CONTENT_EXPORTER_VERSION MAKEVERSION( CONTENT_EXPORTER_MAJOR_VERSION, CONTENT_EXPORTER_MINOR_VERSION, CONTENT_EXPORTER_REVISION )

#define CONTENT_EXPORTER_GLOBAL_TITLE "Samples Content Exporter"
#define CONTENT_EXPORTER_SETTINGS_TOKEN "SamplesContentExporter"
#define CONTENT_EXPORTER_VENDOR "Microsoft Advanced Technology Group"
#define CONTENT_EXPORTER_COPYRIGHT "Copyright (c) Microsoft Corporation.  All Rights Reserved."
#define CONTENT_EXPORTER_FILE_EXTENSION "xatg"
#define CONTENT_EXPORTER_FILE_FILTER "*." CONTENT_EXPORTER_FILE_EXTENSION
#define CONTENT_EXPORTER_FILE_FILTER_DESCRIPTION "XATG Samples Content File"
#define CONTENT_EXPORTER_BINARYFILE_EXTENSION "sdkmesh"
#define CONTENT_EXPORTER_BINARYFILE_FILTER "*." CONTENT_EXPORTER_BINARYFILE_EXTENSION
#define CONTENT_EXPORTER_BINARYFILE_FILTER_DESCRIPTION "SDK Mesh Binary File"

#ifdef _DEBUG
#define BUILD_FLAVOR "Debug"
#else
#define BUILD_FLAVOR "Release"
#endif

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the CONTENTEXPORTER_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// CONTENTEXPORTER_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef CONTENTEXPORTER_EXPORTS
#define CONTENTEXPORTER_API __declspec(dllexport)
#else
#define CONTENTEXPORTER_API __declspec(dllimport)
#endif

#ifndef ARRAYSIZE
#define ARRAYSIZE(A) (sizeof(A)/sizeof((A)[0]))
#endif
