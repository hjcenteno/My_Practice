package Java;

public class Test {
    static final char[] VOWELS={'A', 'a', 'E', 'e', 'I', 'i', 'O', 'o', 'U', 'u', 'Y', 'y'};
    public static void main(String[] args) {
        String city="knoxville";
        System.out.println(city);
        StringBuilder sb  = new StringBuilder();
        int i=0;
        boolean vowelFound=false;
        //finds the index of the vowel
        while (!vowelFound) {
            for (int j = 0; j < VOWELS.length; j++) {
                if(city.charAt(i)==VOWELS[j]){ //breaks once the char at i equals any value in VOWELS
                    vowelFound=true;
                    break;
                }
            }
            i++; //the index of the char +1
        }

        sb.append(city, i-1, city.length()); //takes the portion of the word beginning at the first vowel ending to the end of the world and assigning it to sb
        sb.append(city, 0, i-1); //takes the portion before the first vowel and appends to the end of the world
        sb.append("ay"); 
        System.out.println(sb.toString());
    }
}
