#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>

#include "linux_parser.h"
#include "format.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}


float LinuxParser::MemoryUtilization() {
  string line;
  string key;
  string value;
  float en = 0;
  float to = 69;
  string unit;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value >> unit) {
        if (key == "MemTotal") { en = std::stof(value); }
          if (key == "MemFree") { to = std::stof(value); }
      }
  	}
    filestream.close();
  }
  
  return (en - to)/en;
}


long int LinuxParser::UpTime() {
  
  string line;
  string upTime;
  string irrelevant;
  string error = "error";
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
  	std::istringstream linestream(line);
    
    linestream >> upTime >> irrelevant;

    long int uptime2 = std::stol(upTime);
    
    filestream.close();
    
    return uptime2;
	}
  
  return 0;
}



int LinuxParser::TotalProcesses() { 
  
  string line;
  string key;
  string value;
  int valuE = 0;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") {
          valuE = std::stoi(value);
          
          filestream.close();
          
          return valuE;
        }
      } 
    } 
  }
  return valuE;
}


int LinuxParser::RunningProcesses() {
  
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          int valuE = std::stoi(value);
          
          filestream.close();
          
          return valuE;
        }
      } 
    } 
  }
  return 0;
}


string LinuxParser::Command(int pid) {
  string line, value;
  string error = "error";
  
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
  	std::istringstream linestream(line);
    linestream >> value;
    
    filestream.close();
    
    return value;
  }
  return error;
}


string LinuxParser::Ram(int pid) {
  string line, key, value, unit;
  string error = "error";
  float value2, value3;
  int value4;
  
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value >> unit) {
        if (key == "VmRSS:") {
        // I have changed the key above to VmRSS from VmSize.
        // This gives the physical RAM and not the virtual memory.
          value2 = std::stof(value);
          value3 = value2/1000;
          value4 = value3;
          
          filestream.close();
          
          return std::to_string(value4);
        }
      } 
    } 
  }
  return error;          
}


string LinuxParser::Uid(int pid) { 
  
  string line, key, value, en, to, tre;
  string error = "error";
 
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value >> en >> to >> tre) {
        if (key == "Uid:") {
          
          filestream.close();
          
          return value;
        }
      } 
    } 
  }
  return error;  
}


string LinuxParser::User(int pid) { 
  
  string en, to, value, line, key;
  string error = "error";
  
  string uid_ = LinuxParser::Uid(pid);
  
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
	  std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> value >> en >> to >> key) {
        if (key == uid_) {
          
          filestream.close();
          
          return value;
        }
      } 
    } 
  }
  return error;  
}


long LinuxParser::UpTime(int pid) {
  string value, line;
  long int starttime, uptime;
  vector<string> stat_list;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    while (linestream >> value) {
		stat_list.emplace_back(value);
    }
    stream.close();
  }
  starttime = std::stol(stat_list[21])/sysconf(_SC_CLK_TCK);

  uptime = LinuxParser::UpTime() - starttime;
   

  return uptime;
}

