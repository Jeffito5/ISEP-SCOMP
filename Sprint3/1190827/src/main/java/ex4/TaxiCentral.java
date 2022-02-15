package ex4;

import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;

/**
 * @author Luís Araújo
 */
public class TaxiCentral {
    private LinkedList<Request> waiting_list = new LinkedList<>();
    private LinkedList<Taxi> available_taxi = new LinkedList<>();
    private LinkedList<Taxi> unavailable_taxi = new LinkedList<>();

    public boolean addRequestToWaitingList(Request request){
        return waiting_list.add(request);
    }
    public boolean addRequestToWaitingList(int passagers ,String address){
        Request request = new Request(passagers,address);
        return waiting_list.add(request);
    }

    public boolean addNewTaxi(Taxi taxi){
        return available_taxi.add(taxi);
    }
    public boolean addTaxi(Taxi taxi){
        unavailable_taxi.remove(taxi);
        return available_taxi.add(taxi);
    }
    public boolean addTaxi(int maxi){
        return available_taxi.add(new Taxi(maxi));
    }

    public Taxi getAvailableTaxi(){
        try {
            Taxi taxi1 = available_taxi.getFirst();
            available_taxi.remove(taxi1);
            unavailable_taxi.add(taxi1);
            return taxi1;
        }catch(Exception e){
            return null;
        }
    }

    public Request getNextRequest(){
        return waiting_list.getFirst();
    }

    public LinkedList<Taxi> getAvailable_taxi() {
        return available_taxi;
    }

    public LinkedList<Taxi> getUnavailable_taxi() {
        return unavailable_taxi;
    }

    public LinkedList<Request> getWaiting_list() {
        return waiting_list;
    }
}
