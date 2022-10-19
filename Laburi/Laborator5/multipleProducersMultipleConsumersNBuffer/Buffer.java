package multipleProducersMultipleConsumersNBuffer;

import java.util.Queue;

public class Buffer {
    
    Queue<Integer> queue;
    int maxValue;
    
    public Buffer(int size) {
        queue = new LimitedQueue<>(size);
        maxValue = size;
    }

	synchronized public void put(int value) {
        while (queue.size() == maxValue) {
            try {
                this.wait();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
        queue.add(value);
        this.notifyAll();
	}

	synchronized public int get() {
        while (queue.isEmpty()) {
            try {
                this.wait();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
        int a = -1;
        Integer result = queue.poll();
        if (result != null) {
            a = result;
        }
        this.notifyAll();
        return a;
	}
}
