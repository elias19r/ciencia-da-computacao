package br.usp.icmc.so.simulador;

import java.util.*;

public class MemoryManagerFF extends MemoryManager {

	public MemoryManagerFF(int size, ArrayList<Process> processes) {
		super(size,processes);
	}

	// Metodo que aloca a memoria de acordo com o algoritmo First-Fit
	public int allocMemory(int blockSize, int pid) {
		if (holes.size() == 0) {
			return -1;
		}
		
		Hole holeAux = new Hole();
		for (int i = 0; i < holes.size(); i++) {
			holeAux = holes.get(i);
			if (blockSize <= holeAux.getSize()) {
				int pointer = holeAux.getPointer();
				for (int j = pointer; j < (pointer + blockSize); j++) {
					cell[j] = pid;
				}
				if (blockSize == holeAux.getSize())
					holes.remove(holeAux);
				else
					holeAux.setPointer(pointer + blockSize);

				return pointer;
			}
		}

		return -1;
	}
}
