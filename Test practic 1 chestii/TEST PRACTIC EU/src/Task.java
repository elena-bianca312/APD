public class Task extends Thread {

    int id;
    int sleepTime;

    public Task(int id, int sleepTime) {
        this.id = id;
        this.sleepTime = sleepTime;
    }

    public void run() {

        // Do abstract time: sleep time
        try {
            sleep(sleepTime);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        // Attempt to enter so it can notify Initiator that it is done
        try {
            Testapd.initiator_sem.acquire();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        // Update value with current thread so it can be printed from Thread 0 (Initiator)
        // Increment count so Initiator knows when to stop
        Testapd.value = id;
        Testapd.count++;

        // Allow notifier to print it
        Testapd.task_sem.release();
    }
}
