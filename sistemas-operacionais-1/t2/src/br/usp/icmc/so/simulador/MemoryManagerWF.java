package br.usp.icmc.so.simulador;

import java.util.*;

public class MemoryManagerWF extends MemoryManager {

	public MemoryManagerWF(int size, ArrayList<Process> processes) {
		super(size, processes);
	}

	// Metodo que aloca a memoria de acordo com o algoritmo Worst-Fit
	public int allocMemory(int blockSize, int pid) {
		if (holes.size() == 0) {
			return -1;
		}
		
		Hole holeAux = new Hole();
		Hole worstHole = holes.get(0);
		Integer indexWorst = -1;

		for (int i = 0; i < holes.size(); i++) {
			holeAux = holes.get(i);
			if (blockSize <= holeAux.getSize()) {
				if (worstHole.getSize() <= holeAux.getSize()) {
					worstHole = holeAux;
					indexWorst = i;
				}
			}
		}

		if (indexWorst == -1) {
			return -1;
		}
		
		int pointer = worstHole.getPointer();
		for (int j = pointer; j < (pointer + blockSize); j++) {
			cell[j] = pid;
		}

		if (blockSize == worstHole.getSize())
			holes.remove(worstHole);
		else
			worstHole.setPointer(pointer + blockSize);

		return pointer;
	}
}
