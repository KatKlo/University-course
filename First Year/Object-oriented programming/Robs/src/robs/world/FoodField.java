package robs.world;

public class FoodField extends Field {
    private int ageOfFood;
    private static int timeToGrow = -1;
    private static float energyGiven = -1;

    public FoodField(int column, int row) {
        super(column, row);

        if (timeToGrow == -1)
            throw new Error("You can't create filed without giving parameters first");

        this.ageOfFood = timeToGrow;
    }

    public static void setParameters(Parameters parameters) {
        energyGiven = parameters.getEnergyFromFood();
        timeToGrow = parameters.getTimeFoodGrow();
    }

    @Override
    public String toString() {
        if (this.ageOfFood >= timeToGrow)
            return "x";

        return " ";
    }

    @Override
    public boolean isThereFood() {
        return this.ageOfFood >= timeToGrow;
    }

    @Override
    public void update() {
        this.ageOfFood++;
    }

    @Override
    public float eat() {
        this.ageOfFood = 0;
        return energyGiven;
    }
}
