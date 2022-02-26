package robs.creatures;

import robs.world.Board;
import robs.world.Directions;
import robs.world.Parameters;

import java.util.ArrayList;
import java.util.Comparator;
import java.util.Random;

public class Population {
    private ArrayList<Rob> robs;
    private final int startRobsNumber;
    private final ArrayList<Instructions> startProgram;
    private final float startEnergy;
    private final float roundCost;
    private final Duplication duplication;
    private final Board board;
    private final Statistics statistics;
    private final Random r = new Random();

    public Population(Parameters parameters, Board board) {
        this.startRobsNumber = parameters.getStartRobsNumber();
        this.startProgram = parameters.getStartProgram();
        this.startEnergy = parameters.getStartEnergy();
        this.roundCost = parameters.getEnergyLossInRound();
        this.board = board;
        this.createRobs();
        this.duplication = new Duplication(parameters);
        this.statistics = new Statistics(this.robs);
    }

    /**
     * Creates robes at the beginning according to parameters.
     */
    private void createRobs() {
        this.robs = new ArrayList<>();

        for (int i = 0; i < this.startRobsNumber; i++)
            this.robs.add(new Rob(this.board.getRandomField(),
                                  Directions.getRandom(),
                                  this.startProgram,
                                  this.startEnergy
            ));
    }

    /**
     * Changes order of robs in table, so they're taking turns in different order in different rounds.
     */
    private void changeOrder() {
        for (int i = 0; i < this.robs.size(); i++) {
            int toChange = r.nextInt(this.robs.size());
            Rob temp = this.robs.get(toChange);
            this.robs.set(toChange, this.robs.get(i));
            this.robs.set(i, temp);
        }
    }

    /**
     * Executes one round on whole population.
     * Every rob in turns execute 1 instruction till has any left.
     */
    public void executeRound() {
        this.changeOrder();
        int endedCounter = 0;

        while (endedCounter < this.robs.size()) {
            endedCounter = 0;

            for (Rob rob : this.robs) {
                if (rob.isLeftInstruction() && rob.isAlive())
                    rob.executeInstruction(this.board);
                else
                    endedCounter++;
            }
        }

        // endedCounter = robs.size()
        for (int i = 0; i < endedCounter; i++) {
            Rob rob = this.robs.get(i);
            rob.update(this.roundCost);

            if (this.duplication.canDuplicate(rob))
                this.robs.add(this.duplication.duplicate(rob));
        }

        this.robs.removeIf(r -> !r.isAlive());
        this.statistics.update(this.robs);
    }

    /**
     * Returns number of alive robs.
     */
    public int getAliveNumber() {
        return this.robs.size();
    }

    /**
     * Returns statistics at the end of the simulation.
     */
    public String getStatistics() {
        return this.statistics.toString();
    }

    /**
     * Sorts and prints states of all robs.
     */
    private void printAll() {
        System.out.println("ALL ROBS:");
        robs.sort(Comparator.comparingInt(Rob::getAge));

        for (Rob rob : robs) {
            System.out.println(rob);
        }
    }

    /**
     * Wypisuje stan populacji.
     */
    public void printState() {
        System.out.println("ROBS NUMBER - " + "START: " + this.startRobsNumber + " NOW: " + this.robs.size());
        System.out.println("AVERAGE ENERGY - " + "START: " + this.startEnergy + " NOW: " + this.statistics.getAverageEnergy());
        System.out.println("AVERAGE PROGRAM'S LENGTH - " + "START: " + this.startProgram.size() + " NOW: " + this.statistics.getAverageProgramLength());
        System.out.println("AVERAGE AGE - " + "START: " + 0 + " NOW: " + this.statistics.getAverageAge());
        this.printAll();
    }


}
