from numpy import*
import sys
import getopt
import fileinput
from numpy import*

def main():
    rtt=[]
    for line in fileinput.input(['cliTimeRecvRTT.dat']):
        line = line.split()	
        rtt.append(line[0]) #pega tempo de echo
        
    max=0
    min=1000000
    sum=0
    for i in range(len(rtt)):
        sum = sum+int(rtt[i]);

        if(max<int(rtt[i])):
            max=int(rtt[i])
        if(min>int(rtt[i])):
            min=int(rtt[i])

    media=(float(sum)/(1000*len(rtt))) 
    devRTT=0
    for i in range(len(rtt)):
        devRTT=devRTT+math.pow(float(rtt[i])/1000-media,2)
    devRTT=sqrt(devRTT/len(rtt))     
    
    print "=== PING - MC823 ==="
    print "Max %.3f ms" % (float(max)/1000)
    print "Min %.3f ms" % (float(min)/1000)
    print "Media %.3f ms" % media  
    print "Desvio %.3f ms" % devRTT	

if __name__ == "__main__":
    main() 
