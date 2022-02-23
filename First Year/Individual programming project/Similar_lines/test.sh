#!/bin/bash

temp_file1=$(mktemp /tmp/output.XXX)
temp_file2=$(mktemp /tmp/output.XXX)

PROG=$1
DIR=$2

for wejscie in "${DIR}"/*.in
do
    ./"${PROG}" <"${wejscie}" 1>"${temp_file1}" 2>"${temp_file2}"
    wyjscie=$?
    if [ $wyjscie != "0" ]
		then echo -e "\033[31mDla pliku $f program zakończył się kodem $wyjscie.\033[0m"
    fi

    echo -n "Test ${wejscie%.in} "
    if (diff "${temp_file1}" "${wejscie%in}out" > /dev/null) \
	&& (diff "${temp_file2}" "${wejscie%in}err" > /dev/null)
		then echo -n -e "\033[32mjest OK \033[0m"
	else 
		echo -n -e "\033[31mjest ZLE: \033[0m"
		if !(diff "${temp_file1}" "${wejscie%in}out" > /dev/null)
			then echo -n -e "\033[91mniepoprawny plik out \033[0m"
		fi
		if !(diff "${temp_file2}" "${wejscie%in}err"> /dev/null)
			then echo -n -e "\033[91mniepoprawny plik err \033[0m"
		fi
	fi

	echo -n "i "
	if valgrind --error-exitcode=1 --leak-check=full ./"$PROG" < "$wejscie" &> /dev/null
    then
        echo -e "\033[32mbrak problemów z pamiecia\033[0m"
    else
        echo -e "\033[31mwystepuje problem z pamiecia\033[0m"
    fi
done

rm "${temp_file1}" "${temp_file2}"
