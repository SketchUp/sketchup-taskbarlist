#include "TaskbarProgress.h"


Win7TaskbarProgress::Win7TaskbarProgress()
{
  m_pITaskBarList3 = NULL;
  m_bFailed = false;
}

Win7TaskbarProgress::~Win7TaskbarProgress()
{
  if (m_pITaskBarList3)   
  {
    m_pITaskBarList3->Release();
    CoUninitialize();
  }
}

HRESULT Win7TaskbarProgress::SetProgressState(HWND hwnd, TBPFLAG flag)
{
  if (Init())
	{
		return m_pITaskBarList3->SetProgressState(hwnd, flag);
	}
	// http://msdn.microsoft.com/en-us/library/cc231198.aspx
	return 0x20000000 | 0x00000001;
}

HRESULT Win7TaskbarProgress::SetProgressValue(HWND hwnd,
	ULONGLONG ullCompleted, ULONGLONG ullTotal)
{
  if (Init())
	{
		return m_pITaskBarList3->SetProgressValue(hwnd, ullCompleted, ullTotal);
	}
	// http://msdn.microsoft.com/en-us/library/cc231198.aspx
	return 0x20000000 | 0x00000001;
}

bool Win7TaskbarProgress::Init()
{
  if (m_pITaskBarList3)
	{
		return true;
	}

  if (m_bFailed)
	{
		return false;
	}

  // Initialize COM for this thread...
  CoInitialize(NULL);

  CoCreateInstance(CLSID_TaskbarList, NULL, CLSCTX_INPROC_SERVER,
		IID_ITaskbarList3, (void **)&m_pITaskBarList3);

  if (m_pITaskBarList3)
	{
		return true;
	}

  m_bFailed = true;
  CoUninitialize();
  return false;
}