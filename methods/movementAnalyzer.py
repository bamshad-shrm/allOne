import matplotlib.pyplot as plt 
import pandas as pd

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
movStatistics = pd.DataFrame (columns = ['rewardGroup','meanABiggerThanThreePercentage','meanAZeroPercentage','meanBNotZeroPercentage'])

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
                                              'meanAZeroPercentage': sum(aZeroPercentageS)/(numberOfReplicate+1),
                                              'meanBNotZeroPercentage': sum(bNotZeroPercentageS)/(numberOfReplicate+1)
                                              },
                                             ignore_index=True)
        aBiggerThanThreePercentageS.clear()
        aZeroPercentageS.clear()
        bNotZeroPercentageS.clear()

#print(movStatisticsLog)
#movStatisticsLog.to_csv(movStatisticsFolder+"movStatisticsLog"+".csv", index=False)

#print(movStatistics)
movStatistics.to_csv(movStatisticsFolder+"movStatistics"+".csv", index=False)


'''

# 2. Making the lod files same number of rows
# We do this step to avoid dimension error in plotting 

for rewardMode in rewardModes:
    for groupMode in groupModes:
        for replicateNumber in replicateNumbers:
            fileName = "LOD"+"_"+str(rewardMode)+"_"+str(groupMode)+"_"+str(replicateNumber)
            df = pd.read_csv(filesFolder+fileName+".csv")
            newRow = df.iloc[-1]            
            if len(df.index) < maxLodLengths:
                for i in range(maxLodLengths-len(df.index)):
                    newRow[:1] = newRow[:1] + 100
                    df = df.append(newRow)
                df.to_csv(filesFolder+fileName+".csv", index=False)



# 3. Ploting the generations (x axis) and scores (y axis) 

x = range(0, maxLodLengths*100, 100)

for rewardMode in rewardModes:
    for groupMode in groupModes:
        plotName = "LOD"+"_"+str(rewardMode)+"_"+str(groupMode)
        for replicateNumber in replicateNumbers:
            fileName = "LOD"+"_"+str(rewardMode)+"_"+str(groupMode)+"_"+str(replicateNumber)
            df = pd.read_csv(filesFolder+fileName+".csv")
            plt.plot(x, df["score"])        
            plt.title(plotName)
            plt.ylim(0, 150)
            plt.xlabel('Generation')
            plt.ylabel('Score')
        plt.savefig(graphFolder+plotName+'.png')    
        plt.clf()            



# 4.

# 4.1



rawScores = []
minimums = []
maximums = []
averages = []
sums = []

for rewardMode in rewardModes:
    for groupMode in groupModes:
        plotName = "LOD"+"_"+str(rewardMode)+"_"+str(groupMode)
        for replicateNumber in replicateNumbers:
            fileName = "LOD"+"_"+str(rewardMode)+"_"+str(groupMode)+"_"+str(replicateNumber)
            df = pd.read_csv(filesFolder+fileName+".csv")
            for i in range(maxLodLengths):
                df["rawScores"][i] = df["rawScores"][i].replace('[','')
                df["rawScores"][i] = df["rawScores"][i].replace(']','')
                rawIntegers = df["rawScores"][i].split(',')
                rawScores =[float(rawIntegers[0]),float(rawIntegers[1]),float(rawIntegers[2]),float(rawIntegers[3])]
                minimums.append(min(rawScores))
                maximums.append(max(rawScores))
                averages.append(sum(rawScores)/4)
                sums.append(sum(rawScores))
            df['minimumScore'] = minimums
            df['maximumScore'] = maximums
            df['averageOfScores'] = averages
            df['sumOfScores'] = sums
            df.to_csv(filesFolder+fileName+".csv", index=False)
            minimums.clear()
            maximums.clear()
            averages.clear()
            sums.clear()


# 4.2

allReplicates = pd.DataFrame (columns = ['generation','ID','score','rawScores','ownScore','minimumScore','maximumScore','averageOfScores','sumOfScores'])

rewardGroupModes = []
meanOwnScores = []
meanMinimums = []
meanMaximums = []
meanAverages = []
meanSums = []

for rewardMode in rewardModes:
    for groupMode in groupModes:
        for replicateNumber in replicateNumbers:
            fileName = "LOD"+"_"+str(rewardMode)+"_"+str(groupMode)+"_"+str(replicateNumber)
            df = pd.read_csv(filesFolder+fileName+".csv")
            allReplicates = allReplicates.append(df)
        rewardGroupModes.append("LOD"+"_"+str(rewardMode)+"_"+str(groupMode))
        meanOwnScores.append(sum(allReplicates["ownScore"])/((numberOfReplicate+1)*maxLodLengths))
        meanMinimums.append(sum(allReplicates["minimumScore"])/((numberOfReplicate+1)*maxLodLengths))
        meanMaximums.append(sum(allReplicates["maximumScore"])/((numberOfReplicate+1)*maxLodLengths))
        meanAverages.append(sum(allReplicates["averageOfScores"])/((numberOfReplicate+1)*maxLodLengths))
        meanSums.append(sum(allReplicates["sumOfScores"])/((numberOfReplicate+1)*maxLodLengths))
        allReplicates.drop(allReplicates.index, inplace=True)

dict = {'rewardGroupModes': rewardGroupModes, 'meanOwnScores': meanOwnScores, 'meanMinimums': meanMinimums,'meanMaximums': meanMaximums,'meanAverages': meanAverages,'meanSums': meanSums}  
    
lodsStatistics = pd.DataFrame(dict)

lodsStatistics.to_csv(lodStatisticsFolder+"lodStatistics"+".csv", index=False)




'''
