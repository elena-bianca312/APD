package CountLetters;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Map;
import java.util.concurrent.Semaphore;
import java.util.stream.Collectors;

public class Main {
	public static int noThreads = 4;
	public static Semaphore semaphore;

	public static void main(String[] args) {
		Thread[] threads = new Thread[noThreads];
		semaphore = new Semaphore(-2);
		//Map<String, Integer> map = Collections.synchronizedMap(new HashMap<>());
		List<String> list = Collections.synchronizedList(new ArrayList<>());

		threads[0] = new Read("src/file1", list);
		threads[1] = new Read("src/file2", list);
		threads[2] = new Read("src/file3", list);
		threads[3] = new Count(semaphore, list);

		for(int i = 0; i < noThreads; i++){
			threads[i].start();
		}

		for (int j = 0; j < noThreads; j++) {
			try {
				threads[j].join();
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
		Map<String, Long> counts =
				list.stream().collect(Collectors.groupingBy(e -> e, Collectors.counting()));
		System.out.println(counts);
//		for(String l : list){
//			//System.out.println(l);
//			for(int j = 97; j < 123; j++){
//
//			}
//		}
	}
}
