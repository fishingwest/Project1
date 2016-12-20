#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#include <vector>
#include <iomanip>
#include <map>
using namespace std;

class StockData {
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
	void activelyTrade();
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
private:
	int quantity;
	double price;
	double avgCost;
	double mktValue;
	double bookValue;
	double gain_loss;
	double percentPortfolio;
	map <string,vector <double> > currentHoldings;
public:
	void printPortfolio ();
	double calculateGainLoss(double _mktValue, double _bookValue);
	double calculateBookValue(double _price, int _quantity);
	double calculateMktValue(double _price, int _quantity);
	double calculateAvgCost (double inPrice, int inQuantity);	
	double calculatePercentPortfolio(string symb);
	void buyStock(string _date, StockData & _stock, int addQuantity);
};
//StockData constructor assigns name and symbol of stocks that we are keeping track of
StockData :: StockData(string ticker, string stockName){
	name=stockName;
	symbol=ticker;
}

string StockData :: getName(){
	return name;
}

string StockData :: getSymbol(){
	return symbol;
}
//gets price for a stock on a given date
double StockData :: getPrice(string _date){
	double price;
	int position = dateFinder(_date);
	price = CLOSE[position];
	return price;
}
//finds where in the array the date is stored
int StockData :: dateFinder(string STRING){
	int position=10000;
	vector <string> :: iterator it;
	it = find(DATE.begin(), DATE.end(), STRING);
	if (it != DATE.end()){
		position=distance(DATE.begin(),it);
	}
	else {
		cout << "This is not a trading Date";
	}	
	return position;
}
//takes input data and stores it in memory
void StockData :: createDatabase (StockData & stock, ifstream & data){
	while (data >> stock.date >> stock.open >> stock.high >> stock.low >> stock.close >> stock.adj_close >> stock.volume){
			DATE.push_back(stock.date);
			OPEN.push_back(stock.open);
			HIGH.push_back(stock.high);
			LOW.push_back(stock.low);
			CLOSE.push_back(stock.close);
			ADJ_CLOSE.push_back(stock.adj_close);
			VOLUME.push_back(stock.volume);
		}
	data.close();
}
//prints stock data in tabular form
void StockData :: print() {
		cout << name << " (" << symbol << ")" << endl;
		cout << "DATE" << "\t \t" << "OPEN" << "\t" << "HIGH" << "\t" << "LOW" << "\t" << "CLOSE" << "\t" << "ADJCLO" << "\t" << "VOLUME" << endl;
		for (int i=0; i < OPEN.size();++i){
		cout << DATE[i] << "\t" << OPEN[i] << "\t" << HIGH[i] << "\t" << LOW[i] << "\t" << CLOSE[i] << "\t" << ADJ_CLOSE[i] << "\t" << VOLUME[i] << endl;
	}
		cout << endl; 
}
//calcualtes moving averages inputs are the duration and the current date
double StockData :: movingAverage(int length, string date){
	int position;
	
	double value=0.00;
	vector <string> :: iterator it;
	
	it = find(DATE.begin(), DATE.end(), date);
	if (it != DATE.end()){
		position=distance(DATE.begin(),it);
	}
	else {
		cout << "This is not a trading Date";
	}
	
	value= CLOSE[position];
	for (int i=position+length-1; i>position; i--){
		value+=CLOSE[i];
	}
	value = value/(double)length;
		
return value;
} 
//The algorithm constantly is taking in dates to forecast when to make a trade
//************************maybe reverse how this works
void StockData :: activelyTrade(){
	//implement input of todays date..... and ouput past trades completed.
	string todays_date = "12/16/2016";
	int position;
	vector <string> :: iterator it;
	ofstream myFile;
	myFile.open ("stock_trades.txt");
	
	it = find(DATE.begin(),DATE.end(),todays_date);
		if (it != DATE.end()){
		position=distance(DATE.begin(),it);
		}
		else {
		cout << "Not a Trading Date" << endl;
		}
	 
	for (it; it <DATE.end(); it++){
		string yesterdays_date=DATE[position+1];
		
		
		if (it==DATE.end()-200) {myFile << "Not enough data prior to " << todays_date << endl; break;};
		
		if (movingAverage(50,todays_date) >= movingAverage(200,todays_date) && movingAverage(50,yesterdays_date) < movingAverage(200,yesterdays_date)){
			myFile << todays_date << "\t" << getSymbol() << "\t" <<  CLOSE[position] << "\t BUY" << endl;
			position++;
			todays_date=yesterdays_date;
		}
		else if (movingAverage(50,todays_date) <= movingAverage(200,todays_date) && movingAverage(50,yesterdays_date) > movingAverage(200,yesterdays_date)){
			myFile << todays_date << "\t" << getSymbol() << "\t" <<  CLOSE[position] << "\t SELL" << endl;
			position++;
			todays_date=yesterdays_date;
		}
		else {
			position++;
			todays_date=yesterdays_date;
			}
			
	}
	myFile.close();
}

double Portfolio :: calculateAvgCost (double inPrice, int inQuantity){
		return calculateBookValue(inPrice,inQuantity)/inQuantity;
	}
	
double Portfolio :: calculateMktValue(double _price, int _quantity){
	return _price*_quantity;
}
	
double Portfolio :: calculateBookValue(double _price, int _quantity){
	return _price*_quantity;
}

double Portfolio :: calculateGainLoss(double _mktValue, double _bookValue){
	return 100*(_mktValue-_bookValue)/_bookValue;
}

void Portfolio :: printPortfolio (){
	map <string, vector <double> > :: iterator it=currentHoldings.begin();
	cout << "Quantity" << "\t" << "Price" << "\t \t" << "Average Cost" << "\t" << "Market Value" << "\t" << "Book Value" << "\t" << "Loss/Gain" << "\t" << "% Portfolio" << endl;
	for (it; it!=currentHoldings.end();it++){
		for (int i=0; i<7; i++){
			cout << fixed << setprecision(2) << it->second[i]<< "\t\t";
			}
		cout << endl;
		}
	}

double Portfolio :: calculatePercentPortfolio(string symb){
	double temp = currentHoldings.find(symb)->second[3];
	double sum;
	map <string, vector <double> > :: iterator it = currentHoldings.begin();
	for (it; it!=currentHoldings.end(); it++){
		sum += it->second[3];
	}
	double percentOwned=temp/sum*100;
	return percentOwned;
}	
//Takes in date stock is purchased the stock data and the quantity purchased in that order to add these to the portfolio	
void Portfolio :: buyStock(string _date, StockData & _stock, int addQuantity) {
	quantity=addQuantity;
	price=_stock.getPrice(_date);
	string stk_ticker= _stock.getSymbol();
	//std :: map <string>
	//if stock already exists in portfolio need to recalculate avg cost/quantity/bookvalue
	if (currentHoldings.find(stk_ticker)!=currentHoldings.end()){
		currentHoldings.find(stk_ticker)->second[0]=currentHoldings.find(stk_ticker)->second[0]+quantity;
		currentHoldings.find(stk_ticker)->second[1]=price;
		currentHoldings.find(stk_ticker)->second[3]=calculateMktValue(price,currentHoldings.find(stk_ticker)->second[0]);
		currentHoldings.find(stk_ticker)->second[4]=currentHoldings.find(stk_ticker)->second[4]+calculateBookValue(price,quantity);
		currentHoldings.find(stk_ticker)->second[5]=calculateGainLoss(currentHoldings.find(stk_ticker)->second[3],currentHoldings.find(stk_ticker)->second[4]);
		currentHoldings.find(stk_ticker)->second[2]=currentHoldings.find(stk_ticker)->second[4]/currentHoldings.find(stk_ticker)->second[0];
	}
	else {
		addStock stk_ticker;
		mktValue=calculateMktValue(price,quantity);
		bookValue=calculateBookValue(price, quantity);
		
		stk_ticker.info.push_back(quantity);
		stk_ticker.info.push_back(price);
		stk_ticker.info.push_back(calculateAvgCost(price,quantity));
		stk_ticker.info.push_back(mktValue);
		stk_ticker.info.push_back(bookValue);
		stk_ticker.info.push_back(calculateGainLoss(mktValue,bookValue));
		currentHoldings.insert(std :: pair<string,vector <double> >(_stock.getSymbol(),stk_ticker.info));
		currentHoldings[_stock.getSymbol()].push_back(calculatePercentPortfolio(_stock.getSymbol()));
	}	
	}
	
