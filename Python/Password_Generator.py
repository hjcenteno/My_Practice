"""Password Generator
* ask for length of password
    - at least 8 characters
* ask if they want to include special characters
* default alphanumeric
* output password
"""

import random as rdm

length = 0
specialChar = False
password = ""

#create password from user's input
#param: length of password and if special charaters are allowed
def generatePW(n, sC):
    newPassword = ""
    for i in range(n):
        if(sC == True):
            currentChar = chr(rdm.randint(33, 126)) #choose a random character from the ascii table starting from 33 till 126
        else:
            choice  = rdm.randint(0, 2)
            #get characters from ascii table
            if(choice == 0): #digit
                currentChar = chr(rdm.randint(48, 57)) #0 - 9

            if(choice == 1): #uppercase
                currentChar = chr(rdm.randint(65, 90)) #A - Z

            if(choice == 2): #lowercase
                currentChar = chr(rdm.randint(97, 122))#a - z

        newPassword += currentChar
            
    return newPassword

#ask for length
while True:
    try:
        length = int(input("Length of password (at least 8 characters): "))
        if(length < 8): #since length is initially 0, this condition will run true
            print("Length of passwords must be at least 8 characters")
        else:
            break
    except ValueError:
        print("Invalid input, enter only an integer.")

#ask for special characters
while True:
    try:
        specialChar = input("Would you like to include special characters? (#, $, %, ...) y/n: ")
        if(specialChar == 'y'): #since length is initially 0, this condition will run true
            specialChar = True
            break
        elif(specialChar == 'n'):
            specialChar = False
            break
    except ValueError:
        print("Invalid input, enter only y or n.")

password = generatePW(length, specialChar)

print(f"Your password is {password}")