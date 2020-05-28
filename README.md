# sudoku
general solver for sudoku

抓取题目脚本
#!/bin/zsh
while :
do
    curl https://sudoku.com/api/getLevel/expert >> expert.csv
    echo >> expert.csv
    sleep 30s 
done
