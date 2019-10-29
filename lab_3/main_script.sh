#!/bin/bash
max=5
declare ArStr
count=0

echo "Running $max tasks..."
## Запускаем и записываем вывод arcsub во временный файл
for i in `seq 1 $max`; do
  arcsub nordujob | grep "Job submitted with jobid" |tee -a temp_file1
done

## Оставляем в строках только jobid
while read LINE; do
  echo ${LINE:26} >> temp_file2
done < temp_file1
rm temp_file1

## Переносим из файла в массив
ArStr=( `cat "temp_file2"` )
rm temp_file2

ar_length=${#ArStr[@]}

echo "$ar_length tasks are started. $(($max - $ar_length)) task(s) could not be started."
echo "Expected to be completed..."
## Проверяем завершение
while [ "$count" -ne "$ar_length" ]; do
  count=0
  sleep 60
  ## Считаем количество завершенных задач
  for k in `seq 0 $(($ar_length - 1))`; do
    count=$(($count + $(arcstat ${ArStr[$k]} | grep -wc Finished)))
  done
  if [ "$count" -ne "$ar_length" ]
  then
    echo "$(($ar_length - $count)) task(s) have not been completed yet."
    echo "Expected to be completed..."
  fi
done

echo "All tasks completed!"
## Получаем результат
for g in `seq 0 $(($ar_length - 1))`; do
  arcget ${ArStr[$g]}
done

## Оставляем в массиве только названия директорий
for n in `seq 0 $(($ar_length - 1))`; do
  ArStr[$n]=${ArStr[$n]:35}
done

## Переписываем полученные данные в один файл
for m in `seq 0 $(($ar_length - 1))`; do
  echo "============================================================" >> final_result.txt
  echo "************************ $(($m+1)) *************************" >> final_result.txt
  echo "============================================================" >> final_result.txt
  cat ${ArStr[$m]}/result.txt >> final_result.txt
  echo " " >> final_result.txt
done

echo "The result is saved in a file final_result.txt" 
