#include "project1.h"
//opens input data file and check is if valid

int main() {
	//creates the file object
	ifstream Data;
	// Filepath holder
	string filePath;
	
	//These are all required in order to track a stock
	//**********************************
	filePath = "ABX.txt";
	openInputFile(Data, filePath);
	//Track a stock by using its name as an instance and pass the stock symbol and name to the constructor
	StockData ABX("ABX","Barrick Gold Corp");
	ABX.createDatabase(ABX,Data);
	trackedStocks.push_back(ABX);
	//**********************************
	//ABX.print();
	
	//Beginning to Track Amazon
	filePath =  "AMZN.txt";
	openInputFile(Data, filePath);
	StockData AMZN("AMZN","Amazon, Inc.");
	AMZN.createDatabase(AMZN,Data);
	trackedStocks.push_back(AMZN);
	//AMZN.print();
	
	filePath =  "TMF.txt";
	openInputFile(Data, filePath);
	StockData TMF("TMF","Amazon, Inc.");
	TMF.createDatabase(TMF,Data);
	trackedStocks.push_back(TMF);
	
	//You must Update your Portfolio before you print your portfolio
	//sellStock and buyStock are the functions for performing each task.
	Portfolio AmazonPlus;
	amazonPlusInitiate(AmazonPlus, "12/14/2015", AMZN, TMF);
	AmazonPlus.printPortfolio();
	updatePortfolio("12/16/2016",AmazonPlus);
	AmazonPlus.printPortfolio();
	
	//AmazonPlus.printPortfolio();
	
	//Portfolio A;
	//ABX.activelyTrade(A,ABX);
	//A.buyStock("12/14/2016", ABX, 50);
	//updatePortfolio("12/16/2016",A);
	//A.printPortfolio();
	//
	//A.sellStock("12/15/2016",ABX, 25);
	//updatePortfolio("12/15/2016",A);
	//A.printPortfolio();
	
	return 0;
}