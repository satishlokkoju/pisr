import java.io.*;

public  class CommandExecution {

public static String launchCE(String commandline){
	String output = "";
try {
	
 	String line;
 	Process p = Runtime.getRuntime().exec(commandline);
 	BufferedReader input =
 	new BufferedReader
 	(new InputStreamReader(p.getInputStream()));

 	while ((line = input.readLine()) != null) {
 		//System.out.println(line);
 		output += line;
 	}

 	input.close();
} catch (Exception err) {
 	err.printStackTrace();
}

	return output;
}

}