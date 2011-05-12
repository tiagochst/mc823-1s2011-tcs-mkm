package client;
import java.rmi.registry.Registry;
import java.rmi.registry.LocateRegistry;
import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;
import java.io.File;
import java.io.FileWriter;
import java.io.PrintWriter;
import java.io.FileReader;
import java.io.RandomAccessFile;


public class Server implements MC823Server{

    private Opr op = new Opr();
	
    public Server(){}
	
    /**
     * @param args
     */
    public static void main(String[] args) {
		
	try {
	    Server obj = new Server();
	    MC823Server stub = (MC823Server) UnicastRemoteObject.exportObject(obj, 0);

	    // Bind the remote object's stub in the registry
	    Registry registry = LocateRegistry.getRegistry();
	    registry.bind("MC823Server", stub);

	    System.err.println("Server ready");
	} catch (Exception e) {
	    System.err.println("Server exception: " + e.toString());
	    e.printStackTrace();
	}

    }

    //Inicializa a agenda de um usuario caso ainda nao existir
    public void inicializaAgenda (File f) {
	int i;

	try {
	    FileWriter writer = new FileWriter(f);
	    PrintWriter saida = new PrintWriter(writer);

	    for (i=1;i<72000;i++) {
		if (i%100==0)
		    saida.println();
		else if(((i%100)-1) == 0)
		    saida.print('\0');
		else
		    saida.print(" ");
	    }

	    saida.close();
	    writer.close();

	} catch (Exception e) {
	    System.err.println("File exception: " + e.toString());
	}
    }


    public boolean marcarCompromisso(StructOperacao op) throws RemoteException{
	File file = new File(op.getLogin() + ".dat");
		
	try {
	    //Verifica a existencia da agenda
	    if(!file.exists()){
		inicializaAgenda(file);
	    }
	    RandomAccessFile f = new RandomAccessFile(op.getLogin() + ".dat","rw");
			
	    f.seek((op.getDia()-1)*2400+(op.getHora()*100));
	    f.writeBytes(op.getString());
	    f.close();
			
	    return true;

	} catch (Exception e) {
	    System.err.println("File exception: " + e.toString());
	    return false;
	}
		
    }
    public boolean desmarcarCompromisso(StructOperacao op) throws RemoteException{
		
	int i;
	File file = new File(op.getLogin() + ".txt");
		
	try {
	    //Verifica a existencia da agenda
	    if(!file.exists()){
		inicializaAgenda(file);
	    }
	    RandomAccessFile f = new RandomAccessFile(op.getLogin() + ".dat","rw");


	    //Apaga o compromisso
	    f.seek((op.getDia()-1)*2400+(op.getHora()*100));
	    f.writeByte('\0');

	    for (i=0;i<98;i++)
		f.writeByte(' ');
			
	    f.close();
			
	    return true;

	} catch (Exception e) {
	    System.err.println("File exception: " + e.toString());
			
	    return false;
	}
    }

    public String obterCompromissoHora(StructOperacao op) throws RemoteException{
	File file = new File(op.getLogin() + ".txt");
		
	try {
	    //Verifica a existencia da agenda
	    if(!file.exists()){
		inicializaAgenda(file);
	    }
	    RandomAccessFile f = new RandomAccessFile(op.getLogin() + ".dat","rw");
	    f.seek((op.getDia()-1)*2400+(op.getHora()*100));
	    op.setString(f.readLine());
			
	    //Retorna uma string com o compromisso
	    f.close();
			
	    return op.getString();

	} catch (Exception e) {
	    System.err.println("File exception: " + e.toString());
			
	    //retorna a string com o erro
	    return "File exception: " + e.toString();
	}


    }
    public String obterCompromissoDia(StructOperacao op) throws RemoteException{
		
	int i;
	File file = new File(op.getLogin() + ".dat");
		
	try {
	    if(!file.exists()){
		inicializaAgenda(file);
	    }
	    RandomAccessFile f = new RandomAccessFile(op.getLogin() + ".dat","rw");
	    f.seek((op.getDia()-1)*2400);
			
	    StringBuffer sb = new StringBuffer();
			
	    //Coleta os compromissos do dia
	    for (i=0;i<24;i++) {
		sb.append(f.readLine() + "\n");
	    }
			
	    op.setString(sb.toString());
			
	    f.close();

	    //Retorna uma string com todos os compromissos do dia
	    return op.getString();
			
	} catch (Exception e) {
	    System.err.println("File exception: " + e.toString());
			
	    //retorna a string com o erro
	    return "File exception: " + e.toString();
	}
		

    }
    public String obterCompromissoMes(StructOperacao op) throws RemoteException{
	int i;
	File file = new File(op.getLogin() + ".dat");
		
	try {
	    if(!file.exists()){
		inicializaAgenda(file);
	    }
	    RandomAccessFile f = new RandomAccessFile(op.getLogin() + ".dat","rw");
			
	    StringBuffer sb = new StringBuffer();
			
	    //Coleta todos os compromissos do mes deixando ja no formato apresentavel
	    for (i=0;i<720;i++) {
		f.seek(i*100);
		if(f.readByte() != '\0'){
		    f.seek(i*100);
		    sb.append("\nDia: " + (i/24)+1 + "\n " + i%24 + "h - " + f.readLine() + "\n");
		}
	    }
			
	    op.setString(sb.toString());
			
	    f.close();
			
	    //Retorna uma string com todos os compromissos do dia
	    return op.getString();
			
	} catch (Exception e) {
	    System.err.println("File exception: " + e.toString());
			
	    //retorna a string com o erro
	    return "File exception: " + e.toString();
	}
		
    }
	
}
