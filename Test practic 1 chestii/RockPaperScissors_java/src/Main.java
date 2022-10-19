import java.util.ArrayList;
import java.util.HashMap;
import java.util.SortedMap;
import java.util.TreeMap;
import java.util.concurrent.CyclicBarrier;
import java.util.concurrent.Semaphore;

public class Main {
    public static CyclicBarrier barrier;
    public static Semaphore semaphore;

    public static void main(String[] args) {

        if (args.length < 1) {
            System.err.println("Usage: Main <no_threads>");
            return;
        }

        int noThreads = Integer.parseInt(args[0]);
        if (noThreads % 2 != 0) {
            System.err.println("Number of threads should be even");
            return;
        }

        barrier = new CyclicBarrier(noThreads);
        semaphore = new Semaphore(1);
        Printer printer = new Printer();
        ArrayList<Task> list = new ArrayList<>();

        Thread[] t = new Thread[noThreads];

        for (int i = 0; i < noThreads; i++) {
            t[i] = new Thread(new Task(i, noThreads, semaphore, printer, list));
            t[i].setPriority(noThreads - i);
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