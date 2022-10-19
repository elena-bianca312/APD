package doubleVectorElements;

public class MyThread implements Runnable {

    private final int id, v[];
    private final int start, end;
    private final int cores;

    public MyThread(int id, int[] v, int N, int cores) {
        this.id = id;
        this.v = v;
        this.cores = cores;
        start = id * N / cores;
        end = Math.min((id + 1) * N / cores, N);
    }

    @Override
    public void run() {
        for (int i = start; i < end; i++) {
            v[i] *= 2;
        }
    }
}
