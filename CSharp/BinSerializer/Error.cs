using System.Diagnostics;

public class Error : System.Exception {

	public Error(string msg) {
		Debug.Assert(false, msg);
	}

};