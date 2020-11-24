import matplotlib.pyplot as plt 
import pandas as pd

# Settings:
# Folder's path of the LOD files.
filesFolder="C:/Users/blue/Documents/study/BI/Thesis/allOne/lod/second/"
# Folder's path of the graphs
graphFolder="C:/Users/blue/Documents/study/BI/Thesis/allOne/graph/second/"
# Folder's path of the lod statistics
lodStatisticsFolder="C:/Users/blue/Documents/study/BI/Thesis/allOne/lodStatistics/second/"
# number of replicate
numberOfReplicate = 99
replicateNumbers = range(numberOfReplicate+1)
# reward modes:
rewardModes = [0,1,2,3]
# group modes:
groupModes = [0,1]
# End of settings

# 1. Finding the maximum length of the lod files 

lodLengths = []
maxLodLengths = 0

for rewardMode in rewardModes:
    for groupMode in groupModes:
        for replicateNumber in replicateNumbers:
            fileName = "LOD"+"_"+str(rewardMode)+"_"+str(groupMode)+"_"+str(replicateNumber)
            df = pd.read_csv(filesFolder+fileName+".csv")
            lodLengths.append(len(df.index))

maxLodLengths = max(lodLengths)

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




