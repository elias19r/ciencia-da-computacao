package br.usp.icmc.so.simulador;

import java.util.*;

/**
 * Escalonador que implementa o algoritmo de Filas de Prioridade
 */
public class SchedulerPriority extends Scheduler {

	/**
	 * Construtor
	 */
	public SchedulerPriority(MainMemory mem, float switchingTime) {
		super(mem, switchingTime);
	}

	/**
	 * Executa o algoritmo de escalonamento
	 */
	public void execute() {
		int amount = this.processes.size();
		float endings = 0;
		Process p;
		System.out.println("\nPRIORITY\n--------");

		Collections.sort(this.processes);
		
		while(!this.processes.isEmpty()) {
			ArrayList<Process> queue = new ArrayList<Process>();
			int priority = this.processes.get(0).getPriority();

			while(!this.processes.isEmpty() && this.processes.get(0).getPriority() == priority) 
				queue.add(this.processes.remove(0));

			/* Enquanto houver algum processo na fila:
			 * 	remove o primeiro processo,
			 * 	executa-o durante um quantum,
			 * 	decrementa seu tempo requerido de execucao,
			 * 	se ainda nao terminou: eh adicionado novamente na lista
			 * 	se ja terminou: liberado da memoria
			 */
			while (!queue.isEmpty()) {
				// Tempo atual
				System.out.println("Now is: " + this.getTime());

				// Coloca o processo do comeco da fila na CPU e executa
				System.out.println("Switching time: " + this.switchingTime);
				this.incrementTime(this.switchingTime);
				p = queue.remove(0);
				p.setRequiredExecutionTime(p.getRequiredExecutionTime() - p.getQuantumTime());

				// Se ainda nao terminou, volta o processo para o fim da fila (fazendo-a circular)
				if (p.getRequiredExecutionTime() > 0) {
					this.incrementTime(p.getQuantumTime());
					p.setTotalExecutionTime(this.getTime());
					queue.add(p);
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

		}

		// Imprime o turnaroundTime 
		System.out.println("TURNAROUND TIME\n----------\n" + (endings/amount) + "\n");

		// Imprime o throughputTime
		System.out.println("THROUGHPUT\n----------\n" + (amount/this.getTime()) + "\n");
		

	}
}

