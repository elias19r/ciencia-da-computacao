package messenger;

/**
 * Classe que representa um entidade no sistema
 */
public class Entity {
	protected int id;
	
	/**
	 * Construtor
	 */
	public Entity(int id) {
		this.setId(id);
	}
	
	/**
	 * Setters
	 */
	public void setId(int id) {
		this.id = id;
	}
	
	/**
	 * Getters
	 */
	public int getId() {
		return this.id;
	}
}
