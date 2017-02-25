package messenger;

import java.io.*;
import java.util.*;
import java.net.*;
import java.sql.*;

public class Server {
	private int port;
	private String dbname;
	private String username;
	private String password;
	private Connection con;
	private ServerSocket socket;
	private ArrayList<User> usersList;
	
	public Server(int port, String dbname, String username, String password) {
		this.setPort(port);
		this.setDbname(dbname);
		this.setUsername(username);
		this.setPassword(password);
		this.usersList = new ArrayList<User>();
	}
	
	public void connectToDatabase() throws Exception {
		Class.forName("org.postgresql.Driver");
		this.con = DriverManager.getConnection("jdbc:postgresql://localhost/"+this.dbname, this.username, this.password);
	}
	
	public boolean isAuthenticated(int id) {
		int i, count = this.usersList.size();
		for (i = 0; i < count; i++)
			if (this.usersList.get(i).getId() == id)
				return true;
		
		return false;
	}
	
	public void closeDatabaseConnection() throws Exception {
		this.con.close();
	}
	
	public String logout(int id) {
		int i, count = this.usersList.size();
		
		for (i = 0; i < count; i++)
			if (this.usersList.get(i).getId() == id) {
				this.usersList.remove(i);
				break;
			}

		return "NOAUTH";
	}
	
	public String sendMessage(String to, String msg, int from) throws Exception {
		int i, count = this.usersList.size();
		for (i = 0; i < count; i++) {
			if (this.usersList.get(i).getUsername().equals(to)) {
				PrintWriter msgWriter = new PrintWriter(this.usersList.get(i).getSocket().getOutputStream());
				msgWriter.println("New message from "+from+":");
				msgWriter.println(msg+"\nEND");
				msgWriter.flush();
				return "";
			}
		}
		
		return "Error: user \""+to+"\" is not online or does not exist.";
	}
	
	public String authenticate(String usrname, String passwd, Socket sckt) throws Exception {
		String query = "SELECT * FROM \"users\" WHERE \"username\" = '"+usrname+"'";
		Statement stmt = this.con.createStatement();
		ResultSet rs = stmt.executeQuery(query);

		while (rs.next()) {
			int id = rs.getInt(1);
			String username = rs.getString(2);
			String password = rs.getString(3);
			String email = rs.getString(4);
			
			if (password.equals(passwd)) {
				this.usersList.add(new User(id, username, password, email, sckt));
				return "AUTH\n"+id+"\n"+username;
			}
		}

		return "Wrong username or password. Try again.";
	}

	public String createUser(String username, String password, String email) throws Exception {
		String query = "INSERT INTO \"users\" (\"username\", \"password\", \"email\") VALUES ('"+username+"', '"+password+"', '"+email+"')";
		Statement stmt = this.con.createStatement();
		stmt.executeQuery(query);
		stmt.close();
		return "";
	}	
	public void execute() throws Exception {
		while (true)
			(new Shell(this, this.socket.accept())).start();
	}

	public void setDbname(String dbname) {
		this.dbname = dbname;
	}

	public void setUsername(String username) {
		this.username = username;
	}

	public void setPassword(String password) {
		this.password = password;
	}

	public void setPort(int port) {
		this.port = port;
	}

	public String getDbname() {
		return this.dbname;
	}

	public String getUsername() {
		return this.username;
	}

	public int getPort() {
		return this.port;
	}
	
	public void up() throws Exception {
		this.socket = new ServerSocket(this.port);
	}

	public static void main(String args[]) {

		if (args.length < 4) {
			System.out.println("Error: incorrect params.\nUsage: port dbname username password");
			System.exit(0);
		}

		Server server = new Server(Integer.parseInt(args[0]), args[1], args[2], args[3]);
		
		try {
			System.out.println("Connecting to database...");
			server.connectToDatabase();
			System.out.println("Connected.");
		} catch (Exception e) {
			System.out.println("Error: couldn't connect to database.");
			System.out.println(e.getMessage());
		}
		
		try {
			server.up();
			System.out.println("Server is up and running.");
		} catch (Exception e) {
			System.out.println("Error: couldn't bring the server up.");
			System.out.println(e.getMessage());
		}
		
		try {
			server.execute();
		} catch (Exception e) {
			System.out.println("Error: couldn't execute server.");
			System.out.println(e.getMessage());
		}
	}
}
