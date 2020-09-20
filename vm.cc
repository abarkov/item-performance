#include <cstddef>
#include "vm.h"

void VM::exec_instr(const Instr &i)
{
  switch (i.m_cmd) {
  case MOV_LL_TO_LL0:
    m_ll0= i.m_param_ll;
    break;
  case MOV_LL_TO_LL1:
    m_ll1= i.m_param_ll;
    break;
  case ADD_LL0_LL1:
    m_ll0+= m_ll1;
    break;
  case ADD_LL0_LLI:
    m_ll0+= i.m_param_ll;
    break;
  }
}


void VM::exec()
{
  for (size_t i= 0; i < m_instr_count; i++)
    exec_instr(m_instr[i]);
}
