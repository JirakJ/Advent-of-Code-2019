package adventOfCode.dayFour;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class Main {
    private final static int minValue = 153517;
    private final static int maxValue = 630395;

    private static boolean exactlyADouble = false;

    private static boolean validateNumber(int i) {
        Map<Character, Integer> numberCount = new HashMap<>();

        char lastNumber = ' ';
        for(char number : String.valueOf(i).toCharArray()) {

            if (number < lastNumber){
                return false;
            }

            numberCount.put(number, numberCount.getOrDefault(number, 0) + 1);
            lastNumber = number;
        }

        for(int numOfNumber : numberCount.values()) {
            if ((exactlyADouble && numOfNumber == 2) || (!exactlyADouble && numOfNumber >= 2)) {
                return true;
            }
        }

        return false;
    }

    public static void main(String[] args) {
        List<Integer> validAnswers = new ArrayList<>();
        for (int i = minValue; i <= maxValue; i++) {
            if (validateNumber(i)) {
                validAnswers.add(i);
            }
        }

        System.out.println((exactlyADouble==false?"Part 1:":"Part 2:")+"Number of Valid Answers : " + validAnswers.size());
    }
}