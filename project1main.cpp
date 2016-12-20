#include "project1.h"
//opens input data file and check is if valid
void openInputFile(ifstream &inFile, string inFileName){
	//string i= inFileName;
	inFile.open(inFileName.c_str());
	if (!inFile.is_open()){
		cout << "Unable to open file " << inFileName << endl;
	}
}

int main() {
	//creates the file object
	ifstream Data;
	// Filepath holder
	string filePath;
	//Filepath of ABX Data
	filePath = "C:/assignments/ABX.txt";
	
	openInputFile(Data, filePath);
	StockData ABX("ABX","Barrick Gold Corp");
	ABX.createDatabase(ABX,Data);
	//ABX.print();
	
	filePath =  "C:/assignments/AMZN.txt";
	openInputFile(Data, filePath);
	StockData AMZN("AMZN","Amazon, Inc.");
	AMZN.createDatabase(AMZN,Data);
	//AMZN.print();
	//cout << ABX.movingAverage(10, "11/16/2016");
	//ABX.activelyTrade();
	
	
	Portfolio A;
	A.buyStock("12/16/2016", ABX, 50);
	A.printPortfolio();
	A.buyStock("12/15/2016",ABX,50);
	A.printPortfolio();
	return 0;
}