// Copyright (C) 2015 Jérôme Leclercq
// This file is part of the "Nazara Engine - Core module"
// For conditions of distribution and use, see copyright notice in Config.hpp

#pragma once

#ifndef NAZARA_ENUMS_CORE_HPP
#define NAZARA_ENUMS_CORE_HPP

enum nzCoordSys
{
	nzCoordSys_Global,
	nzCoordSys_Local,

	nzCoordSys_Max = nzCoordSys_Local
};

enum nzCursorPosition
{
	nzCursorPosition_AtBegin,   // Début du fichier
	nzCursorPosition_AtCurrent, // Position du pointeur
	nzCursorPosition_AtEnd,     // Fin du fichier

	nzCursorPosition_Max = nzCursorPosition_AtEnd
};

enum nzEndianness
{
	nzEndianness_Unknown = -1,

	nzEndianness_BigEndian,
	nzEndianness_LittleEndian,

	nzEndianness_Max = nzEndianness_LittleEndian
};

enum nzErrorFlag
{
	nzErrorFlag_None = 0,

	nzErrorFlag_Silent                 = 0x1,
	nzErrorFlag_SilentDisabled         = 0x2,
	nzErrorFlag_ThrowException         = 0x4,
	nzErrorFlag_ThrowExceptionDisabled = 0x8,

	nzErrorFlag_Max = nzErrorFlag_ThrowExceptionDisabled*2-1
};

enum nzErrorType
{
	nzErrorType_AssertFailed,
	nzErrorType_Internal,
	nzErrorType_Normal,
	nzErrorType_Warning,

	nzErrorType_Max = nzErrorType_Warning
};

enum nzHash
{
	nzHash_CRC32,
	nzHash_Fletcher16,
	nzHash_MD5,
	nzHash_SHA1,
	nzHash_SHA224,
	nzHash_SHA256,
	nzHash_SHA384,
	nzHash_SHA512,
	nzHash_Whirlpool,

	nzHash_Max = nzHash_Whirlpool
};

enum nzOpenModeFlags
{
	nzOpenMode_Current   = 0x00, // Utilise le mode d'ouverture actuel

	nzOpenMode_Append    = 0x01, // Empêche l'écriture sur la partie déjà existante et met le curseur à la fin
	nzOpenMode_Lock      = 0x02, // Empêche le fichier d'être modifié tant qu'il est ouvert
	nzOpenMode_ReadOnly  = 0x04, // Ouvre uniquement en lecture
	nzOpenMode_ReadWrite = 0x08, // Ouvre en lecture/écriture
	nzOpenMode_Text      = 0x10, // Ouvre en mode texte
	nzOpenMode_Truncate  = 0x20, // Créé le fichier s'il n'existe pas et le vide s'il existe
	nzOpenMode_WriteOnly = 0x40, // Ouvre uniquement en écriture, créé le fichier s'il n'existe pas

	nzOpenMode_Max = nzOpenMode_WriteOnly
};

enum nzParameterType
{
	nzParameterType_Boolean,
	nzParameterType_Float,
	nzParameterType_Integer,
	nzParameterType_None,
	nzParameterType_Pointer,
	nzParameterType_String,
	nzParameterType_Userdata,

	nzParameterType_Max = nzParameterType_Userdata
};

enum nzPlugin
{
	nzPlugin_Assimp,
	nzPlugin_FreeType
};

enum nzPrimitiveType
{
	nzPrimitiveType_Box,
	nzPrimitiveType_Cone,
	nzPrimitiveType_Plane,
	nzPrimitiveType_Sphere,

	nzPrimitiveType_Max = nzPrimitiveType_Sphere
};

enum nzProcessorCap
{
	nzProcessorCap_x64,
	nzProcessorCap_AVX,
	nzProcessorCap_FMA3,
	nzProcessorCap_FMA4,
	nzProcessorCap_MMX,
	nzProcessorCap_XOP,
	nzProcessorCap_SSE,
	nzProcessorCap_SSE2,
	nzProcessorCap_SSE3,
	nzProcessorCap_SSSE3,
	nzProcessorCap_SSE41,
	nzProcessorCap_SSE42,
	nzProcessorCap_SSE4a,

	nzProcessorCap_Max = nzProcessorCap_SSE4a
};

enum nzProcessorVendor
{
	nzProcessorVendor_Unknown = -1,

	nzProcessorVendor_AMD,
	nzProcessorVendor_Centaur,
	nzProcessorVendor_Cyrix,
	nzProcessorVendor_Intel,
	nzProcessorVendor_KVM,
	nzProcessorVendor_HyperV,
	nzProcessorVendor_NSC,
	nzProcessorVendor_NexGen,
	nzProcessorVendor_Rise,
	nzProcessorVendor_SIS,
	nzProcessorVendor_Transmeta,
	nzProcessorVendor_UMC,
	nzProcessorVendor_VIA,
	nzProcessorVendor_VMware,
	nzProcessorVendor_Vortex,
	nzProcessorVendor_XenHVM,

	nzProcessorVendor_Max = nzProcessorVendor_XenHVM
};

enum nzSphereType
{
	nzSphereType_Cubic,
	nzSphereType_Ico,
	nzSphereType_UV,

	nzSphereType_Max = nzSphereType_UV
};

enum nzStreamOptionFlags
{
	nzStreamOption_None = 0,

	nzStreamOption_Text = 0x1,

	nzStreamOption_Max = nzStreamOption_Text*2-1
};

enum nzTernary
{
	nzTernary_False,
	nzTernary_True,
	nzTernary_Unknown,

	nzTernary_Max = nzTernary_Unknown
};

#endif // NAZARA_ENUMS_CORE_HPP
