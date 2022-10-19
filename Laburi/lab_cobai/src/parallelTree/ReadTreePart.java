package parallelTree;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;

public class ReadTreePart implements Runnable {
	TreeNode tree;
	String fileName;
	//CyclicBarrier barrier = new CyclicBarrier(2);

	public ReadTreePart(TreeNode tree, String fileName) {
		this.tree = tree;
		this.fileName = fileName;
	}

	@Override
	public void run() {
		try {
			Scanner scanner = new Scanner(new File(fileName));
			TreeNode treeNode;

			while (scanner.hasNextInt()) {
				int child = scanner.nextInt();
				int root = scanner.nextInt();

				treeNode = tree.getNode(root);
				while (treeNode == null) {
					treeNode = tree.getNode(root);
				}
				treeNode.addChild(new TreeNode(child));

			}
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		}
		//try {
			//Main.barrier.await();
			Main.semaphore.release();
//		} catch (BrokenBarrierException | InterruptedException e){
//			e.printStackTrace();
//		}
	}
}
