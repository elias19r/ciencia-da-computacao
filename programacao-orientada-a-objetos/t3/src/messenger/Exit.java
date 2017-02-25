package messenger;

import java.net.*;

public class Exit implements Command {
	private Socket socket;
	private int id;
	
	public Exit(Socket socket, int id) {
		this.socket = socket;
		this.id = id;
	}
	
	public String execute() {
		if (this.id != 0)
			return "You must logout first.";

		try {
			this.socket.close();
		} catch (Exception e) {
			e.printStackTrace();
		}

		return "";
	}
}
