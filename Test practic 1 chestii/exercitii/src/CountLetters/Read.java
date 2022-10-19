package CountLetters;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.List;
import java.util.Scanner;

public class Read extends Thread{
	public int id;
	public int noThreads;
	public String filename;
	//public Map<String, Integer> letters;
	//public String arr = "";
	public List<String> letters;

	public Read(String filename, List<String> letters){
		this.filename = filename;
		this.letters = letters;
	}

	@Override
	public void run() {
		try {
			Scanner scanner = new Scanner(new File(filename));
			while (scanner.hasNext()) {
				letters.add(scanner.next());
				//System.out.println(characterList);
				//arr = arr + scanner.next();
				//System.out.println(arr);
			}
			//System.out.println(letters);
//			for(int i = 97; i < 123; i++){
//				int nr = 0;
//				for(int j = 0; j < arr.length(); j++){
//					char l = arr.charAt(j);
//					//System.out.println(l);
//					if ((char) i == l){
//						//System.out.println((char) i + " " + l);
//						//System.out.println((char) i);
//						nr++;
//					}
//				}
//				letters.put(String.valueOf((char) i), nr);
//				//System.out.println(nr);
//			}
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		}
		Main.semaphore.release();
	}
}
