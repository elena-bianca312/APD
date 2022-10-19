package multipleProducersMultipleConsumers;

public class Buffer {
    private int a = -1;

    synchronized public void put(int value) {
        while (a > -1) {
            try {
                this.wait();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
        a = value;
        this.notifyAll();
    }

    synchronized public int get() {
        while (a == -1) {
            try {
                this.wait();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
        int result = a;
        a = -1;
        this.notifyAll();
        return result;
    }
}
