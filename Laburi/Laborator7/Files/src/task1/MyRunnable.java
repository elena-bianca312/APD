package task1;

import java.util.ArrayList;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.atomic.AtomicInteger;

public class MyRunnable implements Runnable {
    ExecutorService executorService;
    ArrayList<Integer> partialPath;
    int destination;
    AtomicInteger inQueue;

    public MyRunnable(ExecutorService executorService, ArrayList<Integer> partialPath, int destination, AtomicInteger inQueue) {
        this.executorService = executorService;
        this.partialPath = partialPath;
        this.destination = destination;
        this.inQueue = inQueue;
    }


    @Override
    public void run() {
        if (partialPath.get(partialPath.size() - 1) == destination) {
            System.out.println(partialPath);
        } else {

            // Se verifica nodurile pentru a evita ciclarea in graf
            int lastNodeInPath = partialPath.get(partialPath.size() - 1);
            for (int[] ints : Main.graph) {
                if (ints[0] == lastNodeInPath) {
                    if (partialPath.contains(ints[1]))
                        continue;
                    ArrayList<Integer> newPartialPath = new ArrayList<>(partialPath);
                    newPartialPath.add(ints[1]);

                    // Adaugam nou task
                    inQueue.incrementAndGet();
                    executorService.submit(new MyRunnable(executorService, newPartialPath, destination, inQueue));
                }
            }
        }

        // Verificam daca mai sunt task-uri in coada
        int left = inQueue.decrementAndGet();
        if (left == 0) {
            executorService.shutdown();
        }

    }
}
