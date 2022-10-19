package task8;

import util.BinarySearchTreeNode;

import java.util.concurrent.RecursiveTask;

public class BSTHeightCalculator<T extends Comparable<T>> extends RecursiveTask<Integer> {
    BinarySearchTreeNode<T> binarySearchTreeNode;

    public BSTHeightCalculator(BinarySearchTreeNode<T> binarySearchTree) {
        this.binarySearchTreeNode = binarySearchTree;
    }

    @Override
    protected Integer compute() {
        if (binarySearchTreeNode == null) {
            return 0;
        }

        BSTHeightCalculator<T> left = new BSTHeightCalculator<>(binarySearchTreeNode.getLeft());
        BSTHeightCalculator<T> right = new BSTHeightCalculator<>(binarySearchTreeNode.getRight());

        left.fork();
        right.fork();

        return 1 + Math.max(left.join(), right.join());
    }
}
