public class Elev extends Thread {
    int id;

    public Elev(int id) {
        this.id = id;
    }

    void iaServetel() {
        System.out.println("Elevul cu nr "  + id + "a luat servetel\n");
        Main.decNrServetele();
    }

    public void run() {
        this.iaServetel();
    }

}
