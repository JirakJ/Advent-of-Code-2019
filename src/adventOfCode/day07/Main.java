package adventOfCode.day07;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.concurrent.*;
import java.util.stream.IntStream;

import static java.util.Collections.swap;
import static java.util.stream.Collectors.toList;
import static java.util.stream.IntStream.range;

public class Main {
    private final static Integer ADD = 1;
    private final static Integer MULT = 2;
    private final static Integer INPUT = 3;
    private final static Integer OUTPUT = 4;
    private final static Integer JMP_IF_TRUE = 5;
    private final static Integer JMP_IF_FALSE = 6;
    private final static Integer LESS_THAN = 7;
    private final static Integer EQUAL = 8;
    private final static Integer EXIT = 99;

    private final static String filePath = "src/resources/daySevenInput.txt";

    public static void main(String[] args) throws IOException {
        Integer[] code = Arrays.stream(new String(Files.readAllBytes(Paths.get(filePath))).split(","))
                .mapToInt(Integer::parseInt)
                .boxed()
                .toArray(Integer[]::new);

        result(code, 0, 4, "Part 1: ", false);

        result(code, 5, 9, "Part 2: ", true);
    }

    private static void result(Integer[] code, int i, int i2, String s, boolean b) {
        List<List<Integer>> permutationsPart2 = getPermutations(range(i, i2 + 1).boxed().collect(toList()), 0);
        System.out.println(s + permutationsPart2.parallelStream()
                .mapToInt(l -> findLargestThrust(code, l, b)).max()
                .orElseThrow(() -> new RuntimeException("Unable to find a maximum value")));
    }

    private static List<List<Integer>> getPermutations(List<Integer> arr, int k) {
        ArrayList<List<Integer>> permutations = new ArrayList<>();
        for (int i = k; i < arr.size(); i++) {
            swap(arr, i, k);
            permutations.addAll(getPermutations(arr, k + 1));
            swap(arr, k, i);
        }
        if (k == arr.size() - 1) {
            permutations.add(new ArrayList<>(arr));
        }
        return permutations;
    }

    private static Integer findLargestThrust(Integer[] code, List<Integer> phases, boolean part2) {
        try {
            ThreadPoolExecutor executor = (ThreadPoolExecutor) Executors.newFixedThreadPool(8);
            List<BlockingQueue<Integer>> wires = IntStream.range(0, phases.size() + 1)
                    .mapToObj(c -> new ArrayBlockingQueue<Integer>(8))
                    .collect(toList());
            if(part2) {
                wires.remove(wires.size() - 1);
                wires.add(wires.size(), wires.get(0));
            }
            for (int i = 0; i < phases.size(); i++) {
                wires.get(i).put(phases.get(i));
                executor.execute(new Amp(code, wires.get(i), wires.get(i + 1)));
            }
            wires.get(0).put(0); // Initial Input
            executor.shutdown();
            executor.awaitTermination(1, TimeUnit.DAYS);
            return wires.get(wires.size() - 1).poll(10, TimeUnit.DAYS);
        } catch (Throwable t) {
            throw new RuntimeException(t);
        }
    }

    private static class Amp implements Runnable {
        private final Integer[] code;
        private int pc = 0;
        private final BlockingQueue<Integer> in;
        private final BlockingQueue<Integer> out;

        private Amp(Integer[] code, BlockingQueue<Integer> in, BlockingQueue<Integer> out) {
            this.code = Arrays.copyOf(code, code.length);
            this.in = in;
            this.out = out;
        }

        public void run() {
            try {
                do {
                    String cmd = String.format("%05d", code[pc]);
                    int opCode = Integer.parseInt(cmd.substring(3));
                    if (opCode == INPUT) {
                        code[code[pc + 1]] = in.poll(10, TimeUnit.DAYS);
                        pc += 2;
                        continue;
                    } else if (opCode == OUTPUT) {
                        Integer outVal = code[code[pc + 1]];
                        out.put(outVal);
                        pc += 2;
                        continue;
                    }
                    var param1 = cmd.charAt(2) == '0' ? code[code[pc + 1]] : code[pc + 1];
                    var param2 = cmd.charAt(1) == '0' ? code[code[pc + 2]] : code[pc + 2];

                    if (opCode == ADD) {
                        code[code[pc + 3]] = param1 + param2;
                        pc += 4;
                    } else if (opCode == MULT) {
                        code[code[pc + 3]] = param1 * param2;
                        pc += 4;
                    } else if (opCode == JMP_IF_TRUE) {
                        pc = param1 > 0 ? param2 : pc + 3;
                    } else if (opCode == JMP_IF_FALSE) {
                        pc = param1 == 0 ? param2 : pc + 3;
                    } else if (opCode == LESS_THAN) {
                        code[code[pc + 3]] = Math.toIntExact(param1 < param2 ? 1 : 0);
                        pc += 4;
                    } else if (opCode == EQUAL) {
                        code[code[pc + 3]] = Math.toIntExact(param1.equals(param2) ? 1 : 0);
                        pc += 4;
                    }
                } while (!code[pc].equals(EXIT));
            } catch (Throwable t) {
                throw new RuntimeException(t);
            }
        }
    }
}
