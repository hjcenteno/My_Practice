#include <iostream>
#include <sstream>
#include <vector>
#include <iomanip>

using namespace std;
typedef vector <int> iVec;

int main(int argc, char const *argv[]){
    int row;
    vector < iVec > pascal; //usage of typedef
    istringstream sin;


    if(argc != 2){
        cerr << "usage: ./pascal rows" <<endl;
        return 1;
    }

    //argv[1] hold number of rows to make pascal's triangle
    //could use stoi but practice for sstreams
    sin.str(argv[1]);
    if(!(sin >> row)){
        cerr << "bad rows we need an int" << endl;
        return 1;
    }

    pascal.resize(row); //resizes the outer vector to hold the size of rows
    for(int i = 0; i < pascal.size(); i++){
        for(int j = 0; j <= i; j++){
            /*TODO:
                algorithm for cells
                will be using pascal[i].pushback(TODO);
            */
            if(j == 0 || j == i){
                pascal[i].push_back(1);
            }else{
                pascal[i].push_back(pascal[i - 1][j - 1] + pascal[i - 1][j]);
                
            }
        }
    }

    //print triangle
    for(int i = 0; i < pascal.size(); i++){
        for(int j = 0; j < pascal[i].size(); j++){
            cout << pascal[i][j] << " ";
        }
        cout << endl;
    }

    return 0;
}
