/*
Creator: TORRYANA_TANIS
Project: Phone Book 
Directions:put contacts.csv file in same folder as this 
program to make it excute properly
*/
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cstdio>

using namespace std;

const string tmpFileName = "tmp.csv";//declaring temporary file

// function prototypes
int countContacts(ifstream &file); //function to read contact info
string findContact(ifstream &file, const string lastName); //function to find individual contact
void addContact (
    ofstream &file,
    const string firstName,
    const string lastName,
    const string phone1,
    const string phone2,
    const string email
    ); //function for outputing line of contact infp
bool deleteContact(const string fileName, const string lastName); //function to delete line of contact info

int main (int argc, char* argv[]) 
{
    if (argc != 2) //ERROR message if more than one argument was entered
    {
        cout << "Usage: phonebook <filename>\n";
        return 0;
    }

    //variables declartion
    string fileName = argv[1];
    ifstream contactsRead;
    ofstream contactsWrite;
    bool deleted = false;
    string searchResult; 

    int count = 0;

    contactsRead.open(fileName);
    
    if (!contactsRead.is_open()) //ERROR message for file wasnt found
    {
        cout << "ERROR: File not found.\n" << argv[1];
        return 0;
    }

    count = countContacts(contactsRead);
    contactsRead.close(); //close file

    char selection;
    string firstName, lastName, phone1, phone2, email; //string declartion

    cout << "Found " << count << " contacts in " << fileName << ".\n"; 
    do 
    {
        //diplays menu for user
        cout << "=======Phonebook Management=======\n";
        cout << " l) Lookup Contact\n";
        cout << " a) Add Entry\n";
        cout << " d) Delete Entry\n";
        cout << " q) Quit\n";
        cout << ">> ";
        cin >> selection;
        selection = tolower(selection);
        
        switch(selection)
        {
            case 'l'://looks for contact

                cout << "\nEnter last name: "; //prompts user to enter last name
                cin >> lastName;
                contactsRead.open(fileName); //open input file
                findContact(contactsRead,lastName); //calls function
                contactsRead.close();//closes input file
            
                 break;

            case 'a': //adds contact to file

                cout << "\nEnter contact details...\n"; //prompts user for contact details
                cout << "\nFirst Name : ";
                cin >> firstName;
                cout << "Last Name : ";
                cin >> lastName;
                cout << "Phone 1 : ";
                cin >> phone1;
                cout << "Phone 2 : ";
                cin >> phone2;
                cout << "Email : ";
                cin >> email;
                
                contactsWrite.open(fileName, ios::app);//open file
                addContact(contactsWrite, firstName, lastName, phone1, phone2, email); //calling function
                contactsWrite.close();//close file
            
                cout << "Contact Added.\n"; //adds contact
                break;

            case 'd': //deletes contact

                contactsWrite.open(fileName, ios::app);//open file
                cout << "\nEnter last name: "; //prompts use for contact name to delete
                cin >> lastName; //recieves last name
                deleted = deleteContact(fileName, lastName); //calls function
                if (deleted) //if fuction is sucuessfull out put message to terminal
                {
                    cout << "Contact Deleted.\n";
                } //Sucess message
                if(!deleted) //if function doesn't properly work out put error message to terminal
                {
                    cout << "No such contact found.\n";
                } //ERROR message
                contactsWrite.close();//close file
            
                break;//breaks out of code

            case 'q': //quit program
                break;///exits code

            default:
                cout << "\nInvalid option\n\n"; // message for invalid input
                break;//exits code
        }

    } while(selection != 'q'); //do the statement until q is seleceted

    return EXIT_SUCCESS;
}
int countContacts(ifstream &file)  //reads contacts
{
    string row; //string declaration
    int count = 0;
    while (getline(file, row))
    {
        count++; //reads every row until eof
    }
   
    return count;
}

string findContact(ifstream &file, const string lastName) //finds contact
{
    string line, phone1, phone2, fname, lname, email; //string declaration
    ostringstream result;//output file stream
    bool found = false; //flag declaration
    

    while (getline(file, line)) 
    {
        istringstream line_s(line); //input file stream

        //reads all contact info
        getline(line_s, fname, ',');
        getline(line_s, lname, ',');
        getline(line_s, phone1, ',');
        getline(line_s, phone2, ',');
        getline(line_s, email);

        if (lastName ==lname) //if last name is in file break
        {
            found = true;
            break; //exits if statement
        }
    }

    if (found) //prints out contact info
    {
    result << left << fname << " " << lname << setw(4) << " "
            << setw(15) << phone1 << setw(15) << phone2 
                << email << "\n"; //format
    cout<<result.str(); //converts result to a string
    cout<<endl;//format
    }
    else
    {
        cout<<"No such contact found."<<endl; //ERROR message
        cout<<endl; //format
    }
    return "";
}

void addContact(
ofstream &file,
const string firstName,
const string lastName,
const string phone1,
const string phone2,
const string email) 
{
   file<<firstName<<","<<lastName<<","<<phone1<<","<<phone2<<","<<email<<endl;
}
//made so it will list out contact info in console

bool deleteContact(const string fileName, const string lastName)
{
    bool found = false; //flag declaration
    string line, phone1, phone2, fname, lname, email;//string declaration
    ifstream file; //input file stream
    ofstream fileOut; //output file stream
   
    
    file.open(fileName); //open  input file
    fileOut.open(tmpFileName); //open output file
    int count = 0;
    int lineToDelete;

    while (getline(file, line)) 
    {
        istringstream line_s(line);
        //reads file information
        getline(line_s, fname, ',');
        getline(line_s, lname, ',');
        getline(line_s, phone1, ',');
        getline(line_s, phone2, ',');
        getline(line_s, email);
        count++;

        //cout << lname;
        if (lname == lastName) //if last name is in file continue to next
        {
            found = true;
            lineToDelete = count;
            continue; //continue to next part of code
        } 
        else 
        {
            fileOut << line << endl; 
            
        }
    }
        
    
    file.close(); //close inpur file
    fileOut.close(); //close output file




    remove(fileName.c_str()); //Removes orginial file
    rename(tmpFileName.c_str(), fileName.c_str()); //renames temporary file to old file name
    return found; //returns what is found
}
