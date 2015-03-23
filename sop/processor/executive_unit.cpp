#include "./sop/processor/executive_unit.h"
#include "./sop/processor/scheduler.h"
#include "./sop/processor/interpreter.h"
#include "./sop/system/shell.h"
#include <iostream>
sop::processor::ExecutiveUnit::ExecutiveUnit(sop::logger::Logger* logger):
  logger(logger),
  interpreter(logger),
  scheduler(logger),
  _standardQuantTime(5)
  {
    this->logger->logProcessor(3,"Processing unit initialized!");
    //this->logger->logProcessor(3,"Looking for errors...");
    resetQuantTime();
    //this->logger->logProcessor(3,"Initialization done.");
    //testerMethod();
  }

void sop::processor::ExecutiveUnit::testerMethod()
{
  boost::shared_ptr<sop::process::Process> test (new sop::process::Process());
  scheduler.addToActiveTaskArray(test);
}



sop::processor::ExecutiveUnit::~ExecutiveUnit(){
}
void sop::processor::ExecutiveUnit::activateProcessor()
{
   this->logger->logProcessor(6,"Activating processor");
  _runningProcess = scheduler.getHighestPriorityProcess();
  // if _runningProcess.isKilled then cout<<...
  _lastUsedProcess = _runningProcess;
}
short sop::processor::ExecutiveUnit::getQuantTimeLeft()
{
  return _quantTimeLeft;
}
void sop::processor::ExecutiveUnit::resetQuantTime()
{
  _quantTimeLeft = _standardQuantTime;
}
std::string sop::processor::ExecutiveUnit::processorTick()
{
  if (_runningProcess==0)
  {
    return "EXT";
  }
  else 
  {
    interpreter.buildProgramLine(_runningProcess); 
    std::string msg = interpreter.interpretLine(_runningProcess);
    interpreter.interpreterReset();
    return msg;
  }
  
}

void sop::processor::ExecutiveUnit::mainExecutiveLoop()
{
   this->logger->logProcessor(6,"Tick is proceeding");
  
  activateProcessor();
  int i = getQuantTimeLeft();
  while (i!=0)
  {
    if (processorTick()=="EXT")
    {
      this->logger->logProcessor(3, "Program has ended, Program has ended, Program has ended!");
      return;
    }
    i--;
  }
  _runningProcess->procek.age+=getQuantTimeLeft();
  scheduler.addToUnactiveTaskArray(_runningProcess);
  _runningProcess = nullptr; //disactivating processor
  if (scheduler.isEraChangeNeeded())
    scheduler.eraChange();
}
std::string choose_program(uint16_t x){
  if (x==1){
    //hello world//
    std::cout<<"You have choosen 'Hello world' porgram!"<<std::endl;
    return "MOV D,0117\nMOM\nMOR B,C\nSWP B\nWRC\nSWP B\nWRC\nADV D,0002\nMOM\nMOR B,C\nSWP B\nWRC\nSWP B\nWRC\nADV D,0002\nMOM\nMOR B,C\nSWP B\nWRC\nSWP B\nWRC\nADV D,0002\nMOM\nMOR B,C\nSWP B\nWRC\nSWP B\nWRC\nADV D,0002\nMOM\nMOR B,C\nSWP B\nWRC\nSWP B\nWRC\nADV D,0002\nMOM\nMOR B,C\nSWP B\nWRC\nSWP B\nWRC\nADV D,0002\nEXT 0000\n\nHello world!\n";
  }
  else if (x==2)
  {
    //male na duze//
    std::cout<<"You have choosen 'a to A, b to B' program, that changes small letters into great"<<std::endl;
    return "MOV B,0000\nREB\nMOV D,0053\nMOV A,0061\nCMP B,A\nJIB\nMOV A,007B\nCMP B,A\nJAE\nSUV B,0020\nWRC\nEXT 0000\n";
  }
  else if (x==3)
  {
    //odejmowanie//
    std::cout<<"You have choosen a program 'Division of 2 numbers!'"<<std::endl;
    return "REU\nMOR A,B\nREU\nSUB A,B\nMOR B,A\nWRI\nEXT 0000\n";
  }
  else if (x==4)
  {
    //parzystosc//
    std::cout<<"You have choosen a program that checks if number is even"<<std::endl;
    return "REU\nMOR A,B\nMOV B,0002\nDIV\nMOV A,0000\nCMP D,A\nMOV D,0055\nJNZ\nMOV B,0054\nWRC\nEXT 0001\nMOV B,0046\nWRC\nEXT 0000\n";
  }
  else if (x==5)
  {
    //suma_n//
    std::cout<<"You have choosen a 'sum_n' program"<<std::endl;
    return "REU\nMOV A,0000\nMOV C,0000\nMOV D,004F\nCMP A,B\nJIA\nADD C,A\nINC A\nMOV D,001A\nJMP\n\nMOR B,C\nWRU\nEXT 0000\n";
  } 
}
void sop::processor::ExecutiveUnit::cH_addProcess(const std::vector<const std::string> & params)
{
  if (!sop::system::Shell::hasParam(params,"-h"))
  {
    this->logger->logProcessor(3, "Processor is about to be added:");

    boost::shared_ptr<sop::process::Process> test (new sop::process::Process());
    test->priority = rand()%10-5;
    this->logger->logProcessor(3, "Processor is created, and in active task array:");
    scheduler.addToActiveTaskArray(test);
    std::cout<<"Choose 1 - 5, which file should he exec?\nINPUT:\n"<<std::endl;
    int i;
    std::cin>>i;
    test->code_name = "Program nr "+i;
    test->code_program = choose_program(i);
    
  }
  else if(sop::system::Shell::hasParam(params,"-h") || params.size()==1)
  {
    std::cout<<"addProcess [-h]"<<std::endl;
    std::cout<<"Adding process and setting program to execute"<<std::endl;
  }
}
void sop::processor::ExecutiveUnit::cH_showQuantTimeLeft(const std::vector<const std::string> & params)
{
  if (!sop::system::Shell::hasParam(params,"-h"))
  {
    this->logger->logProcessor(3, "Quant time is:");
    short q = getQuantTimeLeft();
    std::cout<<q<<std::endl;
  }
  else if(sop::system::Shell::hasParam(params,"-h") || params.size()==1)
  {
    std::cout<<"quanttime [-h]"<<std::endl;
    std::cout<<"Shows the quant time that is set"<<std::endl;
  }
}

void sop::processor::ExecutiveUnit::cH_showActiveTaskQueue(const std::vector<const std::string> & params)
{
  if (!sop::system::Shell::hasParam(params,"-h"))
  {
    this->logger->logProcessor(3, "Active task list:");
    scheduler.printOutActiveTasks();
  }
  else if(sop::system::Shell::hasParam(params,"-h") || params.size()==1)
  {
    std::cout<<"sacttask [-h]"<<std::endl;
    std::cout<<"Shows the queue of active tasks"<<std::endl;
  }
}

void sop::processor::ExecutiveUnit::cH_showUnactiveTaskQueue(const std::vector<const std::string> & params)
{
  if (!sop::system::Shell::hasParam(params,"-h"))
  {
    this->logger->logProcessor(3, "Unactive task list:");
    scheduler.printOutUnactiveTasks();
  }
  else if(sop::system::Shell::hasParam(params,"-h") || params.size()==1)
  {
    std::cout<<"suacttask [-h]"<<std::endl;
    std::cout<<"Shows the queue of unactive tasks"<<std::endl;
  }
}

void sop::processor::ExecutiveUnit::cH_showActualProcessorState(const std::vector<const std::string> & params)
{
   if (!sop::system::Shell::hasParam(params,"-h"))
   {
     if (_lastUsedProcess!=0)
     {
       //if(_lastUsedProcess) 
        ProcessorHandler::printOutProcessorState(&_lastUsedProcess->procek);
     }
     else 
     {
       std::cout<<"No processor state to show!"<<std::endl;
     }
   }
   else if(sop::system::Shell::hasParam(params,"-h") || params.size()==1)
   {
    std::cout<<"procstate [-h]"<<std::endl;
    std::cout<<"Shows the state of the processor and its fields"<<std::endl;
   }
}

void sop::processor::ExecutiveUnit::cH_showAnyProcessorState(const std::vector<const std::string> & params)
{
   if (1)
   {
    //to be done eventually

   }
   else if(sop::system::Shell::hasParam(params,"-h") || params.size()==1)
   {
    std::cout<<"procstate [-h] [-PID]"<<std::endl;
    std::cout<<"Shows the state of the processor(PID) and its fields"<<std::endl;
   }
}

void sop::processor::ExecutiveUnit::cH_fullTick(const std::vector<const std::string> & params)
{
  //testerMethod();
  if (!sop::system::Shell::hasParam(params,"-h"))
   {
     mainExecutiveLoop();
   }
   else if(sop::system::Shell::hasParam(params,"-h") || params.size()==1)
   {
    std::cout<<"fulltick [-h]"<<std::endl;
    std::cout<<"Takes task with highest priority, and gives it processor resourcers, func. stops if quant time is 0 or finds EXT executed code "<<std::endl;
   }
}
