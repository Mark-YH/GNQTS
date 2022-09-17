## `UBH.py` 
跑UBH實驗用的，第89行設定股價路徑，如`path = os.path.realpath('../data/DJI30_2010-202106/M2M/test')`



## `djia.py`
跑DJIA數據用的，第14行設定DJIA股價路徑，如`p = path.realpath('../data/DJIA/DJI_2010-202106.csv')`



## `plot_ideal_situation.py`
畫理想圖說明資金分配議題，直接run就好



## `plot_sr_tr_diff.py`
畫趨勢值、夏普值、心情指數的舉例說明圖，直接run就好



## `measurements.py`
我自己的分析工具，不用管，有些script會import他，不能刪



## `fine_tune_analyzer.py`
畫fine-tune參數數據圖，

第16行 `matplotlib.use('macosx')` 非macos執行時須註解掉。

第17行設定圖表標題，如`title = 'Test 1$-$4'`。

第21行設定路徑`root = os.path.realpath('root = os.path.realpath('/Users/Mark/Desktop/交接檔案/2. 實驗數據/Fine-tune/Fine-tune experimental results/Fine-tune_Test1+2+3+4')')`，

對應到`Fine-tune experimental results`裡面的`Fine-tune_Test1`等資料夾。

第99到101行註解掉就不會有方框。



## `plot_convergence.py`

畫斂散圖，第14行設定路徑，如`root = '/Users/Mark/Desktop/交接檔案/2. 實驗數據/2010-2019/'`，

接著第30行設定版本，如設定`version = ['ANGQTS', 'GNQTS']`就會找到路徑下的`DJI30 ANGQTS`和`DJI30 GNQTS`資料夾。

第31行設定平分資金或分配資金，如`mode = ['FA']`就會找路徑下的`FA`資料夾，也就是分配資金的數據。


## `plot_fs.py`
畫資金水位圖，第13-19行要一起設定，範例如下：
```python
paths = ['/Users/Mark/Desktop/交接檔案/2. 實驗數據/2010-2019/DJI30 ANGQTS/EWFA/',
         '/Users/Mark/Desktop/交接檔案/2. 實驗數據/2010-2019/DJI30 ANGQTS/FA/',
         '/Users/Mark/Desktop/交接檔案/2. 實驗數據/2010-2019/DJI30 ANGQTS-SR/EWFA/'
         ]
labels = ['ANGQTS-EWFA', 'ANGQTS-NEWFA', 'ANGQTS-SR']
file_prefix = ['ANGQTS_EWFA', 'ANGQTS_FA', 'ANGQTS-SR_EWFA']
colors = [(0.2, 0.4, 1), (1, 0.5, 0.2), (0.2, 0.6, 0.4)]
 ```
1. paths可以設定多個，相對的labels、file_prefix、colors也要有對應設定。
2. labels 是圖例的文字
3. file_prefix是檔案的前綴詞，例如`ANGQTS-SR_EWFA_train_2010(2010 Q1).csv`是Y2Y 2010訓練期的數據，它的prefix就是`ANGQTS-SR_EWFA`。
4. colors 就是顏色設定，麻煩自己去google matplotlib color來設定。


第20行設定訓練期(train)、測試期(test)、整體測試期(total)，只有這三種設定： 
1. `mode = 'train'`
2. `mode = 'test'`
3. `mode = 'total'`


### 輸出特例圖
跑特例的話要拿掉第264行的註解，然後在第189行設定檔案名稱，第193行設定路徑，
如`/Users/Mark/Desktop/交接檔案/2. 實驗數據/2010-2019/`。
第191行設定圖表標題。




## `plot_result.py`
畫報酬、風險、趨勢值等數據，

第16行設定訓練期路徑，如`p  = '/Users/Mark/Desktop/交接檔案/2. 實驗數據/2010-2019/TR vs SR training period.csv'`

第49行設定測試期路徑，如`p = '/Users/Mark/Desktop/交接檔案/2. 實驗數據/2010-2019/TR vs SR testing period.csv'`

第12行`object_number = 3`是指有幾個項目（不含DJIA），如上述路徑中`TR vs SR training period.csv`
有三個比較項目（ANGQTS-EWFA、ANGQTS、FA、ANGQTS-SR），因此設定3。




## 通用說明
- 所有`ax.set_xlim`和`ax.set_ylim`都可以修改或是直接註解掉，可以控制x軸或y軸的scale。
- 所有`ax.set_xlabel`和`ax.set_ylabel`都可以改，這是控制x/y軸的文字。
- 所有`plt.title`都可以改，這是圖表標題。


### Dependency
1. matplotlib
2. numpy
3. pandas

執行前請確認已安裝上面三個library。