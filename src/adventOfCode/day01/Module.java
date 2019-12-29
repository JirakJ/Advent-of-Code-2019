package adventOfCode.day01;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.LinkedList;
import java.util.List;
import java.util.stream.Collectors;
import java.util.stream.Stream;

class Module {

    static int part1(String filePath) {
        List<Integer> input = new LinkedList<>();
        int result = 0;

        try (Stream<String> stream = Files.lines(Paths.get(filePath))) {
            input = stream
                    .map(Integer::valueOf)
                    .collect(Collectors.toList());
        } catch (IOException e) {
            e.printStackTrace();
        }

        for (int current : input) {
            result += Math.floorDiv(current, 3) - 2;
        }

//        System.out.println(result);

        return result;
    }

    static int part2(String filePath) {
        List<Integer> input = new LinkedList<>();
        int result = 0;

        try (Stream<String> stream = Files.lines(Paths.get(filePath))) {
            input = stream
                    .map(Integer::valueOf)
                    .collect(Collectors.toList());
        } catch (IOException e) {
            e.printStackTrace();
        }

        for (int current : input) {
            result += calculateFuel(current);
        }

//        System.out.println(result);

        return result;
    }

    private static int calculateFuel(int current) {
        if (current <= 8) return 0;
        else {
            int result = Math.floorDiv(current, 3) - 2;
            return result + calculateFuel(result);
        }
    }
}