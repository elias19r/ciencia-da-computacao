package br.usp.icmc.so.simulador;

import java.util.*;

public abstract class MemoryManager extends MainMemory {
	protected ArrayList<Hole> holes;

	/**
	 * Construtor
	 */
	public MemoryManager(int size, ArrayList<Process> processes) {
		super(size,processes);
		this.holes = new ArrayList<Hole>();
		this.findHoles();
	}

	/**
	 * Setters
	 */
	public void setHoles(ArrayList<Hole> holes) {
		this.holes = holes;
	}

	/**
	 * Getters
	 */
	public ArrayList<Hole> getHoles() {
		return this.holes;
	}

	// Metodo virtual que realiza a alocacao da memoria
	public abstract int allocMemory(int blockSize, int pid);
	
	// Metodo que encontra todos os holes na memoria e adiciona ao vetor de holes
	public void findHoles() {
		int i = 0;
		while (i < size) {
			if (cell[i] == 0) {
				int holeSize = 0; 
				Hole holeAux = new Hole();
				holeAux.setPointer(i);	
				while(i < size && cell[i] == 0) {
					holeSize++;	
					i++;
				}
				holeAux.setSize(holeSize);
				this.holes.add(holeAux);
			}
			i++;
		}
	}
}
