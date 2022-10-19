package task2;

import java.util.concurrent.ExecutorService;

public class MyRunnable implements Runnable {
    ExecutorService executorService;
    int[] colors;
    int step;

    public MyRunnable(ExecutorService executorService, int[] colors, int step) {
        this.executorService = executorService;
        this.colors = colors;
        this.step = step;
    }

    @Override
    public void run() {
        if (step == Main.N) {
            Main.printColors(colors);
            executorService.shutdown();
        } else {

            // For the node at position step try all possible colors
            for (int i = 0; i < Main.COLORS; i++) {
                int[] newColors = colors.clone();
                newColors[step] = i;
                if (Main.verifyColors(newColors, step)) {
                    executorService.submit(new MyRunnable(executorService, newColors, step + 1));
                }
            }
        }
    }
}
