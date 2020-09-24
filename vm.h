#ifndef VM_H_INCLUDED
#define VM_H_INCLUDED

#include <vector>

#include "sql_type_bool.h"
#include "sql_type_int.h"
#include "sql_type_double.h"
#include "sql_type_decimal.h"

class VM
{
public:
  enum Cmd {
    NOP= 0,
    MOV_LL_TO_LL0, // Move Longlong_null constant to register LL0
    MOV_LL_TO_LL1, // Move Longlong_null constant to register LL1
    ADD_LL0_LL1,   // LL0+= LL1
    ADD_LL0_LLI,   // LL0+= Instr.m_param_ll
    NEG_LL0,       // LL0= -LL0
    EQ_LL0_LLI,    // LL0 == LLI

    MOV_D_TO_D0,   // Move Longlong_null constant to register LL0
    MOV_D_TO_D1,   // Move Longlong_null constant to register LL1
    ADD_D0_D1,     // LL0+= LL1
    ADD_D0_DI,     // LL0+= Instr.m_param_ll
    NEG_D0,        // D0= -D0
    EQ_D0_DI,      // D0 == DI

    MOV_DEC_TO_DEC0, // Move Longlong_null constant to register DEC0
    MOV_DEC_TO_DEC1, // Move Longlong_null constant to register DEC1
    ADD_DEC0_DEC1,   // DEC0+= DEC1
    ADD_DEC0_DECI,   // DEC0+= Instr.m_param_dec
    NEG_DEC0,        // DEC0= -DEC0
    EQ_DEC0_DECI,    // DDEC0 == Instr.m_param_dec
  };
  class Instr
  {
  public:
    Cmd m_cmd;
    Longlong_null m_param_ll;
    Double_null m_param_d;
    Decimal_null m_param_dec;
    Instr()
     :m_cmd(NOP)
    { }
    Instr(Cmd cmd)
     :m_cmd(cmd)
    { }
    Instr(Cmd cmd, const Longlong_null &param)
     :m_cmd(cmd), m_param_ll(param)
    { }
    Instr(Cmd cmd, const Double_null &param)
     :m_cmd(cmd), m_param_d(param)
    { }
    Instr(Cmd cmd, const Decimal_null &param)
     :m_cmd(cmd), m_param_dec(param)
    { }
  };

  std::vector<Instr> m_instr;
  Longlong_null m_ll0;
  Longlong_null m_ll1;
  Double_null   m_d0;
  Double_null   m_d1;
  Bool_null     m_b0;
  Decimal_null  m_dec0;
  Decimal_null  m_dec1;

  VM() { }
  void exec_instr(const Instr &i);
  void push_back(const Instr &instr)
  {
    m_instr.push_back(instr);
  }
  void exec();
};

#endif
