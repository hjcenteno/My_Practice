#include <iostream>
#include <sstream>
#include <limits>
#include <string>
#include <fstream>

using namespace std;

int main()
{
    //TODO open file and error check
    string name, email, jan, feb, mar, apr, may, line;
    string file = "dues.csv";
    int age;
    bool j, f, m;

    ifstream fin(file);
    if(!fin.is_open()){
        cerr << "file failed to open for read -- check permissions" << endl;
        return 1;
    }

    //TODO read from the file
    getline(fin, line);
    cout << line << endl;

    while (getline(fin, line))
    {
        istringstream sin(line);
        getline(sin, name, ',');
        cout << name << endl;
    }
    
    //TODO and store
    return 0;
}
