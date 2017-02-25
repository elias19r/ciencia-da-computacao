package msgr.app;

import msgr.core.*;
import java.io.*;
import java.util.*;
import java.net.*;

public class Channel extends Thread {
	private int id;
	private Server server;
	private Socket socket;
	private ArrayList<Message> messageList;
	private Scanner reader;
	private PrintWriter writer;
	private String args[];
	
	public Channel(Server server) {
		this.id = 0;
		this.server = server;
		this.socket = null;
		this.messageList = new ArrayList<Message>();
	}
	
	public boolean busy() {
		if (this.socket != null)
			return true;
		
		return false;
	}
	
	public void free() {
		this.socket = null;
		this.id = 0;
		this.messageList.clear();
		this.reader = null;
		this.writer = null;
	}
	
	public void setSocket(Socket socket) throws Exception {
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
					this.push("");		
				else
					this.push("");
			}

		} catch (Exception e) {
			System.out.println("A client went off.");
		}
	}
	
}
