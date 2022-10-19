import java.nio.charset.Charset;
import java.nio.charset.StandardCharsets;
import java.util.Random;
import java.util.concurrent.BrokenBarrierException;

public class Task extends Thread {
    int id;

    public Task(int id) {
        this.id = id;
    }

    synchronized public void addMessage(String message, int poz) {
        while(Main.mesaje[poz] != null) {
            poz = (poz + 1) % (Main.p * 3);
        }

        Main.mesaje[poz] = message + id;
    }

    public void run() {
        byte[] array = new byte[5];
        int poz;

        if (id % 2 == 0) {
            for (int i = 0; i < 3; i++) {
                new Random().nextBytes(array);
                String generatedString = new String(array, StandardCharsets.UTF_8);
                poz = new Random().nextInt(Main.p * 3);

                addMessage(generatedString, poz);

            }
        }
//////////////////////
        try {
            Main.barrier.await();
        } catch (InterruptedException | BrokenBarrierException e) {
            e.printStackTrace();
        }

        if (id == 0) {
            for (String i : Main.mesaje) {
                System.out.println(i + "  ");
            }
            System.out.println("\n");
        }

        try {
            Main.barrier.await();
        } catch (InterruptedException | BrokenBarrierException e) {
            e.printStackTrace();
        }
//////////////////////

        if (id % 2 == 1) {
            for (int i = 0; i < 3; i++) {
                new Random().nextBytes(array);
                String generatedString = new String(array, StandardCharsets.UTF_8);

                addMessage(generatedString, 0);
            }
        }




    }
}
