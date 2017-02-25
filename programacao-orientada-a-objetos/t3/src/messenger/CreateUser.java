package messenger;

import java.net.*;

public class CreateUser implements Command {
	private Server server;
	private String args[];
	private int id;
	
	public CreateUser(Server server, String args[], int id) {
		this.server = server;
		this.args = args;
		this.id = id;
	}
	
	public String execute() {
		if (args.length < 4)
			return "Error: incorrect params.\nUsage: "+this.args[0]+" username password email";

		try {
			this.server.createUser(this.args[1], this.args[2], this.args[3]);
		} catch (Exception e) {
			e.printStackTrace();
			return "";
		}

		return "";
	}
}
