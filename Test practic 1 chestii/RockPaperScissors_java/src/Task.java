import java.util.ArrayList;
import java.util.HashMap;
import java.util.TreeMap;
import java.util.concurrent.BrokenBarrierException;
import java.util.concurrent.Semaphore;

public class Task implements Runnable {
    public int id;
    public int noThreads;
    String choice = "";

    Semaphore semaphore;
    Printer print;
    ArrayList<Task> list;

    public Task(int id, int noThreads, Semaphore semaphore, Printer print, ArrayList<Task> list) {
        this.id = id;
        this.noThreads = noThreads;
        this.semaphore = semaphore;
        this.print = print;
        this.list = list;
    }

    public void run() {

        int randomNum = (int) (Math.random() * 2);
        switch (randomNum) {
            case 0 -> choice = "Piatra";
            case 1 -> choice = "Foarfeca";
            case 2 -> choice = "Hartie";
        }

        // CHESTIA ASTA E F BUNA DACA VREAU SA PRINTEZ CHESTII ALTERNATIV
//        if (id % 2 == 0) {
//            // System.out.print("Threadul " + id + " care a ales " + choice);
//            print.printEven(id, choice);
//        }
//
//        if (id % 2 != 0) {
//            //System.out.println(" se bate cu threadul " + id + " care a ales " + choice + ".");
//            print.printOdd(id, choice);
//        }

        list.add(this);

        try {
            Main.barrier.await();
        } catch (InterruptedException | BrokenBarrierException e) {
            e.printStackTrace();
        }

        int blockSize = (int) Math.ceil((float) list.size() / (noThreads * 2));
        int startIndex = id * blockSize;
        int endIndex;
        if (noThreads - 1 == id) {
            endIndex = list.size();
        } else {
            endIndex = (id + 1) * blockSize;
        }

        ArrayList<Task> newList = new ArrayList<>();
        for (int i = startIndex; i < endIndex; i = i + 2) {
            System.out.println("Threadul " + list.get(i).id + " care a ales " + list.get(i).choice +
                    " se bate cu threadul " + list.get(i + 1).id + " care a ales " + list.get(i + 1).choice + ".");
            if (list.get(i).choice.equals("Piatra")) {
                if (list.get(i + 1).choice.equals("Piatra")) {
                    newList.add(list.get(i + 1));
                }
                if (list.get(i + 1).choice.equals("Hartie")) {
                    newList.add(list.get(i + 1));
                }
                if (list.get(i + 1).choice.equals("Foarfeca")) {
                    newList.add(list.get(i));
                }
            }
            if (list.get(i).choice.equals("Foarfeca")) {
                if (list.get(i + 1).choice.equals("Piatra")) {
                    newList.add(list.get(i + 1));
                }
                if (list.get(i + 1).choice.equals("Hartie")) {
                    newList.add(list.get(i));
                }
                if (list.get(i + 1).choice.equals("Foarfeca")) {
                    newList.add(list.get(i + 1));
                }
            }
            if (list.get(i).choice.equals("Hartie")) {
                if (list.get(i + 1).choice.equals("Piatra")) {
                    newList.add(list.get(i));
                }
                if (list.get(i + 1).choice.equals("Hartie")) {
                    newList.add(list.get(i + 1));
                }
                if (list.get(i + 1).choice.equals("Foarfeca")) {
                    newList.add(list.get(i + 1));
                }
            }
        }

        try {
            Main.barrier.await();
        } catch (InterruptedException | BrokenBarrierException e) {
            e.printStackTrace();
        }

        list = newList;
    }
}