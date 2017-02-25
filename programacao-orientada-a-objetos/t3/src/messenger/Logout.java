package messenger;

public class Logout implements Command {
	private Server server;
	private int id;
	
	public Logout(Server server, int id) {
		this.server = server;
		this.id = id;
	}
	
	public String execute() {
		try {
			return this.server.logout(this.id);
		
		} catch (Exception e) {
			e.printStackTrace();
			return "";
		}
	}
}
