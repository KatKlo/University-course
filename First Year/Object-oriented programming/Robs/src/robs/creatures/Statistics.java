package robs.creatures;

import java.util.ArrayList;

public class Statistics {
    float minEnergy;
    float maxEnergy;
    float averageEnergy;
    int minAge;
    int maxAge;
    float averageAge;
    int minProgramLength;
    int maxProgramLength;
    float averageProgramLength;

    public Statistics(ArrayList<Rob> robs) {
        this.update(robs);
    }

    public float getAverageEnergy() {
        return this.averageEnergy;
    }

    public float getAverageAge() {
        return this.averageAge;
    }

    public float getAverageProgramLength() {
        return this.averageProgramLength;
    }

    /**
     * Aktualizuje stan statystyk.
     */
    public void update(ArrayList<Rob> robs) {
        this.minEnergy = 0;
        this.maxEnergy = 0;
        this.averageEnergy = 0;
        this.averageAge = 0;
        this.averageProgramLength = 0;
        this.minAge = 0;
        this.maxAge = 0;
        this.minProgramLength = 0;
        this.maxProgramLength = 0;

        if (robs.size() != 0) {
            this.minEnergy = robs.get(0).getEnergy();
            this.minAge = robs.get(0).getAge();
            this.minProgramLength = robs.get(0).getProgramLength();

            for (Rob r : robs) {
                float energy = r.getEnergy();
                this.minEnergy = Math.min(energy, this.minEnergy);
                this.maxEnergy = Math.max(energy, this.maxEnergy);
                this.averageEnergy += energy;

                int programLength = r.getProgramLength();
                this.minProgramLength = Math.min(programLength, this.minProgramLength);
                this.maxProgramLength = Math.max(programLength, this.maxProgramLength);
                this.averageProgramLength += programLength;

                int age = r.getAge();
                this.minAge = Math.min(age, this.minAge);
                this.maxAge = Math.max(age, this.maxAge);
                this.averageAge += age;
            }

            int robsNumber = robs.size();
            this.averageProgramLength /= robsNumber;
            this.averageEnergy /= robsNumber;
            this.averageAge /= robsNumber;
        }
    }

    @Override
    public String toString() {
        String result = "";
        result += "prg: " + this.minProgramLength + "/" + String.format("%.2f", this.averageProgramLength) + "/" + this.maxProgramLength;
        result += ", energ: " + String.format("%.2f", this.minEnergy) + "/" + String.format("%.2f", this.averageEnergy) + "/" + String.format("%.2f", this.maxEnergy);
        result += ", wiek: " + this.minAge + "/" + String.format("%.2f", this.averageAge) + "/" + this.maxAge;

        return result;
    }
}
