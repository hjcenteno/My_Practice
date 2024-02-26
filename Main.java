package Java;

import java.util.Scanner;

class Main{
    static Scanner scan=new Scanner(System.in);
    public static void main(String[] args) {
        // char a=49, b=48, c=48;
        // System.out.println(a+""+b+""+c);

        // String s="Hello World", ss;
        // ss=s.substring(6, s.length());
        // System.out.println(ss);
        // System.out.println((int) s.charAt(10)); //String is treated as an array with the characters having their own index starting at 0

        //stringbuilder allows for more effecient memory allocation
        // StringBuilder sb = new StringBuilder(15);
        // sb.append("Stephen is ");
        // sb.append(41);
        // sb.append(4 + 1);
        // sb.append("World");
        // sb.insert(0, "Hello ");
         
        // System.out.println(sb.substring(5, 7));
        // sb.delete(0, 4);
        // System.out.println(sb.toString());
       

        // System.out.println("DO you like COSC 101: y/n");
        // // System.out.println(scan.hasNextInt()); //outputs a true/false

        // if(!scan.hasNextInt()){
        //     System.out.println("You did not give me an integer pendejo");
        //     return; //gets out of the function
        // }
        
        // int myInt=scan.nextInt();

        // if(myInt>=10){
        //     System.out.println("thats too big for me! I need it to be less than 10");
        //     return;
        // }

        // //System.out.println("You like COSC 101!");
       
        // String lookFor;
        // String s;
        // char c;
        // int i;
        
        // System.out.print("enter a string: ");
        // s=scan.nextLine();

        // do {
        //     System.out.print("enter needle: ");
        //     lookFor=scan.next();
        // } while (lookFor.length()!=1);
        
        // c=lookFor.charAt(0);
        // for (i = 0; i < s.length(); i++) { //initialization; comparison; reassignment
        //     if(s.charAt(i)==c){
        //         System.out.printf("character %s found at index %d.%n", c, i);
        //         break;
        //     }
        // }
        // if(i==s.length()) System.out.println("could not find need :'(");

        // scan.close();
        // String hasStudents;
        // int numStudents=0, numGrades, i;
        // double grade, classAvg=0, gradeAvg=0;
        // do {
        //     numStudents++;
        //     for (i = 0; i < numStudents; i++) {
        //         System.out.print("How many graded assignments? ");
        //         numGrades=scan.nextInt();
        //         for (int j = 0; j < numGrades; j++) {
        //             System.out.printf("grade for student %d: ", numStudents);
        //             grade=scan.nextDouble();
        //             gradeAvg=grade/numGrades;
        //             classAvg+= grade;
        //         }
        //         classAvg/=numStudents;
        //         //student # grade average is a .2f
        //         System.out.printf("student %d's grade average is %.2f %n", numStudents, gradeAvg);
        //     }
            
        //     do {
        //         System.out.print("Do you have another student? y/n? \n");
        //         hasStudents=scan.nextLine();
        //     } while (!(hasStudents.equals("y")||hasStudents.equals("n")));
        // } while (hasStudents.equalsIgnoreCase("y"));
        // System.out.printf("class average: %.2f", classAvg);
        
            double v = 10.956;
            System.out.print((int)v);
         
    }
}
