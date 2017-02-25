import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.util.TreeMap;

/**
 * @author hugo.gualdron
 * Modified by elias.rodrigues@usp.br
 */
public class brasil {
	public static void main(String args[]){
		try {
			if (args.length < 2) {
				System.out.println("Error: incorrect params.\nUsage: input_file output_dir");
				System.exit(0);
			}

			String input = args[0];
			String output = args[1];
			
			TreeMap<String, String> map = new TreeMap<String, String>();
			
			BufferedReader reader = new BufferedReader(new FileReader(input));
			String line;
			String[] sections;
			while (reader.ready()){
				line = reader.readLine();
				sections = line.split(":");
				map.put(sections[0], sections[1].substring(1, sections[1].length()-1)); 
			}
			reader.close();
			
			PrintWriter out = new PrintWriter(output + File.separator + "brasil_colored.svg");
			String state;
			String color;
			reader = new BufferedReader(new InputStreamReader(brasil.class.getResourceAsStream("brasil.svg")));
			while (reader.ready()){
				line = reader.readLine();
				if (line.contains("id=")){
					state = line.substring(line.indexOf("\"")+1,  line.lastIndexOf("\""));
					color = map.get(state);
					if (color != null){
						out.println(line);
						line = reader.readLine();
						line = line.replace("estateColor", color);
					}
				}
				out.println(line);
 
			}
			reader.close();
			out.close();
			
		} catch(Exception e){
			e.printStackTrace();
		}
	}
}
