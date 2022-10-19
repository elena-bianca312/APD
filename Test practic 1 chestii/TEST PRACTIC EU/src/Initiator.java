public class Initiator extends Thread {

    int noThreads;

    public Initiator(int noThreads) {
        this.noThreads = noThreads;
    }

    public void run() {

        while (Testapd.count < noThreads - 1) {

            // Allow tasks to remind Thread 0 that they are done
            // Only one can enter so Initiator can print all finished tasks one by one
            Testapd.initiator_sem.release();

            try {
                // Can enter only when a task has finished its job
                Testapd.task_sem.acquire();
                System.out.println("Thread-ul " + Testapd.value + " a terminat");
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }
}
