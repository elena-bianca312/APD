package synchronizedSortedList;

import java.util.Collections;
import java.util.List;
import java.util.concurrent.Semaphore;

public class Sort extends Thread {
    private final List<Integer> list;
    private Semaphore semaphore;

    public Sort(List<Integer> list, Semaphore semaphore) {
        this.semaphore = semaphore;
        this.list = list;
    }

    @Override
    public void run() {
        try {
            semaphore.acquire();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        Collections.sort(list);
        //System.out.println(list);
    }
}
