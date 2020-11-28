#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector> 
#include <deque>
#include <cstdlib>

#include "kernel.h"

using namespace std;

//runtime variables

bool kernel_enabled = true;
string kernel_name = "RavenOS";
string kernel_version = "0.1.0";

ifstream config("config.data");

string user_name = "admin";

string user_input = "";

bool launchSuccess = true;
bool correctInput = true;

//false if error: No such app listed
//true if A-OK
bool programLauncher(string appNameInputted, string input){

	cout << "entered programLauncher module." << endl;

	ifstream appList("apps.data");

	string appNameChecked;
	string appExec;
	
	while(1){
		//APP FILE != APP NAME
		appList >> appExec;
		appList >> appNameChecked;
		
		cout << "checking list: expectedAppName: " << appNameInputted << " appExecutable: " << appExec << " appName: " << appNameChecked << endl;
		if(appNameChecked == appNameInputted){	
			cout << "app: " << appNameInputted << " found" <<endl;
			cout << "appExec: " << appExec << " appName: " << appNameChecked << endl;
			programParser(appExec, input);
			return true;
			//launch app by its filename

		}
		if(appExec == "EOF"){
			cout << "no such program." << endl;
			return false;
		}
	}
}

bool inputManager(string input){
	
	cout << "entered inputManager module." << endl;

	//from here command is either 
	//classified as system command
	//or as app.
	//----------
	//Here the string splitting magic happens
	//And invalid inputs are dismissed
	//input is raw data
	string command = splitString(input, 0);//command is input after splitting
	string arg = splitString(input, 1);
	//this splitting is only for the rare systes commands
	bool is_launchNominal;

	while(1){
		if(input == ""){
		return true;
		}
		if(!(input == "")){
		is_launchNominal = programLauncher(command, input);
		return is_launchNominal;
		}
	}	
}

int main(){
	//terminal buffer pntr to vga casted to UINT16
	TERMINAL_BUFFER = (UINT16*)VGA_ADDRESS;
	//now VGA is easly avabile by array
	
	bool system_nominal = true;
	while(kernel_enabled == true && system_nominal == true){
		cout << user_name << "/:> ";	
		getline(cin, user_input);
		inputManager(user_input);
	}
}

