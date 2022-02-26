package robs.creatures;

import robs.world.Parameters;

import java.util.ArrayList;
import java.util.Random;

public class Mutation {
    private final ArrayList<Instructions> instructionsList;
    private final float prDeleteInstr;
    private final float prAddInstr;
    private final float prChangeInstr;
    private final Random r = new Random();

    public Mutation(Parameters parameters) {
        this.instructionsList = parameters.getInstructionsList();
        this.prDeleteInstr = parameters.getPrDeleteInstr();
        this.prAddInstr = parameters.getPrAddInstr();
        this.prChangeInstr = parameters.getPrChangeInstr();
    }

    /**
     * Return random instruction from instructionsList.
     */
    private Instructions getRandominstruction() {
        int random = r.nextInt(this.instructionsList.size());

        return this.instructionsList.get(random);
    }

    /**
     * Mutate given program.
     */
    public void mutateProgram(ArrayList<Instructions> program) {
        if (r.nextFloat() < this.prDeleteInstr)
            if (!program.isEmpty())
                program.remove(program.size() - 1);

        if (r.nextFloat() < this.prAddInstr)
            program.add(this.getRandominstruction());

        if (r.nextFloat() < this.prChangeInstr) {
            if (!program.isEmpty()) {
                int index = r.nextInt(program.size());
                program.set(index, this.getRandominstruction());
            }
        }
    }
}
