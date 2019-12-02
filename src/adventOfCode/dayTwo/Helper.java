package adventOfCode.dayTwo;

import java.util.List;

class Helper {
    List<Integer> codes;
    int checkSum;

    void run() {
        for (int address = 0; address < codes.size(); address += 4) {
            int value = codes.get(address);

            switch (value) {
                case 1:
                    codes.set(codes.get(address + 3), codes.get(codes.get(address + 2)) + codes.get(codes.get(address + 1)));
                    break;

                case 2:
                    codes.set(codes.get(address + 3), codes.get(codes.get(address + 2)) * codes.get(codes.get(address + 1)));
                    break;

                case 99:
                    return;
            }
        }
    }

}
