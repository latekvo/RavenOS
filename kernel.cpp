#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <bits/stdc++.h>//idk if needed, but its for stringstream

#include "kernelHeader.h"

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

//a 16bit rendering of characters
static UINT16 VFA_DefaultEntry(unsigned char ch_to_print){
	//left shift converts color to character value pixel encoding
	return (UINT16)ch_to_print | (UINT16)WHITE_COLOR << 8;
}

//will split strings and return word from zero
string splitString(string str, int index){/*index from 0*/
	stringstream stringSplitter_stream(str);
	string spliced = null;
	for(int i = 0; i <= index; i++){
		stringSplitter_stream >> spliced;
	}
	stringSplitter_stream.clear();
	return spliced;
}

//false if error
//true if everything is A-OK
//!!! CHANGE THIS TO SINGLE STRING INPUT FOR MORE FLEXABILITY
bool programParser(string program, string arg, string input){
	ifstream launched(program);//opens stream to designated program
	string processedWord;
	string processedString; 
	string program_input = null;
	while(launched >> processedWord){
		if(processedWord == "EOF"){
			return true;
		}
		if(processedWord == arg){
			while(1){//executes argument
				launched >> processedWord;
				if(processedWord == "EOA"){
					return true;
				}
				if(processedWord == "input"){
					cin >> program_input;
					continue;
				}
				if(processedWord == "write"){
					while(1){
						getline(launched, processedString);
						if(processedString == "endwrite"){
						break;
						}
						else{
							cout << processedString << endl;
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
					continue;
				}
			}
		}
	}
	return false;
}
//false if error
//true if A-OK
bool programLauncher(string app, string arg, string argOther){
	ifstream appList("apps.data");
	string appExec;
	string appName;
	while(1){
		//APP FILE != APP NAME
		appList >> appExec;
		appList >> appName;
		//appName will never be EOF but i check it for caution
		if(appExec == "EOF" || appName == "EOF"){
			return false;
			//this app doesnt exist
		}
		if(app == appName){
			programParser(appExec, arg, argOther);
			return true;
			//launch app by its filename
		}
	}

}

bool inputManager(string input){
	//from here command is either 
	//classified as system command
	//or as app.
	//---
	//Here the string splitting magic happens
	//And invalid inputs are dismissed
	//input is raw data
	string command = splitString(input, 0);//command is input after splitting
	string arg = splitString(input, 1);
	string argOther = splitString(input, 2);
	
	//MAKE THE SPLTTING HERE

	while(1){
		if(input == null){
		return true;
		}
		if(!(input == null)){
		programLauncher(command, arg, argOther);
		return true;
		}
	}	
}

int main(){
	
	//terminal buffer pntr to vga casted to UINT16
	TERMINAL_BUFFER = (UINT16*)VGA_ADDRESS;
	//now VGA is easly avabile by array
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

