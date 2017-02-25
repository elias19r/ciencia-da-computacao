package messenger;

import java.net.*;

public class User extends Entity {
	private String username;
	private String password;
	private String email;
	public Socket socket;
	
	public User(int id, String username, String password, String email, Socket socket) {
		super(id);
		this.setUsername(username);
		this.setPassword(password);
		this.setEmail(email);
		this.socket = socket;
	}

	public User(int id, String username, String password, String email) {
		super(id);
		this.setUsername(username);
		this.setPassword(password);
		this.setEmail(email);
		this.socket = null;
	}
	
	public void setUsername(String username) {
		this.username = username;
	}

	public void setPassword(String password) {
		this.password = password;
	}

	public void setEmail(String email) {
		this.email = email;
	}

	public void setSocket(Socket socket) {
		this.socket = socket;
	}
	
	public String getUsername() {
		return this.username;
	}

	public String getPassword() {
		return this.password;
	}

	public String getEmail() {
		return this.email;
	}
	
	public Socket getSocket() {
		return this.socket;
	}
}
