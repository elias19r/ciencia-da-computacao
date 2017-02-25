package br.usp.icmc.so.simulador;

import java.util.*;

/**
 * Classe do escalonador de processos
 */
public class Scheduler extends GlobalTime {

	protected ArrayList<Process> processes; // lista de processos
	protected MainMemory mem;               // memoria principal
	protected float switchingTime;          // tempo de chaveamento de contexto

	/**
	 * Construtor
	 */
	public Scheduler(MainMemory mem, float switchingTime) {
		this.processes = new ArrayList<Process>();
		this.mem = mem;
		this.switchingTime = switchingTime;
	}

	/**
	 * Adiciona um processo na lista de processos
	 */
	public void addProcess(Process p) {
		int pointer = mem.allocMemory(p.getRequiredMemory());
		p.setMemoryPointer(pointer);
		this.processes.add(p);
	}

	/**
	 * Executa o escalonar na lista de processos
	 */
	public void execute() { }
}

