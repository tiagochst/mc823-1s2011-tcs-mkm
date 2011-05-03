from numpy import*
import sys
import getopt
import fileinput
from numpy import*

def main():
    client=[]
    server=[]
    diff=[]
    for line in fileinput.input(['clientTime.dat']):
        line = line.split()	
        client.append(line[0]) #pega tempo do total
    for line in fileinput.input(['serverTime.dat']):
        line = line.split()	
        server.append(line[0]) #pega tempo do servidor
  
      
    max=0
    min=1000000
    sum=0
    # diff em milissegunso
    for i in range(len(client)):
        diff.append((float(client[i])-float(server[i]))*1000);

    for i in range(len(diff)):
        sum = sum+float(diff[i]);

        if(max<float(diff[i])):
            max=float(diff[i])
        if(min>float(diff[i])):
            min=float(diff[i])

    media=(float(sum)/(len(diff))) 
    devDIFF=0

    devDIFF=devDIFF+math.pow(float(diff[i])-media,2)
    devDIFF=sqrt(devDIFF/len(diff))     
    
    print "=== PING - MC823 ==="
    print "Max %.3f ms" % (float(max))
    print "Min %.3f ms" % (float(min))
    print "Media %.3f ms" % media  
    print "Desvio %.3f ms" % devDIFF	

if __name__ == "__main__":
    main() 
