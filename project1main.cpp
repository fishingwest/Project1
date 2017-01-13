#include "project1.h"

int main() {
	string todaysDate;
	string beginStrategy = "12/14/2015";
	
	//Latest date it can take is 12/16/2016 as that is when the data was collected
	cout << "Please Input Today's Date MM/D/YYYY or 12/5/2016: ";
	cin >> todaysDate;
	
	ifstream Data;
	string filePath;
	
	//The steps beetween the asterisk are required to begin tracking a stock.
	//Any stock can be tracked
	//Input data to be used is the file path (tab delimited) text file
	
	//*********************************************************************//
	filePath = "ABX.txt";
	openInputFile(Data, filePath);
	//Track a stock by using its name as an instance and pass the stock symbol and name to the constructor
	StockData ABX("ABX","Barrick Gold Corp");
	ABX.createDatabase(ABX,Data);
	trackedStocks.push_back(ABX);
	//*********************************************************************//
	
	//Beginning to Track Amazon
	filePath =  "AMZN.txt";
	openInputFile(Data, filePath);
	StockData AMZN("AMZN","Amazon, Inc.");
	AMZN.createDatabase(AMZN,Data);
	trackedStocks.push_back(AMZN);
	//AMZN.print();
	
	//Beginning to Track Twenty Year Treasury Yield
	filePath =  "TMF.txt";
	openInputFile(Data, filePath);
	StockData TMF("TMF","20 Year Treasury Yield ETF");
	TMF.createDatabase(TMF,Data);
	trackedStocks.push_back(TMF);
	
	//AMAZON Plus Strategy Implementation
	Portfolio AmazonPlus;
	amazonPlusInitiate(AmazonPlus, beginStrategy, AMZN, TMF);
	AmazonPlusStrategy (AmazonPlus, beginStrategy, todaysDate, AMZN, TMF);
	AmazonPlus.printPortfolio(todaysDate, AmazonPlus);
	
	//MovingAverage Trade Strategy Implementation
	Portfolio movingAverageStrategy;
	ABX.activelyTrade(movingAverageStrategy,ABX);
	
	return 0;
}