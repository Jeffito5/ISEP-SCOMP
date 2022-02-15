package ex1;

import java.util.Scanner;

/**
 * @author Luís Araújo
 */
public class Main {
    private static OrderQueue orderQueue = new OrderQueue();
    private static OrderTaker orderTaker = new OrderTaker(orderQueue);
    private static OrderHandler orderHandler = new OrderHandler(orderQueue);

    public static void main(String[] args) {
        Thread1 thread1 = new Thread1();
        thread1.run();
        Thread2 thread2 = new Thread2();
        thread2.run();

    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    static class Thread1 extends Thread {
        Scanner in = new Scanner(System.in);
        Thread thread1;

        @Override
        public void run() {
            System.out.println("Let's add orders.\n");
            boolean flag = true;
            while (flag) {
                orderTaker.start();
                System.out.println("Do you want to add more orders? y/n");
                String answer = in.next();
                switch (answer) {
                    case "y":
                    case "Y":
                        break;
                    case "n":
                    case "N":
                        flag = false;
                    default:
                        break;
                }
            }
        }

        public void start() {
            if (thread1 == null) {
                thread1 = new Thread();
                thread1.start();
            }
        }
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    static class Thread2 extends Thread {
        Scanner in = new Scanner(System.in);
        Thread thread2;

        @Override
        public void run() {
            System.out.println("Let's remove orders.\n");
            boolean flag = true;
            while (flag && !orderQueue.getAllOrders().isEmpty()) {
                orderHandler.start();
                System.out.println("Do you want to remove more orders? y/n");
                String answer = in.next();
                switch (answer) {
                    case "y":
                    case "Y":
                        break;
                    case "n":
                    case "N":
                        flag = false;
                    default:
                        break;
                }
                if(orderQueue.getAllOrders().isEmpty()){
                    System.out.println("There are no more orders.");
                }
            }
        }

        public void start() {
            if (thread2 == null) {
                thread2 = new Thread();
                thread2.start();
            }
        }
    }
}
