package ex1;

/**
 * @author Luís Araújo
 */
public class Order {
    private String description;
    private int number;

    public Order(String description, int number) {
        this.description = description;
        this.number = number;
    }

    public Order() {

    }

    public String getDescription() {
        return description;
    }

    public void setDescription(String description) {
        this.description = description;
    }

    public int getNumber() {
        return number;
    }

    public void setNumber(int number) {
        this.number = number;
    }

    @Override
    public String toString() {
        return "Order{" +
                "description='" + description + '\'' +
                ", number=" + number +
                '}';
    }
}