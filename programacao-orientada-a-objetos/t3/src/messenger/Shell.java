package messenger;

import java.util.*;
import java.io.*;
import java.net.*;

public class Shell extends Thread {
	protected Server server;
	protected Socket socket;
	protected Scanner reader;
	protected PrintWriter writer;
	protected String args[];
	protected int id;

	public Shell(Server server, Socket socket) throws Exception {
		this.server = server;
		this.socket = socket;
		this.reader = new Scanner(this.socket.getInputStream());
		this.writer = new PrintWriter(this.socket.getOutputStream());
	}

	public void push(String message) {
		if (!message.isEmpty())
			this.writer.println(message);
	
		this.writer.println("END");
		this.writer.flush();
	}
	
	public void pop() {
		String params[] = (this.reader.nextLine()).split(" ", 2);
		
		this.id = Integer.parseInt(params[0]);
		this.args = new String[0];
		if (!params[1].isEmpty())
			this.args = params[1].split(" ");
	}
	
	public void run() {
		System.out.println("A client has just connected.");
		this.push("\nWelcome!\nPlease, authenticate yourself.\n");
		
		try {
			while (true) {
				this.pop();
				if (this.args.length > 0)
					this.push((CommandFactory.build(this)).execute());		
				else
					this.push("");
			}

		} catch (Exception e) {
			System.out.println("A client went off.");
		}
	}
}

