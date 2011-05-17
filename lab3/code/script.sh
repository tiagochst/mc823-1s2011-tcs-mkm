ps -fe | grep $1 > lixo
cat lixo | while read linha
do
    cont=0
    echo $linha
    for palavra in $linha
    do
	let cont++
	if [ $cont -eq 2 ]
	then
	    kill $palavra
	fi
    done
done
rm lixo
rmiregistry &
