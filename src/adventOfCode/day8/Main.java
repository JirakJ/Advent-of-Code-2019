package adventOfCode.day8;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.Arrays;

public class Main {
    private final static String filePath = "src/resources/dayEightInput.txt";
    private static int width = 25;
    private static int height = 6;
    private static char[] topLayer = new char[width * height];
    private static int amount = 1000;
    private static String minLayer = "";
    
    private static int getSize(){
        return width * height;
    }

    private static int getZeros(char[] digits) {
        int zeros = 0;
        for (char c : digits) {
            if (c == '0') {
                zeros++;
            }
        }
        return zeros;
    }

    private static void solveBothParts(String input) {
        Arrays.fill(topLayer, '?');
        for (int i = 0; i < input.length() / getSize(); i++) {
            String layer = input.substring(i * getSize(), i * getSize() + getSize());
            char[] digits = getDigits(layer);
            int zeros = getZeros(digits);
            if (amount > zeros) {
                amount = zeros;
                minLayer = layer;
            }
        }
        int countOfOnes = 0, countOfTwos = 0;
        for (char c : minLayer.toCharArray()) {
            if (c == '1') {
                countOfOnes++;
            }
            if (c == '2') {
                countOfTwos++;
            }
        }
        System.out.println("Part 1: " + (countOfOnes * countOfTwos));
        System.out.println("Part 2: ");
        outputImage(topLayer);
        //Add blank line for nicer output in idea terminal.
        System.out.println();
    }

    private static char[] getDigits(String layer) {
        char[] digits = layer.toCharArray();
        for (int j = 0; j < digits.length; j++) {
            if (topLayer[j] == '?') {
                if (digits[j] != '2') {
                    topLayer[j] = digits[j];
                }
            }
        }
        return digits;
    }

    private static void outputImage(char[] chars) {
        for (int i = 0; i < getSize(); i++) {
            if (i != 0 && i % width == 0) {
                System.out.print("\n");
            }
            if (chars[i] == '1') {
                System.out.print("\u2588");
            } else if (chars[i] == '0') {
                System.out.print(" ");
            }
        }
    }

    public static void main(String[] args) throws IOException {
        String input = new String(Files.readAllBytes(Paths.get(filePath)));
        solveBothParts(input);
    }

}
