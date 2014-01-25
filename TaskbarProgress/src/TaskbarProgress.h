#pragma once

#include <ShObjIdl.h>


// http://stackoverflow.com/a/15002979/486990
class Win7TaskbarProgress  
{
public:
  Win7TaskbarProgress();
  virtual ~Win7TaskbarProgress();

  HRESULT SetProgressState(HWND hwnd, TBPFLAG flag);

  HRESULT SetProgressValue(HWND hwnd,
		ULONGLONG ullCompleted, ULONGLONG ullTotal);

private:
  bool Init();
  ITaskbarList3* m_pITaskBarList3;
  bool m_bFailed;
};
