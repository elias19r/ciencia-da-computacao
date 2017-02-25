package br.usp.icmc.so.simulador;

import java.util.*;

public class MemoryManagerNF extends MemoryManager {
	private Integer currentHole;

	public MemoryManagerNF(int size, ArrayList<Process> processes) {
		super(size,processes);
		this.currentHole = 0;
	}

	// Metodo que aloca a memoria de acordo com o algoritmo Next-Fit
	public int allocMemory(int blockSize, int pid) {
		if (this.holes.size() == 0) {
			return -1;
		}

		Hole holeAux = new Hole();
		int i = currentHole;
		do {
			holeAux = this.holes.get(i);

			if (blockSize <= holeAux.getSize()) {
				int pointer = holeAux.getPointer();
				for(int j = pointer; j < (pointer + blockSize); j++) {
					cell[j] = pid;
				}
				
				this.currentHole = i;
				if (blockSize == holeAux.getSize())
					holes.remove(holeAux);
				else
					holeAux.setPointer(pointer + blockSize);

				return pointer;
			}

			i++;
			if (i == this.holes.size()) i = 0;

		} while (i != this.currentHole);

		return -1;
	}
}
