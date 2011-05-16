package client;

import java.rmi.Remote;
import java.rmi.RemoteException;

public interface MC823Server extends Remote {
    boolean marcarCompromisso(Opr op) throws RemoteException;
    boolean desmarcarCompromisso(Opr op) throws RemoteException;
    String obterCompromissoHora(Opr op) throws RemoteException;
    String obterCompromissoDia(Opr  op) throws RemoteException;
    String obterCompromissoMes(Opr op) throws RemoteException;
}
