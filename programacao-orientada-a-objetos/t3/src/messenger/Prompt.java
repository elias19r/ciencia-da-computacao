package messenger;

import java.util.*;
import java.io.*;

public class Prompt {
	private Client client;
	private Scanner keyboard;
	private Scanner reader;
	private PrintWriter writer;
	private String message;

	public Prompt(Client client, InputStream in, OutputStream out) {
		this.client = client;
		this.keyboard = new Scanner(System.in);
		this.reader = new Scanner(in);
		this.writer = new PrintWriter(out);
	}
	
	public void push(String message) {
		this.message = message;
		this.writer.println(this.client.getId()+" "+this.message);
		this.writer.flush();
	}
	
	public void pop() {
		String answer = this.reader.nextLine();
		
		if (answer.equals("AUTH")) {
			this.client.setId(Integer.parseInt(this.reader.nextLine()));
			this.client.setUsername(this.reader.nextLine());
			answer = this.reader.nextLine();
		}

		if (answer.equals("NOAUTH")) {
			this.client.setId(0);
			this.client.setUsername(null);
			answer = this.reader.nextLine();
		}
		
		while (!answer.equals("END")) {
			System.out.println(answer);
			answer = this.reader.nextLine();
		}
	}
	
	public void name() {
		if (this.client.getId() != 0)
			System.out.print(this.client.getUsername()+"@messenger] ");
		else
			System.out.print("messenger] ");
	}
	
	/**
	 * Roda o prompt
	 */
	public void run() {
		try {
			while (true) {
				this.pop();
				this.name();
				this.push(this.keyboard.nextLine());
			}
		} catch (Exception e) {
			System.out.println("\nSee you!\n");
		}
	}
}
