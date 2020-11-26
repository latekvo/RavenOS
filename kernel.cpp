#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector> 
#include <deque>
#include <cstdlib>

#include "kernelHeader.h"

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
	string counterString = "";

	while(countString_stream >> counterString){	
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
	string spliced = "";

	for(int i = 0; i <= index && splitString_stream.rdbuf()->in_avail() != 0; i++){
		splitString_stream >> spliced;
		cout << "debug: splitString: spliced = " << spliced << endl;
	}
	splitString_stream.clear();
	cout << "debug: splitString: output = " << spliced << endl;
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
	int argAmountActual = 0;//only -x arguments
	int argAmountInputs = argAmount;
		
	//cant just check last arg for input as it could be in centre
		
	for(int i = 0; i <= argAmount; i++){
		string tempString = "";
		tempString = splitString(wholeString, i);
		cout << "debug: programParser: tempString = " << tempString << endl;
		if(tempString.size() != 0){
			if(tempString.at(0) == '-'){
				argAmountActual++;
			}
		}
	}
	
	cout << "debug: programParser: argAmount = " << argAmount << endl; /// debug ///
	cout << "debug: programParser: argAmountActual = " << argAmountActual << endl; /// debug ///

	if(argAmount < argAmountActual){ // this should never happen //
		cout << "RUNTIME ERROR: argAmount < argAmountActual" << endl;
		return false;	
	}

	//making arg list	
	int argIndex = 0;
	string argList[argAmountActual];
		
	//cout << "debug: programParser: argIndex = " << argIndex << endl;
	//cout << "debug: programParser: argAmountActual = " << argIndex << endl;
	for(int i = 0, j = 0; i < argAmount + 1; i++){//giving back '1' for it to run if there is anything else than program name
		cout << "debug: programParser: entered arg indexing." << endl; /// debug ///
		string tempString = ""; 
		tempString = splitString(wholeString, i);
		if(tempString.size() != 0){
			if(tempString.at(0) == '-'){
				argList[j] = tempString;
				j++;
				cout << "debug: programParser: added arg '" << tempString << "' to list. " << endl; /// debug ///
			}
		}
		cout << "debug: programParser: iterated arg indexing." << endl; /// debug ///
	}
	

	string program_inputArgument;

	string arg; // searched argument.
	string processedWord;	//for single words
	string processedString; //for strings, just for clarity
				
	bool program_isLaunched = true;
	bool program_argumentCompleted = false;
				
	deque<string> programInputs;	
				
	ifstream launched;	
				
	while(program_isLaunched == true && argIndex < argAmountActual){
		if(launched.is_open()){//temp, this may not exist
			launched.close();
		}			
		launched.open(program);
		arg = argList[argIndex];

		cout << "argument = " << arg << endl;
				
		launched >> processedWord;
		if(processedWord == ""){
			cout << "Error occured while opening a program file" << endl;
			return false;
		}
		cout << processedWord << endl;
				
		deque<string> programDeque;
		int blankSpaces = 0; //Exception handling
		while(1){//i have no idea what to put here, so '1' should work
			if(processedWord == arg){
				while(1){//worry not, this double-infinite loop is intentional, i could
					//move 'else' part higher and exclude it from if, thus shortening
					//the code, but it will be just as efficient as this 'if' is executed only once, 
					////and i have better things to do, like writing this insanely long comment.
					launched >> processedWord;
					programDeque.push_back(processedWord);		
					cout << "debug: programParser: pushed back '" << processedWord << "'" << endl;
					//later checks for special parameters, but still leaves most work
					//for later massive loop. This one will only ease the pain of more
					//important one.
					if(processedWord == "write" || processedWord == "writefile"){
						string tempString;
						while(processedWord != "endwrite"){
							getline(launched, tempString);
							programDeque.push_back(tempString);
						}
					}
					//just pushing to deque
				}
				break; //this will just go out of pseudo infinite loop
			}else{
				launched >> processedWord;
				blankSpaces++;
				if(blankSpaces == 6){
					cout << "Timed out while parsing throught program.";
					return false;
				}
			}
			// !!! 'break;' from up there will end up here !!! //
		}
		while(programDeque.empty() == false){				
			processedWord = programDeque.front();	
			cout << "| " << processedWord << endl;
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
				string tempString;
				cin >> tempString;
				programInputs.push_back(tempString);
				continue;
			}
			if(processedWord == "write"){
				string tempString = "";//resets last changes to string
				while(tempString != "endwrite"){
					tempString = programDeque.front();
				    	programDeque.pop_front();
					cout << tempString << endl;
				}
				continue;
			}
			if(processedWord == "readfile"){
				string tempString;
				processedWord = programDeque.front();
				programDeque.pop_front();
				ifstream program_openFile(processedWord);
				while(getline(program_openFile, tempString)){
					cout << tempString << endl;
				}
				continue;
			}
			if(processedWord == "writefile"){
				processedWord = programDeque.front();
				programDeque.pop_front();
				ofstream program_outputFile(processedWord);
				while(programDeque.empty()){
					processedWord = programDeque.front();
					programDeque.pop_front();
					if(processedWord == "endfile"){
					break;
					}
					program_outputFile << processedWord << " ";
				}
				continue;
			}	
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

