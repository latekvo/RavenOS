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

string temp_string;
double temp_num;
string command_search;
string commandInput;
string null = "";

bool launchSuccess = true;
bool correctInput = true;

//a 16bit rendering of characters
static UINT16 VGA_DefaultEntry(unsigned char ch_to_print){
	//left shift converts color to character value pixel encoding
	return (UINT16)ch_to_print | (UINT16)WHITE_COLOR << 8;
}

//will split strings and return word from zero

int countString(string str){
	cout << "entered countString() module" << endl;
	stringstream countString_stream(str);
	int countString_finalCount = 0;
	string counterString = null;

	while(countString_stream.rdbuf()->in_avail() != 0){
		countString_stream >> counterString;
		countString_finalCount++;
	}

	return countString_finalCount;
}
string splitString(string str, int index){/*index from 0*/
	cout << "entered splitString() module" << endl;
	if(countString(str) <=  1){//if 0 or 1 then return input//
		return str;
	}
	stringstream splitString_stream(str);
	string spliced = null;

	for(int i = 0; i <= index && splitString_stream.rdbuf()->in_avail() != 0; i++){
		splitString_stream >> spliced;
	}
	splitString_stream.clear();
	return spliced;
}

//false if error: Unknown argument/code syntax error
//true if everything is A-OK
//!!! CHANGE THIS TO SINGLE STRING INPUT FOR MORE FLEXABILITY
//! NOTE CHANGED INPUT MANAGER TO SINGLE INPUT BUT MAY NEED TO MOVE IT HERE
//!!!YESS, MOVE THIS HERE, IN-MANAGER WILL ONLY SPLIT FOR CD, LS, ETC...
//!!!NOTE TO LAST NOTE, LAUNCHER CHECKS FOR VALID PROGRAM AND IN-MANAGER SPLITS...
bool programParser(string program, string wholeString){
		
	cout << endl << "entered programParser module." << endl << endl;

	cout << "Program: " << program << endl;///DEBUG///
	cout << "Arguments: " << wholeString << endl;///DEBUG///
		
	//counting args
	int argAmount = countString(wholeString) - 1;//this includes input but not program name
	int argAmountActual;//only -x arguments
	int argAmountInputs = argAmount;
		
	//cant just check last arg for input as it could be in centre
		
	for(int i; i < argAmount; i++){
		temp_string = splitString(wholeString, i);
		if(temp_string.size() != 0){
			if(temp_string.at(0) == '-'){
				argAmountActual++;
			}
		}
	}
	
	cout << "countString(): " << countString(wholeString) << endl; /// debug ///
	cout << "argAmountActual: " << argAmountActual << endl; /// debug ///

	if(argAmount < argAmountActual){ // this should never happen //
		cout << "RUNTIME ERROR: argAmount < argAmountActual" << endl; 
	}

	//making arg list	
	int argIndex;
	string argList[argAmountActual];
		
	for(int i, j = 0; i < countString(wholeString); i++){
		temp_string = splitString(wholeString, i);
		if(temp_string.at(0) == '-'){
			argList[j] = temp_string;
			j++;
			cout << "argOnList: " << argList[j]; /// debug ///
		}
	}
	
	cout << "passed arg indexing." << endl; /// debug ///

	string program_inputArgument;

	string arg; // searched argument.
	string processedWord;	//for single words
	string processedString; //for strings, just for clarity

	bool program_isLaunched = true;
	bool program_argumentCompleted = false;

	string program_input;
	
	ifstream launched;

	while(program_isLaunched == true && argIndex < argAmountActual){

		arg = argList[argIndex];

		cout << "argument: " << arg << endl;

		launched >> processedWord;
		
		while(!(processedWord == arg)){
			launched >> processedWord;	
		}
		if(processedWord == arg){
			while(1){
				launched >> processedWord;
				
				cout << "|" << processedWord << endl;

				//EO section
				if(processedWord == "EOF"){
					return true;//force close program
				}
				if(processedWord == "EOA"){
					argIndex++;
					arg = argList[argIndex];				
					break;
				}

				//input, write, read, 
				//writefile, readfile, openfile.
				if(processedWord == "input"){
					cin >> program_input;
					continue;
				}
				if(processedWord == "write"){
					processedString = null;//resets last changes to string
					while(processedString != "endwrite"){
						getline(launched, processedString);
						cout << processedString << endl;
					}
					continue;
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
			}
			continue;
		}
	
	}
	return true;
}

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
		cout << user_name << "/:>";	
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

