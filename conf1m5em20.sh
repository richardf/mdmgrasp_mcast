#!/bin/bash
#PERC=80;
SUFIXO="pool25_filtro";

SEM=1;
while [ $SEM -lt 11 ]; do

        ./graspmcast -a conf_1.txt -sm 2 -np 10 -m 5 -s $SEM -pct 5 > conf1m5_s"$SEM"_"$SUFIXO"
        let SEM=SEM+1
done

SEM=1;
while [ $SEM -lt 11 ]; do

        ./graspmcast -a conf_1.txt -sm 2 -np 10 -m 20 -s $SEM -pct 5 > conf1m20_s"$SEM"_"$SUFIXO"
        let SEM=SEM+1
done

