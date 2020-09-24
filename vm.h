#ifndef VM_H_INCLUDED
#define VM_H_INCLUDED

#include <vector>

#include "sql_type_bool.h"
#include "sql_type_int.h"
#include "sql_type_double.h"

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
  };
  class Instr
  {
  public:
    Cmd m_cmd;
    Longlong_null m_param_ll;
    Double_null m_param_d;
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
  };

  std::vector<Instr> m_instr;
  Longlong_null m_ll0;
  Longlong_null m_ll1;
  Double_null   m_d0;
  Double_null   m_d1;
  Bool_null     m_b0;

  VM() { }
  void exec_instr(const Instr &i);
  void push_back(const Instr &instr)
  {
    m_instr.push_back(instr);
  }
  void exec();
};

#endif
