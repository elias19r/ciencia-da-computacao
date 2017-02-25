package br.usp.icmc.so.simulador;

/**
 * Escalonador que implementa o algoritmo de FIFO (First In, First Out)
 */
public class SchedulerFifo extends Scheduler {

	/**
	 * Construtor
	 */
	public SchedulerFifo(MainMemory mem, float switchingTime) {
		super(mem, switchingTime);
	}

	/**
	 * Executa o algoritmo de escalonamento
	 */
	public void execute() {
		int amount = this.processes.size();
		float endings = 0;

		System.out.println("\nFIFO\n----");

		// Percorre a fila de processos executando do primeiro ao ultimo
		for (Process p : processes) {
			// Tempo atual
			System.out.println("Now is: " + this.getTime());

			// Coloca no processo na CPU e executa ate o seu fim
			System.out.println("Switching time: " + this.switchingTime);
			this.incrementTime(this.switchingTime);
			this.incrementTime(p.getRequiredExecutionTime());
			p.setTotalExecutionTime(this.getTime());

			// Libera o processo
			this.mem.freeMemory(p.getMemoryPointer(), p.getRequiredMemory());
			System.out.println("Process " + p.getPid() + " ended at " + p.getTotalExecutionTime() + "\n");
			endings += p.getTotalExecutionTime();
		}

		// Imprime o turnaroudTime 
		System.out.println("TURNAROUND TIME\n----------\n" + (endings/amount) + "\n");

		// Imprime o throughputTime
		System.out.println("THROUGHPUT\n----------\n" + (amount/this.getTime()) + "\n");
	}
}

