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

		if (args.length < 2) System.exit(0);

		// Define o algoritmo de escalonamento a ser usado
		int algorithm = Integer.parseInt(args[0]);

		//Define o algoritmo de alocacao da memoria a ser usado
		int memoryAlgorithm = Integer.parseInt(args[1]);

		// Valores para o caso de teste
		int amountInitial = 5; //quantidade de processos iniciais
		int amount = 5;        // quantidade de processos
		int memSize = 100;     // tamanho da memoria principal
		int quantumTime = 4;   // tamanho do quantum em unidades de tempo
		int switchingTime = 1; // tempo para chaveamento de contexto em unidades de tempo

		int maxTime = 8; // tempo maximo requerido por um processo
		int maxMem = 10; // memoria maxima requerida por um processo

		int pid, requiredExecutionTime, requiredMemory, priority;
		Random random = new Random();
		Scheduler scheduler = null;
		MainMemory mainMemory = null;

		ArrayList<Process> initialProcesses = new ArrayList<Process>();

		System.out.println("\nINITIAL PROCESSES\n-----------------");
		System.out.println("pid | requiredExecutionTime | requiredMemory | quantumTime | priority");
		System.out.println("----+-----------------------+----------------+-------------+---------");

		for (pid = 1; pid <= amountInitial; ++pid) {
			requiredExecutionTime = random.nextInt(maxTime) + 1;
			requiredMemory = random.nextInt(maxMem) + 1;
			priority = random.nextInt(amount);
			System.out.format("%-3d | %-3d                   | %-3d            | %-3d         | %-3d\n", pid, requiredExecutionTime, requiredMemory, quantumTime, priority);
			initialProcesses.add(new Process(pid, requiredExecutionTime, requiredMemory, quantumTime, priority));
		}

		switch (memoryAlgorithm) {
			case 0:
				System.out.println("\nFIRST-FIT\n---------");
				mainMemory = new MemoryManagerFF(memSize, initialProcesses);
				break;
			case 1:
				System.out.println("\nNEXT-FIT\n--------");
				mainMemory = new MemoryManagerNF(memSize, initialProcesses);
				break;
			case 2:
				System.out.println("\nBEST-FIT\n--------");
				mainMemory = new MemoryManagerBF(memSize, initialProcesses);
				break;
			case 3:
				System.out.println("\nWORST-FIT\n---------");
				mainMemory = new MemoryManagerWF(memSize, initialProcesses);
				break;
		}

		System.out.println("\nMEMORY\n------");
		mainMemory.printMemory();

		int count = 0;

		// De acordo com o algoritmo escolhido, cria processos e manda executa-los
		switch (algorithm) {

			// FIFO
			case 0:
				scheduler = new SchedulerFifo(mainMemory, switchingTime, initialProcesses);
		     		for (pid = amountInitial+1; pid <= (amount+amountInitial); pid++) {
		     			requiredExecutionTime = random.nextInt(maxTime) + 1;
		     			requiredMemory = random.nextInt(maxMem) + 1;
					System.out.println("\nNEW PROCESS\n-----------");
					System.out.println("pid | requiredExecutionTime | requiredMemory | quantumTime | priority");
					System.out.println("----+-----------------------+----------------+-------------+---------");
		     			System.out.format("%-3d | %-3d                   | %-3d            | -           | -\n", pid, requiredExecutionTime, requiredMemory);
					count += scheduler.addProcess(new Process(pid, requiredExecutionTime, requiredMemory, 0, 0));
		     		}
				scheduler.execute();
				break;

			// Round-robin
			case 1:
				scheduler = new SchedulerRoundRobin(mainMemory, switchingTime, initialProcesses);
		     		for (pid = amountInitial+1; pid <= (amount+amountInitial); pid++) {
		     			requiredExecutionTime = random.nextInt(maxTime) + 1;
		     			requiredMemory = random.nextInt(maxMem) + 1;
					System.out.println("\nNEW PROCESS\n-----------");
					System.out.println("pid | requiredExecutionTime | requiredMemory | quantumTime | priority");
					System.out.println("----+-----------------------+----------------+-------------+---------");
		     			System.out.format("%-3d | %-3d                   | %-3d            | %-3d         | -\n", pid, requiredExecutionTime, requiredMemory, quantumTime);
					count += scheduler.addProcess(new Process(pid, requiredExecutionTime, requiredMemory, quantumTime, 0));
				}
				scheduler.execute();
				break;

			// Lottery
			case 2:
				scheduler = new SchedulerLottery(mainMemory,switchingTime, initialProcesses);
		     		for (pid = amountInitial+1; pid <= (amount+amountInitial); pid++) {
		     			requiredExecutionTime = random.nextInt(maxTime) + 1;
		     			requiredMemory = random.nextInt(maxMem) + 1;
					System.out.println("\nNEW PROCESS\n-----------");
					System.out.println("pid | requiredExecutionTime | requiredMemory | quantumTime | priority");
					System.out.println("----+-----------------------+----------------+-------------+---------");
		     			System.out.format("%-3d | %-3d                   | %-3d            | %-3d         | -\n", pid, requiredExecutionTime, requiredMemory, quantumTime);
					count += scheduler.addProcess(new Process(pid, requiredExecutionTime, requiredMemory, quantumTime, 0));
		     		}
				scheduler.execute();
				break;

			// Priority
			case 3:
				scheduler = new SchedulerPriority(mainMemory, switchingTime, initialProcesses);
		     		for (pid = amountInitial+1; pid <= (amount+amountInitial); pid++) {
		     			requiredExecutionTime = random.nextInt(maxTime) + 1;
		     			requiredMemory = random.nextInt(maxMem) + 1;
		     			priority = random.nextInt(amount);
					System.out.println("\nNEW PROCESS\n-----------");
					System.out.println("pid | requiredExecutionTime | requiredMemory | quantumTime | priority");
					System.out.println("----+-----------------------+----------------+-------------+---------");
		     			System.out.format("%-3d | %-3d                   | %-3d            | %-3d         | %-3d\n", pid, requiredExecutionTime, requiredMemory, quantumTime, priority);
					count += scheduler.addProcess(new Process(pid, requiredExecutionTime, requiredMemory, quantumTime, priority));
		     		}
				scheduler.execute();
				break;

			default: break;
		}

		System.out.println(count + " Processes couldn't allocated");
	}
}
