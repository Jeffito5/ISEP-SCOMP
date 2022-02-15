package ex1;

import java.util.Scanner;

/**
 * @author Luís Araújo
 */
public class OrderHandler {
    private static OrderQueue orderQueue;

    public static boolean removeOrderFromQueue(Order order) {
        return orderQueue.removeOrderFromQueue(order);
    }

    public OrderHandler(OrderQueue queue){
        orderQueue=queue;
    }

    public void start() {
        Scanner in = new Scanner(System.in);
        System.out.println("Enter the number of the order you want to remove");
        int num = in.nextInt();
        Order order = orderQueue.searchOrderByNumber(num);
        if (removeOrderFromQueue(order)) {
            System.out.println("Order removed.\n");
        } else {
            System.out.println("Try again.\n");
        }
    }
}
