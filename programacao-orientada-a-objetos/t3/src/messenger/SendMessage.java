package messenger;

public class SendMessage implements Command {
	private Server server;
	String args[];
	private int id;
	
	public SendMessage(Server server, String args[], int id) {
		this.server = server;
		this.args = args;
		this.id = id;
	}
	
	public String execute() {
		try {
			return this.server.sendMessage(args[1], args[2], id);
		} catch (Exception e) {
			e.printStackTrace();
			return "";
		}
	}
}
