package task7;

import util.BinarySearchTreeNode;

import java.util.concurrent.CompletableFuture;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.atomic.AtomicInteger;

public class MyRunnable<T extends Comparable<T>> implements Runnable {
    ExecutorService executorService;
    CompletableFuture<String> completableFuture;
    BinarySearchTreeNode<T> binarySearchTree;
    T value;
    AtomicInteger inQueue;

    public MyRunnable(ExecutorService executorService,
                      CompletableFuture<String> completableFuture,
                      BinarySearchTreeNode<T> binarySearchTree,
                      T value,
                      AtomicInteger inQueue) {
        this.executorService = executorService;
        this.completableFuture = completableFuture;
        this.binarySearchTree = binarySearchTree;
        this.value = value;
        this.inQueue = inQueue;
    }


    @Override
    public void run() {
        if (binarySearchTree != null) {
            if (value.equals(binarySearchTree.getValue())) {
                completableFuture.complete("Found value: " + binarySearchTree.getValue());
                executorService.shutdown();
            } else if (binarySearchTree.getValue().compareTo(value) > 0) {
                inQueue.incrementAndGet();
                Runnable t = new MyRunnable<>(executorService,
                        completableFuture,
                        binarySearchTree.getLeft(),
                        value,
                        inQueue);
                executorService.submit(t);
            } else {
                inQueue.incrementAndGet();
                Runnable t = new MyRunnable<>(executorService,
                        completableFuture,
                        binarySearchTree.getRight(),
                        value,
                        inQueue);
                executorService.submit(t);
            }
        }

        int left = inQueue.decrementAndGet();
        if (left == 0) {
            completableFuture.complete("Value not found");
            executorService.shutdown();
        }
    }
}
