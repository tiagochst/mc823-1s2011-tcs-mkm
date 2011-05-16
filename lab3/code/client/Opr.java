package client;
import java.io.Serializable;

/* Armazena as informaçoes necessarias na classe Opr */
public class Opr implements Serializable{
	private int operacao;
	private int dia;
	private int hora;
	private String string;
	private String login;
	private String password;

	public int getDia() {
		return dia;
	}
	public void setDia(int dia) {
		this.dia = dia;
	}
	public int getHora() {
		return hora;
	}
	public void setHora(int hora) {
		this.hora = hora;
	}
	public String getLogin() {
		return login;
	}
	public void setLogin(String login) {
		this.login = login;
	}
	public String getPassword() {
		return password;
	}
	public void setPassword(String password) {
		this.password = password;
	}
	public int getOperacao() {
		return operacao;
	}
	public void setOperacao(int operacao) {
		this.operacao = operacao;
	}
	public String getString() {
		return string;
	}
	public void setString(String string) {
		this.string = string;
	}
}
