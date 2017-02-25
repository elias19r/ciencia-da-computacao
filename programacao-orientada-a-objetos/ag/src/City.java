import java.util.*;

/**
 * Classe que armazena uma cidade
 */
public class City {
	public int index;
	public double distances[];

	/**
	 * Construtor
	 */
	public City(int index, double distances[]) {
		this.index = index;
		this.distances = distances;
	}

	/**
	 * Imprime o vetor de distancias desta cidade para as outras
	 */
	public void print() {
		System.out.print("City "+this.index+"\nDistances: ");

		for (int i = 0; i < this.distances.length; i++)
			System.out.print(i+":"+distances[i]+"|");

		System.out.println("\n");
	}
}

