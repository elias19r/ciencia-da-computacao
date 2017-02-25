import java.util.*;

/**
 * Classe que representa uma solucao (caminho)
 */
public class Solution implements Comparable<Solution> {
	public ArrayList<City> cities;
	public double cost;
	public double fitness;
	public double prob;
	public double cumulativeProb;

	/**
	 * Construtor
	 */
	public Solution(ArrayList<City> cities) {
		this.cities = cities;
		this.calculateCost();
		this.calculateFitness();
		this.prob = 0.0;
		this.cumulativeProb = 0.0;
	}

	/**
	 * Retorna uma copia deste objeto
	 */
	public Solution clone() {
		return new Solution(new ArrayList<City>(this.cities));
	}

	/**
	 * Calcula o custo do caminho desta solucao
	 */
	public void calculateCost() {
		this.cost = 0.0;
		
		int i, count = this.cities.size()-1;
		for (i = 0; i < count; i++)
			this.cost += this.cities.get(i).distances[this.cities.get(i+1).index];

		this.cost += this.cities.get(count).distances[this.cities.get(0).index];
	}

	/**
	 * Calcula o fitness
	 */
	public void calculateFitness() {
		this.fitness = (1.0 / this.cost) * 100000.0;
	}

	/**
	 * Imprime dados
	 */
	public void print() {
		System.out.print("Path: ");

		int i, count = this.cities.size();
		for (i  = 0; i < count; i++)
			System.out.print(cities.get(i).index+" ");

		System.out.println("\nCost: "+this.cost+"\nFitness: "+this.fitness+"\n");
	}

	/**
	 * Implementa o metodo compareTo da classe Comparable, de maneira a
	 * 	ordenar decrescente pelo fitness
	 */	
	public int compareTo(Solution s) {
		if (this.fitness > s.fitness)
			return -1;

		if (this.fitness < s.fitness)
			return 1;

		return 0;
	}
}

