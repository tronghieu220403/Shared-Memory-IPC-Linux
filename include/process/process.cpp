#include "process.h"

namespace process
{
	Process::Process(): pid_(0) {};

	Process::Process(int pid): pid_(pid) {};

	Process::Process(const std::string_view& name): name_(name), pid_(Process::FindProcessByName(name)) {};

	int Process::GetPid() const{
		return pid_;
	}

	void Process::SetPid(int pid){
		pid_ = pid;
	};

    bool Process::IsActive()
    {
        return false;
    };

    std::string Process::GetName() const
    {
        return name_;
    }

    void Process::SetName(const std::string_view &name)
    {
        name_ = name;
    }

    int Process::FindProcessByName(const std::string_view& name)
    {
        int pid = 0;

		DIR* dir = ::opendir("/proc");
		if (!dir) {
			return false;
		}

		bool found = false;
		dirent* entry;
		while ((entry = ::readdir(dir)) != nullptr && !found) {
			// check if the entry is a directory and its name is a number
			if (entry->d_type == DT_DIR && std::isdigit(entry->d_name[0])) {
				// read the process name from the cmdline file
				std::string cmdPath = std::string("/proc/") + entry->d_name + "/cmdline";
				//std::cout << cmdPath.c_str() << std::endl;
				
				std::ifstream cmdFile(cmdPath.c_str());
				std::string cmdLine;
				std::getline(cmdFile, cmdLine);

				// check if the process name matches
				if (!cmdLine.empty() && cmdLine.find(name) != std::string::npos) {
					pid = std::stoi(entry->d_name);
					break;
				}
				
			}
		}
		::closedir(dir);

        return pid;
	};

}