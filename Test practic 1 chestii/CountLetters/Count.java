package CountLetters;

import java.util.Collections;
import java.util.List;
import java.util.concurrent.Semaphore;

public class Count extends Thread{
	public Semaphore semaphore;
	//public Map<String, Integer> map;
	//public String arr = "";
	public List<String> list;

	public Count(Semaphore semaphore, List<String> list){
		this.semaphore = semaphore;
		this.list = list;
		//this.map = map;
	}

	@Override
	public void run() {
		try{
			semaphore.acquire();
		} catch (InterruptedException e){
			e.printStackTrace();
		}
		Collections.sort(list);
		//System.out.println(list);
//		for(int i = 97; i < 123; i++){
//			int nr = 0;
//			for(String l : list){
//				for(int j = 0; j < l.length(); j++){
//					if ((char) i == l.charAt(j)){
//						nr ++;
//					}
//				}
//			}
//			map.put(String.valueOf((char) i), nr);
//			System.out.println(map);
//		}
//		for(int i = 97; i < 123; i++){
//			int nr = 0;
//			for(int j = 0; j < arr.length(); j++){
//				char l = arr.charAt(j);
//				//System.out.println(l);
//				if ((char) i == l){
//					//System.out.println((char) i + " " + l);
//					//System.out.println((char) i);
//					nr++;
//				}
//			}
//			list.put(String.valueOf((char) i), nr);
//			//System.out.println(nr);
//		}
	}
}
