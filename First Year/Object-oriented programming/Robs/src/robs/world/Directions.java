package robs.world;

import java.util.Random;

public enum Directions {
    UP(0, -1),
    RIGHT(1, 0),
    DOWN(0, 1),
    LEFT(-1, 0);

    private final int changeColumn;
    private final int changeRow;
    private static final Random r = new Random();

    Directions(int changeColumn, int changeRow) {
        this.changeColumn = changeColumn;
        this.changeRow = changeRow;
    }

    public int getChangeColumn() {
        return this.changeColumn;
    }

    public int getChangeRow() {
        return this.changeRow;
    }

    /**
     * Returns direction of given moves.
     */
    private static Directions getDirection(int changeColumn, int changeRow) {
        for (Directions k : Directions.values())
            if (k.getChangeColumn() == changeColumn && k.getChangeRow() == changeRow)
                return k;

        return null;
    }

    /**
     * Returns direction after left rotate.
     */
    public Directions getLeft() {
        return getDirection(this.changeRow, -this.changeColumn);
    }

    /**
     * Returns direction after right rotate.
     */
    public Directions getRight() {
        return getDirection(-this.changeRow, this.changeColumn);
    }

    /**
     * Returns opposite direction.
     */
    public Directions getOpposite() {
        return getDirection(-this.changeColumn, -this.changeRow);
    }

    /**
     * Returns random direction.
     */
    public static Directions getRandom() {
        int random = r.nextInt(4);

        return switch (random) {
            case 0 -> RIGHT;
            case 1 -> LEFT;
            case 2 -> UP;
            case 3 -> DOWN;
            default -> throw new IllegalStateException("Unexpected value: " + random);
        };
    }
}

