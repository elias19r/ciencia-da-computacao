package msgr.core;

public class Entity {
	protected int id;
	
	public Entity() { this.id = 0; }
	public Entity(int id) { this.setId(id); }
	public void setId(int id) { this.id = id; }
	public int getId() { return this.id; }
}
