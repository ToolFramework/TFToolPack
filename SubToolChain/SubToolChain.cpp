#include "SubToolChain.h"
#include "ToolChain.h"

SubToolChain::SubToolChain():Tool(){}


bool SubToolChain::Initialise(std::string configfile, DataModel &data){

  if(configfile!="")  m_variables.Initialise(configfile);
  //m_variables.Print();

  m_data= &data;
  m_log= m_data->Log;

  std::string tools_conf="";
  int errorlevel=0;

  if(!m_variables.Get("verbose",m_verbose)) m_verbose=1;
  if(!m_variables.Get("Tools_file",tools_conf)) return false;
  if(!m_variables.Get("error_level",errorlevel)) errorlevel=2;

  m_subtoolchain=new ToolChain(m_verbose, errorlevel, true, false, "", false, m_data);

  if(!m_subtoolchain->LoadTools(tools_conf)) return false;
  if(!m_variables.Get("repeats_var", m_repeats_var)) m_repeats_var="";
  m_repeates=1;
  return m_subtoolchain->Initialise();
}


bool SubToolChain::Execute(){
 
  if(!m_repeats_var.empty() && !m_data->CStore.Get(m_repeats_var, m_repeats)){
    throw std::runtime_error("SubToolChain:Execute : repeat flag set, but no variable in m_data->CStore matches the name "+repeats_var+"\n");
  }  

  return m_subtoolchain->Execute(m_repeats);
}


bool SubToolChain::Finalise(){
  
  bool ret = m_subtoolchain->Finalise();
  delete m_subtoolchain;
  m_subtoolchain=0;

  return ret;
}

