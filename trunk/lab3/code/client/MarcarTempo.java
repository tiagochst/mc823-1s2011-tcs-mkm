package client;
import java.io.File;
import java.io.FileWriter;
import java.io.PrintWriter;


public class MarcarTempo {

	private File f;
	private long start,finish;
	private FileWriter writer;
	private PrintWriter saida;
	
	public MarcarTempo(String nomeArq){
		try {
			f = new File(nomeArq + ".txt");
			writer = new FileWriter(f,true);
			saida = new PrintWriter(writer);
			start = System.nanoTime();
		} catch (Exception e) {
			System.err.println("File exception: " + e.toString());
		}
	}
	
	public void pararTempo(){
			finish = (System.nanoTime() - start) / ((long)1000);
        
        try {
        	saida.print(finish);
        	saida.print('\n');
        	saida.close();
        } catch (Exception e) {
        	System.err.println("File exception: " + e.toString());
        }
	}
}
