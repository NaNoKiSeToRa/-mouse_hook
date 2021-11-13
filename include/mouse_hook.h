#pragma once

#include <cstdlib>
#include <cstdint>
#include <iostream>
#include <windows.h>

#ifdef __cplusplus
extern "C"
{
  __declspec(dllexport) int64_t __stdcall hook_callback(int32_t n_code, uintptr_t w_param, int64_t l_param);
}
#endif

typedef struct __mouse_hook_t
{
  static __mouse_hook_t& instance()
  {
    static mouse_hook_t m_hook{};
    return m_hook;
  }

  MSG msg;
  HHOOK hook;

  __mouse_hook_t& set_hook()
  {
    if ( !(hook == SetWindowsHookEx(WH_MOUSE_LL, hook_callback, nullptr, 0)) )
    {
      std::cout << "error: " << GetLastError() << '\n';
    }
    return *this;
  }

  __mouse_hook_t& unhook()
  {
    UnhookWindowsHookEx(hook);
    return *this;
  }

  int32_t get_messages()
  {
    while ( msg.message != WM_QUIT )
    {
      if ( PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE) )
      {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
      }
    }
    unhook();
    return static_cast<int>(msg.wParam);
  }
} mouse_hook_t;

__declspec(dllexport) int64_t __stdcall hook_callback(int32_t n_code, uintptr_t w_param, int64_t l_param)
{
  MSLLHOOKSTRUCT* m_struct = reinterpret_cast<MSLLHOOKSTRUCT*>(l_param);
  if ( n_code == HC_ACTION )
  {
    if ( m_struct != nullptr )
      printf_s("<x: %i, y: %i>\n", m_struct->pt.x, m_struct->pt.y);
    switch ( w_param )
    {
      case WM_LBUTTONUP:
        std::cout << "mouse left key up" << '\n';
        break;

      case WM_LBUTTONDOWN:
        std::cout << "mouse left key down" << '\n';
        break;

      case WM_RBUTTONUP:
        std::cout << "mouse right key up" << '\n';
        break;

      case WM_RBUTTONDOWN:
        std::cout << "mouse right key down" << '\n';
        break;

      default:
        break;
    }
  }
  return CallNextHookEx(mouse_hook_t::instance().hook, n_code, w_param, l_param);
}
