package br.usp.icmc.so.simulador;

import java.util.*;

/**
 * Classe que implementa a memoria principal
 */
public abstract class MainMemory {
	protected int size;
	public int cell[];

	public MainMemory(int size) {
		this.size = size;
		cell = new int[size];
	}

	public MainMemory(int size, ArrayList<Process> processes) {
		this.size = size;
		cell = new int[size];

		Random random = new Random();
		int partition = this.size / processes.size();
		int counter = 0;

		for (Process p : processes) {
			int pointer = random.nextInt(partition - p.getRequiredMemory());
			pointer += counter * partition;

			for(int i = pointer; i < pointer + p.getRequiredMemory(); ++i)
				cell[i] = p.getPid();

			counter++;
			p.setMemoryPointer(pointer);
		}
	}

	public abstract int allocMemory(int blockSize, int pid);
	

	// Metodo que libera a memoria
	public void freeMemory(int pointer, int blockSize) {
		for (int i = pointer; i < (pointer + blockSize); i++)
			cell[i] = 0;
	}

	public void printMemory() {
		int i, j, k = 0;

		for (i = 0; i < size / 20; i++) {
			for (j = 0; j < 20; j++) {
				System.out.format("%-2d ", k+j);				
			}
			System.out.println();
			for (j = 0; j < 20; j++, k++) {
				if (cell[k] != 0)
					System.out.format("%-2d ", cell[k]);
				else
					System.out.format("%-2c ", '-');
			}
			System.out.print("\n\n");
		}
	}

}
