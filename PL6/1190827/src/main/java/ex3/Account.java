package ex3;

/**
 * @author Luís Araújo
 */
public class Account {
    private String name, id;
    private EurosAmount money;

    public Account(String name, String id, EurosAmount money) {
        this.name = name;
        this.id = id;
        this.money = money;
    }

    public Account() {

    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getId() {
        return id;
    }

    public void setId(String id) {
        this.id = id;
    }

    public EurosAmount getMoney() {
        return money;
    }

    public void setMoney(EurosAmount money) {
        this.money = money;
    }

    @Override
    public String toString() {
        return "Account{" +
                "name='" + name + '\'' +
                ", id='" + id + '\'' +
                ", money=" + money +
                '}';
    }
}
