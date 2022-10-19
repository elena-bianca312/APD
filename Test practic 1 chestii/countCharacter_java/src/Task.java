import java.util.ArrayList;
import java.util.concurrent.BrokenBarrierException;

public class Task extends Thread{
    public int id;
    public int noThreads;
    ArrayList<Integer> readArray, resultArray;

    public Task(int id, int noThreads, ArrayList<Integer> readArray,
                ArrayList<Integer> resultArray) {
        this.id = id;
        this.noThreads = noThreads;
        this.readArray = readArray;
        this.resultArray = resultArray;
    }

    public void run() {
        int blockSize = (int) Math.ceil((float) readArray.size() / noThreads);
        int startIndex = id * blockSize;
        int endIndex;
        if (noThreads - 1 == id) {
            endIndex = readArray.size();
        } else {
            endIndex = (id + 1) * blockSize;
        }

        // Rank sort
        for (int j = startIndex; j < endIndex; j++) {
            int currentItem = readArray.get(j);
            int currentPosition = 0;
            for (Integer integer : readArray) {
                if (currentItem > integer) {
                    currentPosition++;
                }
            }
            resultArray.set(currentPosition, currentItem);
        }

        try {
            Main.barrier.await();
        } catch (InterruptedException | BrokenBarrierException e) {
            e.printStackTrace();
        }

        if (id == 0) {
            for (Integer integer : resultArray) {
                System.out.print(integer + " ");
            }
            System.out.println();
        }
    }
}