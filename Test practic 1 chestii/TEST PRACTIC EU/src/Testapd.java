import java.util.ArrayList;
import java.util.concurrent.Semaphore;

public class Testapd {

    static Semaphore task_sem;
    static Semaphore initiator_sem;
    static Integer value = 0;
    static Integer count = 0;

    public static void main(String[] args) {

        ArrayList<Integer> sleepTimes = new ArrayList<>();

        // Read input
        int n = Integer.parseInt(args[0]);

        sleepTimes.add(0);
        for (int i = 1; i < n; i++) {
            int time = Integer.parseInt(args[i]);
            sleepTimes.add(time);
        }

        task_sem = new Semaphore(0);
        initiator_sem = new Semaphore(0);
        Thread[] t = new Thread[n];

        // Initiate threads
        for (int i = 0; i < n; i++) {

            if (i == 0) {
                t[0] = new Initiator(n);
                t[0].start();
            } else {
                t[i] = new Task(i, sleepTimes.get(i));
                t[i].start();
            }
        }

        for (int i = 0; i < n; ++i) {
            try {
                t[i].join();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }
}
