package br.usp.icmc.so.simulador;

import java.util.*;

/**
 * Escalonador que implementa o algoritmo de Loteria
 */
public class SchedulerLottery extends Scheduler {

	/**
	 * Construtor
	 */
	public SchedulerLottery (MainMemory mem, float switchingTime) {
		super(mem, switchingTime);
	}

	/**
	 * Executa o algoritmo de escalonamento
	 */
	public void execute() {
		int amount = this.processes.size();
		float endings = 0;
		Process p;
		Random random = new Random();

		System.out.println("\nLOTTERY\n-------");

		/* Enquanto houver algum processo na lista:
		 * 	remove um processo aleatorio,
		 * 	executa-o durante um quantum,
		 * 	decrementa seu tempo requerido de execucao,
		 * 	se ainda nao terminou: eh adicionado novamente na lista
		 * 	se ja terminou: liberado da memoria
		 */
		while (!this.processes.isEmpty()) {
			// Tempo atual
			System.out.println("Now is: " + this.getTime());

			// Coloca um processo aleatorio na CPU e executa
			System.out.println("Switching time: " + this.switchingTime);
			this.incrementTime(this.switchingTime);
			p = this.processes.remove(random.nextInt(this.processes.size()));
			p.setRequiredExecutionTime(p.getRequiredExecutionTime() - p.getQuantumTime());

			// Se ainda nao terminou, volta o processo para a fila
			if (p.getRequiredExecutionTime() > 0) {
				this.incrementTime(p.getQuantumTime());
				p.setTotalExecutionTime(this.getTime());
				this.processes.add(p);
				System.out.println("Process " + p.getPid() + " leaving at " + p.getTotalExecutionTime() + "\n");

			// Se ja terminou: libera da memoria
			} else {
				if (p.getRequiredExecutionTime() == 0)
					this.incrementTime(p.getQuantumTime());
				else
					this.incrementTime(p.getQuantumTime() + p.getRequiredExecutionTime());

				this.mem.freeMemory(p.getMemoryPointer(), p.getRequiredMemory());
				p.setTotalExecutionTime(this.getTime());
				System.out.println("Process " + p.getPid() + " ended at " + p.getTotalExecutionTime() + "\n");
				endings += p.getTotalExecutionTime();
			}
		}

		// Imprime o turnaroundTime 
		System.out.println("TURNAROUND TIME\n----------\n" + (endings/amount) + "\n");

		// Imprime o throughputTime
		System.out.println("THROUGHPUT\n----------\n" + (amount/this.getTime()) + "\n");
	}
}

