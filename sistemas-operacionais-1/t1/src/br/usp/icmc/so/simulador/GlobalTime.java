package br.usp.icmc.so.simulador;

/**
 * Classe que simula o tempo global da execucao
 */
public class GlobalTime {
	private static float time = 0; // tempo global

	/**
	 * Retorna o tempo atual
	 */
	protected static float getTime() {
		return GlobalTime.time;
	}

	/**
	 * Incrementa o tempo em time unidade(s)
	 */
	protected static void incrementTime(float time) {
		GlobalTime.time += time;
	}

	/**
	 * Incrementa o tempo em 1 unidade
	 */
	protected static void incTime() {
		GlobalTime.time += 1.0;
	}
}

