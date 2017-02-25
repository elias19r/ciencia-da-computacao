package br.usp.icmc.so.simulador;

import java.util.*;

public class MemoryManagerBF extends MemoryManager {

	public MemoryManagerBF(int size, ArrayList<Process> processes) {
		super(size,processes);
	}

	// Metodo que aloca a memoria de acordo com o algoritmo Best-Fit
	public int allocMemory(int blockSize, int pid) {
		int i;
		
		if (holes.size() == 0) {
			return -1;
		}
		
		Hole holeAux = new Hole();
		Hole bestHole = holes.get(0);
		Integer indexBest = -1;
		
		// Inicializa o bestHole com o primeiro buraco em que o processo cabe
		for (i = 0; i < holes.size(); i++) {
			holeAux = holes.get(i);
			if (blockSize <= holeAux.getSize()) {
				bestHole = holeAux;
				indexBest = i;
				break;
			}
		}
		
		if (indexBest == -1) {
			return -1;
		}
		
		for (i = 0; i < holes.size(); i++) {
			holeAux = holes.get(i);
			if (blockSize <= holeAux.getSize() && bestHole.getSize() >= holeAux.getSize()) {
				bestHole = holeAux;
				indexBest = i;
			}
		}

		if (indexBest == -1) {
			return -1;
		}
		
		int pointer = bestHole.getPointer();
		for (int j = pointer; j < (pointer + blockSize); j++) {
			cell[j] = pid;
		}

		if (blockSize == bestHole.getSize())
			holes.remove(bestHole);
		else
			bestHole.setPointer(pointer + blockSize);

		return pointer;
	}
}
