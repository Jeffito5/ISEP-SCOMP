package ex1;

import java.util.Scanner;

/**
 * @author Luís Araújo
 */
public class OrderTaker {
    private boolean flag = false;
    private OrderQueue orderQueue;

    public OrderTaker(OrderQueue queue){
        orderQueue=queue;
    }

    public Order createOrder(String description, int num) {
        return new Order(description, num);
    }

    public boolean addOrderToQueue(Order order) {
        return orderQueue.addOrderToQueue(order);
    }

    public void start() {
        Scanner in = new Scanner(System.in);
        System.out.println("Enter the description");
        String description = in.next();
        System.out.println("Enter the number");
        int num = in.nextInt();
        Order order = createOrder(description, num);
        if (addOrderToQueue(order)) {
            System.out.println("Order added.\n");
        } else {
            System.out.println("Try again.\n");
        }
    }
}
