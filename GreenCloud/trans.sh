#!/bin/sh

tot_machine=12583

for (( i=1; i<=tot_machine; i++ ))
do
	let pidx=i-1
	pccap=`sed -n ${i}p MemCPUCap.csv | awk 'BEGIN{FS=","}{print $2}'`
	pmcap=`sed -n ${i}p MemCPUCap.csv | awk 'BEGIN{FS=","}{print $3}'`
#echo "${pidx} ${pcap}"
	cat usage/usage-${pidx}.data | awk -f trans.awk -v ccap=${pccap} mcap=${pmcap} > /var/services/homes/dozhang/data/analysis/nusage/nusage-${pidx}.data
done

