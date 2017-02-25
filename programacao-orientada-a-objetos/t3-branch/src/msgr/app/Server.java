package msgr.app;

import msgr.core.*;
import msgr.util.*;

import java.util.*;
import java.net.*;

public class Server {
	private int port;
	private int amount;
	private ArrayList<Channel> channelList;
	private ServerSocket socket;
	
	public Server(int port, int amount) {
		this.port = port;
		this.amount = amount;
		this.allocChannels();
	}

	public int getPort() { return this.port; }
	public int getAmount() { return this.amount; }

	public void createUser(String username, String password, String email, String name, String country) {
		User user = new User(username, password, email, name, country);
		user.insert();
	}

	public void authenticate() {
		
	}

	public void sendMessage() {
		
	}
	
	public void createGroup(int user_id, String name) {
		Group group = new Group(user_id, name);
		group.insert();
	}
	
	public void dropGroup(int id) {
		Group.delete(id);
	}
	
	public void addUserToGroup() {
		
	}
	
	public void removeUserFromGroup() {
		
	}
	
	public void transferFile() {
		
	}
	
	public void allocChannels() {
		this.channelList = new ArrayList<Channel>(this.amount);
		
		for (int i = 0; i < this.amount; i++)
			this.channelList.add(new Channel(this));
	}
	
	public void freeChannels() {
		this.channelList.clear();
		this.channelList = null;
		this.channelList = new ArrayList<Channel>();
	}

	public void execute() throws Exception {
		int i = 0;

		while (true) {
			if (!this.channelList.get(i).busy()) {
				this.channelList.get(i).setSocket(this.socket.accept());
				this.channelList.get(i).start();
			}
			if (++i >= this.amount) i = 0;
		}
	}

	public void up() throws Exception {
		this.socket = new ServerSocket(this.port);
	}

	public static void main(String args[]) {

		if (args.length < 2) {
			System.out.println("Error: incorrect params.\nUsage: port channels");
			System.exit(0);
		}

		Server server = new Server(Integer.parseInt(args[0]), Integer.parseInt(args[1]));
		
		try {
			server.up();
			System.out.println("Server is up and running.");
		} catch (Exception e) {
			System.out.println("Error: couldn't bring the server up.");
			e.printStackTrace();
		}
		
		try {
			server.execute();
		} catch (Exception e) {
			System.out.println("Error: couldn't execute server.");
			e.printStackTrace();
		}
	}
}
