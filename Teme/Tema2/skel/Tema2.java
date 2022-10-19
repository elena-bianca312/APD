import java.io.*;
import java.util.*;

public class Tema2 {
    public static void main(String[] args) {
        if (args.length < 3) {
            System.err.println("Usage: Tema2 <workers> <in_file> <out_file>");
            return;
        }

        Parser parser = new Parser(args[1], Integer.parseInt(args[0]), args[2]);
        parser.makeTasks();
    }
}
