package robs.creatures;

import robs.world.Parameters;
import robs.world.Directions;

import java.util.ArrayList;
import java.util.Random;

public class Duplication {
    private final float prDuplication;
    private final float partParentEnergy;
    private final float duplicationLimit;
    private final Mutation mutation;
    private final Random r = new Random();

    public Duplication(Parameters parameters) {
        this.prDuplication = parameters.getPrDuplication();
        this.partParentEnergy = parameters.getPartParentEnergy();
        this.duplicationLimit = parameters.getDuplicationLimit();
        mutation = new Mutation(parameters);
    }

    /**
     * Checks whether given rob can duplicate.
     */
    public boolean canDuplicate(Rob rob) {
        if (rob.getEnergy() < this.duplicationLimit)
            return false;

        return r.nextFloat() < this.prDuplication;
    }

    /**
     * Duplicate rob and return the new one.
     */
    public Rob duplicate(Rob rob) {
        ArrayList<Instructions> newProgram = new ArrayList<>(rob.getProgram());
        mutation.mutateProgram(newProgram);
        Directions newDirection = rob.getDirection().getOpposite();
        float newEnergy = rob.passEnergyToChild(this.partParentEnergy);

        return new Rob(rob.getField(), newDirection, newProgram, newEnergy);
    }
}
