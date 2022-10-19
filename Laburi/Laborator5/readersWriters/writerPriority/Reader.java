package readersWriters.writerPriority;

import java.util.concurrent.BrokenBarrierException;

public class Reader extends Thread {
    private final int id;

    public Reader(int id) {
        super();
        this.id = id;
    }

    @Override
    public void run() {
        try {
            Main.barrier.await();
        } catch (InterruptedException | BrokenBarrierException e) {
            e.printStackTrace();
        }

        do {
            // TODO
            try {
                Main.enterSem.acquire();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            if (Main.currentWriters > 0 || Main.waitingWriters > 0) {
                Main.waitingReaders++;
                Main.enterSem.release();
                try {
                    Main.readersSem.acquire();
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }

            Main.currentReaders++;
            // TODO
            if (Main.waitingReaders > 0) {
                Main.waitingReaders--;
                Main.readersSem.release();
            } else {
                if (Main.waitingReaders == 0) {
                    Main.enterSem.release();
                }
            }

            try {
                Thread.sleep(100);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            System.out.println("Reader " + id + " is reading");
            Main.hasRead[id] = true;

            // TODO
            try {
                Main.enterSem.acquire();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            Main.currentReaders--;

            // TODO
            if (Main.currentReaders == 0 && Main.waitingReaders > 0) {
                Main.waitingWriters--;
                Main.writersSem.release();
            } else {
                if (Main.currentReaders > 0 || Main.waitingWriters == 0) {
                    Main.enterSem.release();
                }
            }
        } while (!Main.hasRead[id]);
    }
}
