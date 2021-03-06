#!/bin/csh -f
setenv LOGFILE $1
setenv TXTFILE `echo ${LOGFILE} | awk -F. '{print "getresult_"$1".txt"}'`
rm -f ${TXTFILE}_1 ; grep "Tot Err:" ${LOGFILE} | tail -36 | sed -e 's/M_GAMMA/Gamma/g' | awk '{printf "%-8s %8.6f %8.6f\n", $1, $2, $(NF-3)}'  > ${TXTFILE}_1
rm -f ${TXTFILE}_2 ; grep "All: Sum of Multi-Quantity Average" ${LOGFILE}  | tail -1 | awk -F"=" '{print $2}' | awk '{printf "Gamma103 %8.6f %8.6f\n",1-$1,$3}' > ${TXTFILE}_2
rm -f ${TXTFILE}_3 ; grep "P1: Sum of Multi-Quantity Average" ${LOGFILE} | tail -1 |  awk -F"=" '{print $2}' | awk '{printf "Gamma110 %8.6f %8.6f\n",$1,$3}' > ${TXTFILE}_3
cat ${TXTFILE}_1 ${TXTFILE}_2 ${TXTFILE}_3 | tee ${TXTFILE}
rm -f ${TXTFILE}_1 ${TXTFILE}_2 ${TXTFILE}_3
