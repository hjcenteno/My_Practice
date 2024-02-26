/* class review in c++
marthon class, location struct
*/

#include <iostream>
#include <iomanip>
using namespace std;

class Marathon {
    private: 
        int hours, minutes;
        void privateHlprFunc();
    public:
        Marathon(int h, int m);
        void printTotalTime();
};

//constructor
Marathon::Marathon(int h, int m){
    hours=h;
    minutes=m;
}

//printTotalTime() belongs to the marathon class
void Marathon::printTotalTime(){
    cout << hours << ':' << setw(2) << setfill('0') << minutes << endl;
}

//random helper class associated with no class
int addTwoNumbers(int a, int b){
    return a+b;
}

//holds data
struct Location{
    string city, state;
};


int main(){
    //initializes the Marathon objects boston and nyc
    Marathon boston(3, 5);
    Marathon nyc(4, 10);

    boston.printTotalTime();
    nyc.printTotalTime();

    //using structs here
    Location raceLocation;
    raceLocation.city = "Boston";
    raceLocation.state = "MA";

    cout << "the race location is " << raceLocation.city << ", " << raceLocation.state << '.' << endl;
    return 0;
}
