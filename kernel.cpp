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

ifstream config("config.data");

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
//!!! CHANGE THIS TO SINGLE STRING INPUT FOR MORE FLEXABILITY
bool programParser(string program, string arg, string input){
	ifstream launched(program);//opens stream to designated program
	string processedWord;
	string processedString; 

	while(launched >> processedWord){
		if(processedWord == EOF){
			return true;
		}
		if(processedWord == arg){
			while(1){//executes argument
				launch >> processedWord;
				if(processedWord == "EOA"){
				return true;
				}
				if(processedWord == "write"){
					while(1){
						getline(launched, processedString);
						if(processedString == "endwrite"){
						break;
						}
					}
					continue;//checks next word after 'endwrite'
				}
				if(processedWord == "readfile"){
					launched >> processedWord;
					ifstream program_openFile(processedWord);
					while(getline(program_openFile, processedString)){
						cout << processedString << endl;
					}
				}
			}
		}
	}
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
	while(1){
		if(input == null){
		return true;
		}else
		if(){
		}else{
		programParser(input);
		return true;
		}
	}	
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

