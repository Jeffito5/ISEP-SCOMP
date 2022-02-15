package ex4;

import java.util.Scanner;

/**
 * @author Luís Araújo
 */
public class Main {
    static TaxiCentral taxiCentral = new TaxiCentral();
    static Scanner in = new Scanner(System.in);

    public static void main(String[] args) {
        boolean flag = false;
        boolean exit = false;


        do {
            do {
                taxiCentral.addNewTaxi(new Taxi(4));
                taxiCentral.addNewTaxi(new Taxi(5));
                System.out.println("What you want to do?");
                System.out.println("1. Get a Taxi");
                System.out.println("2. Register your Taxi");
                int option = in.nextInt();
                switch (option) {
                    case 1:

                        if (taxiCentral.getAvailable_taxi().isEmpty() && taxiCentral.getUnavailable_taxi().isEmpty()) {
                            System.out.println("No taxi available for now. Pls get an uber.");
                            break;
                        }
                        (new Thread(new GetTaxi())).run();
                        flag = true;
                        break;
                    case 2:
                        (new Thread(new RegisterTaxi())).run();
                        flag = true;
                        break;

                    default:
                        System.out.println("Select a valid option.");
                        break;
                }
            } while (!flag);
            System.out.println("Do u want to exit?[y/n(OR NOTHING)]");
            String ex = in.next();
            switch (ex) {
                case "y":
                case "Y":
                    exit = true;
                    break;
                case "n":
                case "N":
                    break;
                default:
                    break;
            }
        } while (!exit);
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    static class GetTaxi extends Thread {
        Request request;
        @Override
        public void run() {
            System.out.println("How many passagers want to go in the Taxi?");
            int passagers = in.nextInt();
            System.out.println("Where are you?");
            String address = in.next();
            request= new Request(passagers,address);
            taxiCentral.addRequestToWaitingList(request);
            taxiRun();
            System.out.println("Rate the race: [1-10]");
            String nada=in.next();
            System.out.println("Just kidding");
        }

        public void taxiRun() {
            boolean sucess=false;
            Taxi taxi;
            Request request1;
            do {
                System.out.println("Waiting for a taxi");
                do {
                    taxi = taxiCentral.getAvailableTaxi();
                    request1 = taxiCentral.getNextRequest();
                } while (taxi == null || !(request1.equals(request)));
                if (request1.getNum_passengers()>taxi.getMaximum()){
                    System.out.println("The num passagers exceed taxi capacity, someone will wait for other taxi");
                    request1.setNum_passengers(request1.getNum_passengers()- taxi.getMaximum());
                }else{
                    sucess=true;
                }

                    System.out.println("Initializing the race...");
                try {
                    Thread.sleep(5000);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
                System.out.println("Race ended. I hope you didn't die...");
                taxiCentral.addTaxi(taxi);
                System.out.println("See you later.");
            }while (!sucess);
            taxiCentral.getWaiting_list().remove(request1);
        }
    }

    static class RegisterTaxi extends Thread {

        @Override
        public void run() {
            System.out.println("The capacity of the taxi");
            int capacity=in.nextInt();
            taxiCentral.addTaxi(capacity);
        }
    }

}
