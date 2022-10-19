public class Prof extends Thread {

    public void run() {

        while (true) {
            if (!Main.maiSuntServetele()) {
                break;
            }
        }

        System.out.println("putem iesi afara\n");
    }
}
