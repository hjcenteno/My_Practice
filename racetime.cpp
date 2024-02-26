#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;

string calculateAvgPace(int h, int m, int s){
    //get distance of marathon divided by the total time in seconds to get the velocity of mile/s, then we multiply the velocity by 1 mile to only have seconds left and we convert those seconds into min / mile time
    double distance = 26.2, pace;
    int    paceh = h, paceM = m, paceS = s;
    int totalSeconds = (paceH * 3660) + (paceM * 60) + paceS; //finds total secodns ran
    
    pace  = totalSeconds / distance; //speed of the runner in s/mi
    paceM = (int) (pace  / 60);
    paceS = (int) ((pace + 0.5)  % 60); //0.5 is added to allow for the number to be rounded up if the decimal is greater than 0.5 because concatenation does not round the number

    cout "paceH = " << paceH << << "paceM = " << paceM << "paceS = " << paceS << endl;
    return " ";
}   

int main()
{
    ifstream fin;
    ofstream fout;
    string first, last;
    int h, m, s;

    string inputFile = "racetimeHMS.txt";
    string outputFile = "results.txt";

    fin.open(inputFile);
    if(!fin.is_open()){
        cerr << "file failed to open for read" << endl;
        return 1;
    }

    fout.open(outputFile);
    if(fin.fail()){
        cerr << "file failed to open for write" << endl;
        return 1;
    }

    while(fin >> first >> last >> h >> m >> s){
        fout << last << ", " << first << ' ';
        fout << setfill('0');
        fout << h << ':' << setw(2) << m << ':' << setw(2) << s << endl;
    }

    fin.close();
    fout.close();

    calculateAvgPace(h, m, s);
    return 0;
}


