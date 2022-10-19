package oneProducerOneConsumer;

public class Buffer {
    // Initializare buffer cu o valoare default;
    private int a = -1;

    synchronized void put(int value) {
        if (a > -1) {
            try {
                this.wait();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
        a = value;
        this.notify();
    }

    synchronized int get() {
        if (a == -1) {
            try {
                this.wait();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
        int result = a;
        a = -1;
        this.notify();
        return result;
    }
}
