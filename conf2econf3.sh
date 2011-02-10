#!/bin/bash
#PERC=80;
SUFIXO="pool25_filtro";

SEM=1;
while [ $SEM -lt 11 ]; do

	./graspmcast -a conf_2.txt -sm 2 -np 10 -m 10 -s $SEM -pct 5 > conf2m10_s"$SEM"_"$SUFIXO"
	let SEM=SEM+1
done

SEM=1;
while [ $SEM -lt 11 ]; do

	./graspmcast -a conf_2.txt -sm 2 -np 10 -m 20 -s $SEM -pct 5 > conf2m20_s"$SEM"_"$SUFIXO"
	let SEM=SEM+1
done

SEM=1;
while [ $SEM -lt 11 ]; do

	./graspmcast -a conf_2.txt -sm 2 -np 10 -m 30 -s $SEM -pct 5 > conf2m30_s"$SEM"_"$SUFIXO"
	let SEM=SEM+1
done

SEM=1;
while [ $SEM -lt 11 ]; do

	./graspmcast -a conf_2.txt -sm 2 -np 10 -m 40 -s $SEM -pct 5 > conf2m40_s"$SEM"_"$SUFIXO"
	let SEM=SEM+1
done


SEM=1;
while [ $SEM -lt 11 ]; do

	./graspmcast -a conf_3.txt -sm 2 -np 10 -m 20 -s $SEM -pct 5 > conf3m20_s"$SEM"_"$SUFIXO"
	let SEM=SEM+1
done

SEM=1;
while [ $SEM -lt 11 ]; do

	./graspmcast -a conf_3.txt -sm 2 -np 10 -m 40 -s $SEM -pct 5 > conf3m40_s"$SEM"_"$SUFIXO"
	let SEM=SEM+1
done

SEM=1;
while [ $SEM -lt 11 ]; do

	./graspmcast -a conf_3.txt -sm 2 -np 10 -m 60 -s $SEM -pct 5 > conf3m60_s"$SEM"_"$SUFIXO"
	let SEM=SEM+1
done

SEM=1;
while [ $SEM -lt 11 ]; do

	./graspmcast -a conf_3.txt -sm 2 -np 10 -m 80 -s $SEM -pct 5 > conf3m80_s"$SEM"_"$SUFIXO"
	let SEM=SEM+1
done

