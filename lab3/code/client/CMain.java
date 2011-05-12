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
	MC823Server stub = null;

	/* Recebo o nome/ip do servidor para a conexão */		
	String host = (args.length < 1) ? null : args[0];
	try {
	    Registry registry = LocateRegistry.getRegistry(host);
	    stub = (MC823Server) registry.lookup("MC823Server");
	} catch (Exception e) {
	    System.err.println("Client exception: " + e.toString());
	    e.printStackTrace();
	}

	/* Inicia com usuário não cadastrado*/	
	user.NonUserMenu();
	opSelect = 0;
	System.out.print("\n    Digite a opçao desejada: ");
	
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
	
	    switch(opSelect){
	    case '1':
		user.Login();
		break;
	    case '2':
		user.NewCal();
		break;
	    case '3':
		try{
		    leitor.close();
		} catch (Exception e){
		    System.err.println("Read exception: " + e.toString());
		}
		System.exit(0);
	    default:
	    }
	    if(true)
		break;
	}

	for(;;){
	    user.UserMenu();
	    
	    //Leitura da operacao com blindagem de erros
	    for(;;){
		opSelect = 0;
		System.out.print("\n    Digite a opçao desejada: ");
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
