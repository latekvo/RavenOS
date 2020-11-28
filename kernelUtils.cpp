#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector> 
#include <deque>
#include <cstdlib>

#include "kernel.h"

using namespace std;

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