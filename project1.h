#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#include <vector>
#include <iomanip>
#include <map>
#include <tgmath.h>
using namespace std;

class StockData;
class addStock;
class Portfolio;
extern vector <StockData> trackedStocks;
void openInputFile(ifstream &inFile, string inFileName);
void amazonPlusInitiate(Portfolio & myPortfolio, string _date, StockData &amazon, StockData &twentyYear);
void AmazonPlusStrategy (Portfolio & myPortfolio, string _date, string today, StockData & amazon, StockData &twentyYear);

class StockData {
friend void updatePortfolio(string theDate, Portfolio & myPortfolio);
friend void AmazonPlusStrategy (Portfolio & myPortfolio, string _date, string today, StockData & amazon, StockData &twentyYear);
private:
	string name;
	string symbol;
	string date;
	double open;
	double high;
	double low;
	double close;
	double adj_close;
	double volume;
	
	vector <string> DATE;
	vector <double> OPEN;
	vector <double> HIGH;
	vector <double> LOW;
	vector <double> CLOSE;
	vector <double> ADJ_CLOSE;
	vector <int> VOLUME;
public:		
	void createDatabase(StockData & stock, ifstream & data);
	void print();
	StockData(string, string);
	string getName();
	string getSymbol();
	double getPrice(string);
	double movingAverage(int length, string date);
	void activelyTrade(Portfolio & myPortfolio, StockData & currentStock);
	int dateFinder(string STRING);	
};

//class that is a container for stock data that is relevant to a portfolio
//for each stock purchased a new vector is created to hold the relevant information
class addStock {
	public:
	vector <double> info;
};

//class which stores the stocks quantities, values, of one persons stocks
class Portfolio{
friend class StockData;
friend void updatePortfolio(string theDate, Portfolio & myPortfolio);
friend void amazonPlusInitiate(Portfolio & myPortfolio, string _date, StockData &amazon, StockData &twentyYear);
friend void AmazonPlusStrategy (Portfolio & myPortfolio, string _date, string today,  StockData & amazon, StockData &twentyYear);
private:
	double accountCash;
	double investments;
	double accountBalance;
	int quantity;
	double price;
	double avgCost;
	double mktValue;
	double bookValue;
	double gain_loss;
	double percentPortfolio;
	map <string,vector <double> > currentHoldings;
public:
	Portfolio();
	void printAlgorithmResults (string _date,Portfolio &myPortfolio);
	void printPortfolio (string _date,Portfolio &myPortfolio);
	double calculateGainLoss(double _mktValue, double _bookValue);
	double calculateBookValue(double _price, int _quantity);
	double calculateMktValue(double _price, int _quantity);
	double calculateAvgCost (double inPrice, int inQuantity);	
	double calculatePercentPortfolio(string symb);
	void buyStock(string _date, StockData & _stock, int addQuantity);
	void sellStock (string _date, StockData & _stock, int addQuantity);
};