#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) {
  pid_ = pid;
}


int Process::Pid() const { return pid_; }


float Process::CpuUtilization() const {
  
  int utime, stime, cutime, cstime, starttime;
  string value, line;
  vector<string> statList;
  
  std::ifstream stream(LinuxParser::kProcDirectory + std::to_string(pid_) + LinuxParser::kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    while (linestream >> value) {
			statList.emplace_back(value);
        }
    stream.close();
    }
  utime = std::stoi(statList[13]);
  stime = std::stoi(statList[14]);
  cutime = std::stoi(statList[15]);
  cstime = std::stoi(statList[16]);
  starttime = std::stoi(statList[21]);
  
  string line2, upTime, unneccesary2;
  std::ifstream filestream(LinuxParser::kProcDirectory + LinuxParser::kUptimeFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line2);
  	std::istringstream linestream(line2);
    
    linestream >> upTime >> unneccesary2;

  float uptime = std::stof(upTime);
  long int Hertz = sysconf(_SC_CLK_TCK);
  float totalTime = utime + stime;
  totalTime = totalTime + cutime + cstime;
  float seconds = uptime - (starttime / Hertz);
  float cpuUsage = ((totalTime / Hertz) / seconds);
    
    filestream.close();
  
  return cpuUsage;
}
return 0;
}


string Process::Command() { 
  std::string cmd = LinuxParser::Command(pid_);
  if (cmd.size()>40) {
  	cmd.resize(37);
    
  
  }
  
  return cmd + "...";
}


string Process::Ram() {
  string ram = LinuxParser::Ram(pid_);
  
  return ram;
}


string Process::User() { 
  string user = LinuxParser::User(pid_);
  
  return user;
}


long int Process::UpTime() {
  long int uptime = LinuxParser::UpTime(pid_);
  
  return uptime;
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function

// The reason we are overloading the comparison for process objects
// is that it makes it possible to do a "less than" comparison
// between processes. Otherwise it wouldnt be possible.
// the condition we want to regulate on is how much cpu a process
// takes. That will prioritize the process in the monitor.
bool Process::operator<(Process const& a) const {
  return this->CpuUtilization() < a.CpuUtilization();
}