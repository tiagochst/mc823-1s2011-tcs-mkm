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


    public boolean marcarCompromisso(Opr op) throws RemoteException{
	System.out.println("Agenda de usuário a ser criada:"+op.getLogin());	
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
    
    public boolean IsUsr(Opr op) throws RemoteException{
	File file = new File("users.dat");
	try{
	    
	    RandomAccessFile f = new RandomAccessFile("users.dat","r");
	    String usr,psw;
	    while((usr = f.readLine())!= null){
		psw = f.readLine();
		/*verifico usuario*/
		if(usr.equals(op.getLogin())){
		    /*verifico senha do usuario*/
		    System.out.println("Encontrei:\n");
		    System.out.println(op.getPassword());
		    if(psw.equals(op.getPassword()))
			return true;
		}
		System.out.println(usr);
		System.out.println(psw);
	    }
	
	} catch (Exception e) {
	    System.err.println("File exception: " + e.toString());
	    return false;
	}
	
	return false;
    }

    public boolean NewUsr(Opr op) throws RemoteException{
	File file = new File("users.dat");
	try{
	    
	    RandomAccessFile f = new RandomAccessFile("users.dat","rw");
	    String usr,psw;

	    /*Vou até o final do arquivo*/
	    f.seek(f.length());
	    f.writeBytes(op.getLogin());
	    f.writeBytes("\n");
	    f.writeBytes(op.getPassword());
	    f.writeBytes("\n");
	    f.close();	    
	} catch (Exception e) {
	    System.err.println("File exception: " + e.toString());
	    return false;
	}
	
	return true;
    }

    public boolean desmarcarCompromisso(Opr op) throws RemoteException{
		
	int i;
	File file = new File(op.getLogin() + ".dat");
		
	try {
	    //Verifica a existencia da agenda
	    if(!file.exists()){
		inicializaAgenda(file);
	    }
	    RandomAccessFile f = new RandomAccessFile(op.getLogin() + ".dat","rw");


	    /*Apaga o compromisso, cada dia tem um campo de 24 compromisso
	     cada um com 100 bytes, ou seja nossa agenda tem tamanho fixo 
	     dos compromissos */
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

    public String obterCompromissoHora(Opr op) throws RemoteException{
	File file = new File(op.getLogin() + ".dat");
		
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
    public String obterCompromissoDia(Opr op) throws RemoteException{
		
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
    public String obterCompromissoMes(Opr op) throws RemoteException{
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
