package messenger;

import java.net.*;

public class CreateUser extends Command {

	public CreateUser(Server server, Socket socket, String args[], int id) {
		super(server, socket, args, id);
	}
	
	public String execute() {
		if (this.args.length < 4)
			return "Error: incorrect params.\nUsage: createuser username password email";

		// ....
		

		return "";
	}
}
