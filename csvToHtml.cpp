#include <iomanip>
#include <iostream>
#include <fstream>

#include <algorithm>
#include <iterator>
#include <map>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

template <typename Out>
void split(const string &s, char delim, Out result){
	istringstream iss(s);
	string item;
	while (getline(iss, item, delim)){
		*result++ = item;
	}
}

vector<string> split(const string &s, char delim){
	vector<string> elems;
	split(s, delim, back_inserter(elems));
	return elems;
}

bool getvector(ifstream &fin, vector<string> &v){
	string line;
	getline(fin, line);
	bool x = fin.good();
	if(x){
		line.erase(remove(line.begin(), line.end(), '\n'), line.end());
		line.erase(remove(line.begin(), line.end(), '\r'), line.end());
		v = split(line, ',');
	}
	return x;
}

int main(int argc, char** argv){
	vector<string> curLine(argv + 1, argv + argc);
	string csvTableFName, outFName;

	for(auto i = curLine.begin(); i != curLine.end(); ++i){
		if(*i == "-h" || *i == "--help"){
			cout << "Syntax: -f <inputTable.csv> -o <output.htm>\n";
			return 0;
		}
		else if(*i == "f"){
			csvTableFName = *++i;
			ifstream fileTest(csvTableFName);
			if(fileTest.fail()){
				cout << "File: " << csvTableFName << " has set failbit andor badbit flags\n";
				return 1;
			} 
		}
		else if(*i == "-o"){
			outFName = *++i;
		}
	}

	ifstream csvF(csvTableFName);
	ofstream outF(outFName, ofstream::app);

	outF << "<table id=\"MyTable\" class=\"table table-bordered table-hover table-condensed\">\n"
		 << "\t<thead>\n"
		 << "\t\t<tr>\n";

	getvector(csvF, curLine);

	for(int i = 0; i < curLine.size(); i++){
		outF << "\t\t\t<th style=\"position:sticky;top:50px;z-index:5;background:#fff\" title=\"Field #" << i + 1 << "\">" << curLine[i] << "</th>\n";
	}

	outF << "\t\t</tr>\n"
	     << "\t</thead>\n"
	     << "\t<tbody>\n";

	while(getvector(csvF, curLine)){
		outF << "\t\t<tr>\n";
		for(const auto &i : curLine){
			outF << "\t\t\t<td>";
			if(i.empty()) outF << "&nbsp;";
			else          outF << i;
			outF << "</td>\n";
		}
		outF << "\t\t</tr>\n";
	}

	outF << "\t</tbody>\n"
	     << "</table>";
}