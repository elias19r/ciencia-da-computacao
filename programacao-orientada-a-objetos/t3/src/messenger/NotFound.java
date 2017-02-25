package messenger;

public class NotFound implements Command {
	private int id;
	
	public NotFound(int id) {
		this.id = id;
	}
	
	public String execute() {
		if (this.id == 0)
			return "Command not found...\nWhy don't you try to authenticate yourself?";

		return "Command not found.";
	}
}
