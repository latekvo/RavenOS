#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector> 
#include <deque>
#include <cstdlib>

#include "kernel.h"

using namespace std;

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
				
		deque<string> programDeque;
		int blankSpaces = 0; //Exception handling
		while(1){//i have no idea what to put here, so infinite should work
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
					if(processedWord == "EOA" || processedWord == "EOF"){
						break;//right after this break, there is a second break
					}
					/*
					 * this part is a bit corrupted, and code will kinda work without it.
					 *
					if(processedWord == "write" || processedWord == "writefile"){
						string tempString;
						while(processedWord != "endwrite"){
							getline(launched, tempString);
							programDeque.push_back(tempString);
						}
					}*/
					//just pushing to deque
				}
				break; //this will just go out of pseudo infinite loop
			}else{
				launched >> processedWord;
				blankSpaces++;
				if(blankSpaces == 6){
					cout << "Timed out while parsing throught program. " << endl;
					return false;
				}
			}
			// !!! 'break;' from up there will end up here !!! //
		}
		while(programDeque.empty() == false){				
			processedWord = programDeque.front();	
			programDeque.pop_front();
			cout << "parsed | " << processedWord << endl;
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
					cout << "o: " << tempString << endl;
				}
				continue;
			}
			if(processedWord == "readfile"){
				string tempString;
				processedWord = programDeque.front();
				programDeque.pop_front();
				ifstream program_openFile(processedWord);//open text file
				while(getline(program_openFile, tempString)){//reading text file
					cout << "o: " << tempString << endl;//
				}
				if(program_openFile.is_open()){
					program_openFile.close();
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
