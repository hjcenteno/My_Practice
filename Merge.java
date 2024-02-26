package Java;

import java.util.Arrays;

public class Merge {
    //although faster with a runtime of O(log n), merge sorting does take more space at O(n)
    //creates a random array of with random length
    static int arrlen=(int) Math.floor(Math.random()*100);
    static int[] unsorted=new int[arrlen];

    Merge(){
        //random elements for the array
        for (int i = 0; i < unsorted.length; i++) {
            unsorted[i]=(int) Math.floor(Math.random()*100);
        }
    }

    public static void main(String[] args) {
        new Merge(); 
        System.out.println("unsorted: "+Arrays.toString(unsorted));
        mergeSort(unsorted);
        System.out.println("sorted: "+Arrays.toString(unsorted));

    }

    private static void mergeSort(int[] array) {
        int len=array.length; //length of current array in the recursion
        if(len<=1) return; //base case when array has one elem

        //halving the array
        int mid=len/2;
        int[] leftArr=new int[mid];
        int[] rightArr=new int[len-mid];

        int i=0; //index for left array
        int j=0; //index for right array
        for(; i<len; i++){
            if(i<mid){
                leftArr[i]=array[i]; //copies index in the current array into left array
            }else{
                rightArr[j]=array[i]; //copies for the right array
                j++;
            }
        }

        //sorts the halves with recursion with the previous array
        mergeSort(leftArr); 
        mergeSort(rightArr);
        merge(leftArr, rightArr, array);
    }

    private static void merge(int[] leftArr, int[] rightArr, int[]array){
        //merges the two arrays after sorting into the original array
        int leftSize=array.length/2;
        int rightSize=array.length-leftSize;
        int i=0, l=0, r =0; //index for original array, left array, and right array

        while(l<leftSize && r<rightSize){
            if(leftArr[l]<rightArr[r]){
                array[i]=leftArr[l];
                i++;
                l++;
            }else{
                array[i]=rightArr[r];
                i++;
                r++;
            }
        }

        //case when lengths of arrays are odd
        while (l<leftSize) {
            array[i]=leftArr[l];
                i++;
                l++;
        }

        while (r<rightSize) {
            array[i]=rightArr[r];
                i++;
                r++;
        }
    }
}