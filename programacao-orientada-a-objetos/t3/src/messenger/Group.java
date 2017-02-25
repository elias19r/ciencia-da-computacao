package messenger;

import java.util.*;

/**
 * Classe que representa um grupo no messenger
 */
public class Group extends Entity {
	private String name;
	private ArrayList<User> usersList;
	
	/**
	 * Construtor
	 */
	public Group(int id, String name) {
		super(id);
		this.setName(name);
		this.usersList = new ArrayList<User>();
	}

	/**
	 * Setters
	 */
	public void setName(String name) {
		this.name = name;
	}
	
	/**
	 * Getters
	 */
	public String getName() {
		return this.name;
	}
	
	/**
	 * Adiciona um usuario no grupo
	 */
	public void addUser(User user) {
		this.usersList.add(user);
	}
	
	/**
	 * Remove um usuario no grupo
	 */
	public void removeUser(User user) {
		this.usersList.remove(user);
	}
	
	/**
	 * Retorna a quantidade de usuarios no grupo
	 */
	public int size() {
		return this.usersList.size();
	}
}
