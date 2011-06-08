package client;

import java.rmi.Remote;
import java.rmi.RemoteException;

public interface MC823Server extends Remote {
    boolean IsUsr(Opr op) throws RemoteException;
    boolean NewUsr(Opr op) throws RemoteException;
    boolean marcarCompromisso(Opr op) throws RemoteException;
    boolean desmarcarCompromisso(Opr op) throws RemoteException;
    String obterCompromissoHora(Opr op) throws RemoteException;
    String obterCompromissoDia(Opr  op) throws RemoteException;
    String obterCompromissoMes(Opr op) throws RemoteException;
}
