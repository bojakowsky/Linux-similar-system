#include "./sop/processor/interpreter.h"
sop::interpreter::InterpreterHandler::InterpreterHandler(sop::logger::Logger* logger):logger(logger)
{
  this->logger->logProcessor(3,"Interpreter initialized!");
}
char sop::interpreter::InterpreterHandler::getByteFromMemory(boost::shared_ptr<sop::process::Process> p)
{
  //char x = sop::memory::.. pobranie byte'u danych od Fiszera  
  //char x = sop::memory::LogicalMemory::read(
  //return x;
  return '0';
}

std::string sop::interpreter::InterpreterHandler::interpretLine(boost::shared_ptr<sop::process::Process> p)
{
  
  pickCommandPart(_program_line);
  if (_program_line.size() > 3)
    pickDataPart(_program_line);
  p->procek.ip += _program_line.size();
  if (_command_part == "ADD")
  {
    this->logger->logProcessor(6,"Interpreting ADD cmd");
    char src = _data_part[0];
    char dst = _data_part[2];
    sop::processor::ProcessorHandler::addSourceRegisterToDestinationRegister(&p->procek,dst,src);    
  }
  else if (_command_part == "ADV")
  {
    this->logger->logProcessor(6,"Interpreting ADV cmd");
    char reg = _data_part[0];
    std::string s_value = _data_part.substr(2);
    uint16_t value = sop::StringConverter::convertStringToHex<uint16_t>(s_value);
    sop::processor::ProcessorHandler::registerIncrementByValue(&p->procek,reg,value);
  }
  else if (_command_part == "SUB")
  {
    this->logger->logProcessor(6,"Interpreting SUB cmd");
    char src = _data_part[0];
    char dst = _data_part[2];
    sop::processor::ProcessorHandler::subtractDestinationRegisterBySourceRegister(&p->procek,dst,src);   
  }
  else if (_command_part == "SUV")
  {
    this->logger->logProcessor(6,"Interpreting SUV  cmd");
    char reg = _data_part[0];
    std::string s_value = _data_part.substr(2);
    uint16_t value = sop::StringConverter::convertStringToHex<uint16_t>(s_value);
    sop::processor::ProcessorHandler::registerDecrementByValue(&p->procek,reg,value);   
    
  }
  else if (_command_part == "MUL")
  {
    this->logger->logProcessor(6,"Interpreting MUL cmd");
    sop::processor::ProcessorHandler::multipliesAandB(&p->procek);
  }
  else if (_command_part == "DIV")
  {
    this->logger->logProcessor(6,"Interpreting DIV cmd");
    sop::processor::ProcessorHandler::dividesAandB(&p->procek);
  }
  else if (_command_part == "INC")
  {
    this->logger->logProcessor(6,"Interpreting INC cmd");
    char reg = _data_part[0];
    sop::processor::ProcessorHandler::registerIncrement(&p->procek,reg);
  }
  else if (_command_part == "DEC")
  {
    this->logger->logProcessor(6,"Interpreting DEC cmd");
    char reg = _data_part[0];
    sop::processor::ProcessorHandler::registerDecrement(&p->procek,reg);
  }
  else if (_command_part == "CMP")
  {
    this->logger->logProcessor(6,"Interpreting CMP cmd");
    char src = _data_part[0];
    char dst = _data_part[2]; 
    sop::processor::ProcessorHandler::compareRegisters(&p->procek,src,dst);    
  }
  else if (_command_part == "OR")
  {
    this->logger->logProcessor(6,"Interpreting OR cmd");
    char first = _data_part[0];
    char second = _data_part[2];
    sop::processor::ProcessorHandler::bitwiseOR(&p->procek,first,second);  
  }
  else if (_command_part == "AND")
  {
    this->logger->logProcessor(6,"Interpreting AND cmd");
    char first = _data_part[0];
    char second = _data_part[2];
    sop::processor::ProcessorHandler::bitwiseAND(&p->procek,first,second); 
  }
  else if (_command_part == "XOR")
  {
    /////////// narazie nie
  }
  else if (_command_part == "NEG")
  {
    this->logger->logProcessor(6,"Interpreting NEG cmd");
    char first = _data_part[0];
    sop::processor::ProcessorHandler::bitNEG(&p->procek,first); 
  }
  else if (_command_part == "SHR")
  {
    /////////// narazie nie
  }
  else if (_command_part == "SHL")
  {
    /////////// narazie nie
  }
  else if (_command_part == "MOV")
  {
    this->logger->logProcessor(6,"Interpreting MOV cmd");
    char reg = _data_part[0];
    std::string s_value = _data_part.substr(2);
    uint16_t value = sop::StringConverter::convertStringToHex<uint16_t>(s_value);
    sop::processor::ProcessorHandler::setRegisterField(&p->procek,reg,value);
  }
  else if (_command_part == "MOR")
  {
    this->logger->logProcessor(6,"Interpreting MOR cmd");
    char src = _data_part[0];
    char dst = _data_part[2];
    sop::processor::ProcessorHandler::copySourceRegisterToDestinationRegister(&p->procek,dst,src);
  }
  else if (_command_part == "PUS")
  {
    this->logger->logProcessor(6,"Interpreting PUS cmd");
    char reg = _data_part[0];
    sop::processor::ProcessorHandler::addRegisterToStack(&p->procek,reg);
  }
  else if (_command_part == "PUSF")
  {
    //////////narazie nie
  }
  else if (_command_part == "POP")
  {
    this->logger->logProcessor(6,"Interpreting POP cmd");
    char reg = _data_part[0];
    sop::processor::ProcessorHandler::popFromStack(&p->procek,reg);
  }
  else if (_command_part == "POF")
  {
    ////narazie nie
  }
  else if (_command_part == "MOM")
  {
    this->logger->logProcessor(6,"Interpreting MOM cmd");
    std::string buf = "MOV D,0117\nMOM\nMOR B,C\nSSS B\nWRC\nSWP B\nWRC\nADV D,0002\nMOM\nMOR B,C\nSWP B\nWRC\nSWP B\nWRC\nADV D,0002\nMOM\nMOR B,C\nSWP B\nWRC\nSWP B\nWRC\nADV D,0002\nMOM\nMOR B,C\nSWP B\nWRC\nSWP B\nWRC\nADV D,0002\nMOM\nMOR B,C\nSWP B\nWRC\nSWP B\nWRC\nADV D,0002\nMOM\nMOR B,C\nSWP B\nWRC\nSWP B\nWRC\nADV D,0002\nEXT 0000\n\nHello world!\n";
    uint16_t *off = &p->procek.c;
    uint8_t *byte = (uint8_t*) off;
    byte[1] = buf[(p->procek.d)]; 
    byte[0] = buf[(p->procek.d)+1]; 
    //MOM - wczytuje zaw. z pamieci zapisanym pod adresem w rej. D do rej. C
    //funkcja fiszerowa
  }

  else if (_command_part == "EXT")
  {
    return "EXT";//+value // dodaæ wartoœæ z jak¹ siê program zakonczyl 
    //pobraæ wartosc zakocnzenie programu od Michala
  }
  else if (_command_part == "CAL")
  {
    this->logger->logProcessor(6,"Interpreting CAL cmd");
    sop::processor::ProcessorHandler::savesReturnAdresOnStack(&p->procek);
  }
  else if (_command_part == "RET")
  {
    this->logger->logProcessor(6,"Interpreting RET cmd");
    sop::processor::ProcessorHandler::loadsValueFromStackAndJumpOnIt(&p->procek);
  }
  else if (_command_part == "JMP")
  {
    this->logger->logProcessor(6,"Interpreting JMP cmd");
    std::string s_value = _data_part.substr(0);
    sop::processor::ProcessorHandler::doJMP(&p->procek);
  }
  else if (_command_part == "JIZ")
  {
    this->logger->logProcessor(6,"Interpreting JIZ cmd");
    std::string s_value = _data_part.substr(0);
    sop::processor::ProcessorHandler::doJIZ(&p->procek);
  }
  else if (_command_part == "JNZ")
  {
    this->logger->logProcessor(6,"Interpreting JNZ cmd");
    std::string s_value = _data_part.substr(0);
    sop::processor::ProcessorHandler::doJNZ(&p->procek);
  }
  else if (_command_part == "JIA")
  {
    this->logger->logProcessor(6,"Interpreting JIA cmd");
    std::string s_value = _data_part.substr(0);
    sop::processor::ProcessorHandler::doJIA(&p->procek);
  }
  else if (_command_part == "JAE")
  {
    this->logger->logProcessor(6,"Interpreting JAE cmd");
    std::string s_value = _data_part.substr(0);
    sop::processor::ProcessorHandler::doJAE(&p->procek);
  }
  else if (_command_part == "JIB")
  {
    this->logger->logProcessor(6,"Interpreting JIB cmd");
    std::string s_value = _data_part.substr(0);
    sop::processor::ProcessorHandler::doJIB(&p->procek);
  }
  
  else if (_command_part == "JBE")
  {
    this->logger->logProcessor(6,"Interpreting JBE cmd");
    std::string s_value = _data_part.substr(0);
    sop::processor::ProcessorHandler::doJNZ(&p->procek);
  }
  else if (_command_part == "REU")
  {
    this->logger->logProcessor(6,"Interpreting REU cmd");
    sop::processor::ProcessorHandler::REBbut16(&p->procek);
  }
  else if (_command_part == "REB")
  {
    this->logger->logProcessor(6,"Interpreting REB cmd");
    sop::processor::ProcessorHandler::readOneByteFromInputAndSavesItOnYoungestByte(&p->procek);
  }
  else if (_command_part == "WRB")
  {
    this->logger->logProcessor(6,"Interpreting WRB cmd");
    sop::processor::ProcessorHandler::printsOutYoungestByte(&p->procek);
  }
  else if (_command_part == "WRC")
  {
    this->logger->logProcessor(6,"Interpreting WRC cmd");
    sop::processor::ProcessorHandler::printsOutYoungestByteAsASCII(&p->procek);
  }
  else if (_command_part == "WRI")
  {
    this->logger->logProcessor(6,"Interpreting WRI cmd");
    sop::processor::ProcessorHandler::printsOutRegisterWithSign(&p->procek);
  }
  else if (_command_part == "WRU")
  {
    this->logger->logProcessor(6,"Interpreting WRU cmd");
    sop::processor::ProcessorHandler::printsOutRegisterWithoutSign(&p->procek);
  }
  else if (_command_part == "SWP")
  {
    this->logger->logProcessor(6,"Interpreting SWP cmd");
    char reg = _data_part[0];
    sop::processor::ProcessorHandler::swapBytes(&p->procek, reg);
  }
  else if (_command_part == "FRK")
  {
    this->logger->logProcessor(6,"Interpreting FRK cmd");
  }
  else if (_command_part == "EXC")
  {
    this->logger->logProcessor(6,"Interpreting EXC cmd");
  }
  else if (_command_part == "WAT")
  {
    this->logger->logProcessor(6,"Interpreting WAT cmd");
  }
  //FRK wywolywane od M. Pietrzaka, 0 gdy potomek, PID potomka gdy dziecko
  //PID do rej A

  //EXC otwiera plik o nazwie zapisanej w pamieci pod adresem pam w rej B
  //w rejestr C 0- nie udalo sie albo 1  - udalo


  //WAT czeka na zakonczenie procesu potomka o PID zap w rej A.
  //Pietrzak w tym momencie przeszuka ziomka o podanym PID, jezeli go nei znajdzie -> kill him, albo nie bedzie potomkiem -> kill him
  //w przeciwnim razie czekam az Michal mi go wrzuci spowrotem
  //na WAT wyrzucam z schedulera tak jak EXT
  _program_line = "";
  return "X";
}

void sop::interpreter::InterpreterHandler::pickDataPart(std::string s)
{
  if (s.size() > 3)
  _data_part = s.substr(4);
}

void sop::interpreter::InterpreterHandler::pickCommandPart(std::string s)
{
  _command_part = s.substr(0,3);
}

void sop::interpreter::InterpreterHandler::printCommandPart()
{
  std::cout<<_command_part<<std::endl;
}

void sop::interpreter::InterpreterHandler::printDataPart()
{
  std::cout<<_command_part<<std::endl;
}

std::string sop::interpreter::InterpreterHandler::getDataPart()
{
  return _data_part;
}

std::string sop::interpreter::InterpreterHandler::getCommandPart()
{
  return _command_part;
}
void sop::interpreter::InterpreterHandler::buildProgramLine(boost::shared_ptr<sop::process::Process> p)
{
  
  _program_line = "";
  this->logger->logProcessor(6,"Building program line");
  
  uint16_t ibuf = p->procek.ip;
  std::string buf = p->code_program;
  char getChar = 's';
  while (getChar!='\n')
  {
    _program_line += buf[ibuf];
    getChar = buf[ibuf];
    ibuf++;
    //program_line+ = getChar;
  }
}
void sop::interpreter::InterpreterHandler::interpreterReset()
{
  _command_part = "";
  _data_part = "";
  _program_line = "";
}