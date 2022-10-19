package shortestPathsFloyd_Warshall;

import java.util.concurrent.BrokenBarrierException;
import java.util.concurrent.CyclicBarrier;

public class MyThread implements Runnable {

    private final int id, graph[][], N;
    private final int start, end;
    private final int cores;
    private CyclicBarrier barrier;

    public MyThread(int id, int[][] graph, int N, int cores, CyclicBarrier barrier) {
        this.id = id;
        this.graph = graph;
        this.cores = cores;
        this.N = N;
        this.barrier = barrier;
        start = id * N / cores;
        end = Math.min((id + 1) * N / cores, N);
    }

    @Override
    public void run() {
        for (int k = 0; k < N; k++) {
            for (int i = start; i < end; i++) {
                for (int j = 0; j < N; j++) {
                    graph[i][j] = Math.min(graph[i][k] + graph[k][j], graph[i][j]);
                }
            }
            try {
                barrier.await();
            } catch (InterruptedException | BrokenBarrierException e) {
                e.printStackTrace();
            }
        }
    }
}
