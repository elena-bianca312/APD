package doubleVectorElements;

public class Main {

    public static void main(String[] args) {
        int N = 100000013;
        int[] v = new int[N];

        for (int i = 0; i < N; i++) {
            v[i] = i;
        }

//        // Parallelize me
//        for (int i = 0; i < N; i++) {
//            v[i] = v[i] * 2;
//        }

        int cores = Runtime.getRuntime().availableProcessors();
        Thread[] threads = new Thread[cores];

        for (int i = 0; i < cores; i++) {
            threads[i] = new Thread(new MyThread(i, v, N, cores));
            threads[i].start();
        }

        for (int i = 0; i < cores; i++) {
            try {
                threads[i].join();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

        for (int i = 0; i < N; i++) {
            if (v[i] != i * 2) {
                System.out.println("Wrong answer");
                System.exit(1);
            }
        }
        System.out.println("Correct");
    }

}
