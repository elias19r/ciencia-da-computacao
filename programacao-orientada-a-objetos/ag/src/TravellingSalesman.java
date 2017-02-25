import java.util.*;

/**
 * Classe do caixeiro-viajante
 */
public class TravellingSalesman {
	public int times;
	public int n;	
	public int size;
	public double crossover;
	public double mutation;
	public double distances[][];
	public ArrayList<Solution> solutionsList;

	/**
	 * Construtor
	 */
	public TravellingSalesman(int times, int n, int size, double crossover, double mutation, double distances[][]) {
		this.times = times;
		this.n = n;
		this.size = size;
		this.crossover = crossover;
		this.mutation = mutation;
		this.distances = distances;
		this.createSolutionsList();
	}
	
	/**
	 * Calcula valores das probablidades de cada solucao
	 */
	public void calculateProbs() {
		int i;
		double Sum = 0.0;

		// Calcula o somatorio dos fitness
		for (i = 0; i < this.size; i++)
			Sum += this.solutionsList.get(i).fitness;

		// Calcula a probabilidade de cada solucao
		for (i = 0; i < this.size; i++)
			this.solutionsList.get(i).prob = this.solutionsList.get(i).fitness / Sum;

		// Calcula o acumulativo de probablidades para cada solucao
		this.solutionsList.get(0).cumulativeProb = this.solutionsList.get(0).prob;
		for(i = 1; i < this.size; i++)
			this.solutionsList.get(i).cumulativeProb = this.solutionsList.get(i).prob
				+ this.solutionsList.get(i-1).cumulativeProb;		
	}

	/**
	 * Cria aleatoriamente a lista de solucoes inicial
	 */
	public void createSolutionsList() {
		ArrayList<City> cities = new ArrayList<City>();
		int i, j;
		
		// Faz uma lista de cidades
		for (i = 0; i < this.n; i++)
			cities.add(new City(i, this.distances[i]));

		this.solutionsList = new ArrayList<Solution>(this.size);
		for (i = 0; i < this.size; i++) {
			// Embaralha a lista de cidades e cria uma nova solucao com esta lista
			Collections.shuffle(cities);
			this.solutionsList.add(new Solution(new ArrayList<City>(cities)));
		}
	}

	/**
	 * Executa o algoritmo genetico
	 */
	public Solution execute() {
		// Reproduz as solucoes
		int countCrossover = (int) Math.floor(((double) this.size * (this.crossover / 100.0)) / 2.0);
		int countMutation = (int) ((double) countCrossover * 2.0 * (this.mutation / 100.0));

		for (int i = 0; i < this.times; i++)
			this.reproduce(countCrossover, countMutation);

		// Ordena a lista de solucoes final e retorna a melhor solucao encontrada
		Collections.sort(this.solutionsList);
		return this.solutionsList.get(0);
	}

	/**
	 * Faz a reproducao, criando uma nova geracao de solucoes
	 */
	public void reproduce(int countCrossover, int countMutation) {
		int i, j, k, x, amountCrossover = countCrossover * 2;
		boolean valid = false;
		ArrayList<Solution> sons = null;
		ArrayList<Solution> newSolutionsList = new ArrayList<Solution>(this.size);
		
		// Calcula valores das probablidades de cada solucao
		this.calculateProbs();

		// Conjuntos de cidades para verificar se nao ocorreu
		//	cidades repetidas nos caminhos das solucoes-filhas
		HashSet<City> set1 = new HashSet<City>();
		HashSet<City> set2 = new HashSet<City>();
		Solution father, mother;
		for (i = 0; i < countCrossover; i++) {
			// Enquanto nao gerar filhos validos
			while (!valid) {	

				// Seleciona duas solucoes-pais
				father = this.selection();
				mother = this.selection();

				// Cruza e produz duas solucoes-filhas
				sons = this.crossover(father, mother);

				// Verifica se sao validas
				set1.addAll(sons.get(0).cities);
				set2.addAll(sons.get(1).cities);
				if (set1.size() == this.n && set2.size() == this.n) {
					if (sons.get(0).fitness >= father.fitness && sons.get(0).fitness >= mother.fitness)
						if (sons.get(1).fitness >= father.fitness && sons.get(1).fitness >= mother.fitness)
							valid = true;
				} else {
					sons.clear();
					sons = null;
				}
				set1.clear();
				set2.clear();
			}
			
			// Adiciona os filhos validos numa nova geracao de solucoes
			newSolutionsList.add(sons.get(0)); 
			newSolutionsList.add(sons.get(1)); 
		}

		// Faz a mutacao nos caminhos das solucoes-filhas
		//	trocando duas cidades de posicao
		Random rand = new Random();
		for (k = 0; k < countMutation; k++) {
			x = rand.nextInt(amountCrossover);
			i = rand.nextInt(this.n);
			j = rand.nextInt(this.n);
			Collections.swap(newSolutionsList.get(x).cities, i, j);
		
			// Recalcula custo e fitness
			newSolutionsList.get(x).calculateCost();
			newSolutionsList.get(x).calculateFitness();
		}

		// Completa a nova geracao com as melhores solucoes da geracao atual (elitismo)
		Collections.sort(this.solutionsList);
		k = this.size - amountCrossover;
		for (i = 0; i < k; i++)
			newSolutionsList.add(this.solutionsList.get(i).clone());

		// Substitui a geracao de solucoes atual pela geracao nova
		this.solutionsList.clear();
		this.solutionsList = null;
		this.solutionsList = newSolutionsList;
	}

	/**
	 * Seleciona um solucao-pais (metodo da roleta)
	 */
	public Solution selection() {
		Solution parent = null;
		
		// Gera um numero aletorio entre 0 e 1
		double r = (new Random()).nextDouble();

		// Ve qual solucao-pais foi escolhida (ainda nao entendo direito essa parte...)
		if (r < this.solutionsList.get(0).cumulativeProb) {
			parent = this.solutionsList.get(0);
		} else {
			for (int i = 1; i < this.size; i++)
				if (this.solutionsList.get(i-1).cumulativeProb < r && this.solutionsList.get(i).cumulativeProb >= r) {
					parent = this.solutionsList.get(i);
					break;
				}
		}

		return parent;
	}

	/**
	 * Faz o cruzamento entre duas solucoes-pais, gerando duas novas solucoes-filhas
	 */
	public ArrayList<Solution> crossover(Solution father, Solution mother) {
		ArrayList<Solution> ret = new ArrayList<Solution>(2);
		ArrayList<City> cities1 = new ArrayList<City>();
		ArrayList<City> cities2 = new ArrayList<City>();
		
		// Define um ponto de corte no vetor de cidades
		int i, cutoff = 1 + (new Random()).nextInt(this.n-2);

		// A partir do ponto de corte, copia parte para um filho e parte para outro
		//	fazendo o cruzamento
		for (i = 0; i < cutoff; i++) {
			cities1.add(father.cities.get(i));
			cities2.add(mother.cities.get(i));
		}
		for (i = cutoff; i < this.n; i++) {
			cities1.add(mother.cities.get(i));
			cities2.add(father.cities.get(i));
		}

		// Cria e retorna as solucoes-filhas geradas
		ret.add(new Solution(cities1));
		ret.add(new Solution(cities2));
		
		return ret;
	}
}

