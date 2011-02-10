#!/bin/bash
#PERC=80;
SUFIXO="pool25_filtro";

SEM=1;
while [ $SEM -lt 11 ]; do

        ./graspmcast -a conf_1.txt -sm 2 -np 10 -m 10 -s $SEM -pct 5 > conf1m10_s"$SEM"_"$SUFIXO"
        let SEM=SEM+1
done

SEM=1;
while [ $SEM -lt 11 ]; do

        ./graspmcast -a conf_1.txt -sm 2 -np 10 -m 15 -s $SEM -pct 5 > conf1m15_s"$SEM"_"$SUFIXO"
        let SEM=SEM+1
done

