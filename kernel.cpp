
#include <iostream>
#include <fstream>
#include <sstream>

#include <string>
#include <vector>
using namespace std;

//runtime variables

bool kernel_enabled = true;
string kernel_name = "RavenOS";
string kernel_version = "0.1.0";

string user_name = "admin";

string command_search = "EOF";
string commandInput;
string null = "";

bool launchSuccess = true;
bool correctInput = true;

string stringSplitter(string str, int index){/*index from 0*/
	
}

//false if error
//true if everything is A-OK
bool programParser(string program){
	
}

bool programLauncher(string app){
	ifstream appList("apps.data");
	while(true){
		while(1){
			appExec << appList;
			appNameCheck << appList;
			if(appExec == "EOF" || appNameCheck == "EOF"){
				return false;
			}
			if(app == appNameCheck){
				programParser(appExec);
				return true;
			}
		}
	}
}

bool inputManager(string input){
	if(input == null){
	return true;
	}else
	if(input == 
}

int main(){
	
	bool system_nominal = true;
	while(kernel_enabled == true && system_nominal == true){
		cout << user_name << ">>";	
		getline(cin, commandInput);
		inputManager(commandInput);
		if(correctInput == false){
			cout << "Unknown command" << endl;
		}
		if(launchSuccess == false){
			cout << "Unknown command" << endl;
		}
	}
}

