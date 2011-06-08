package client;
import java.io.File;
import java.io.FileWriter;
import java.io.PrintWriter;
import java.io.RandomAccessFile;


public class timeStamp {

	RandomAccessFile f;
	private long start,finish;
	
	public timeStamp(){
		try {
			start = System.nanoTime();
		} catch (Exception e) {
			System.err.println("File exception: " + e.toString());
		}
	}
	
	public void pararTempo(String nomeArq){
			finish = (System.nanoTime() - start) / ((long)1000);
        try {
            f = new RandomAccessFile(nomeArq,"rw");
            f.seek(f.length());
            f.writeBytes(finish + "\n");
            f.close();
        } catch (Exception e) {
        	System.err.println("File exception: " + e.toString());
        }
	}
}
