import matplotlib.pyplot as plt 
import pandas as pd
import numpy
import math  

# Settings:
# Folder's path of the movement files.
filesFolder="C:/Users/blue/Documents/study/BI/Thesis/allOne/data/mov/second/"
# Folder's path of the movement statistics
movStatisticsFolder="C:/Users/blue/Documents/study/BI/Thesis/allOne/results/movStatistics/second/"
# number of replicate
numberOfReplicate = 99
replicateNumbers = range(numberOfReplicate+1)
# reward modes:
rewardModes = [0,1,2,3]
# group modes:
groupModes = [0,1]
# End of settings

# 1. Finding the percentage of the following values:
#  I. Number of times that action (a columns) has higher value than 3 
#  II. Number of times that action (a columns) iz zero
#  II. Number of times that b is not zero

movStatisticsLog = pd.DataFrame (columns = ['fileName','movLength*4','aBiggerThanThree','aBiggerThanThreePercentage','aZero','aZeroPercentage','bNotZero','bNotZeroPercentage'])
movStatistics = pd.DataFrame (columns = ['rewardGroup','meanABiggerThanThreePercentage','stdErrMeanABiggerThanThreePercentage',
                                         'meanAZeroPercentage','stdErrMeanAZeroPercentage',
                                         'meanBNotZeroPercentage','stdErrMeanBNotZeroPercentage'])

movLength = 0
aBiggerThanThree = 0
aBiggerThanThreePercentage = 0
aZero = 0
aZeroPercentage = 0
bNotZero = 0
bNotZeroPercentage = 0

aBiggerThanThreePercentageS = []
aZeroPercentageS = []
bNotZeroPercentageS = []

for rewardMode in rewardModes:
    for groupMode in groupModes:
        for replicateNumber in replicateNumbers:
            fileName = "movement"+"_"+str(rewardMode)+"_"+str(groupMode)+"_"+str(replicateNumber)+".csv"
            rewardGroup = "movement"+"_"+str(rewardMode)+"_"+str(groupMode)
            df = pd.read_csv(filesFolder+fileName)
            movLength = len(df.index)
            for i in range(len(df)) :
                # check if a iz bigger than 3
                if(df.loc[i, "a0"]>3):
                    aBiggerThanThree = aBiggerThanThree + 1
                if(df.loc[i, "a1"]>3):
                    aBiggerThanThree = aBiggerThanThree + 1
                if(df.loc[i, "a2"]>3):
                    aBiggerThanThree = aBiggerThanThree + 1
                if(df.loc[i, "a3"]>3):
                    aBiggerThanThree = aBiggerThanThree + 1
                # Check if a is zero    
                if(df.loc[i, "a0"]==0):
                    aZero = aZero + 1
                if(df.loc[i, "a1"]==0):
                    aZero = aZero + 1
                if(df.loc[i, "a2"]==0):
                    aZero = aZero + 1
                if(df.loc[i, "a3"]==0):
                    aZero = aZero + 1
                # Check if b is NOT zero    
                if(df.loc[i, "b0"]!=0):
                    bNotZero = bNotZero + 1
                if(df.loc[i, "b1"]!=0):
                    bNotZero = bNotZero + 1
                if(df.loc[i, "b2"]!=0):
                    bNotZero = bNotZero + 1
                if(df.loc[i, "b3"]!=0):
                    bNotZero = bNotZero + 1

            aBiggerThanThreePercentage = (aBiggerThanThree/(movLength*4))*100
            aZeroPercentage = (aZero/(movLength*4))*100
            bNotZeroPercentage = (bNotZero/(movLength*4))*100
            aBiggerThanThreePercentageS.append(aBiggerThanThreePercentage)
            aZeroPercentageS.append(aZeroPercentage)
            bNotZeroPercentageS.append(bNotZeroPercentage)

            #print(fileName," ",aBiggerThanThree," ",movLength*4," ",aBiggerThanThreePercentage, " ",aZero)
            movStatisticsLog = movStatisticsLog.append({'fileName': fileName, 'movLength*4' : movLength*4,'aBiggerThanThree' : aBiggerThanThree, 'aBiggerThanThreePercentage' : aBiggerThanThreePercentage,'aZero':aZero,'aZeroPercentage':aZeroPercentage,'bNotZero':bNotZero,'bNotZeroPercentage':bNotZeroPercentage}, ignore_index=True)
            movLength = 0
            aBiggerThanThree = 0
            aBiggerThanThreePercentage = 0
            aZero = 0
            aZeroPercentage = 0
            bNotZero = 0
            bNotZeroPercentage = 0
        #print(sum(aBiggerThanThreePercentageS)/(numberOfReplicate+1))
        movStatistics = movStatistics.append({'rewardGroup': rewardGroup,
                                              'meanABiggerThanThreePercentage': sum(aBiggerThanThreePercentageS)/(numberOfReplicate+1),
                                              'stdErrMeanABiggerThanThreePercentage': numpy.std(aBiggerThanThreePercentageS)/math.sqrt(numberOfReplicate+1),  
                                              'meanAZeroPercentage': sum(aZeroPercentageS)/(numberOfReplicate+1),
                                              'stdErrMeanAZeroPercentage': numpy.std(aZeroPercentageS)/math.sqrt(numberOfReplicate+1),
                                              'meanBNotZeroPercentage': sum(bNotZeroPercentageS)/(numberOfReplicate+1),
                                              'stdErrMeanBNotZeroPercentage': numpy.std(bNotZeroPercentageS)/math.sqrt(numberOfReplicate+1),
                                              },
                                             ignore_index=True)
        aBiggerThanThreePercentageS.clear()
        aZeroPercentageS.clear()
        bNotZeroPercentageS.clear()

#print(movStatisticsLog)
#movStatisticsLog.to_csv(movStatisticsFolder+"movStatisticsLog"+".csv", index=False)

#print(movStatistics)
movStatistics.to_csv(movStatisticsFolder+"movStatistics"+".csv", index=False)



