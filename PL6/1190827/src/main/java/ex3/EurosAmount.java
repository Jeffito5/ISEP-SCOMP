package ex3;

import java.util.Random;

/**
 * @author Luís Araújo
 */
public class EurosAmount {
    private float amount;

    public EurosAmount(float max){
        amount=max;
    }

//    public int subtrair(int max, int valor){
//        return max-valor;
//    }
//
//    public int somar(int max, int valor){
//        return max+valor;
//    }

    public float getAmount() {
        return amount;
    }

    public void setAmount(float amount) {
        this.amount = amount;
    }
}
