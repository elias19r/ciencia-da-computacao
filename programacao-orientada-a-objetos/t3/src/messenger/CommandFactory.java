package messenger;

import java.net.*;

public class CommandFactory {

	public static Command build(Shell shell) {
		String cmd = shell.args[0];

		if (cmd.equals("createuser") || cmd.equals("newuser"))
			return new CreateUser(shell.server, shell.args, shell.id);

		if (cmd.equals("authenticate") || cmd.equals("login"))
			return new Authenticate(shell.server, shell.socket, shell.args, shell.id);

		if (cmd.equals("logout"))
			return new Logout(shell.server, shell.id);

		if (cmd.equals("sendmessage") || cmd.equals("send"))
			return new SendMessage(shell.server, shell.args, shell.id);

		if (cmd.equals("exit"))
			return new Exit(shell.socket, shell.id);
		
		return new NotFound(shell.id);
	}
}
