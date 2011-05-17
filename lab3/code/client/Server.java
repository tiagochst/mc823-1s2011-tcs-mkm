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

    public boolean marcarCompromisso(Opr op) throws RemoteException{

	RandomAccessFile f;
	
	try {
	    //Verifica a existencia da agenda
	    f = new RandomAccessFile(op.getLogin() + ".dat","rw");
	    
	    /*Vou até o final do arquivo*/
	    f.seek(f.length());
	    /*Insere nome compromisso*/
	    f.writeBytes(op.getString());
	    f.writeBytes("\n");
	    /*Insere dia compromisso*/
	    f.writeBytes(Integer.toString(op.getDia()));
	    f.writeBytes("\n");
	    /*Insere hora compromisso*/
	    f.writeBytes(Integer.toString(op.getHora()));
	    f.writeBytes("\n");
	    /*Insere minuto compromisso*/
	    f.writeBytes(Integer.toString(op.getMinuto()));
	    f.writeBytes("\n");

	    f.close();	    
		
	    return true;

	} catch (Exception e) {
	    System.err.println("File exception: " + e.toString());
	    return false;
	}
		
    }
    
    public boolean IsUsr(Opr op) throws RemoteException{
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
	RandomAccessFile f;
	  
	try{
	    
	    f = new RandomAccessFile("users.dat","rw");
	    String usr,psw;

	    /*Vou até o final do arquivo*/
	    f.seek(f.length());
	    f.writeBytes(op.getLogin());
	    f.writeBytes("\n");
	    f.writeBytes(op.getPassword());
	    f.writeBytes("\n");
	    f.close();	    

	    /*Devo criar agenda para o usuário*/
	    File file = new File(op.getLogin()+".dat");
	    f = new RandomAccessFile(op.getLogin()+".dat","rw");
	    f.close();	    

	} catch (Exception e) {
	    System.err.println("File exception: " + e.toString());
	    return false;
	}
	
	return true;
    }

    public boolean desmarcarCompromisso(Opr op) throws RemoteException{

	RandomAccessFile f; /*Arquivo*/
	boolean found = false;
		
	/*Lista de compromissos não apagados*/
	StringBuffer sb = new StringBuffer();
		
	try {
	    f = new RandomAccessFile(op.getLogin() + ".dat","rw");

	    /*Procura compromisso pelo nome*/
	    String name,dia,hora,minuto;
	    while((name = f.readLine())!= null){
		/*Ignoro dia hora minuto*/
		dia = f.readLine();
		hora = f.readLine();
		minuto = f.readLine();

		System.out.println("\nEstou procurando por:"+op.getString());
		
		/*verifico se nome procurado não é o a ser apagado*/
		if(!(name.equals(op.getString()))){
		    System.out.println("\nNao eh");

		    /*Vou manter compromisso na agenda*/
		    sb.append(name+"\n");
		    sb.append(dia+"\n"+hora+"\n"+minuto+"\n");
		    
		    /*TO BE DONE*/
		    found = true;
		}
	    }
	    
	    f.close();
	    /*se achei tarefa reescrevo na agenda*/
	    if(found){
		op.setString(sb.toString());

		File trash = new File(op.getLogin() + ".dat");
		trash.delete();
		f = new RandomAccessFile(op.getLogin() + ".dat","rw");
		f.writeBytes(op.getString());
		f.close();
	  
	    }
			
	    return found;

	} catch (Exception e) {
	    System.err.println("File exception: " + e.toString());
			
	    return found;
	}
    }

    public String obterCompromissoHora(Opr op) throws RemoteException{

	/*Lista de compromissos*/
	StringBuffer sb = new StringBuffer();
	op.setString("Nenhum compromisso nesse dia e horario");
	
	try {
	    RandomAccessFile f = new RandomAccessFile(op.getLogin() + ".dat","rw");
	    
	    /*Procura compromisso pelo nome*/
	    String name,dia,hora,minuto;
	    while((name = f.readLine())!= null){
		/*Ignoro dia hora minuto*/
		dia = f.readLine();
		hora = f.readLine();
		minuto = f.readLine();
		
		/*verifico se nome procurado é o mesmo*/
		if(dia.equals(Integer.toString(op.getDia()))){
		    if(hora.equals(Integer.toString(op.getHora()))){

			/*Preciso retornar lista de compromissos*/
			sb.append("-----------\nNome:"+name);
			sb.append("\nDia: "+dia+"\nHora: "+hora+"\nMinuto: "+minuto + "\n");
			
			op.setString(sb.toString());

		    }
		}
	    }
	    f.close();
	    
	    return op.getString();
	    
	} catch (Exception e) {
	    System.err.println("File exception: " + e.toString());
	    
	    //retorna a string com o erro
	    return "File exception: " + e.toString();
	}
	
    }

    public String obterCompromissoDia(Opr op) throws RemoteException{

	/*Lista de compromissos*/
	StringBuffer sb = new StringBuffer();
	op.setString("Nenhum Compromisso nesse dia");
	
	try {
	    RandomAccessFile f = new RandomAccessFile(op.getLogin() + ".dat","rw");
	    
	    /*Procura compromisso pelo nome*/
	    String name,dia,hora,minuto;
	    while((name = f.readLine())!= null){
		/*Ignoro dia hora minuto*/
		dia = f.readLine();
		hora = f.readLine();
		minuto = f.readLine();
		
		/*verifico se nome procurado é o mesmo*/
		if(dia.equals(Integer.toString(op.getDia()))){

		    /*Preciso retornar lista de compromissos*/
		    sb.append("-----------\nNome:"+name);
		    sb.append("\nDia: "+dia+"\nHora: "+hora+"\nMinuto: "+minuto + "\n");

		    op.setString(sb.toString());
		}
	    }
	    f.close();
	    
	    return op.getString();
	    
	} catch (Exception e) {
	    System.err.println("File exception: " + e.toString());
	    
	    //retorna a string com o erro
	    return "File exception: " + e.toString();
	}
	
    }

    public String obterCompromissoMes(Opr op) throws RemoteException{

	/*Lista de compromissos*/
	StringBuffer sb = new StringBuffer();
	op.setString("Nenhum Compromisso no mes");
	
	try {
	    RandomAccessFile f = new RandomAccessFile(op.getLogin() + ".dat","rw");
	    
	    /*Procura compromisso pelo nome*/
	    String name,dia,hora,minuto;
	    while((name = f.readLine())!= null){
		/*Ignoro dia hora minuto*/
		dia = f.readLine();
		hora = f.readLine();
		minuto = f.readLine();
		
		/*Preciso retornar lista de compromissos*/
		sb.append("-----------\nNome:"+name);
		sb.append("\nDia: "+dia+"\nHora: "+hora+"\nMinuto: "+minuto + "\n");

		op.setString(sb.toString());
		
	    }
	    f.close();
	    
	    return op.getString();
	    
	} catch (Exception e) {
	    System.err.println("File exception: " + e.toString());
	    
	    //retorna a string com o erro
	    return "File exception: " + e.toString();
	}
	
    }
    
}
