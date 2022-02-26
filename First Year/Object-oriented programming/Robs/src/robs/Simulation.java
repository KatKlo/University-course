package robs;

import robs.world.Parameters;
import robs.world.Board;
import robs.creatures.Population;

import java.io.File;
import java.io.FileNotFoundException;

public class Simulation {

    /**
     * Prints short statistics of simulation's state after each round.
     */
    private static void printRound(int roundCounter, Population population, Board board) {
        System.out.print(roundCounter);
        System.out.print(", rob: " + population.getAliveNumber());
        System.out.print(", żyw: " + board.getNumberOfFood());
        System.out.println(", " + population.getStatistics());
    }

    /**
     * Prints full statistics of simulation's state.
     */
    private static void printState(int roundCounter, Population population, Board board) {
        System.out.println("PASSED ROUNDS: " + roundCounter);
        board.printBoard();
        population.printState();
    }

    /**
     * Runs all simulations for given population and board, and prints statistics.
     */
    private static void runSimulation(Parameters parameters, Population population, Board board) {
        int roundCounter = 1;

        printState(0, population, board);

        for (; roundCounter <= parameters.getRoundsNumber(); roundCounter++) {
            population.executeRound();
            board.update();
            printRound(roundCounter, population, board);

            if (roundCounter % parameters.getStatisticsAfter() == 0)
                printState(roundCounter, population, board);
        }

        printState(roundCounter - 1, population, board);
    }

    public static void main(String[] args) throws FileNotFoundException {
        if (args.length != 2)
            throw new Error("Wrong number of arguments!");

        Parameters parameters = new Parameters(new File(args[1]));
        Board board = new Board(new File(args[0]), parameters);
        Population population = new Population(parameters, board);

        runSimulation(parameters, population, board);
    }
}
