#include "../include/mouse_hook.h"

int main()
{
  mouse_hook_t::instance().set_hook();
  return mouse_hook_t::instance().get_messages();
}

