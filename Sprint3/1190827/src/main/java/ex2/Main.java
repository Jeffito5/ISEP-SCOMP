package ex2;

import java.util.concurrent.Semaphore;

/**
 * @author Luís Araújo
 */
public class Main {

    private static final int NUMBER_OF_SELLERS = 5;
    private static final int NUMBER_OF_TICKETS = 10;

    public static void main(String[] args) throws InterruptedException {
        Semaphore sem = new Semaphore(1);
        Thread[] threads = new Thread[NUMBER_OF_SELLERS];
        for (int i = 0; i < NUMBER_OF_SELLERS; i++) {
            // acquiring the lock
            threads[i] = new SellTickets(NUMBER_OF_TICKETS, sem, i);
            threads[i].start();
        }
        for (int i = 0; i < NUMBER_OF_SELLERS; i++) {
            try {
                threads[i].join();
            } catch (InterruptedException ex) {
                // Ignore this
            }
        }
    }
}