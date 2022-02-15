package ex3;

import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

/**
 * @author Luís Araújo
 */
public class Main {
    private static Account fromAcct, toAcct;
    private static float amount1, amount2;
    private static List<Account> importList = new ArrayList<>();

    public static void main(String[] args) throws InterruptedException {
        Scanner in = new Scanner(System.in);

        System.out.println("Let's create 2 accounts");

        System.out.println("First account");
        System.out.println("Insert the name of the account");
        String name = in.next();
        System.out.println("Insert the id of the account");
        String id = in.next();

        System.out.println("Second account");
        System.out.println("Insert the name of the account");
        String name2 = in.next();
        System.out.println("Insert the id of the account");
        String id2 = in.next();

        System.out.println("How much money do you have in account 1?");
        amount1 = in.nextFloat();

        System.out.println("How much money do you have in account 2?");
        amount2 = in.nextFloat();

        fromAcct = createAccount(name, id, amount1);
        toAcct = createAccount(name2, id2, amount2);
        addAccount(fromAcct);
        addAccount(toAcct);

        Thread1 thread = new Thread1();
        thread.run();

    }

    public static boolean addAccount(Account account) {
        return importList.add(account);
    }

    public static Account createAccount(String name, String id, float amount) {
        EurosAmount eurosAmount = new EurosAmount(amount);
        return new Account(name, id, eurosAmount);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    static class Thread1 extends Thread {
        Scanner in = new Scanner(System.in);
        Thread thread1;

        @Override
        public void run() {
            System.out.println("How much money do you want to transfer?");
            System.out.println("You can't transfer more money than that you have at your account.");
            float transfer = in.nextFloat();
            EurosAmount transferMoney = new EurosAmount(transfer);

            boolean success = transferMoney(fromAcct, toAcct, transferMoney);
            if (success) {
                System.out.println("The operation was a success. You are getting poor.");
            } else {
                System.out.println("The operation failed because the transfer amount was greater that you have :') .");
                System.out.println("Your money: " + fromAcct.getMoney().getAmount() + "\nValue to tranfer: " + transferMoney.getAmount());
            }
            System.out.println("Account 1: " + fromAcct.getMoney().getAmount() + "€");
            System.out.println("Account 2: " + toAcct.getMoney().getAmount() + "€");
        }

            public void start() {
                if (thread1 == null) {
                    thread1 = new Thread();
                    thread1.start();
                }
            }

        public boolean transferMoney(Account fromAcct, Account toAcct, EurosAmount amount) {
            if (fromAcct.getMoney().getAmount() < amount.getAmount()) {
                return false;
            }
            fromAcct.getMoney().setAmount(fromAcct.getMoney().getAmount() - amount.getAmount());
            toAcct.getMoney().setAmount(toAcct.getMoney().getAmount() + amount.getAmount());
            return true;
        }

    }
}