package task8;

import util.BSTOperations;
import util.BinarySearchTreeNode;

import java.util.concurrent.ExecutionException;
import java.util.concurrent.ForkJoinPool;

public class Main {
    private static <T extends Comparable<T>> int calculateMaximumHeight(BinarySearchTreeNode<T> binarySearchTree) {
        if (binarySearchTree == null) {
            return 0;
        }

        return 1 + Math.max(
                calculateMaximumHeight(binarySearchTree.getRight()),
                calculateMaximumHeight(binarySearchTree.getLeft())
        );
    }

    public static void main(String[] args) throws ExecutionException, InterruptedException {
        BinarySearchTreeNode<Integer> binarySearchTree = new BinarySearchTreeNode<>(3);
        binarySearchTree = BSTOperations.insertNode(binarySearchTree, 6);
        binarySearchTree = BSTOperations.insertNode(binarySearchTree, 9);
        binarySearchTree = BSTOperations.insertNode(binarySearchTree, 2);
        binarySearchTree = BSTOperations.insertNode(binarySearchTree, 10);
        binarySearchTree = BSTOperations.insertNode(binarySearchTree, 1);
        binarySearchTree = BSTOperations.insertNode(binarySearchTree, 11);

//        System.out.println(calculateMaximumHeight(binarySearchTree));

        ForkJoinPool forkJoinPool = new ForkJoinPool(4);
        BSTHeightCalculator<Integer> heightCalculator = new BSTHeightCalculator<>(binarySearchTree);
        forkJoinPool.execute(heightCalculator);
        System.out.println(heightCalculator.get());
    }

}
