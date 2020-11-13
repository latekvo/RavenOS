#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>

#include "kernelHeader.h"

using namespace std;

//runtime variables

bool kernel_enabled = true;
string kernel_name = "RavenOS";
string kernel_version = "0.1.0";

ifstream config("config.data");

string user_name = "admin";

string command_search;
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

int countString(string str){
	stringstream countString_stream(str);
	int countString_finalCount = 0;
	string counterFooString = null;
	while(countString_stream >> counterFooString){
		countString_finalCount++;
	}
	return countString_finalCount;
}
string splitString(string str, int index){/*index from 0*/
	stringstream stringSplitter_stream(str);
	string spliced = null;
	for(int i = 0; i <= index; i++){
		stringSplitter_stream >> spliced;
	}
	stringSplitter_stream.clear();
	return spliced;
}

//false if error: Unknown argument/code syntax error
//true if everything is A-OK
//!!! CHANGE THIS TO SINGLE STRING INPUT FOR MORE FLEXABILITY
//! NOTE CHANGED INPUT MANAGER TO SINGLE INPUT BUT MAY NEED TO MOVE IT HERE
//!!!YESS, MOVE THIS HERE, IN-MANAGER WILL ONLY SPLIT FOR CD, LS, ETC...
//!!!NOTE TO LAST NOTE, LAUNCHER CHECKS FOR VALID PROGRAM AND IN-MANAGER SPLITS...
bool programParser(string program, string wholeString){
	//split 'zero' is ignored as its already avabile
	int argAmount = countString(wholeString) - 2;//-1 as we count from zero and
       						     //-1 as last thing is input
	//only one input is allowed for now, but unlimited amount of arguments
	string program_inputArgument = splitString(wholeString, argAmount + 1);
	//adding back 'input' argument, as final amount of argAmount here is -1
	string arg; // searched argument.
	string processedWord;	//for single words
	string processedString; //for strings
	
	string program_input; //seperated program input
		
	int argIndex = 1; //0 is program name, updated every arg execution.	
	
	ifstream launched;//opens stream to designated program DOES THIS EVERY 

	while(1){
		cout << "entered TAPL file loop" << endl;///DEBUG///
		if(launched.is_open()){
			launched.close();
		}
		cout << "opened/reopened program file" << endl;///DEBUG///
		launched.open(program);//opens stream to designated program DOES THIS EVERY 
		cout << "passed opening/reopening of program file" << endl;///DEBUG///
		
		launched >> processedWord;
				
		arg = splitString(wholeString, argIndex);
		if(processedWord == "EOF"){
			return true;
		}
		if(processedWord == "EOA"){
			return false;
			//something is wrong, if this is accessible,
			//there wasnt argument befor 'close argument'
		}
		if(processedWord == "null"){
			cout << "converted 'null' to NULL" << endl;///DEBUG///
			processedWord == null;
			//convert null argument to literal nothing
		}
		if(processedWord == arg){
			argIndex++;
			while(1){//executes argument
				cout << "entered TAPL argument loop" << endl;///DEBUG///
				launched >> processedWord;
				//EO section
				cout << "entered EO parser section" << endl;///DEBUG///
				if(processedWord == "EOF"){
					return false;
				}
				if(processedWord == "EOA" || processedWord == "EOE"){
					return true;
				}
				//LOGIC section
				if(processedWord == "var"){
					launched >> processedWord;
				}
				//input, write, read, 
				//write file, read file, open file.
				cout << "entered IO/FILE parser section" << endl;///DEBUG///
				if(processedWord == "input"){
					cin >> program_input;
					continue;
				}
				if(processedWord == "write"){
					while(1){
						getline(launched, processedString);
						//insert 'escape' here//
						if(processedString == "endwrite"){
						break;
						}
						else{	//cout line
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
				if(processedWord == "writefile"){
					launched >> processedWord;
					ofstream program_outputFile(processedWord);
					while(1){
						launched >> processedWord;
						if(processedWord == "endfile"){
						break;
						}
						program_outputFile << processedWord << " ";
					}
					continue;
				}
				else{
					cout << "PROGRAM ERROR: INVALID COMMAND OR SYNTAX ERROR" << endl;
					//syntax err
					return false;
				}
			}
		}
	}
	return false;
}
//false if error: No such app listed
//true if A-OK
bool programLauncher(string app, string input){
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
			programParser(appExec, input);
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
	//this splitting is only for the rare systes commands
	bool is_launchNominal;

	while(1){
		if(input == null){
		return true;
		}
		if(!(input == null)){
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

