package br.usp.icmc.so.simulador;

import java.util.*;

public class Hole {
	private int pointer;
	private int size;

	public Hole() {
		this.pointer = -1;
		this.size = 0;
	}

	public Hole(int pointer, int size) {
		this.pointer = pointer;
		this.size = size;
	}

	public void setPointer(int pointer) {
		this.size += this.pointer - pointer;
		this.pointer = pointer;
	}

	public void setSize(int size) {
		this.size = size;
	}

	public int getPointer() {
		return this.pointer;
	}

	public int getSize() {
		return this.size;
	}
}
