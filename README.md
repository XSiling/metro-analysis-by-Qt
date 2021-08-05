# metro-analysis-by-Qt
#### Introductions

​	As the popularity of online ride-hailing grows recently, there are more needs for analysis on the online ride-hailing order data to help companies like Didi and Uber to solve challenges. In this project, I have accomplished some analysis and visualization of online ride-hailing order data in Chengdu according to the data file offered on canvas.

​	In my project, I divided work into two parts: loading and analyzing.  The loading part includes allowing users to select what .zip file and which parts they need to load. Parts includes the travel time, departure locations, arrival locations and the fee. The analyzing part includes three patterns of analysis: chart, picture and prediction. Here are the specific analysis contents:

- Chart-analysis

  This part mainly includes:

  - the numbers of orders over time in line-chart form
  - the numbers of orders in each grids over time in line-chart form (including departure grids and arrival grids)
  - the distributions of travel time  in pie chart form
  - the distribution of fee in pie chart form
  - the revenue over time in line-chart form 
  - the revenue over time in histogram form

  You can tune the parameters if possible.

- Picture-analysis

  This part mainly includes the thermal diagram of departure places over a period of time.

  You can tune the size of specific viewing zone of grids and color bar.

- Prediction-analysis

  This part allows you to choose your own departure place and we offer you three possible destinations. You choose your departure place by clicking on the valid area in the picture. Then the possible destinations will be given in text forms.

  

#### Implementation Details

​	Just as mentioned above, the project can be divided into loading and analysis parts. 

- Loading

  ​	This part is implemented via multi-thread to prevent the program from being  stuck when I am computing while displaying.

  ​	The check buttons and spin boxes of begin and end days above are in connection with internal properties in mainwindow, once you have changed them, a signal will be sent to change the properties.

  ​	You can click the "open file" button and choose your .zip dataset, then it begins to load data into internal storage. As the data set is quiet big, this part is implemented via another thread. The progress bar is in the main thread and their connections are through signals and slots.

  ​	The progress is based on the numbers of files that have been loaded.

  ​	Fields that already loaded can be viewed by looking at whether there are a "√" next to the check buttons.

- Analysis

  ​	This part is in the main thread. The chart, picture and prediction patterns are for different kinds of analysis. The switch of three patterns are through clicking the button next to "loading" label. 

  - Chart-analysis

    ​	The chart-analysis mainly includes three kinds of chart: line chart, histogram and pie chart. The specific kinds of chart can be chosen by the combo box. 

    ​	The implementation is via **QChart** ,an useful built-in library.

    ​	The basic chart is none, if there is no enough data for other charts, the chart will turn to none, a blank area with no parameters.

    ​	The number-day pattern is showing the numbers of orders over time in line-chart form. The parameters that can be tuned includes the begin day, the end day and the interval time(1,2). 

    ​	The number-day(departure grid) pattern is showing the numbers of orders in certain departure grid over time in line-chart form. The parameters include the begin day, the end day, the interval time and specific grid.

    ​	The number-day(arrival grid) pattern is showing the numbers of orders in certain arrival grid over time in line-chart form. The parameters include the begin day, the end day, the interval time and specific grid.	

    ​	The distribution of travel time pattern is showing the distribution of users' travel time(arrival time minus departure time), the gap time is 5 min, 10 min and 20 min. The choice of gap time is based on the experience in daily life.

    ​	The distribution of fee pattern is showing the distribution of users' fee, the gap fee is 2￥，5￥，10￥, the choice of gap fee is based on the experience in daily life.

    ​	The revenue line chart pattern shows the revenue over time. You can tune the begin day and the end day.

    ​	The revenue histogram pattern shows the revenue each day. You can tune the begin day and the end day.

  - picture-analysis

    ​	The picture-analysis (the thermal diagram of departure places over a period of time) is implemented via **QCustomPlot**. After dividing the whole area into 100 parts, we calculate the numbers of each grid and change the weight. Through QCustomPlot, it's easy to paint such thermal diagram.

  - prediction analysis

    ​	The pixmap which can be clicked are divided into 100 grids by pixel coordinates. Once you have clicked the certain grid, we will calculate all the data that leaves from this grid, then offer users the top 3 as the prediction in text form. 

#### Results

​	The results are shown through show.mp4.

#### Discussions

​	Generally speaking, the program is running smoothly. And you can see different kinds of analysis. Below are something that I think **could be improved:**

- the interaction of map. Most of the operations are through click button and spinbox. Because of the lack of time and knowledge, I think the interaction through map could be more intuitionistic and easy-to-operate.

- the beauty of the interface. The general interface is simple now.

- more parameters' tuning space. There are still some space left for users to operate. Such as the gap time and gap fee while seeing the chart.

  After analyzing, there are some **inclusions.**

- The highest number of orders seem to happen on day 5 and day 11. Searching for the calendar, they are both Fridays. In Chengdu, people prefer to take online ride-hailing on Friday. And the least numbers happen on Monday.

- The highest distribution of travel time is 10 to 20 min. The second is 20 to more. It seems that the single time people spent on the way is quiet long. More than 75% people will spend more than 10 min on ride-hailing.

- The highest distribution of fee is 2 to 5￥. Honestly saying, it's cheap. Compared to Shanghai, fee in Chengdu is much lower. Nearly 75% people only spend less than 5￥ one tour.

- Just looking at this half of month, the company's revenue grows steadily. 

- Most people take ride-hailing from the center of Chengdu, and the southern part seems to be more popular than the northern part.

- Chengdu's degree of centralization is high that most orders focus on the center of Chengdu(both departure places and arrival places).
