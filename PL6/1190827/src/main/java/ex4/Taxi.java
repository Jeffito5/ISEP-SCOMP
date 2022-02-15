package ex4;

/**
 * @author Luís Araújo
 */
public class Taxi {
    private int maximum;

    public Taxi() {
        maximum=0;
    }

    public Taxi(int maximum) {
        this.maximum = maximum;
    }

    public int getMaximum() {
        return maximum;
    }

    public void setMaximum(int maximum) {
        this.maximum = maximum;
    }

    @Override
    public String toString() {
        return "Taxi{" +
                "maximum=" + maximum +
                '}';
    }
}
