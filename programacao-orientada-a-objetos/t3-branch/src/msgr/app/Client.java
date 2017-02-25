package msgr.app;

import java.io.*;
import java.util.*;
import java.net.*;

public class Client {
	private String host;
	private int port;
	private int id;
	private String username;
	private Socket socket;

	private Scanner keyboard;
	private Scanner reader;
	private PrintWriter writer;
	private String message;
	
	public Client(String host, int port) throws Exception {
		this.id = 0;
		this.username = null;
		this.host = host;
		this.port = port;
		this.connectToServer();
	}
	
	public void setId(int id) { this.id = id; }
	public void setUsername(String username) { this.username = username; }
	
	public int getId() { return this.id; }
	public String getUsername() { return this.username; }

	public void connectToServer() throws Exception {
		this.socket = new Socket(this.host, this.port);
		this.keyboard = new Scanner(System.in);
		this.reader = new Scanner(this.socket.getInputStream());
		this.writer = new PrintWriter(this.socket.getOutputStream());
	}
	
	public void push(String message) {
		this.message = message;
		this.writer.println(this.getId()+" "+this.message);
		this.writer.flush();
	}
	
	public void pop() {
		String answer = this.reader.nextLine();
		
		if (answer.equals("AUTH")) {
			this.setId(Integer.parseInt(this.reader.nextLine()));
			this.setUsername(this.reader.nextLine());
			answer = this.reader.nextLine();
		}

		if (answer.equals("NOAUTH")) {
			this.setId(0);
			this.setUsername(null);
			answer = this.reader.nextLine();
		}
		
		while (!answer.equals("END")) {
			System.out.println(answer);
			answer = this.reader.nextLine();
		}
	}
	
	public void name() {
		if (this.getId() != 0)
			System.out.print("["+this.getUsername()+"@messenger] ");
		else
			System.out.print("[messenger] ");
	}
	
	public void execute() {
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
	
	public static void main(String args[]) {

		if (args.length < 2) {
			System.out.println("Error: incorrect params.\nUsage: host port");
			System.exit(0);
		}

		try {
			Client client = new Client(args[0], Integer.parseInt(args[1]));
			
			try {
				client.connectToServer();
			} catch (Exception e) {
				System.out.println("Error: couldn't connect to server.");
				e.printStackTrace();
			}
		
			try {
				client.execute();
			} catch (Exception e) {
				System.out.println("Error: couldn't execute client.");
				e.printStackTrace();
			}
			
		} catch (Exception e) {
			System.out.println("Error: couldn't create client.");
			e.printStackTrace();
		}
	}
}
