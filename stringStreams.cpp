#include <iostream>
#include <sstream>
#include <limits>
#include <string>

using namespace std;

int main()
{
    //INPUT STRING STREAMS
    string input = "512 Main St";

    // Declare and init
    istringstream sin;
    sin.str(input);

    //could be done in one line
    int stNumber;
    string stName, stType;
    sin >> stNumber >> stName >> stType; 

    cout <<"The street number is " << stNumber << endl;
    cout << "The street type is "  << stType << endl;
    
    /*if I want to reuse sin wuth a new input string 
     then I need to use .clear() and give it a new string */
    string input2 = "416 Broadway Ave";
    sin.clear();
    sin.str(input2);

    //repeat extraciton step

    /* OUTPUT STRINGSTREAM*/
    ostringstream sout; 
    int num = 10;
    string name = "Maloney";
    string type = "St";

    sout << num << " " << name << " " << type;

    /*use .str() slightly differently here - we use it to get the string out of sout*/ 
    string address = sout.str();
    cout << address << endl;

    return 0;
}
