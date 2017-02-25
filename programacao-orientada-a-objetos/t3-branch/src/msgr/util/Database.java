package msgr.util;

import java.sql.*;

public class Database implements Config {
	private Connection conn;

	public Database() throws Exception {
		Class.forName(this.driver);
		this.conn = DriverManager.getConnection(this.url, this.username, this.password);
	}

	public boolean execute(String sql) throws Exception {
		Statement stmt = this.conn.createStatement();
		boolean ret = stmt.execute(sql);
		stmt.close();
		return ret;
	}

	public ResultSet query(String sql) throws Exception {
		Statement stmt = this.conn.createStatement();
		return stmt.executeQuery(sql);
	}

	public void close() throws Exception {
		this.conn.close();
	}
}
