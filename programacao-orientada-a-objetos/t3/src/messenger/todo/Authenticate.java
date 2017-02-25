package messenger;

import java.net.*;

public class Authenticate extends Command {
	protected Server server;
	protected Socket socket;
	protected String args[];
	protected int id;
	
	public Authenticate(Server server, Socket socket, String args[], int id) {
		this.server = server;
		this.socket = socket;
		this.args = args;
		this.id = id;
	}

	public String execute() {
		if (args.length < 3)
			return "Error: incorrect params.\nUsage: authenticate username password";

		return "";
	}
}
