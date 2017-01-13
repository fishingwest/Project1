#include "project1.h"
vector <StockData> trackedStocks;

//void amazonPlusTrading
void amazonPlusInitiate(Portfolio & myPortfolio, string _date, StockData &amazon, StockData &twentyYear){
	double priceAmazon = amazon.getPrice(_date);
	double priceTwenty = twentyYear.getPrice (_date);
	double amountForAmazon = myPortfolio.accountCash*0.66;
	double amountForTwenty = myPortfolio.accountCash*0.34;
	int quantityAmazon=amountForAmazon/priceAmazon;
	int quantityTwenty=amountForTwenty/priceTwenty;
	
	myPortfolio.buyStock(_date, amazon, quantityAmazon);
	myPortfolio.buyStock(_date, twentyYear, quantityTwenty);
}

void AmazonPlusStrategy (Portfolio & myPortfolio, string _date, string today, StockData & amazon, StockData &twentyYear){
	std :: vector <string> :: iterator it;
	it = find(amazon.DATE.begin(), amazon.DATE.end(), _date);		
	double priceAmazon;
	double priceTwenty;
	int amountForAmazon;
	int amountForTwenty;
	
	int quantityAmazon=0;
	int quantityTwenty=0;
	
	for (it; *it != today; it--){
		updatePortfolio(*it,myPortfolio);
		
		if (myPortfolio.currentHoldings.find(amazon.getSymbol())->second[6]>76){
		priceAmazon=myPortfolio.currentHoldings.find(amazon.getSymbol())->second[1];
		priceTwenty=myPortfolio.currentHoldings.find(twentyYear.getSymbol())->second[1];
		amountForAmazon=myPortfolio.accountBalance*0.66;
		amountForTwenty=myPortfolio.accountBalance*0.34;

		//How many we need to buy
		quantityTwenty=floor(amountForTwenty/priceTwenty)-floor(myPortfolio.currentHoldings.find(twentyYear.getSymbol())->second[0]);
		//How many we need to sell
		quantityAmazon=floor(myPortfolio.currentHoldings.find(amazon.getSymbol())->second[0])-floor(amountForAmazon/priceAmazon);
		
		//uncomment next 4 lines if you want to see what is bought and sold during each day the strategy is used
		//cout << "DATE: " << *it << endl;
		myPortfolio.buyStock(*it,twentyYear,quantityTwenty);
		myPortfolio.sellStock(*it,amazon,quantityAmazon);
		//myPortfolio.printAlgorithmResults(*it, myPortfolio);
		}
		
		if (myPortfolio.currentHoldings.find(amazon.getSymbol())->second[6]<56){
		priceAmazon=myPortfolio.currentHoldings.find(amazon.getSymbol())->second[1];
		priceTwenty=myPortfolio.currentHoldings.find(twentyYear.getSymbol())->second[1];
		amountForAmazon=myPortfolio.accountBalance*0.66;
		amountForTwenty=myPortfolio.accountBalance*0.34;
		
		//How many we need to sell
		quantityTwenty=floor(-(amountForTwenty/priceTwenty))+floor(myPortfolio.currentHoldings.find(twentyYear.getSymbol())->second[0]);
		//How many we need to buy
		quantityAmazon=floor(-myPortfolio.currentHoldings.find(amazon.getSymbol())->second[0])+floor(amountForAmazon/priceAmazon);
		
		//uncomment next 4 lines if you want to see what is bought and sold during each day the strategy is used
		//cout << "DATE: " << *it << endl;
		myPortfolio.sellStock(*it,twentyYear,quantityTwenty);
		myPortfolio.buyStock(*it,amazon,quantityAmazon);	
 		//myPortfolio.printAlgorithmResults(*it, myPortfolio);
		}
	}
}

//Updates Portfolio Values to todays date
void updatePortfolio (string theDate, Portfolio & myPortfolio) {
	std :: vector <StockData> :: iterator iter = trackedStocks.begin();
	std :: map <string, vector<double> > :: iterator i = myPortfolio.currentHoldings.begin();
	myPortfolio.investments=0;
	
	for (iter; iter!=trackedStocks.end();iter++){
		for (i; i !=myPortfolio.currentHoldings.end(); i++){
			
			if (i->first==iter->getSymbol()){
			int position = iter->dateFinder(theDate);
			i->second[1]= iter->ADJ_CLOSE[position];
			i->second[3]= iter->ADJ_CLOSE[position]*i->second[0];
			i->second[5]= 100*(i->second[3]-i->second[4])/i->second[4];
			myPortfolio.investments += i->second[3];
			}
			else {}
		}
		i=myPortfolio.currentHoldings.begin();
	}
		double sum = 0.0;
		map <string, vector <double> > :: iterator iterate = myPortfolio.currentHoldings.begin();
		for (iterate; iterate!=myPortfolio.currentHoldings.end(); iterate++){
			sum += iterate->second[3];
		}
		iterate = myPortfolio.currentHoldings.begin(); 
		for (iterate;iterate !=myPortfolio.currentHoldings.end(); iterate++){
		double temp = iterate->second[3];
		iterate->second[6]=temp/sum*100;		
		}
	
	myPortfolio.accountBalance=myPortfolio.accountCash+myPortfolio.investments;	
}

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
	price = ADJ_CLOSE[position];
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
//calculates moving averages inputs are the duration and the current date
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
void StockData :: activelyTrade(Portfolio & myPortfolio, StockData & currentStock){
	//Provides moving average trades for all dates stored in data file for stock.
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
			myPortfolio.buyStock(todays_date,currentStock,100);
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

//constructs portfolio with $100,000 in starting cash
Portfolio :: Portfolio () {
	accountCash=100000;
	accountBalance=100000;
	investments=0;
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

//Prints what the algorithm is doing this isn't very useful just to show you when the algorithm works
void Portfolio :: printAlgorithmResults (string _date,Portfolio &myPortfolio){

	updatePortfolio(_date,myPortfolio);
	map <string, vector <double> > :: iterator it=currentHoldings.begin();
	cout << "Cash: $" << accountCash << "\t Investments: $" << investments << "\t Total Balance: $" << accountBalance << endl << endl;
	cout << "Symbol" << "\t\t"  << "Quantity" << "\t" << "Price" << "\t\t" << "Average Cost" << "\t" << "Market Value" << "\t\t" << "Book Value" << "\t" << "% Loss/Gain" << "\t" << "% Portfolio" << endl;
	for (it; it!=currentHoldings.end();it++){
		cout << it->first << "\t\t";
		for (int i=0; i<7; i++){
			cout << fixed << setprecision(2) << it->second[i]<< "\t\t";
			}
		cout << endl;
		}
	}

// print portfolio to output file todays_performance.txt
void Portfolio :: printPortfolio (string _date,Portfolio &myPortfolio){
	ofstream myFile;
	myFile.open ("todays_performance.txt");
	myFile << _date << endl;
	updatePortfolio(_date,myPortfolio);
	map <string, vector <double> > :: iterator it=currentHoldings.begin();
	myFile << "Cash: $" << accountCash << "\t Investments: $" << investments << "\t Total Balance: $" << accountBalance << endl << endl;
	myFile << "Symbol" << "\t\t"  << "Quantity" << "\t" << "Price" << "\t\t" << "Average Cost" << "\t" << "Market Value" << "\t\t" << "Book Value" << "\t" << "% Loss/Gain" << "\t" << "% Portfolio" << endl;
	for (it; it!=currentHoldings.end();it++){
		myFile << it->first << "\t\t";
		for (int i=0; i<7; i++){
			myFile << fixed << setprecision(2) << it->second[i]<< "\t\t";
			}
		myFile << endl;
		}
		myFile.close();
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
void Portfolio :: sellStock (string _date, StockData & _stock, int addQuantity){
 quantity=addQuantity;
 price=_stock.getPrice(_date);
 
 //uncomment this if you want to see the transactions every time a stock is sold
 //cout << "SELL "<< _stock.getSymbol() << ": " << price*quantity << endl;
 
 accountCash=accountCash+price*quantity;
 investments=investments-price*quantity;
 accountBalance=accountCash+investments;
 string stk_ticker=_stock.getSymbol();

 if (currentHoldings.find(stk_ticker) != currentHoldings.end()){
	currentHoldings.find(stk_ticker)->second[0]=currentHoldings.find(stk_ticker)->second[0]-quantity;
	if (currentHoldings.find(stk_ticker)->second[0]!=0){
	currentHoldings.find(stk_ticker)->second[1]=price;
	currentHoldings.find(stk_ticker)->second[3]=currentHoldings.find(stk_ticker)->second[0]*price;
	currentHoldings.find(stk_ticker)->second[4]=currentHoldings.find(stk_ticker)->second[2]*currentHoldings.find(stk_ticker)->second[0];
	currentHoldings.find(stk_ticker)->second[5]=calculateGainLoss(currentHoldings.find(stk_ticker)->second[3],currentHoldings.find(stk_ticker)->second[4]);
	currentHoldings.find(stk_ticker)->second[6]=calculatePercentPortfolio(_stock.getSymbol());
	}
	else {
	currentHoldings.erase(stk_ticker);
	}
	
 }
 else {
	 cout << stk_ticker << " is not part of your portfolio and cannot be sold" << endl;
 }
}

void Portfolio :: buyStock(string _date, StockData & _stock, int addQuantity) {
	quantity=addQuantity;
	price=_stock.getPrice(_date);
	
	//uncomment this if you want to see the transactions every time a stock is bought
	//cout << "BUY " << _stock.getSymbol() << ": " << price*quantity << endl;
	
	accountCash=accountCash-price*quantity;
	investments=investments+price*quantity;
	accountBalance=accountCash+investments;
	string stk_ticker= _stock.getSymbol();
	
	//if stock already exists in portfolio need to recalculate avg cost/quantity/bookvalue
	if (currentHoldings.find(stk_ticker)!=currentHoldings.end()){
		
		currentHoldings.find(stk_ticker)->second[0]=currentHoldings.find(stk_ticker)->second[0]+quantity;
		if (currentHoldings.find(stk_ticker)->second[0] != 0){
		currentHoldings.find(stk_ticker)->second[1]=price;
		currentHoldings.find(stk_ticker)->second[3]=calculateMktValue(price,currentHoldings.find(stk_ticker)->second[0]);
		currentHoldings.find(stk_ticker)->second[4]=currentHoldings.find(stk_ticker)->second[4]+calculateBookValue(price,quantity);
		currentHoldings.find(stk_ticker)->second[5]=calculateGainLoss(currentHoldings.find(stk_ticker)->second[3],currentHoldings.find(stk_ticker)->second[4]);
		currentHoldings.find(stk_ticker)->second[2]=currentHoldings.find(stk_ticker)->second[4]/currentHoldings.find(stk_ticker)->second[0];
		currentHoldings.find(stk_ticker)->second[6]=calculatePercentPortfolio(_stock.getSymbol());
		}
		//erases the stock from the portfolio if a quantity of zero is owned
		else {
			currentHoldings.erase(stk_ticker);
		}
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

//opens input data files for creating database
void openInputFile(ifstream &inFile, string inFileName){
	inFile.open(inFileName.c_str());
	if (!inFile.is_open()){
		cout << "Unable to open file " << inFileName << endl;
	}
}
