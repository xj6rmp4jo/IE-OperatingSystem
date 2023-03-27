# IE-OperatingSystem

完成 4 個任務 :
  1. 將 N 筆數目字直接進行 BubbleSort，並顯示 CPU 執行時間。
  2. 將 N 筆數目字切成 K 份，先在 1 個 process 內對 K 份資料進行 BubbleSort 之後，再用同一個 process 作 MergeSort，並顯示 CPU 執行時間。
  3. 將 N 筆數目字切成 K 份，並由 K 個 processes 各別進行 BubbleSort 之後，再用 K-1 個 process(es) 作 MergeSort，並顯示 CPU 執行時間。
  4. 將 N 筆數目字切成 K 份，並由 K 個 threads 各別進行 BubbleSort 之後，再用 K-1 個 thread(s) 作 MergeSort，並顯示 CPU 執行時間。
  
  其中 N 筆數目字會經由讀檔的方式輸入、K 由使用者輸入。
  必須以寫檔的方式輸出，輸出檔為已排序後的 N 筆數目字，且需另外加上 CPU 執行時間及執行當下的時間。

I/O :
  讀檔:
    基礎測資如附檔,提供四種預設測資進行排序:
    input_1w包含一萬筆資料
    input_10w包含十萬筆資料
    input_50w包含五萬筆資料
    input_100w包含一百萬筆資料
    測資皆為正整數(包含0)。
    ※进階测資檔名未定,進階测資名稱不包含資料數,且勿於程式中限定檔名字串。
    
  寫檔:
    Output 檔名請依照格式:Input FileNameupuskNumber.xt例檔名如圖2所示。
    ※輸出檔名應依照输入檔名做變化,請勿固定输出檔名。
