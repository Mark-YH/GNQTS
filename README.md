# 目錄架構
```
.
├── EWFA    QTS平分資金編碼
│   ├── Particle.h
│   ├── QTS.cpp
│   └── QTS.h
├── FundAllocation      QTS分配資金編碼
│   ├── ParticleFA.h
│   ├── QTS.cpp
│   └── QTS.h
├── Model       股票相關model
│   ├── Logger.h
│   ├── Model.cpp
│   ├── Model.h     修改多種參數的標頭檔
│   ├── Result.h
│   └── Stock.h
├── data    放股價檔案的目錄
├── log     輸出實驗數據的目錄
├── main.cpp    主程式
└── py      python相關程式，詳細說明請看內層README
    ├── img             python輸出圖檔的目錄
    ├── py_output       python輸出各種檔案的目錄
    ├── UBH.py
    ├── djia.py
    ├── fine_tune_analyzer.py
    ├── measurements.py
    ├── plot_convergence.py
    ├── plot_fs.py
    ├── plot_ideal_situation.py
    ├── plot_result.py
    └── plot_sr_tr_diff.py
```


## 修改參數說明

### `main.cpp`

1. `Model model(10, 10000, 0.0004, 0.0008, 0.0002, 10000000.0, 0.001425, 0.003);`
2. `Model testingModel(10, 10000, 0.0004, 0.0008, 0.0002, 10000000.0, 0.001425, 0.003);`

上面兩個要一起改，參數依序為
`(int population, int generation, double theta, double uTheta, double lTheta, double fund, double fee,
          double tax)`
theta: 旋轉角度
utheta: upper bound
ltheta: lower bound
要改成GNQTS的話就把theta、uTheta、lTheta 設為相同值；使用美股參數時，不論此處設定的fee、tax是多少均不計。

### `Model.h`
第10到37行：
```c++
#define US_MARKET 1 // 0是台股，1是美股
#define EXIST_BANK 0 // 0是沒有銀行的設定，1反之
#define ROUND 50 // 實驗次數
#define RUN_TESTING 1 // 是否跑測試期，0不跑；1跑
#define COMPOUND_INTEREST 1 // 測試期是否有複利，0沒有；1有
#define MODE 1 // 參考下面說明，設定範圍：0~5
#define TR_SR 0 // 0 使用趨勢值；1使用夏普值
#define SHORT_SELLING 0 // 0 使用一般交易；1使用融券交易
#define CONVERGENCE 1 // 是否輸出每一代fitness，0不輸出；1輸出

/** @param MODE
 *  0: execute stock selection program 平分資金
 *  1: execute fund allocation program 分配資金
 *  2: execute fund allocation with method of exhaustion program 窮舉法
 *  3: execute ranking program 單檔排名
 *  4: execute single stock program 單檔數據（return, risk, TR, FS, etc.）
 *  5: execute given portfolio program 跑給定的投資組合
 */

    inline static string slidingWindow = "M2M"; // 13個滑動視窗，不能輸入*，所以 M* 要設定 M#
    inline static string market = "DJI30"; // 跑特定股價檔案，要跟data內資料夾名稱相同
    inline static string alias = "normal"; // 輸出檔案名稱的prefix，通常設定ANGQTS、GNQTS，或是其他方便辨識的任何名稱
```