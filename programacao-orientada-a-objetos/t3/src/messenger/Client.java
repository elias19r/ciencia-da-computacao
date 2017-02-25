package messenger;

import java.net.*;

/**
 * Classe que roda o aplicativo de cliente CLI
 */
public class Client {
	private int id;
	private String username;
	private String host;
	private int port;
	private Prompt prompt;
	public Socket socket;
	
	/**
	 * Construtor
	 */
	public Client(String host, int port) {
		this.id = 0;
		this.setHost(host);
		this.setPort(port);
	}
	
	/**
	 * Setters
	 */
	public void setId(int id) {
		this.id = id;
	}
	public void setUsername(String username) {
		this.username = username;
	}
	public void setHost(String host) {
		this.host = host;
	}
	public void setPort(int port) {
		this.port = port;
	}

	/**
	 * Getters
	 */	
	public int getId() {
		return this.id;
	}
	public String getUsername() {
		return this.username;
	}
	public String getHost() {
		return this.host;
	}
	public int getPort() {
		return this.port;
	}

	/**
	 * Executa o cliente
	 */	
	public void execute() throws Exception {
		this.prompt = new Prompt(this, this.socket.getInputStream(), this.socket.getOutputStream());
		this.prompt.run();
	}
	
	/**
	 * Abre uma conexao com o servidor
	 */
	public void connectToServer() throws Exception {
		this.socket = new Socket(this.host, this.port);		
	}
	
	/**
	 * Metodo principal
	 */
	public static void main(String args[]) {

		// Confere argumentos passados
		if (args.length < 2) {
			System.out.println("Error: incorrect params.\nUsage: host port");
			System.exit(0);
		}
		
		Client client = new Client(args[0], Integer.parseInt(args[1]));

		try {
			client.connectToServer();
		} catch (Exception e) {
			System.out.println("Error: couldn't connect to server.");
		}
		
		try {
			client.execute();
		} catch (Exception e) {
			System.out.println("Error: couldn't execute client.");
		}
	}
}
