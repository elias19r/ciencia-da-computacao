package msgr.core;

import msgr.util.*;
import java.sql.*;

public class User extends Entity {
	private String username;
	private String password;
	private String email;
	private String name;
	private String country;
	
	public User() {
		super();
		this.username = null;
		this.password = null;
		this.email = null;
		this.name = null;
		this.country = null;
	}
	
	public User(String username, String password, String email, String name, String country) {
		super();
		this.setUsername(username);
		this.setPassword(password);
		this.setEmail(email);
		this.setName(name);
		this.setCountry(country);
	}

	public User(int id, String username, String password, String email, String name, String country) {
		super(id);
		this.setUsername(username);
		this.setPassword(password);
		this.setEmail(email);
		this.setName(name);
		this.setCountry(country);
	}
	
	public void setUsername(String username) { this.username = username; }
	public void setPassword(String password) { this.password = password; }
	public void setEmail(String email) { this.email = email; }
	public void setName(String name) { this.name = name; }
	public void setCountry(String country) { this.country = country; }

	public String getUsername() { return this.username; }
	public String getPassword() { return this.password; }
	public String getEmail() { return this.email; }
	public String getName() { return this.name; }
	public String getCountry() { return this.country; }
	
	public void insert() throws Exception {
		Database db = new Database();
		db.execute("INSERT INTO \"users\" (\"username\", \"password\", \"email\", \"name\", \"country\") VALUES ('"+this.username+"', '"+this.password+"', '"+this.email+"', '"+this.name+"', '"+this.country+"')");
		db.close();
	}

	public void delete() throws Exception {
		Database db = new Database();
		db.execute("DELETE FROM \"users\" WHERE \"id\" = '"+this.id+"'");
		db.close();
	}

	public void update() throws Exception {
		Database db = new Database();
		db.execute("UPDATE \"users\" SET \"password\" = '"+this.password+"', \"email\" = '"+this.email+"', \"name\" = '"+this.name+"', \"country\" = '"+this.country+"' WHERE \"id\" = '"+this.id+"'");
		db.close();
	}

	public static ResultSet findAll() throws Exception {
		Database db = new Database();
		return db.query("SELECT * FROM \"users\"");
	}

	public static User findById(int id) throws Exception {
		Database db = new Database();
		ResultSet rs = db.query("SELECT * FROM \"users\" WHERE \"id\" = '"+id+"'");
		User user = User.next(rs);
		db.close();

		return user;
	}

	public static User next(ResultSet rs) throws Exception {
		User user = null;

		if (rs.next())
			user = new User(
				rs.getInt("id"),
				rs.getString("username"),
				rs.getString("password"),
				rs.getString("email"),
				rs.getString("name"),
				rs.getString("country")
			);

		return user;
	}
}
