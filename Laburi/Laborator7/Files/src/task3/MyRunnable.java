package task3;

import java.util.concurrent.ExecutorService;

public class MyRunnable implements Runnable {
    ExecutorService executorService;
    int step;
    int[] graph;

    public MyRunnable(ExecutorService executorService, int step, int[] graph) {
        this.executorService = executorService;
        this.step = step;
        this.graph = graph;
    }

    @Override
    public void run() {
        if (Main.N == step) {
            Main.printQueens(graph);
            executorService.shutdown();
        } else {
            for (int i = 0; i < Main.N; ++i) {
                int[] newGraph = graph.clone();
                newGraph[step] = i;

                if (Main.check(newGraph, step)) {
                    executorService.submit(new MyRunnable(executorService, step + 1, newGraph));
                }
            }
        }
    }
}
