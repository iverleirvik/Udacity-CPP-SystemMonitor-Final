#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using namespace std;

/*You need to complete the mentioned TODOs in order to satisfy the rubric criteria "The student will be able to extract and display basic data about the system."

You need to properly format the uptime. Refer to the comments mentioned in format. cpp for formatting the uptime.*/


Processor& System::Cpu() { return cpu_; }


vector<Process>& System::Processes() { 
  vector<int> pids = LinuxParser::Pids();
  processes_.clear();
  for (long unsigned int i = 0; i < pids.size(); i++) {
    if (!LinuxParser::Ram(pids[i]).empty()) {
      processes_.emplace_back(pids[i]);
    }
  }
  
  std::sort(processes_.rbegin(), processes_.rend());
  return processes_;
}


std::string System::Kernel() { 
  std::string valueKernel = LinuxParser::Kernel();
  
  return valueKernel; 
}


float System::MemoryUtilization() { 
  float MU = LinuxParser::MemoryUtilization();
  
  return MU;
}


std::string System::OperatingSystem() {
	std::string valueOS = LinuxParser::OperatingSystem();
  
  	return valueOS;
}


int System::RunningProcesses() { 
  int procsRunning = LinuxParser::RunningProcesses();
  
  return procsRunning;
}


int System::TotalProcesses() { 
	int totProcesses = LinuxParser::TotalProcesses();
  
  return totProcesses;
}


long int System::UpTime() {
  long int upTime = LinuxParser::UpTime();
  
  return upTime;
}
