
public class Printer {

    private volatile boolean isOdd = true;

    synchronized void printEven(int id, String choice) {
        while (!isOdd) {
            try {
                wait();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
        // System.out.println(Thread.currentThread().getName() + ": " + number);
        System.out.print("Threadul " + id + " care a ales " + choice);
        isOdd = false;
        notify();
    }

    synchronized void printOdd(int id, String choice) {
        while (isOdd) {
            try {
                wait();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
        // System.out.println(Thread.currentThread().getName() + ": " + number);
        System.out.println(" se bate cu threadul " + id + " care a ales " + choice + ".");
        isOdd = true;
        notify();
    }
}
