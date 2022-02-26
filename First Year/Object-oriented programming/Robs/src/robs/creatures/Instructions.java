package robs.creatures;

import java.util.ArrayList;

public enum Instructions {
    LEFT('l'), // rotate 90 degrees left
    RIGHT('p'), // rotate 90 degrees right
    GO('i'), // go forward 1 field and eat food if there's one
    SMELL('w'), // check whether any of 4 next fields has food on it and rotate in this direction, otherwise don't do anything
    EAT('j'); // check whether any of 8 next fields has food on it, go there and eat the food, otherwise don't do anything

    private final char sign;

    Instructions(char sign) {
        this.sign = sign;
    }

    private char getSign() {
        return this.sign;
    }

    private static Instructions getInstruction(char sign) {
        for (Instructions ins: Instructions.values())
            if (ins.getSign() == sign)
                return ins;

        throw new Error("Incorrect sign of instruction!");
    }

    /**
     * Parse string representing instructions to table of instructions.
     */
    public static ArrayList<Instructions> parseToInstructions(String program) {
        ArrayList<Instructions> instructions = new ArrayList<>();

        for (int i = 0; i < program.length(); i++) {
            char sign = program.charAt(i);
            instructions.add(getInstruction(sign));
        }

        return instructions;
    }
}
