package ex1;

public class MyThread extends Thread {

    private int id;

    MyThread(int id) {
        this.id = id;
    }
    @Override
    public void run() {
        System.out.println("Hello from thread " + id);
    }
}
