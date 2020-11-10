#include <iostream>
#include <fstream>
using namespace std;

//runtime variables

bool kernel_enabled = true;
string kernel_name = "kernelTestVersion";
string kernel_version = "0.1.0";

string user_name = "admin";

string commandInput;
string null = "";

void commandParser(string parsed){
	
	if(parsed == "help" || parsed == "man"){

		cout << "========== HELP ==========" << endl;
		//man / help
	}else
	if(parsed == "info"){
		//looks throught asciiart info
		ifstream in("sysinfo.data");
		
		string sysLine;

		while(getline(in, sysLine)){
			cout << sysLine << endl;
		}
		cout << "System version: " <<kernel_version<< endl;
		cout << "Kernel name: " <<kernel_name<< endl;
		
	}
	/*this will look throught commands.sys file*/
	for(int i; ; ){
		//commands
	}

}


void commandOperator(){
	
	cout << user_name <<">>";	
	getline(cin, commandInput);
	commandParser(commandInput);

}

int main(){
	
	bool system_nominal = true;
	while(kernel_enabled == true && system_nominal == true){

		commandOperator();

	}	

	

}

