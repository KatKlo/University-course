package robs.world;

import robs.creatures.Instructions;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Scanner;

public class Parameters {
    //field with food
    private int timeFoodGrow;
    private float energyFromFood;

    // simulation
    private int roundsNumber;
    private int statisticsAfter;

    // population
    private int startRobsNumber;
    private ArrayList<Instructions> startProgram;
    private float startEnergy;
    private float energyLossInRound;

    // mutations
    private ArrayList<Instructions> instructionsList;
    private float prDeleteInstr;
    private float prAddInstr;
    private float prChangeInstr;

    //duplication
    private float prDuplication;
    private float partParentEnergy;
    private float duplicationLimit;

    public Parameters(File file) throws FileNotFoundException {
        this.read(file);
    }

    /**
     * Reads parameters from given file.
     */
    private void read(File file) throws FileNotFoundException {
        HashMap<String, String> loaded = new HashMap<>();
        Scanner data = new Scanner(file);

        while (data.hasNextLine()) {
            String line = data.nextLine();
            Scanner parameter = new Scanner(line).useDelimiter(" ");

            if (!parameter.hasNext())
                throw new Error("Wrong parameters file - empty line");

            String name = parameter.next();

            if (loaded.size() == 15)
                throw new Error("Wrong parameters file - too many data");
            if (loaded.containsKey(name))
                throw new Error("Wrong parameters file - repeated parameter " + name);
            if (!parameter.hasNext())
                throw new Error("Wrong parameters file - no value of parameter " + name);

            String value = parameter.next();
            loaded.put(name, value);

            if (parameter.hasNext())
                throw new Error("Wrong parameters file - incorrect sign");
        }

        if (loaded.size() < 15)
            throw new Error("Wrong parameters file - too little data");

        data.close();
        this.parsujWejscie(loaded);
        this.checkData();
    }

    /**
     * Parse data from map to proper variables.
     */
    private void parsujWejscie(HashMap<String, String> loaded) {
        for (String name : loaded.keySet()) {
            switch (name) {
                case "ile_tur" -> this.roundsNumber = Integer.parseInt(loaded.get(name));
                case "co_ile_wypisz" -> this.statisticsAfter = Integer.parseInt(loaded.get(name));
                case "ile_daje_jedzenie" -> this.energyFromFood = Float.parseFloat(loaded.get(name));
                case "ile_rośnie_jedzenie" -> this.timeFoodGrow = Integer.parseInt(loaded.get(name));
                case "koszt_tury" -> this.energyLossInRound = Float.parseFloat(loaded.get(name));
                case "pocz_ile_robów" -> this.startRobsNumber = Integer.parseInt(loaded.get(name));
                case "pocz_progr" -> //empty program (" ") is also ok
                        this.startProgram = Instructions.parseToInstructions(loaded.get(name));
                case "pocz_energia" -> this.startEnergy = Float.parseFloat(loaded.get(name));
                case "pr_powielenia" -> this.prDuplication = Float.parseFloat(loaded.get(name));
                case "ułamek_energii_rodzica" -> this.partParentEnergy = Float.parseFloat(loaded.get(name));
                case "limit_powielania" -> this.duplicationLimit = Float.parseFloat(loaded.get(name));
                case "pr_usunięcia_instr" -> this.prDeleteInstr = Float.parseFloat(loaded.get(name));
                case "pr_dodania_instr" -> this.prAddInstr = Float.parseFloat(loaded.get(name));
                case "pr_zmiany_instr" -> this.prChangeInstr = Float.parseFloat(loaded.get(name));
                case "spis_instr" -> //empty program (" ") is NOT ok
                        this.instructionsList = Instructions.parseToInstructions(loaded.get(name));
                default -> throw new Error("Wrong parameters file - incorrect parameter " + name);
            }
        }
    }

    /**
     * Checks whether loaded data id correct.
     */
    private void checkData() {
        if (this.roundsNumber < 0)
            throw new Error("Wrong parameters file - incorrect value of parameter ile_tur");
        if (this.statisticsAfter <= 0)
            throw new Error("Wrong parameters file - incorrect value of parameter co_ile_wypisz");
        if (this.energyLossInRound < 0)
            throw new Error("Wrong parameters file - incorrect value of parameter koszt_tury");
        if (this.energyFromFood < 0)
            throw new Error("Wrong parameters file - incorrect value of parameter ile_daje_jedzenie");
        if (this.timeFoodGrow < 0)
            throw new Error("Wrong parameters file - incorrect value of parameter ile_rośnie_jedzenie");
        if (this.startRobsNumber < 0)
            throw new Error("Wrong parameters file - incorrect value of parameter pocz_ile_robów");
        if (this.startEnergy < 0)
            throw new Error("Wrong parameters file - incorrect value of parameter pocz_energia");
        if (this.prDuplication < 0 || this.prDuplication > 1)
            throw new Error("Wrong parameters file - incorrect value of parameter pr_powielenia");
        if (this.partParentEnergy < 0 || this.partParentEnergy > 1)
            throw new Error("Wrong parameters file - incorrect value of parameter ułamek_energii_rodzica");
        if (this.duplicationLimit < 0)
            throw new Error("Wrong parameters file - incorrect value of parameter limit_powielania");
        if (this.prDeleteInstr < 0 || this.prDeleteInstr > 1)
            throw new Error("Wrong parameters file - incorrect value of parameter pr_usunięcia_instr");
        if (this.prAddInstr < 0 || this.prAddInstr > 1)
            throw new Error("Wrong parameters file - incorrect value of parameter pr_dodania_instr");
        if (this.prChangeInstr < 0 || this.prChangeInstr > 1)
            throw new Error("Wrong parameters file - incorrect value of parameter pr_zmiany_instr");
        if (this.instructionsList == null)
            throw new Error("Wrong parameters file - incorrect value of parameter spis_instr");
        for (int i = 0; i < instructionsList.size(); i++) {
            Instructions ins = instructionsList.get(i);
            if (instructionsList.indexOf(ins) != instructionsList.lastIndexOf(ins))
                throw new Error("Wrong parameters file - incorrect value of parameter spis_instr");
        }
    }

    public int getTimeFoodGrow() {
        return this.timeFoodGrow;
    }

    public float getEnergyFromFood() {
        return this.energyFromFood;
    }

    public int getRoundsNumber() {
        return this.roundsNumber;
    }

    public int getStatisticsAfter() {
        return this.statisticsAfter;
    }

    public int getStartRobsNumber() {
        return this.startRobsNumber;
    }

    public ArrayList<Instructions> getStartProgram() {
        return this.startProgram;
    }

    public float getStartEnergy() {
        return this.startEnergy;
    }

    public float getEnergyLossInRound() {
        return this.energyLossInRound;
    }

    public float getDuplicationLimit() {
        return this.duplicationLimit;
    }

    public float getPrDuplication() {
        return this.prDuplication;
    }

    public float getPrDeleteInstr() {
        return this.prDeleteInstr;
    }

    public float getPrAddInstr() {
        return this.prAddInstr;
    }

    public float getPrChangeInstr() {
        return this.prChangeInstr;
    }

    public float getPartParentEnergy() {
        return this.partParentEnergy;
    }

    public ArrayList<Instructions> getInstructionsList() {
        return this.instructionsList;
    }

}
