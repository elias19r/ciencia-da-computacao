package br.usp.icmc.so.simulador;

import java.util.*;

/**
 * Classe Principal
 * Roda um caso de teste para os diferentes algoritmos de escalonamento
 */
public class Main {

	/**
	 * Recebe o tipo de algoritmo de escalonamento em args[0], sendo:
	 * 	0: FIFO
	 * 	1: Round-robin
	 * 	2: Loteria
	 * 	3: Prioridade
	 */
	public static void main(String[] args) {

		if (args.length < 1) System.exit(0);

		// Define o algoritmo a ser usado
		int algorithm = Integer.parseInt(args[0]);

		// Valores para o caso de teste
		int amount = 5;        // quantidade de processos
		int memSize = 100;     // tamanho da memoria principal
		int quantumTime = 4;   // tamanho do quantum em unidades de tempo
		int switchingTime = 1; // tempo para chaveamento de contexto em unidades de tempo

		int maxTime = 8; // tempo maximo requerido por um processo
		int maxMem = 10; // memoria maxima requerida por um processo

		int pid, requiredExecutionTime, requiredMemory, priority;
		Random random = new Random();
		Scheduler scheduler = null;

		System.out.println("\nPROCESSES\n---------");
		System.out.println("pid | requiredExecutionTime | requiredMemory | quantumTime | priority");
		System.out.println("----+-----------------------+----------------+-------------+---------");

		// De acordo com o algoritmo escolhido, cria processos e manda executa-los
		switch (algorithm) {

			// FIFO
			case 0:
				scheduler = new SchedulerFifo(new MainMemory(memSize), switchingTime);
		     		for (pid = 0; pid < amount; pid++) {
		     			requiredExecutionTime = random.nextInt(maxTime) + 1;
		     			requiredMemory = random.nextInt(maxMem) + 1;
		     			System.out.format("%-3d | %-3d                   | %-3d            | -           | -\n", pid, requiredExecutionTime, requiredMemory);
					scheduler.addProcess(new Process(pid, requiredExecutionTime, requiredMemory, 0, 0));
		     		}
				scheduler.execute();
				break;

			// Round-robin
			case 1:
				scheduler = new SchedulerRoundRobin(new MainMemory(memSize), switchingTime);
				for (pid = 0; pid < amount; pid++) {
		     			requiredExecutionTime = random.nextInt(maxTime) + 1;
		     			requiredMemory = random.nextInt(maxMem) + 1;
		     			System.out.format("%-3d | %-3d                   | %-3d            | %-3d         | -\n", pid, requiredExecutionTime, requiredMemory, quantumTime);
					scheduler.addProcess(new Process(pid, requiredExecutionTime, requiredMemory, quantumTime, 0));
				}
				scheduler.execute();
				break;

			// Lottery
			case 2:
				scheduler = new SchedulerLottery(new MainMemory(memSize), switchingTime);
		     		for (pid = 0; pid < amount; pid++) {
		     			requiredExecutionTime = random.nextInt(maxTime) + 1;
		     			requiredMemory = random.nextInt(maxMem) + 1;
		     			System.out.format("%-3d | %-3d                   | %-3d            | %-3d         | -\n", pid, requiredExecutionTime, requiredMemory, quantumTime);
					scheduler.addProcess(new Process(pid, requiredExecutionTime, requiredMemory, quantumTime, 0));
		     		}
				scheduler.execute();
				break;

			// Priority
			case 3:
				scheduler = new SchedulerPriority(new MainMemory(memSize), switchingTime);
		     		for (pid = 0; pid < amount; pid++) {
		     			requiredExecutionTime = random.nextInt(maxTime) + 1;
		     			requiredMemory = random.nextInt(maxMem) + 1;
		     			priority = random.nextInt(amount);
		     			System.out.format("%-3d | %-3d                   | %-3d            | %-3d         | %-3d\n", pid, requiredExecutionTime, requiredMemory, quantumTime, priority);
					scheduler.addProcess(new Process(pid, requiredExecutionTime, requiredMemory, quantumTime, priority));
		     		}
				scheduler.execute();
				break;

			default: break;
		}
	}
}
