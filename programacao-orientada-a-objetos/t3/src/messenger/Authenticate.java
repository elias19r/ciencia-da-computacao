package messenger;

import java.net.*;

public class Authenticate implements Command {
	private Server server;
	private Socket socket;
	private String args[];
	private int id;
	
	public Authenticate(Server server, Socket socket, String args[], int id) {
		this.server = server;
		this.socket = socket;
		this.args = args;
		this.id = id;
	}
	
	public String execute() {
		if (args.length < 3)
			return "Error: incorrect params.\nUsage: "+this.args[0]+" username password";
		try {
			return this.server.authenticate(this.args[1], this.args[2], socket);
		
		} catch (Exception e) {
			e.printStackTrace();
			return "";
		}
	}
}
