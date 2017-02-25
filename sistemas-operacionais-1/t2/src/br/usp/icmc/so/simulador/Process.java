package br.usp.icmc.so.simulador;

/**
 * Classe que representa um processo
 */
public class Process implements Comparable {

	private int pid;                     // id do processo
	private float quantumTime;           // quantidade de tempo que o processo executa ate escalonar
	private float requiredExecutionTime; // tempo que o processo precisa para ser executado
	private float totalExecutionTime;    // tempo em que o processo terminou
	private int requiredMemory;          // quantidade de memoria necessaria para alocar o processo
	private int memoryPointer;           // ponteiro para a regiao de memoria do comeco do processo
	private int priority;                // prioridade do processo

	/**
	 * Construtor
	 */
	public Process(int pid, float requiredExecutionTime, int requiredMemory, float quantumTime, int priority){
		this.pid = pid;
		this.requiredExecutionTime = requiredExecutionTime;
		this.requiredMemory = requiredMemory;
		this.quantumTime = quantumTime;
		this.priority = priority;

		this.totalExecutionTime = 0;
		this.memoryPointer = -1;
	}

	/**
	 * Getters
	 */

	public int getPid() {
		return this.pid;
	}

	public float getQuantumTime() {
		return this.quantumTime;
	}

	public float getRequiredExecutionTime() {
		return this.requiredExecutionTime;
	}

	public float getTotalExecutionTime() {
		return this.totalExecutionTime;
	}

	public int getRequiredMemory() {
		return this.requiredMemory;
	}

	public int getMemoryPointer() {
		return this.memoryPointer;
	}

	public int getPriority() {
		return this.priority;
	}

	/**
	 * Setters
	 */

	public void setTotalExecutionTime(float totalExecutionTime) {
		this.totalExecutionTime = totalExecutionTime;
	}

	public void setRequiredExecutionTime(float requiredExecutionTime) {
		this.requiredExecutionTime = requiredExecutionTime;
	}

	public void setRequiredMemory(int requiredMemory) {
		this.requiredMemory = requiredMemory;
	}

	public void setMemoryPointer(int memoryPointer) {
		this.memoryPointer = memoryPointer;
	}

	public int compareTo(Object p) {
		return ((Process)p).getPriority() - this.priority;
	}
}
