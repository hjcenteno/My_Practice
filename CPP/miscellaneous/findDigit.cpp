/*The purpose of this program is to test the functionality of a method I found on StackOverflow in finding the number of digits and
printing the output back to the user.*/

#include <iostream>
#include <math.h>

using namespace std;

int main(){
    int input, digit;
    cout << "Enter an integer: ";
    cin  >> input;
    digit = input > 0 ? (int) log10((double) input) + 1 : 1;
    printf("%d has %d digits.", input, digit);
    return 0;
}
