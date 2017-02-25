package br.usp.icmc.so.simulador;

/**
 * Classe que implementa a memoria principal
 */
public class MainMemory {
	private int size;
	public int cell[];

	public MainMemory(int size) {
		this.size = size;
		cell = new int[size];
	}

	// Metodo que aloca a memoria
	public int allocMemory(int blockSize) {
		int i = 0, pointer;
		while (true) {
			for ( ; i < size && cell[i] != 0; i++) ;
			//System.out.println("Found available pointer at " + i);

			int available = 0;
			for ( ; i < size && available < blockSize && cell[i] == 0; i++) {
				available++;
			}

			if (available == blockSize) {
				pointer = i - blockSize;
				for (i = pointer; i < (pointer + blockSize); i++) {
					cell[i] = 1;
				}
				return pointer;
			} else if (i > size) {
				//System.err.println("Not enough space... No memory allocated.");
				return -1;
			}
		}
	}
	
	// Metodo que libera a memoria
	public void freeMemory(int pointer, int blockSize) {
		for (int i = pointer; i < (pointer + blockSize); i++)
			cell[i] = 0;
	}
}
