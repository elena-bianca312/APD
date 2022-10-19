package multipleProducersMultipleConsumers;

import java.util.concurrent.ArrayBlockingQueue;

public class Buffer {
	int value;
	public ArrayBlockingQueue<Integer> buffer;

	public Buffer() {
		this.buffer =  new ArrayBlockingQueue<>(Main.N);
	}

	void put(int value) {
		try {
			this.buffer.put(value);
		} catch (InterruptedException e){
			e.printStackTrace();
		}
	}

	int get() {
		try {
			return buffer.take();
		} catch (InterruptedException e){
			return -1;
		}
	}
}
