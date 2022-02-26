package robs.world;

public class Field {
    private final int row;
    private final int column;

    public Field(int column, int row) {
        this.column = column;
        this.row = row;
    }

    public int getRow() {
        return this.row;
    }

    public int getColumn() {
        return this.column;
    }

    @Override
    public String toString() {
        return " ";
    }

    /**
     * Checks whether there's food on this field.
     */
    public boolean isThereFood() {
        return false;
    }

    /**
     * Updates state of this filed.
     */
    public void update() {}

    /**
     * Handles eating food from this field and returns energy given by it.
     */
    public float eat() {
        return 0;
    }

    /**
     * Returns coordinates of the field for printing.
     */
    public String getCoordinates() {
        return "(w:" + (this.row + 1) + ", k:" + (this.column + 1) + ")";
    }
}
