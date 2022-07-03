#include "processor.h"
#include "linux_parser.h"
using namespace std;


float Processor::Utilization() { 

  string line, key;
  std::ifstream filestream(LinuxParser::kProcDirectory+LinuxParser::kStatFilename);
  if (filestream.is_open()) { 
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.begin()+4, ' ', '_');
      std::istringstream linestream(line);
      while (linestream >> key) {	// I looked around and found a solution which uses the key, value and while-loop.
        if (key == "cpu_") {		// this seems to work much better.
          linestream >> user_ >> nice_ >> system_ >> idle_ >> iowait_ >> irq_ >> softirq_ >> steal_ >> guest_ >> guest_nice_;
        }
      }
    }
    filestream.close();
 }
  
  idle = idle_ + iowait_;
  nonIdle = user_ + nice_ + system_ + irq_ + softirq_ + steal_;
  total = idle + nonIdle;
  
  totald = total - prevTotal;
  idled = idle - prevIdle;
  prevTotal = total;
  prevIdle = idle;

  return (totald - idled)/totald;
}