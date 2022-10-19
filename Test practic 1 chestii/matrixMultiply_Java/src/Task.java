import java.util.ArrayList;
import java.util.concurrent.BrokenBarrierException;

public class Task extends Thread {

    public int id;
    public int noThreads;
    public int size;
    long[][] matrix, matrixHelper, multiplication;
    int power;

    public Task(int id, int noThreads, int size, long[][] matrix, long[][] matrixHelper,
                long[][] multiplication, int power) {
        this.id = id;
        this.noThreads = noThreads;
        this.size = size;
        this.matrix = matrix;
        this.matrixHelper = matrixHelper;
        this.multiplication = multiplication;
        this.power = power;
    }

    public void run() {

        int numberOfRows = (int) Math.ceil((float) size / noThreads);   // if 2 matrices multiplication, it would be number of rows in final matrix instead of size
        int start = id * numberOfRows;
        int end;
        if (noThreads - 1 == id) {
            end = size;
        } else {
            end = (id + 1) * numberOfRows;
        }

        // Multiply
        for (int multiply = 0; multiply < power - 1; multiply++) {

            for (int cnt = start; cnt < end; cnt++) {
                for (int i = 0; i < size; i++) {
                    multiplication[cnt][i] = 0;
                    for (int j = 0; j < size; j++) {
                        multiplication[cnt][i] += matrixHelper[cnt][j] * matrix[j][i];
                        System.out.println("thread id: " + id + " cnt: " + cnt + " i: " + i + " j: " + j + " " + matrixHelper[cnt][j] + " " + matrix[j][i]);
                        // System.out.println("Thread id: " + id + "   " + cnt + " " + i + "   value: " + multiplication[cnt][i]);
                    }
                }
            }

            try {
                Main.barrier.await();
            } catch (InterruptedException | BrokenBarrierException e) {
                e.printStackTrace();
            }

            matrixHelper = multiplication;

            if (id == 0) {
                for (int i = 0; i < size; i++) {
                    for (int j = 0; j < size; j++) {
                        System.out.print(multiplication[i][j] + " ");
                    }
                    System.out.println("");
                }
                System.out.println("");
            }

            try {
                Main.barrier.await();
            } catch (InterruptedException | BrokenBarrierException e) {
                e.printStackTrace();
            }

        }
    }
}