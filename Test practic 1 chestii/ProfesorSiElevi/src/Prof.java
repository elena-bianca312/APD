public class Prof extends Thread {

    public void run() {

        try {
            Main.elev_sem.acquire();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        Main.prof_sem.release();
        System.out.println("putem iesi afara\n");

    }
}
