package adventOfCode.dayTwo;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.*;
import java.util.stream.Collectors;

public class Main {
    private static String filePath = "src/resources/dayTwoInput.txt";
    private Helper helper = new Helper();

    public static void main(String[] args) throws IOException {
        List<Integer> codes = Arrays.stream(Files.readString(Paths.get(filePath)).split(",")).mapToInt(s -> Integer.parseInt(s.trim())).boxed().collect(Collectors.toCollection(ArrayList::new));

        Main firstPart = new Main(new ArrayList<>(codes), 12, 2);
        System.out.println("Part1: " + firstPart.helper.codes.get(0));

        List<Main> part2 = new ArrayList<>();
        for (int i = 0; i < 100; i++)
            for (int j = 0; j < 100; j++) part2.add(new Main(new ArrayList<>(codes), i, j));
        for (Main p : part2) if (p.helper.codes.get(0) == 19690720) System.out.println("Part2: " + p.helper.checkSum);
    }

    private Main(List<Integer> codes, int noun, int verb) {
        helper.codes = codes;
        helper.codes.set(1, noun);
        helper.codes.set(2, verb);
        helper.checkSum = noun * 100 + verb;
        helper.run();
    }

}