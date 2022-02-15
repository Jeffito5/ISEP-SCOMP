package ex1;

import java.util.LinkedList;

/**
 * @author Luís Araújo
 */
public class OrderQueue {
    static LinkedList<Order> allOrders = new LinkedList<Order>();

    public Order searchOrderByNumber(int num) {
        for (Order order : allOrders) {
            if (order.getNumber() == num) {
                return order;
            }
        }
        return null;
    }

    public boolean addOrderToQueue(Order order) {
        allOrders.addLast(order);
        return allOrders.getLast().equals(order);
    }

    public boolean removeOrderFromQueue(Order order) {
        return allOrders.remove(order);
    }

    public LinkedList<Order> getAllOrders() {
        return allOrders;
    }
}
