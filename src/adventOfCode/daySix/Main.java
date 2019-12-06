package adventOfCode.daySix;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;

public class Main {

    public static void main(String[] args){
        try {
            String filePath = "src/resources/daySixInput.txt";
            String[] inputArray = Files.readString(Paths.get(filePath)).split("\n");
            System.out.println(Arrays.toString(inputArray));
            Main day6 = new Main(inputArray);
            day6.doPart1();
            day6.doPart2();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }


    private OrbitMap map = new OrbitMap();
    private int totalOrbits = 0;

    private Main(String[] input){
        calculateOrbits(input);
    }

    private void doPart1() {
        System.out.println(totalOrbits);
    }

    private void doPart2() {
        ArrayList<String> YOUtoCOM = new ArrayList<String>();
        YOUtoCOM.add(map.get("YOU").getImmediateOrbitID());
        while(!YOUtoCOM.get(YOUtoCOM.size()-1).equals("COM")){
            YOUtoCOM.add(map.get(YOUtoCOM.get(YOUtoCOM.size()-1)).getImmediateOrbitID());
        }

        ArrayList<String> SANtoCOM = new ArrayList<String>();
        SANtoCOM.add(map.get("SAN").getImmediateOrbitID());
        while(!SANtoCOM.get(SANtoCOM.size()-1).equals("COM")){
            SANtoCOM.add(map.get(SANtoCOM.get(SANtoCOM.size()-1)).getImmediateOrbitID());
        }
        for(int y = 0; y<YOUtoCOM.size() ; y++) {
            for(int s = 0; s<SANtoCOM.size() ; s++) {
                if(SANtoCOM.get(s).equals(YOUtoCOM.get(y))){
                    System.out.println(y+s);
                    return;
                }
            }
        }
    }


    private void calculateOrbits(String[] inputArray) {
        int lastMapSize	= 0;
        while(map.size()>lastMapSize){
            lastMapSize = map.size();
            for (String i: inputArray) {
                String[] a = i.split("[)]");
                if(map.containsKey(a[0])&&!(map.containsKey(a[1]))){
                    map.put(a[1], new Orbit(a[0],map.get(a[0]).getTotalOrbits()+1));
                    totalOrbits+= map.get(a[1]).getTotalOrbits();
                }
            }
        }
    }
}

class OrbitMap extends HashMap<String, Orbit> {
    OrbitMap(){
        put("COM", new Orbit());
    }
}

class Orbit{
    private String immediateOrbitID;
    private int totalOrbits;

    Orbit(String imediateOrbitID, int totalOrbits){
        this.totalOrbits = totalOrbits;
        this.immediateOrbitID = imediateOrbitID;
    }
    Orbit(){
        this.totalOrbits = 0;
        this.immediateOrbitID = "COM";
    }

    String getImmediateOrbitID(){
        return immediateOrbitID;
    }

    int getTotalOrbits(){
        return totalOrbits;
    }

}
