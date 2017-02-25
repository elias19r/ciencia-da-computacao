package msgr.core;

public class Message {
	private int from;
	private int to;
	private String content;
	
	public Message(int from, int to, String content) {
		this.from = from;
		this.to = to;
		this.content = content;
	}
	
	public int getFrom() { return this.from; }
	public int getTo() { return this.to; }
	public String getContent() { return this.content; }
	
}
