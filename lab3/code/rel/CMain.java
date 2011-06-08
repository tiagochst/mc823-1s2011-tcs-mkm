package client;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;

public class CMain {
	
    private CMain(){}
	
    /**
     * @param args
     */
    public static void main(String[] args) {

	Client user = new Client();
	BufferedReader leitor = new BufferedReader(new InputStreamReader(System.in));
	int opSelect;
	boolean done = false ;
	MC823Server stub = null;

	/* Recebo o nome/ip do servidor para a conexao */		
	String host = (args.length < 1) ? null : args[0];
	try {
	    /*Procura pelo registro usado pelo host,
	      o registro e usado para referenciar um objeto remoto */
	    Registry registry = LocateRegistry.getRegistry(host);
	    /* Cria o stub para processos distribuidos:
	      toda comunicacao passa por ele. 
	      Cria a abstracao da comunicao */
	    stub = (MC823Server) registry.lookup("MC823Server");
	} catch (Exception e) {
	    System.err.println("Client exception: " + e.toString());
	    e.printStackTrace();
	}

	
	for(;;){
	    /* Limpando a tela*/
	    //System.out.println((char) 27+ "[2J");
	    /* Inicia com usuario nao cadastrado*/	
	    user.NonUserMenu();
	    opSelect = 0;
	    System.out.print("\n Digite a opcao desejada: ");
	    
	    for(;;){
		try {
		    String inBuffer = leitor.readLine();
		    if(inBuffer.length() == 1){
			opSelect = inBuffer.charAt(0);
		    }
		} catch (Exception e) {
		System.err.println("Read exception: " + e.toString());
		}
		
		if ((opSelect < '1') || (opSelect > '3'))
		    System.out.println("\n    Opcao invalida, digite a opcao novamente.");
		else
		    break;
	    }
	    switch(opSelect){
	    case '1':
		done = user.Login(stub);
		break;
	    case '2':
		done = user.NewCal(stub);
		break;
	    case '3':
		try{
		    leitor.close();
		} catch (Exception e){
		    System.err.println("Read exception: " + e.toString());
		}
		System.exit(0);
		done = true;
	    default:
		done = false;
	    }
	    if(done == true)
		break;
	}

	for(;;){
	    /* Usuario logado, fornecer menu agenda */
	    user.UserMenu();
	    
	    for(;;){
		opSelect = 0;
		System.out.print("\n    Digite a opcao desejada: ");
		try {
		    String inBuffer = leitor.readLine();
		    if(inBuffer.length() == 1){
			opSelect = inBuffer.charAt(0);
		    }
		} catch (Exception e) {
		    System.err.println("Read exception: " + e.toString());
		}
		
		if ((opSelect < '1') || (opSelect > '6'))
		    System.out.println("\n    Opcao invalida, digite a opcao novamente.");
		else
		    break;
		
	    }
	    
	    switch(opSelect){
	    case '1':
		user.NewComp(stub);
		break;
	    case '2':
		user.DelComp(stub);
		break;
	    case '3':
		user.ShowHour(stub);
		break;
	    case '4':
		user.ShowDay(stub);
		break;
	    case '5':
		user.ShowMonth(stub);
		break;
	    case '6':
		try{
		    leitor.close();
		} catch (Exception e){
		    System.err.println("Read exception: " + e.toString());
		}
		System.exit(0);
	    default:
	    }
	}
	
    }
}
