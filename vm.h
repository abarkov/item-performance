#ifndef VM_H_INCLUDED
#define VM_H_INCLUDED

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
    ADD_LL0_LLI    // LL0+= Instr.m_param_ll
  };
  class Instr
  {
  public:
    Cmd m_cmd;
    Longlong_null m_param_ll;
    Instr()
     :m_cmd(NOP), m_param_ll(0, false)
    { }
    Instr(Cmd cmd)
     :m_cmd(cmd), m_param_ll(Longlong_null())
    { }
    Instr(Cmd cmd, const Longlong_null &param)
     :m_cmd(cmd), m_param_ll(param)
    { }
  };

  Instr m_instr[100];
  size_t m_instr_count;
  Longlong_null m_ll0;
  Longlong_null m_ll1;

  VM() :m_instr_count(0) { }
  void exec_instr(const Instr &i);
  bool add(const Instr &instr)
  {
    m_instr[m_instr_count]= instr;
    m_instr_count++;
    return false;
  }
  void exec();
};

#endif
