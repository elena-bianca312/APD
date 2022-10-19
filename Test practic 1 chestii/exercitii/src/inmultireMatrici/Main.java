package inmultireMatrici;

public class Main {
	public static void main(String[] args) {
		int N = 4;
		int[][] c = new int[N][N];
		int[][] a = new int[N][N];
		int[][] b = new int[N][N];

		for(int i = 0; i < N; i++){
			for(int j = 0; j < N; j++){
				a[i][j] = i;
				b[i][j] = i;
			}
		}

		Thread[] threads = new Thread[4];
		for (int i = 0; i < 4; i++) {
			threads[i] = new Thread(new Matrici(i, 4, N, a, b, c));
			threads[i].start();
		}

		for (int i = 0; i < 2; i++) {
			try {
				threads[i].join();
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}

		for(int i = 0; i < N; i++){
			for(int j = 0; j < N; j++){
				System.out.println(c[i][j]);
			}
		}
	}
}
