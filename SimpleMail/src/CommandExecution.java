import java.io.*;

public  class CommandExecution {

public static String launchOpenCV(String filename){
	String result = "";
	String OPENCV_PATH = "../opencv2/bin/main ";
	result = launchCE(OPENCV_PATH + filename );
	return result;
}
	
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
 	p.waitFor();
 	input.close();
} catch (Exception err) {
 	err.printStackTrace();
}
	
	return output;
}

}