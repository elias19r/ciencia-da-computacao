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

	public Scheduler(MainMemory mem, float switchingTime, ArrayList<Process> processes) {
		this.processes = processes;
		this.mem = mem;
		this.switchingTime = switchingTime;
	}

	/**
	 * Adiciona um processo na lista de processos
	 */
	public int addProcess(Process p) {
		int pointer = mem.allocMemory(p.getRequiredMemory(), p.getPid());
		if(pointer == -1) {
			System.out.println("\nProcess cannot be allocated ... Error");
			return 1;
		}
		System.out.println("\nMEMORY\n------");
		mem.printMemory();
		p.setMemoryPointer(pointer);
		this.processes.add(p);

		return 0;
	}

	/**
	 * Executa o escalonar na lista de processos
	 */
	public void execute() { }
}

