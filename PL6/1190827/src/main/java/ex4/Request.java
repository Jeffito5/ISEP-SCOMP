package ex4;

/**
 * @author Luís Araújo
 */
public class Request {
    private int num_passengers;
    private String address;
    static int index=0;

    public Request() {
        index++;
    }

    public Request(int num_passengers, String address) {
        this.num_passengers = num_passengers;
        this.address = address;
        index++;
    }

    public int getNum_passengers() {
        return num_passengers;
    }

    public void setNum_passengers(int num_passengers) {
        this.num_passengers = num_passengers;
    }

    public String getAddress() {
        return address;
    }

    public void setAddress(String address) {
        this.address = address;
    }

    @Override
    public String toString() {
        return "Request{" +
                "num_passengers=" + num_passengers +
                ", address='" + address + '\'' +
                '}';
    }

    public int getIndex() {
        return index;
    }

    @Override
    public boolean equals(Object o){
        if (this==o){
            return true;
        }

        if (!(o instanceof Request)){
            return false;
        }
        if (address.equalsIgnoreCase(((Request) o).getAddress())&&num_passengers==((Request) o).getNum_passengers()&&index==((Request) o).getIndex()){
            return true;
        }
        return false;
    }
}
