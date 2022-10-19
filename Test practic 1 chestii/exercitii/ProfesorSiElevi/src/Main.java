import java.util.ArrayList;
import java.util.List;

public class Main {

    static int n = 6;

    synchronized static public void decNrServetele() {
        n--;
        System.out.println(n + "\n");
    }

    static public boolean maiSuntServetele() {
        if (n == 0) {
            return false;
        }
        return true;
    }

    public static void main(String[] args) {

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
