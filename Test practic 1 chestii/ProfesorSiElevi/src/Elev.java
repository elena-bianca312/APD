public class Elev extends Thread {
    int id;

    public Elev(int id) {
        this.id = id;
    }

    void iaServetel() {
        System.out.println("Elevul cu nr "  + id + " a luat servetel\n");
    }

    public void run() {
       this.iaServetel();
       Main.elev_sem.release();

        try {
            Main.prof_sem.acquire(0);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        System.out.println("Elevul cu nr "  + id + " iese\n");

    }

}
