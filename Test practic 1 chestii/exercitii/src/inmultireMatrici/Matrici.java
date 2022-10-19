package inmultireMatrici;

public class Matrici extends Thread{
	public int id;
	public int noThreads;
	public int start;
	public int end;
	public int N;
	public int[][] A;
	public int[][] B;
	public int[][] C;

	public Matrici(int id, int noThreads, int N, int[][] A, int[][] B, int[][] C){
		this.id = id;
		this.noThreads = noThreads;
		this.N = N;
		this.A = A;
		this.B = B;
		this.C = C;
		start = id * N / noThreads;
		end = Math.min((id + 1) * N / noThreads, N);
	}

	@Override
	public void run() {
		for(int j = start; j < end; j++){
			for(int i = 0; i < N; i++){
				for(int k = 0; k < N; k++) {
					C[i][j] += A[i][k] * B[k][j];
				}
			}
		}
	}
}
