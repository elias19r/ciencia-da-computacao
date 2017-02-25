import java.util.*;

/**
 * Classe que roda o programa
 */
public class Program {
	
	/**
	 * Metodo principal
	 */
	public static void main(String args[]) {
		// Leitor de teclado
		Scanner scanner = new Scanner(System.in);

		// Le o cabecalho dos dados de entrada
		int times = scanner.nextInt();
		int n = scanner.nextInt();
		int size = scanner.nextInt();
		double crossover = scanner.nextDouble();
		double mutation = scanner.nextDouble();

		// Le a matriz de distancias entre as cidades
		double distances[][] = new double[n][n];
		int i, j, k, count = n * n;
		for (k = 0; k < count; k++) {
			i = scanner.nextInt();
			j = scanner.nextInt();
			distances[i][j] = scanner.nextDouble();
		}

		// Cria o caixeiro-viajante
		TravellingSalesman ts = new TravellingSalesman(times, n, size, crossover, mutation, distances);

		long startTime = System.currentTimeMillis();

		// Executa o algoritmo genetico e pega a melhor solucao
		Solution best = ts.execute();

		long endTime = System.currentTimeMillis();
		System.out.println("Total time\n----------\n"+(endTime-startTime)+" ms\n\n");

		// Imprime a melhor solucao
		System.out.println("Best solution found\n-------------------");
		best.print();
/*
		// Imprime todas as solucoes da ultima iteracao
		System.out.println("\nAll solutions\n-------------");
		for (i = 0; i < size; i++)
			ts.solutionsList.get(i).print();
*/
	}
}

