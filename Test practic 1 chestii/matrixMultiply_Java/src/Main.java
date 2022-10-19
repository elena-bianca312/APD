import java.util.ArrayList;
import java.util.concurrent.CyclicBarrier;

public class Main {
    public static CyclicBarrier barrier;

    public static void main(String[] args) {

        // Read parameters
        int noThreads   = Integer.parseInt(args[0]);          // P
        int noElements  = Integer.parseInt(args[1]);          // N
        int power       = Integer.parseInt(args[2]);          // K

        // Declare variables
        long[][] matrix          = new long[noElements][noElements];
        long[][] matrixHelper    = new long[noElements][noElements];
        long[][] multiplication  = new long[noElements][noElements];

        for (int i = 0; i < noElements; i++) {
            for (int j = 0; j < noElements; j++) {
                matrix[i][j]        = 1 + (long) i * noElements + j;
                matrixHelper[i][j]  = 1 + (long) i * noElements + j;
            }
        }

        // Thread logic
        barrier = new CyclicBarrier(noThreads);
        Thread[] t = new Thread[noThreads];

        for (int i = 0; i < noThreads; i++) {
            t[i] = new Task(i, noThreads, noElements, matrix, matrixHelper, multiplication, power);
            t[i].start();
        }


        for (int i = 0; i < noThreads; i++) {
            try {
                t[i].join();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

        // Print matrix
//        for (int i = 0; i < noElements; i++) {
//            for (int j = 0; j < noElements; j++) {
//                System.out.print(matrix[i][j] + " ");
//            }
//            System.out.println("");
//        }

//        for (int i = 0; i < noElements; i++) {
//            for (int j = 0; j < noElements; j++) {
//                System.out.print(multiplication[i][j] + " ");
//            }
//            System.out.println("");
//        }

    }
}