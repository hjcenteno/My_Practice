package Java;

import java.util.*;

public class MyScanner {
    public static Scanner scan=new Scanner(System.in);
    public static void main(String[] args) {
        String name;
        int age;

        System.out.print("What is your name? ");
        name=scan.next();

        System.err.println("Hwo old are you? ");
        age=scan.nextInt();
        scan.close();

        System.out.printf("Hello %s, you are %d years old.%n", name, age); 
    }
}
