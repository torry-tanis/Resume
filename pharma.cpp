/*
Creator: TORRYANA_TANIS
Project: warehouse_manager
Directions: put warehousedb.csv file in same
folder as this project to ensure the program
executes properly
*/
#include <climits>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>



using namespace std;

//enum declaration & definiton
enum ItemCategory 
{
	ANTIBIOTIC,
	PAINKILLER,
	ANTIBACTERIAL,
	ANTIHISTAMINE,
	SUPPLEMENT,
	ANTACID,
	UNCATEGORIZED
};


//struct ddeclaration & efiniton
struct pharmaItem 
{
	int id = -1;
	string name = "";
	int price = -1;
	int quantity = -1;
	ItemCategory category = ItemCategory::UNCATEGORIZED;
};
//constant declarations 
const int DATABASE_SIZE = 100; //< sets the sixe of database to 100           
const string DATABASE_FILE_KEY = "db"; //<declaration of database file key     
const string RECEIPT_COUNT_KEY = "receipt"; //<declaration of reciept

//function declaration/prototypes
bool loadConfiguration(string filename, string &dbFilename, int &receiptCount);

pharmaItem parseItem(string csvRow);

ItemCategory stringToCategory(string categoryString);

string categoryToString(ItemCategory categoryEnum);

int findItem(pharmaItem database[], int id);

void showItems(pharmaItem items[]);

bool updateStock(pharmaItem database[], int id, int qtyAdd);

void showReceipt(pharmaItem database[], int itemID[], int itemQty[], int &receiptNumber);

bool processPurchaseOrder(pharmaItem database[], string orderFilename, int &receiptNumber);

void showCategory(pharmaItem database[], ItemCategory category);

void saveDatabase(pharmaItem database[], string databaseFilename, string configFilename,
                 int receiptCount);
/*
* @Function takes user input to output selceted menu item
* @param <int argc, char*argv[]> command line arguement is passed to funtion
* @returns Error message if user inpute more than 2 command line arguments
* @returns Error message if configuraton was not found
* @returns Error message if databaseFileName is not found
* @return corresping menu element upon selection from the user
*/

int main(int argc, char *argv[]) 
{
	if (argc != 2) //if more than two command-line arguments
	{
		cout << "Usage: pharma <configuration file>\n"; //outputs erroe message
		return EXIT_SUCCESS;
	}
	string databaseFilename = "";	//<variable used to open warehouse database
	int receiptCounter;				//<variable used to make recipt for user		
    string configFilename = argv[1];//<variable tells compiler that the second argument to the file to open



	if (!loadConfiguration(configFilename, databaseFilename, receiptCounter)) { //calls loadconfiguraion funtion
		cout << "Error loading configuration from file " << argv[1] << ".\n"; //outputs error message
		return EXIT_FAILURE; //returns 0
	}
    

   ifstream databaseFile(databaseFilename); //input file stream for database file

	if (!databaseFile) //if statement for error
	{
		cout << "Error loading database from file " << databaseFilename << "\n"; //outputs error message
		return EXIT_FAILURE; //returns 0
	}

    pharmaItem database[DATABASE_SIZE]; //<array for pharmaItem struct and it has the array size 100
	string itemRow; 					//<Variable used to read in row of file
	getline(databaseFile, itemRow);  //reads first line of file
    
   
    
	for (int currentEntry = 0; getline(databaseFile, itemRow); ++currentEntry) //get file line by line
	{
		database[currentEntry] = parseItem(itemRow);  //makes database array equal to parseitem array
	}
    
	databaseFile.close(); //closes file

    char userInput;
	do 
	{
		cout << "\n**********WAREHOUSE MANAGER**********\n";
		cout << " s) Show Inventory\n"
		     << " u) Update Stock\n"
		     << " p) Process Purchase Order\n"
		     << " c) Show Category\n"
			 << " w) Write Changes to File\n"
		     << " q) Quit\n"
		     << "\n  >> ";
		cin >> userInput;
		userInput = tolower(userInput); 
		switch (userInput) 
		{
		case 's': //this option show all itmes
			showItems(database);
			break;
		case 'u': //this option uodates stocks
		{
            
            
			int id, qty; //vairable declarations
			cout << "ID and Quantity to Add (separated by space): ";
			cin >> id >> qty;
			if (updateStock(database, id, qty)) {
				cout << "Stock updated successfully!\n";
			} else {
				cout << "Item not found!\n";
			}
            
			break;
		}
		case 'p': //option processes order
		{
            
            
			string orderFile;
			cout << "Order file name: ";
			cin >> orderFile;
			processPurchaseOrder(database, orderFile, receiptCounter);
			break;
            
		}
		case 'c': //option shows category
		{
           
            
			string categoryInput;
			cout << "Category to display: ";
			cin >> categoryInput;
			showCategory(database, stringToCategory(categoryInput)); 	
            
			break;
		}
		case 'w': //option saves changes
            
            
			saveDatabase(database, databaseFilename, configFilename, receiptCounter);
			cout << "Database Saved!\n";
            
			break;
		case 'q': //option quits
		{
            
            
			char save;
			cout << "Save Database? (y/n): ";
			cin >> save;
			save = tolower(save);
			if (save == 'y') {
				saveDatabase(database, databaseFilename, argv[1], receiptCounter);
				cout << "Database Saved!\n";
			} else {
				cout << "Quit without saving.\n";
			}
            
			break;
		}
		default:
			//clears out user's input
			cin.clear();
			cin.ignore(100, '\n');
		}
	} while (userInput != 'q');
}
/*
* @Function opens the config file, gets the database filename, and gets urrent receipt counter 
* @param <string filename> passes name of file to function
* @param <string &dbFilename> passes a pass by reference filename to function
* @param <int &reciptCount> passes a pass by reference reciept count to function
* @returns logical statement (true or false) will be used as a flag
*/
bool loadConfiguration(string filename, string &dbFilename, int &receiptCount) 
{
	
    ifstream reader; //< input file stream reading file

	
	string key; 		//< variable declared her to later use for reading in key
	string valuePairs;	//< variable declared her to later use for reading in value pairs
	int flag = 0;

	reader.open(filename); //opens file

	if (!reader.is_open()) //if statement if file does not open
    {
        cout << "ERROR: File not found."<<endl; //erroe message is outputed
        return false;
    }

	while(!reader.eof()) //excutes when file is not open
	{
		
			getline(reader, key, '='); //reads file until delimiter
			cout<<key<<endl;
		

		
			getline(reader, valuePairs, '\n'); //reads file until end of line
			cout<<valuePairs<<endl;
		
		if(reader.eof()) //excutes if end of file is reached
		{
			break;
		}
		if(key == DATABASE_FILE_KEY) //excutes if keys match
		{
			dbFilename= valuePairs; 
			flag++; //triggers the flag
		}
		if(key == RECEIPT_COUNT_KEY) //excutes if key matches recipet counter key
		{ 
			receiptCount = stoi(valuePairs);//receipt count becomes same value as value pair
			flag++;
		}

	}
	reader.close(); //close files

  if (flag == 2) //iif both are foun true
  {
	return true;
  }
  else
  {
	return false;
  }
  
}
/*
* @Function takes a string of the database file and converts the values to pharmaItem object
* @param <sring csvRow> passes string to fucntion
*@returns a new item
*/

pharmaItem parseItem(string csvRow) 
{
	string tmpCsv; //< variable is declared to later use as a temporary file
	pharmaItem newItem; //< variable used to set default values.

	istringstream line_s(csvRow); //< input string stream
	getline(line_s,tmpCsv, ',');
	newItem.id = stoi(tmpCsv);
	getline(line_s,newItem.name, ',');
	getline(line_s,tmpCsv, ',');
	newItem.price = stoi(tmpCsv);
	getline(line_s,tmpCsv, ',');
	newItem.quantity = stoi(tmpCsv);
	getline(line_s,tmpCsv,'\n'); 
	newItem.category = stringToCategory(tmpCsv);
	
	return newItem;
}
/*
* @Function takes a string in order to return ItemCategory enum value. 
* @param <string categoryString> passes string to fucntion
*@returns an item's category
*/
ItemCategory stringToCategory(string categoryString) 
{
	if (categoryString == "Antibiotic") return ItemCategory::ANTIBIOTIC;
	if (categoryString == "Painkiller") return ItemCategory::PAINKILLER;
	if (categoryString == "Antibacterial") return ItemCategory::ANTIBACTERIAL;
	if (categoryString == "Antihistamine") return ItemCategory::ANTIHISTAMINE;
	if (categoryString == "Antacid") return ItemCategory::ANTACID;
	if (categoryString == "Supplement") return ItemCategory::SUPPLEMENT;
	return ItemCategory::UNCATEGORIZED;
}
/*
* @Function takes an enum value of type ItemCategory in oder to return string
* @param <ItemCategory categoryEnum> passes item category enum valaue to fucntion
*@returns string
*/

string categoryToString(ItemCategory categoryEnum) 
{
	switch (categoryEnum) {
	case ANTIBIOTIC: return "Antibiotic";
	case PAINKILLER: return "Painkiller";
	case ANTIBACTERIAL: return "Antibacterial";
	case ANTIHISTAMINE: return "Antihistamine";
	case ANTACID: return "Antacid";
	case SUPPLEMENT: return "Supplement";
	case UNCATEGORIZED: return "None";
	default: return "";
	}
}
/*
* @Function takes in database and the id to get parameter
* @param <pharmaItem database[]> passes pharama Item database arrray to function
* @param <int id> pass id value to fucntion
*@returns arrray index of string
*/
int findItem(pharmaItem database[], int id) 
{
	int index = -1; //< variable is declared in order to later use in
	if(id<0)
	{
		return index;
	}
	for(int i = 0; i < DATABASE_SIZE; i++)
	{
			if(id == database[i].id)
			{
				index = i;
				break;
			}
	}

	return index;
}

/*
* @Function take in an array of  and prints it to console
* @param <pharmaItem items[]> passes pharamaItem items array to funtiocn
*@returns printed array
*/
void showItems(pharmaItem items[]) 
{
	cout << "\n ";
	for (int i = 0; i < 71; i++) cout << "-";
	cout << " \n";
	cout << left;
	cout << "|" << setw(6) << "ID"
	     << "|" << setw(25) << "Name"
	     << "|" << setw(8) << "Price"
	     << "|" << setw(8) << "Qty"
	     << "|" << setw(20) << "Category"
	     << "|\n";
	cout << setfill('=');
	cout << "|" << setw(6) << ""
	     << "|" << setw(25) << ""
	     << "|" << setw(8) << ""
	     << "|" << setw(8) << ""
	     << "|" << setw(20) << ""
	     << "|\n";
	cout << setfill(' ');
	for (int i = 0; i < DATABASE_SIZE; i++) 
	{
		if (items[i].id == -1) break;
		cout << left;
		cout << "|" << setw(6) << items[i].id
		     << "|" << setw(25) << items[i].name
		     << "|" << fixed << setprecision(2) << setw(1) << "$"
		     << setw(7) << (float)items[i].price / 100
		     << "|" << setw(8) << items[i].quantity
		     << "|" << setw(20)
		     << categoryToString(items[i].category) << "|\n";
	}
	cout << " ";
	for (int i = 0; i < 71; i++) cout << "-";
	cout << "\n";
}
/*
* @Function takes entire database, id, and quantity in order to add to quanitiy
* @param <pharmaItem database[]> passes pharaItem database array to function
* @param <int id> passes id value to function
* @param <int qtyAdd> pass quanitiy number to fuction
*@returns updated stock
*/

bool updateStock(pharmaItem database[], int id, int qtyAdd) 
{
	int index = findItem(database, id); //calls finditem function
	
	if(index>=0) //updates quanity
	{
		database[index].quantity += qtyAdd; //new quanitity is new plus old
		return true;
	}
	if(database[index].quantity<0) //set quanitity equal to zero if negative
	{
		database[index].quantity=0;
	}

	return false;
}
/*
* @Function generates a receipt and print it to the console for user
* @param <pharmaItem database[]> passes pharmaItem database array to function
* @param <int itemID[]> passes item id value to function
* @param <int itemQty[]> passes item quanitiy value to function
* @param <int &receiptNo> passes a pass by referece receipt number to function
*@returns printed receipt
*/

void showReceipt(pharmaItem database[], int itemID[], int itemQty[], int &receiptNo) 
{
	int total = 0;
	cout << "\n ";
	for (int i = 0; i < 59; i++) cout << "*";
	cout << " \n|";
	cout << left << setw(59) << "Receipt #" + to_string(receiptNo)
	     << "|\n|";
	for (int i = 0; i < 59; i++) cout << "-";
	cout << "|\n";
	cout << "|" << setw(6) << "ID"
	     << "|" << setw(25) << "Name"
	     << "|" << setw(8) << "Price"
	     << "|" << setw(8) << "Qty"
	     << "|" << setw(8) << "Total"
	     << "|\n";
	cout << setfill('=');
	cout << "|" << setw(6) << ""
	     << "|" << setw(25) << ""
	     << "|" << setw(8) << ""
	     << "|" << setw(8) << ""
	     << "|" << setw(8) << ""
	     << "|\n";
	cout << setfill(' ');
	for (int i = 0; i < DATABASE_SIZE; i++) 
	{
		if (itemID[i] == -1) break;
		int index = findItem(database, itemID[i]);
		cout << "|" << setw(6) << itemID[i];
		if (index == -1) 
		{
			cout << "|" << setw(25) << "**NO STOCK**"
			     << "|" << fixed << setprecision(2) << setw(1) << " "
			     << setw(7) << " "
			     << "|" << setw(8) << " "
			     << "|" << setw(1) << " "
			     << setw(7) << " "
				 << "|\n";
		} 
		else 
		{
			total += itemQty[i] * database[index].price;
			cout << "|" << setw(25) << database[index].name
			     << "|" << fixed << setprecision(2) << setw(1) << "$"
			     << setw(7) << (float)database[index].price / 100
			     << "|" << setw(8) << itemQty[i]
			     << "|" << setw(1) << "$"
			     << setw(7) << (float)itemQty[i] * database[index].price / 100
				 << "|\n";
		}
	}
	cout << setfill('-');
	cout << "|" << setw(6) << ""
	     << "|" << setw(25) << ""
	     << "|" << setw(8) << ""
	     << "|" << setw(8) << ""
	     << "|" << setw(8) << ""
	     << "|\n";
	cout << setfill(' ');
	cout << "|" << setw(6) << ""
	     << "|" << setw(25) << ""
	     << "|" << setw(8) << ""
	     << "|" << setw(8) << ""
		 << "|" << setw(1) << "$"
	     << setw(7) << (float)total/100
	     << "|\n";
	cout << " ";
	for (int i = 0; i < 59; i++) cout << "*";
	cout << " \n";
}
/*
* @Function processes order text file for purchased items and generates a receipt for it
* @param <pharmaItem database[]> passes pharmaItem database array to function
* @param <string orderFilename> passes string to fucntion
* @param < int &receiptNumber> passes pass by reference reciept number to function
*@returns logical statment (true or false)
*/

bool processPurchaseOrder(pharmaItem database[], string orderFilename, int &receiptNumber) 
{
    int itemID[DATABASE_SIZE], itemQty[DATABASE_SIZE]; 	//<arrays for item Identification and item quaninity
	ifstream orderFile (orderFilename);					//< input file stream for orderFilename
	ofstream outputOrderFile;							//<output file stream for orderFile

	if(!orderFile.is_open()) //excutes if file does not open
	{
		return false;
	}

	for (int i = 0; i < DATABASE_SIZE; i++)
	{
		itemID[i]=-1;
		itemQty[i]=-1;
	}


	int index = 0;
	while(!orderFile.eof()) //excutes while file is still being read
	{
		int quantity = 0;									//<variable is decalred to keep track of quanitiy 
		orderFile >> itemID[index];							//output file takes in item id array
		int item_index = findItem(database, itemID[index]);	//<variable calls find item function
		orderFile >> quantity;

		if(item_index<0)//increases index
		{
			index++;
			continue; 
			
		}
		if (quantity <= database[item_index].quantity)
		{
		
			itemQty[index] =  quantity;
		} else {
			itemQty[index] = database[item_index].quantity;
		}
		updateStock(database, itemID[index], -itemQty[index]); //calls updateStock and subtract quanitity

		index++;
		

	}
	showReceipt(database, itemID, itemQty, receiptNumber);
	receiptNumber++;

	return true;
}
/*
* @Function task is to show all items in the database assigned to a specific category
* @param <pharmaItem database[]> passes pharmaItem database arrayto function
* @param <ItemCategory category> passes ItemCategory category struct to fuction
*@returns none
*/

void showCategory(pharmaItem database[], ItemCategory category) 
{
	pharmaItem pharmarr[DATABASE_SIZE]; //<array for pharma Item and uses database_size to set array size to 100
	int size = 0;						//<variable is used as another size for array
	for (int i = 0; i < DATABASE_SIZE; i++)
	{
		if(database[i].category == category){
			pharmarr[size] = database[i];
			size++;
		}
	}
	 for (int i = 0; i < size; i++) { //selection sort algorithm
        int min_index = 0;
        int min_number = INT_MAX;
        for (int j = i; j < size; j++) 
        {
            if (pharmarr[j].price < min_number) 
			{
                min_number = pharmarr[j].price;
                min_index = j;
            }
        }
        swap(pharmarr[min_index], pharmarr[i]); //calls swap fucntion in order to swap index and pharmarrr
    }
	showItems(pharmarr);//calls showItems function
    
}
/*
* @Function was created to help with selction sort swaps index and pharmarr in fucntion above
* @param <pharmaItem &x> passes pass by reference to function
* @param <pharmaItem &y> passes pass by reference to function
*@returns none
*/
void swap (pharmaItem &x, pharmaItem &y) 
{
    pharmaItem temp;	//<variable is temporary storage for the valuse of x and y
    temp = x; //store value of x in temp
    x = y;
    y = temp; //stores value of y in temp
}
/*
* @Function converts database object to csv, rewrites original file, and updates the config file with the receipt counter.
* @param <pharmaItem database[]> passes pharmaItem database array to function
* @param <string databaseFilename> passes string to function
* @param <string configFilename> passes string to fucntion
* @param <int receiptCount> passes receiptCount value to funtcion
*@returns none
*/
void saveDatabase(pharmaItem database[], string databaseFilename, string configFilename,
                 int receiptCount) 
{
	ifstream wareFile(databaseFilename);	//<input file stream for warehouse data base
	string header;							//< variable declared so we can later get rid of header line in file
	getline(wareFile,header);				//reads first line of file		
	wareFile.close();						//closes file
	ofstream ofile;							//output file stream 
	ofile.open(databaseFilename);			//opens output file steam
	ofile << header << endl;				//output file takes in header
	for (int i = 0; i < DATABASE_SIZE; i++){
		if(database[i].id<0)
		{
			continue;
		}
		ofile<<database[i].id<<","<<database[i].name<<","<<database[i].price<<","<<database[i].quantity
		<<","<<categoryToString(database[i].category)<<endl; //ofiles takes in all elements from database
	}
	ofile.close();	//closes output file
	ofstream config; //output file stream for config
	config.open(configFilename); //opens config file
	config << "db=" << databaseFilename << endl; //config takes in info from orginal file
	config << "receipt=" << receiptCount;
}