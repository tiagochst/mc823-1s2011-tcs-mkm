package client;
import java.io.BufferedReader;
import java.io.InputStreamReader;

public class Client {

    private Opr op;
    private BufferedReader leitor;

    /*___________________________________________________________
      |Construtor do usuário -> Representa um objeto usuário     |
      | Conteúdo: uma estrutura para  comunicação com o servidor |
      | e string de leitura                                      |
      |__________________________________________________________|*/
    public Client() {
	this.op = new Opr();
	this.leitor = new BufferedReader(new InputStreamReader(System.in));
    }

    /*___________________________________________________________
      |Operações possíveis para o usuário                        |
      | As funções são:                                          |
      | Login: Usuário fornece nome se senha que serão comparadas|
      |        com banco de dados no servidor                    |
      | UserMenu: Para um usuário já logado apresenta funções de |
      |           manipulação na agenda                          |
      | NonUserMenu: Menu inicial para Usuário não logado        |
      | NewComp: Insere novo compromisso na agenda               |
      | DelComp: Desmarca um compromisso da agenda               |
      | ShowHour: Mostra compromissos para determinada hora      |
      | ShowMonth: Mostra compromissos do mês                    |
      | ShowDay: Mostra compromissos de um dia inteiro           |
      |__________________________________________________________|*/


    /* Função: Login do usuário 
       Descrição: compara PassWord e UserName com os do banco de dados
    */
    public boolean Login(MC823Server stub) {
		
	BufferedReader leitor = new BufferedReader(new InputStreamReader(System.in));
	
	System.out.println("== Procura por usuário ==\n");
	System.out.print("Digite o nome do usuario:");
	
	try {
	    this.op.setLogin(leitor.readLine());
	} catch (Exception e){
	    System.err.println("Read exception: " + e.toString());
	    return false;
	}

	System.out.print("\nDigite a senha do usuario:");
	try {
	    this.op.setPassword(leitor.readLine());
	} catch (Exception e){
	    System.err.println("Read exception: " + e.toString());
	    return false;
	}


	/*Vou verificar se usuario tem agenda no sistema*/
	/*E se usuario possui a senha correta*/
	boolean Ok=false;
	try{
	    Ok = stub.IsUsr(op);
	    if(Ok == false){
		System.out.println("\nSenha ou usuários incorretos\n");
		return false;
	    }
	} catch (Exception e) {
	    System.err.println("Client exception: " + e.toString());
	    e.printStackTrace();
	}
	
	    System.out.println("\nUsuário logado: "+this.op.getLogin());
	    return true;
    }

    /*Cria agenda de um novo usuário*/
    public boolean NewCal(MC823Server stub) {
		
	BufferedReader leitor = new BufferedReader(new InputStreamReader(System.in));
	
	System.out.println("== Novo usuario ==\n");
	System.out.print("Digite o nome do novo usuario: ");
	
	try {
	    this.op.setLogin(leitor.readLine());
	} catch (Exception e){
	    System.err.println("Read exception: " + e.toString());
	    return false;
	}

	System.out.print("\nDigite a senha do novo usuario:");
	try {
	    this.op.setPassword(leitor.readLine());
	} catch (Exception e){
	    System.err.println("Read exception: " + e.toString());
	    return false;
	}

	/*Vou criar agenda de usuario no sistema*/
	boolean Ok=false;
	try{
	    Ok = stub.NewUsr(op);
	    if(Ok == false){
		System.out.println("\nNão consegui criar usuários\n");
		return false;
	    }
	} catch (Exception e) {
	    System.err.println("Client exception: " + e.toString());
	    e.printStackTrace();
	}
	
	
	System.out.println("\nUsuário logado: "+this.op.getLogin());
	System.out.println("\nSenha: "+this.op.getPassword());
	return true;
    }
	
    public void UserMenu() {
	System.out.println("*************************************************************************");
	System.out.println("*   ============            MENU USUÁRIO           =============        *");
	System.out.println("*   1. Marcar um compromisso.                                           *");
	System.out.println("*   2. Desmarcar um compromisso.                                        *");
	System.out.println("*   3. Ver todos compromissos marcados para um horario de um dia        *");
	System.out.println("*   4. Ver todos compromissos marcados para um dia.                     *");
	System.out.println("*   5. Ver todos compromissos do mes.                                   *");
	System.out.println("*   6. Sair.                                                            *");
	System.out.println("*                                                                       *");
	System.out.println("*************************************************************************");
    }
    public void NonUserMenu() {
	System.out.println("*************************************************************************");
	System.out.println("*   ============            MENU INICIAL           =============        *");
	System.out.println("*   1. Entrar com um usuário.                                           *");
	System.out.println("*   2. Criar um usuário.                                                *");
	System.out.println("*   3. Sair.                                                            *");
	System.out.println("*                                                                       *");
	System.out.println("*************************************************************************");
    }
	
    public void NewComp(MC823Server stub) {

	BufferedReader leitor = new BufferedReader(new InputStreamReader(System.in));
		
	try {
	    System.out.print("\nDigite o nome do compromisso: ");
	    op.setString(leitor.readLine());

	    System.out.print("\nDigite o dia do compromisso: ");
	    op.setDia(Integer.parseInt(leitor.readLine())); 

	    System.out.print("\nDigite o hora do compromisso: ");
	    op.setHora(Integer.parseInt(leitor.readLine())); 

	    System.out.print("\nDigite os minutos do compromisso: ");
	    op.setMinuto(Integer.parseInt(leitor.readLine())); 
		

	} catch (Exception e) {
	    System.err.println("Read exception: " + e.toString());
	}
	
	try {
	    	    timeStamp temp = new timeStamp();
	    if(stub.marcarCompromisso(op)){
		System.out.print("\nA operação foi um sucesso:Compromisso marcado.\nPressione ENTER para continuar...");
		leitor.readLine();
	    } else {
		System.out.print("\nErro!!! Server Exception.\nPressione ENTER para continuar...");
		leitor.readLine();
	    }
	    temp.pararTempo("1/clientTime.dat");
	} catch (Exception e) {
	    System.err.println("Client exception: " + e.toString());
	    e.printStackTrace();
	}
		
    }
	
    public void DelComp(MC823Server stub) {

	try {
	    System.out.print("\nDigite o nome do compromisso: ");
	    op.setString(leitor.readLine());

	} catch (Exception e) {
	    System.err.println("Read exception: " + e.toString());
	}
		
	try {
	      timeStamp temp = new timeStamp();
	    if(stub.desmarcarCompromisso(op)){
		System.out.print("\nA operação foi um sucesso: Compromisso desmarcado.\nPressione ENTER para continuar...");
		leitor.readLine();
	    } else {
		System.out.print("\nErro!!! Server Exception.\nPressione ENTER para continuar...");
		leitor.readLine();
	    }
	    temp.pararTempo("2/clientTime.dat");
	} catch (Exception e) {
	    System.err.println("Client exception: " + e.toString());
	    e.printStackTrace();
	}
		
    }
	
    public void ShowHour (MC823Server stub) {

	try {
	    System.out.print("\nDigite o dia do compromisso: ");
	    op.setDia(Integer.parseInt(leitor.readLine()));

	    System.out.print("\nDigite o horário do compromisso: ");
	    op.setHora(Integer.parseInt(leitor.readLine()));

	} catch (Exception e){
	    System.err.println("Read exception: " + e.toString());
	}
		  
	try {
	    	    timeStamp temp = new timeStamp();
			
	    //Recebe uma string de erro caso aconteca exception on server
	    op.setString(stub.obterCompromissoHora(op));
	     temp.pararTempo("3/clientTime.dat");
	} catch (Exception e) {
	    System.err.println("Client exception: " + e.toString());
	    e.printStackTrace();
	}

	try {
	    System.out.println("\n----------\nCompromissos do dia: " + op.getDia()+"e hora"+ op.getHora()+ "h\n----------");
	    System.out.println(op.getString());
	    System.out.println("----------\n");
	    System.out.print("\nPressione ENTER para continuar...");
	    leitor.readLine();
		
	} catch (Exception e) {
	    System.err.println("Read exception: " + e.toString());
	}


    }
	
    public void ShowDay(MC823Server stub) {
	int i;

	try {

	    System.out.print("\nDigite o dia do compromisso: ");
	    op.setDia(Integer.parseInt(leitor.readLine()));

	} catch (Exception e) {
	    System.err.println("Read exception: " + e.toString());
	}

	try {
	    timeStamp temp = new timeStamp();
			
	    //Recebe uma string de erro caso aconteca exception on server
	    op.setString(stub.obterCompromissoDia(op));
	    temp.pararTempo("4/clientTime.dat");
	} catch (Exception e) {
	    System.err.println("Client exception: " + e.toString());
	    e.printStackTrace();
	}
	
	try {

	    System.out.println("\n----------\nCompromissos do dia: " + op.getDia()+"\n----------");
	    System.out.println(op.getString());
	    System.out.println("----------\n");

			
	    System.out.print("\nPressione ENTER para continuar...");
	    leitor.readLine();
			
	} catch (Exception e) {
	    System.err.println("Read exception: " + e.toString());
	}
    }
	
    public void ShowMonth(MC823Server stub) {

	try {
	    	    timeStamp temp = new timeStamp();
			
	    //Recebe uma string de erro caso aconteca exception on server
	    op.setString(stub.obterCompromissoMes(op));
	       temp.pararTempo("5/clientTime.dat");
	} catch (Exception e) {
	    System.err.println("Client exception: " + e.toString());
	    e.printStackTrace();
	}

	try {
	    System.out.println("\n----------\nCompromissos do mes\n----------");
	    System.out.println(op.getString());
	    System.out.println("----------\n");
	   
	    System.out.print("\nPressione ENTER para continuar...");
	    leitor.readLine();
	} catch (Exception e) {
	    System.err.println("Read exception: " + e.toString());
	}
    }
}
