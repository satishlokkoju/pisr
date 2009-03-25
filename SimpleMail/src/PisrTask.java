
public class PisrTask {
	String file;
	String email;
	String result;
	
	public PisrTask(String pFile, String pEmail, String pResult){
		file = pFile;
		email = pEmail;
		result = pResult;
	}

	public String getFile() {
		return file;
	}

	public void setFile(String file) {
		this.file = file;
	}

	public String getEmail() {
		return email;
	}

	public void setEmail(String email) {
		this.email = email;
	}

	public String getResult() {
		return result;
	}

	public void setResult(String result) {
		this.result = result;
	}
}
