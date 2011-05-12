all:
	make interfaces
	make server
	make client

interfaces:
	# compilando interfaces
	javac compute/Compute.java compute/Task.java
	jar cvf classes/compute.jar compute/*.class

client:
	# compilando cliente
	javac -cp classes/compute.jar client/ComputePi.java client/Pi.java

server:
	# compilando servidor
	javac -cp classes/compute.jar                                                             engine/ComputeEngine.java

PWD := $(shell pwd)
JAR := $(PWD)/classes/computer.jar	
srun:
	java -cp $(PWD):$(JAR) \
	-Djava.rmi.server.codebase=file:///$(JAR) \
	-Djava.rmi.server.hostname=localhost	\
	-Djava.security.policy=$(PWD)/server.policy \
	engine.ComputeEngine

crun:
	java -cp $(PWD):$(JAR) \
	-Djava.rmi.server.codebase=file:///$(PWD)/client/ \
        -Djava.security.policy=$(PWD)/client.policy  \
         client.ComputePi localhost 45