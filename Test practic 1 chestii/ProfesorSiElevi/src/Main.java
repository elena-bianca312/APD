import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.Semaphore;

public class Main {

    static int n = 6;
    static Semaphore elev_sem;
    static Semaphore prof_sem;

    public static void main(String[] args) {

        elev_sem = new Semaphore(-n + 1);
        prof_sem = new Semaphore(-1);
        Thread[] t = new Thread[n + 1];

        for (int i = 0; i < n; i++) {
            t[i] = new Elev(i);
            t[i].start();
        }
        t[n] = new Prof();
        t[n].start();

        for (int i = 0; i < n + 1; ++i) {
            try {
                t[i].join();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }


    }
}
