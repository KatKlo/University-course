package robs.creatures;

import robs.world.*;

import java.util.ArrayList;

public class Rob {
    private Field field;
    private Directions direction;
    private final ArrayList<Instructions> program;
    private float energy;
    private int age;
    private int progIndexCounter;

    public Rob(Field field, Directions direction, ArrayList<Instructions> program, float energy) {
        this.field = field;
        this.direction = direction;
        this.program = program;
        this.energy = energy;
        this.age = 0;
        this.progIndexCounter = 0;
    }

    public int getProgramLength() {
        return this.program.size();
    }

    public float getEnergy() {
        return this.energy;
    }

    public int getAge() {
        return this.age;
    }

    public ArrayList<Instructions> getProgram() {
        return this.program;
    }

    public Directions getDirection() {
        return this.direction;
    }

    public Field getField() {
        return this.field;
    }

    /**
     * Takes given part of rob's energy and returns it.
     */
    public float passEnergyToChild(float part) {
        float energyForChild = this.energy * part;
        this.energy -= energyForChild;

        return energyForChild;
    }

    /**
     * Eats food from field if there's same.
     */
    private void tryEat() {
        if (this.field.isThereFood())
            this.energy += this.field.eat();
    }

    /**
     * Moves rob to given field and eats from it if there's some food.
     */
    private void goToField(Field field) {
        this.field = field;
        this.tryEat();
    }

    /**
     * Executes instruction LEFT.
     */
    private void turnLeft() {
        this.direction = this.direction.getLeft();
    }

    /**
     * Executes instruction RIGHT.
     */
    private void turnRight() {
        this.direction = this.direction.getRight();
    }

    /**
     * Executes instruction GO.
     */
    private void go(Board board) {
        goToField(board.getNextField(this.field, this.direction));
    }

    /**
     * Executes instruction SMELL.
     */
    private void smell(Board board) {
        for (int i = 0; i < 4; i++) {
            Field temp = board.getNextField(this.field, this.direction);

            if (temp.isThereFood())
                return;

            this.turnRight();
        }
    }

    /**
     * Executes instruction EAT.
     */
    private void eat(Board board) {
        for (int i = 0; i < 4; i++) {
            Field temp = board.getNextField(this.field, this.direction);

            if (temp.isThereFood()) {
                goToField(temp);
                return;
            }

            this.turnRight();
            temp = board.getNextField(temp, this.direction);

            if (temp.isThereFood()) {
                goToField(temp);
                return;
            }
        }
    }

    /**
     * Checks if rob is still alive.
     */
    public boolean isAlive() {
        return this.energy >= 0;
    }

    /**
     * Updates robs's parameters after one round.
     */
    public void update(float roundCost) {
        this.energy -= roundCost;
        this.age++;
        this.progIndexCounter = 0;
    }

    /**
     * Checks if any instruction left to execute.
     */
    public boolean isLeftInstruction() {
        return this.progIndexCounter < this.program.size();
    }

    public void executeInstruction(Board board) {
        switch (this.program.get(this.progIndexCounter++)) {
            case LEFT -> this.turnLeft();
            case RIGHT -> this.turnRight();
            case GO -> this.go(board);
            case SMELL -> this.smell(board);
            case EAT -> this.eat(board);
        }

        this.energy--;
    }

    @Override
    public String toString() {
        return "age: " + this.age + ", location: " + this.field.getCoordinates() + ", direction: " + this.direction + ", " +
                "energy: " + this.energy + ", program: " + this.program;
    }

}
