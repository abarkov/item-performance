#include <cstddef>
#include "vm.h"

inline void VM::exec_instr(const Instr &i)
{
#if 0
  // Only "ll" operations are implemented. Run `./test --vm ll_plus` to test.
  // Jump table is not faster than switch
  static const void *jump_table[] =
  {
     &&nop,
     &&mov_ll_to_ll0,
     &&mov_ll_to_ll1,
     &&add_ll0_ll1,
     &&add_ll0_lli,
     &&neg_ll0
  };
  goto *jump_table[i.m_cmd];

nop:
  return;

mov_ll_to_ll0:
  m_ll0= i.m_param_ll;
  return;

mov_ll_to_ll1:
  m_ll1= i.m_param_ll;
  return;

add_ll0_ll1:
  m_ll0+= m_ll1;
  return;

add_ll0_lli:
  m_ll0+= i.m_param_ll;
  return;

neg_ll0:
  m_ll0.neg();
  return;

#else

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
  case NEG_LL0:
    m_ll0.neg();
    break;
  case EQ_LL0_LLI: // Not null comparison
    m_b0= Bool_null(m_ll0.value == i.m_param_ll.value);
    break;

  case MOV_D_TO_D0:
    m_d0= i.m_param_d;
    break;
  case MOV_D_TO_D1:
    m_d1= i.m_param_d;
    break;
  case ADD_D0_D1:
    m_d0+= m_d1;
    break;
  case ADD_D0_DI:
    m_d0+= i.m_param_d;
    break;
  case NEG_D0:
    m_d0.neg();
    break;
 case EQ_D0_DI: // Not null comparison
    m_b0= Bool_null(m_d0.value == i.m_param_d.value);
    break;

  case MOV_DEC_TO_DEC0:
    m_dec0= i.m_param_dec;
    break;
  case MOV_DEC_TO_DEC1:
    m_dec1= i.m_param_dec;
    break;
  case ADD_DEC0_DEC1:
    m_dec0+= m_dec1;
    break;
  case ADD_DEC0_DECI:
    m_dec0+= i.m_param_dec;
    break;
  case NEG_DEC0:
    m_dec0.neg();
    break;
 case EQ_DEC0_DECI: // Not null comparison
    m_b0= Bool_null(decimal_cmp(&m_dec0, &i.m_param_dec) == 0);
    break;
  case NOP:
    break;
  }
#endif
}


void VM::exec()
{
  for (Instr i: m_instr)
    exec_instr(i);
}
