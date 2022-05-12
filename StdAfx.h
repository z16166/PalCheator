// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__E91A16C8_0EE3_4865_B608_C937F5475766__INCLUDED_)
#define AFX_STDAFX_H__E91A16C8_0EE3_4865_B608_C937F5475766__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#pragma pack(push, 1)

	#define NUM_PLAYERS  5                      //5个角色，李逍遥、赵灵儿、林月如、巫后、阿奴

	struct ExpLevel
	{
		int   m_RealEXP;                    //经验值
		int   m_InitialLevel;               //初始等级
	} m_EXP[NUM_PLAYERS];

	struct Resource
	{
		short m_ResID;                      //物品编号
		int   m_ResCount;                   //物品数量
	} m_Resource[256];

	struct GameSaveInfo                         //存盘文件*.rpg的格式
	{
		short m_SaveCount;                      //存盘次数
		BYTE  reserved1[0x28 - 0x00 - 2];
		int   m_Money;                          //金钱
		BYTE  reserved2[0x7C - 0x28 - 4];
		struct ExpLevel m_EXP[NUM_PLAYERS];
		BYTE  reserved3[0x244 - 0x7C  - 8*5];
		short m_Level[NUM_PLAYERS];             //修行等级
		BYTE  reserved4[0x250 - 0x244 - 2*5];
		short m_MaxHP[NUM_PLAYERS];             //体力上限
		BYTE  reserved5[0x25C - 0x250 - 2*5];
		short m_MaxMP[NUM_PLAYERS];             //真气上限
		BYTE  reserved6[0x268 - 0x25C - 2*5];
		short m_HP[NUM_PLAYERS];                //体力
		BYTE  reserved7[0x274 - 0x268 - 2*5];
		short m_MP[NUM_PLAYERS];                //真气
		BYTE  reserved8[0x2C8 - 0x274 - 2*5];
		short m_Attack[NUM_PLAYERS];            //武术
		BYTE  reserved9[0x2D4 - 0x2C8 - 2*5];
		short m_Spirit[NUM_PLAYERS];            //灵力
		BYTE  reserved10[0x2E0 - 0x2D4 - 2*5];
		short m_Defence[NUM_PLAYERS];           //防御
		BYTE  reserved11[0x2EC - 0x2E0 - 2*5];
		short m_Dexterity[NUM_PLAYERS];         //身法
		BYTE  reserved12[0x2F8 - 0x2EC - 2*5];
		short m_Luck[NUM_PLAYERS];              //吉运
		BYTE  reserved13[0x6C0 - 0x2F8 - 2*5];
		struct Resource m_Resource[256];
	};

#pragma pack(pop)

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__E91A16C8_0EE3_4865_B608_C937F5475766__INCLUDED_)
