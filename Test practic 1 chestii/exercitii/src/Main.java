import java.util.ArrayList;
import java.util.concurrent.CyclicBarrier;

public class Main {
    public static CyclicBarrier barrier;

    public static void main(String[] args) {
        int noThreads = Integer.parseInt(args[1]);
        int noElements = Integer.parseInt(args[0]);
        barrier = new CyclicBarrier(noThreads);
        Thread[] t = new Thread[noThreads];
        ArrayList<Integer> vector = new ArrayList<>(noElements);
        ArrayList<Integer> result = new ArrayList<>(noElements);

        for (int i = 0; i < noElements; i++) {
            vector.add(noElements - i + 5);
            result.add(0);
        }

        for (int i = 0; i < noThreads; i++) {
            t[i] = new Task(i, noThreads, vector, result);
            t[i].start();
        }


        for (int i = 0; i < noThreads; i++) {
            try {
                t[i].join();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

    }
}
