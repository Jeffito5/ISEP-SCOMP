package ex2;

import java.util.Random;
import java.util.concurrent.Semaphore;

/**
 * @author Luís Araújo
 */
public class SellTickets extends Thread implements Runnable {
    private int tickets = 0;
    private Semaphore semaphore = null;
    private int number_of_thread;

    public SellTickets(int number_of_tickets, Semaphore sem, int i) {
        tickets = number_of_tickets;
        semaphore = sem;
        number_of_thread=i;
    }

    //Simulated ticket buying system
    @Override
    public void run() {
        Random random = new Random();

        while (tickets > 0) {
            //Set synchronous lock
            synchronized (this) {
                //Set sleep time
                if (tickets > 0) {
                    try {
                        semaphore.acquire();
                        //Every time it runs, sleeps for one second
                        Thread.sleep(random.nextInt(100) + 500);
                        System.out.println("Thread " + number_of_thread + " is selling tickets:" + " " + tickets-- + " ");
                        semaphore.release();
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                } else {
                    return;
                }
            }

        }
    }
}
