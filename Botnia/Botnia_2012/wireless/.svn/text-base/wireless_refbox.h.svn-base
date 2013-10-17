// MyComm.h: interface for the CMyComm class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYCOMM_H__427F08F4_94BD_41AE_8EC6_8FA76F14BC94__INCLUDED_)
#define AFX_MYCOMM_H__427F08F4_94BD_41AE_8EC6_8FA76F14BC94__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "cnComm.h"

class CMyComm : public cnComm
{
public:
	CMyComm(bool fAutoBeginThread = true, DWORD dwIOMode = FILE_FLAG_OVERLAPPED)
			: cnComm(fAutoBeginThread,dwIOMode)
	{
		Init();
	};
        virtual ~CMyComm() {}
public:

//		virtual void OnReceive()
//		{
//		}
};

#endif // !defined(AFX_MYCOMM_H__427F08F4_94BD_41AE_8EC6_8FA76F14BC94__INCLUDED_)
