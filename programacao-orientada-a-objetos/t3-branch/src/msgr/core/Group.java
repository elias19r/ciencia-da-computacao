package msgr.core;

import msgr.util.*;
import java.util.*;
import java.sql.*;

public class Group extends Entity {
	private int user_id;
	private String name;
	private ArrayList<User> userList;
	
	public Group() {
		super();
		this.name = null;
		this.userList = new ArrayList<User>();
	}
	
	public Group(int user_id, String name) {
		super();
		this.setUserId(user_id);
		this.setName(name);
		this.userList = new ArrayList<User>();
	}
	
	public void setUserId(int user_id) { this.user_id = user_id; }
	public void setName(String name) { this.name = name; }

	public int getUserId() { return this.user_id; }
	public String getName() { return this.name; }
	
	public void fetch() throws Exception {
		Database db = new Database();
		ResultSet rs = db.query("SELECT * FROM \"groups_users\" WHERE \"group_id\" = '"+this.id+"'");

		while (rs.next())
			this.userList.add(User.findById(rs.getInt("user_id")));

		db.close();		
	}

	public void insert() throws Exception {
		Database db = new Database();
		db.execute("INSERT INTO \"groups\" (\"user_id\", \"name\") VALUES ('"+this.user_id+"', '"+this.name+"')");

		int i, count = this.userList.size();
		for (i = 0; i < count; i++)
			db.execute("INSERT INTO \"groups_users\" (\"group_id\", \"user_id\") VALUES ('"+this.id+"', '"+this.userList.get(i).getId()+"')");

		db.close();
	}

	public void delete() throws Exception {
		Database db = new Database();
		db.execute("DELETE FROM \"groups_users\" WHERE \"group_id\" = '"+this.id+"'");
		db.execute("DELETE FROM \"groups\" WHERE \"id\" = '"+this.id+"'");
		db.close();
	}

	public void update() throws Exception {
		Database db = new Database();
		db.execute("UPDATE \"groups\" SET \"name\" = '"+this.name+"' WHERE \"id\" = '"+this.id+"'");

		db.execute("DELETE FROM \"groups_users\" WHERE \"group_id\" = '"+this.id+"'");
		int i, count = this.userList.size();
		for (i = 0; i < count; i++)
			db.execute("INSERT INTO \"groups_users\" (\"group_id\", \"user_id\") VALUES ('"+this.id+"', '"+this.userList.get(i).getId()+"')");

		db.close();
	}

	public static ResultSet findAll() throws Exception {
		Database db = new Database();
		return db.query("SELECT * FROM \"groups\"");
	}

	public static Group findById(int id) throws Exception {
		Database db = new Database();
		ResultSet rs = db.query("SELECT * FROM \"groups\" WHERE \"id\" = '"+id+"'");
		Group group = Group.next(rs);
		db.close();

		return group;
	}

	public static Group next(ResultSet rs) throws Exception {
		Group group = null;

		if (rs.next())
			group = new Group(
				rs.getInt("id"),
				rs.getInt("user_id"),
				rs.getString("name")
			);

		return group;
	}
}
