package robs.world;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.Random;
import java.util.Scanner;

public class Board {
    private int columnNumber;
    private int rowNumber;
    private Field[][] fields; // numeration starts at left up corner (r:1, c:1)
    private final Random r = new Random();

    public Board(File file, Parameters parameters) throws FileNotFoundException {
        FoodField.setParameters(parameters);
        this.columnNumber = 0;
        this.rowNumber = 0;
        this.readBoard(file);
    }

    /**
     * Reads board from given file.
     */
    private void readBoard(File file) throws FileNotFoundException {
        Scanner data = new Scanner(file);
        ArrayList<Field> temporary = new ArrayList<>();
        String line;

        if (!data.hasNextLine())
            throw new Error("Wrong board file - there's no board!");

        while (data.hasNextLine()) {
            line = data.nextLine();

            if (this.rowNumber == 0) {
                this.columnNumber = line.length();
                if (this.columnNumber < 1)
                    throw new Error("Wrong board file - incorrect dimensions!");
            } else {
                if (line.length() != this.columnNumber)
                    throw new Error("Wrong board file - different lengths of lines!");
            }

            for (int i = 0; i < this.columnNumber; i++) {
                switch (line.charAt(i)) {
                    case ' ' -> temporary.add(new Field(i, this.rowNumber));
                    case 'x' -> temporary.add(new FoodField(i, this.rowNumber));
                    default -> throw new Error("Wrong board file - incorrect sign!");
                }
            }

            this.rowNumber++;
        }

        data.close();

        int counter = 0;
        this.fields = new Field[this.rowNumber][this.columnNumber];
        for (int i = 0; i < this.rowNumber; i++)
            for (int j = 0; j < this.columnNumber; j++)
                this.fields[i][j] = temporary.get(counter++);
    }

    /**
     * Prints actual state of the board.
     */
    public void printBoard() {
        System.out.println("NUMBER OF FOOD ON BOARDS: " + this.getNumberOfFood());
        System.out.println("BOARD: ");

        for (int j = 0; j < this.columnNumber +2; j++)
            System.out.print("-");
        System.out.println();

        for (int i = 0; i < this.rowNumber; i++) {
            System.out.print("|");
            for (int j = 0; j < this.columnNumber; j++)
                System.out.print(this.fields[i][j].toString());
            System.out.println("|");
        }

        for (int j = 0; j < this.columnNumber + 2; j++)
            System.out.print("-");
        System.out.println();
    }

    /**
     * Returns next field in given direction from given field.
     */
    public Field getNextField(Field from, Directions direction) {
        int newRow = from.getRow() + direction.getChangeRow();
        newRow = (newRow + this.rowNumber) % this.rowNumber;

        int newColumn = from.getColumn() + direction.getChangeColumn();
        newColumn = (newColumn + this.columnNumber) % this.columnNumber;

        return this.fields[newRow][newColumn];
    }

    /**
     * Updates state of the board after one round.
     */
    public void update() {
        for (int i = 0; i < this.rowNumber; i++)
            for (int j = 0; j < this.columnNumber; j++)
                fields[i][j].update();
    }

    /**
     * Returns number of food on board.
     */
    public int getNumberOfFood() {
        int result = 0;

        for (int i = 0; i < this.rowNumber; i++)
            for (int j = 0; j < this.columnNumber; j++)
                if (fields[i][j].isThereFood())
                    result++;

        return result;
    }

    /**
     * Returns random field on the board.
     */
    public Field getRandomField() {
        int row = r.nextInt(this.rowNumber);
        int column = r.nextInt(this.columnNumber);

        return this.fields[row][column];
    }
}
