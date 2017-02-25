package messenger;

import java.net.*;

public class Logout extends Command {
	private Server server;
	
	
	public Logout(Server server, int id) {
		this.server = server
		super(server, socket, args, id);
	}
	
	public String execute() {
		return "You've logged out.";
	}
}
