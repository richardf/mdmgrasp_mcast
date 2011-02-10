#!/bin/bash
#PERC=80;
SUFIXO="pool25_filtro";

SEM=1;
while [ $SEM -lt 11 ]; do

	./graspmcast -a broad_1.txt -sm 2 -np 10 -m 25 -s $SEM -pct 5 > broad1m25_s"$SEM"_"$SUFIXO"
	let SEM=SEM+1
done

SEM=1;
while [ $SEM -lt 11 ]; do

	./graspmcast -a broad_1.txt -sm 2 -np 10 -m 50 -s $SEM -pct 5 > broad1m50_s"$SEM"_"$SUFIXO"
	let SEM=SEM+1
done

SEM=1;
while [ $SEM -lt 11 ]; do

	./graspmcast -a broad_1.txt -sm 2 -np 10 -m 75 -s $SEM -pct 5 > broad1m75_s"$SEM"_"$SUFIXO"
	let SEM=SEM+1
done

SEM=1;
while [ $SEM -lt 11 ]; do

	./graspmcast -a broad_1.txt -sm 2 -np 10 -m 100 -s $SEM -pct 5 > broad1m100_s"$SEM"_"$SUFIXO"
	let SEM=SEM+1
done

SEM=1;
while [ $SEM -lt 11 ]; do

	./graspmcast -a broad_2.txt -sm 2 -np 10 -m 50 -s $SEM -pct 5 > broad2m50_s"$SEM"_"$SUFIXO"
	let SEM=SEM+1
done

SEM=1;
while [ $SEM -lt 11 ]; do

	./graspmcast -a broad_2.txt -sm 2 -np 10 -m 100 -s $SEM -pct 5 > broad2m100_s"$SEM"_"$SUFIXO"
	let SEM=SEM+1
done

SEM=1;
while [ $SEM -lt 11 ]; do

	./graspmcast -a broad_2.txt -sm 2 -np 10 -m 150 -s $SEM -pct 5 > broad2m150_s"$SEM"_"$SUFIXO"
	let SEM=SEM+1
done

SEM=1;
while [ $SEM -lt 11 ]; do

	./graspmcast -a broad_2.txt -sm 2 -np 10 -m 200 -s $SEM -pct 5 > broad2m200_s"$SEM"_"$SUFIXO"
	let SEM=SEM+1
done

