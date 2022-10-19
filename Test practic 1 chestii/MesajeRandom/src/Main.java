import java.util.concurrent.CyclicBarrier;
import java.util.concurrent.Semaphore;

public class Main {

    static int p = 3;
    static String[] mesaje = new String[p * 3];
    //static Semaphore semafor = new Semaphore(p / 2 + p % 2);

    static CyclicBarrier barrier = new CyclicBarrier(p);

    public static void main(String[] args) {

        Thread[] t = new Thread[p];
        for (int i = 0; i < p; i++) {
            t[i] = new Task(i);
            t[i].start();
        }

        for (int i = 0; i < p; ++i) {
            try {
                t[i].join();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

        for (String i : mesaje) {
            System.out.println(i + "  ");
        }

    }
}
