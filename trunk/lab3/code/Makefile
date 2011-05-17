all:
	make interfaces
	make -B server
	make -B client
# compilando interfaces
interfaces:
	javac client/Opr.java client/MC823Server.java
	jar cvf client/MC823Server.jar client/*.class

# compilando cliente
client:
	javac -cp client/MC823Server.jar \
		 client/CMain.java  \
		 client/Client.java \
		client/MarcarTempo.java

# compilando servidor
server:
	javac -cp client/MC823Server.jar \
	       client/Server.java

PWD := $(shell pwd)
JAR := $(PWD)/client/MC823Server.jar	

srun:
	# Precisa ser realizado somente uma vez
	 rmiregistry &
	java -cp $(PWD):$(JAR) \
	-Djava.rmi.server.codebase=file:///$(JAR) \
	-Djava.rmi.server.hostname=localhost	\
	-Djava.security.policy=$(PWD)/server.policy \
	client.Server

crun:
	java -cp $(PWD):$(JAR) \
	-Djava.rmi.server.codebase=file:///$(PWD)/client/ \
        -Djava.security.policy=$(PWD)/client.policy  \
         client.CMain localhost 

