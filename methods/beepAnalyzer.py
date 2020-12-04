import matplotlib.pyplot as plt 
import pandas as pd

# Settings:
# Folder's path of the LOD files.
filesFolder="C:/Users/blue/Documents/study/BI/Thesis/allOne/data/beep/second/"
# Folder's path of the lod statistics
beepStatisticsFolder="C:/Users/blue/Documents/study/BI/Thesis/allOne/results/beepStatistics/second/"
# number of replicate
numberOfReplicate = 99
replicateNumbers = range(numberOfReplicate+1)
# reward modes:
rewardModes = [0,1,2,3]
# group modes:
groupModes = [0,1]
# End of settings


bValues = []
bMinimums = []
bMaximums = []
bAverages = []
bSums = []

mValues = []
mMinimums = []
mMaximums = []
mAverages = []
mSums = []


# 1. Adding the minimum, maximum, sum and average of each row for beep and muted cases.

for rewardMode in rewardModes:
    for groupMode in groupModes:
        for replicateNumber in replicateNumbers:
            fileName = "beep"+"_"+str(rewardMode)+"_"+str(groupMode)+"_"+str(replicateNumber)
            df = pd.read_csv(filesFolder+fileName+".csv")
            for i in range(numberOfReplicate+1):
                bValues.append(df["b0"][i])
                bValues.append(df["b1"][i])
                bValues.append(df["b2"][i])
                bValues.append(df["b3"][i])
                bMinimums.append(min(bValues))
                bMaximums.append(max(bValues))
                bAverages.append(sum(bValues)/4)
                bSums.append(sum(bValues))
                bValues.clear()
                mValues.append(df["m0"][i])
                mValues.append(df["m1"][i])
                mValues.append(df["m2"][i])
                mValues.append(df["m3"][i])
                mMinimums.append(min(mValues))
                mMaximums.append(max(mValues))
                mAverages.append(sum(mValues)/4)
                mSums.append(sum(mValues))
                mValues.clear()

            df['mMinimumScore'] = mMinimums
            df['bMinimumScore'] = bMinimums
            df['mMaximumScore'] = mMaximums
            df['bMaximumScore'] = bMaximums
            df['mAverageOfScores'] = mAverages
            df['bAverageOfScores'] = bAverages
            df['mSumOfScores'] = mSums
            df['bSumOfScores'] = bSums
            
            df.to_csv(filesFolder+fileName+".csv", index=False)
            bMinimums.clear()
            bMaximums.clear()
            bAverages.clear()
            bSums.clear()
            mMinimums.clear()
            mMaximums.clear()
            mAverages.clear()
            mSums.clear()



# 2. Calculation of the mean of minimum, maximum, sum and average of all the replicates of each condition.

allReplicates = pd.DataFrame (columns = ['replicate','m0','m1','m2','m3','b0','b1','b2','b3',
                                         'mMinimumScore','bMinimumScore','mMaximumScore','bMaximumScore',
                                         'mSumOfScores','bSumOfScores','mAverageOfScores','bAverageOfScores'])

rewardGroupModes = []

bMeanMinimums = []
bMeanMaximums = []
bMeanAverages = []
bMeanSums = []

mMeanMinimums = []
mMeanMaximums = []
mMeanAverages = []
mMeanSums = []

for rewardMode in rewardModes:
    for groupMode in groupModes:
        for replicateNumber in replicateNumbers:
            fileName = "beep"+"_"+str(rewardMode)+"_"+str(groupMode)+"_"+str(replicateNumber)
            df = pd.read_csv(filesFolder+fileName+".csv")
            allReplicates = allReplicates.append(df)
        rewardGroupModes.append("beep"+"_"+str(rewardMode)+"_"+str(groupMode))
        bMeanMinimums.append(sum(allReplicates["bMinimumScore"])/((numberOfReplicate+1)*(numberOfReplicate+1)))
        bMeanMaximums.append(sum(allReplicates["bMaximumScore"])/((numberOfReplicate+1)*(numberOfReplicate+1)))
        bMeanAverages.append(sum(allReplicates["bAverageOfScores"])/((numberOfReplicate+1)*(numberOfReplicate+1)))
        bMeanSums.append(sum(allReplicates["bSumOfScores"])/((numberOfReplicate+1)*(numberOfReplicate+1)))
        mMeanMinimums.append(sum(allReplicates["mMinimumScore"])/((numberOfReplicate+1)*(numberOfReplicate+1)))
        mMeanMaximums.append(sum(allReplicates["mMaximumScore"])/((numberOfReplicate+1)*(numberOfReplicate+1)))
        mMeanAverages.append(sum(allReplicates["mAverageOfScores"])/((numberOfReplicate+1)*(numberOfReplicate+1)))
        mMeanSums.append(sum(allReplicates["mSumOfScores"])/((numberOfReplicate+1)*(numberOfReplicate+1)))

        allReplicates.drop(allReplicates.index, inplace=True)

dict = {'rewardGroupModes': rewardGroupModes,
        'bMeanMinimums': bMeanMinimums,
        'bMeanMaximums': bMeanMaximums,
        'bMeanAverages': bMeanAverages,
        'bMeanSums': bMeanSums,
        'mMeanMinimums': mMeanMinimums,
        'mMeanMaximums': mMeanMaximums,
        'mMeanAverages': mMeanAverages,
        'mMeanSums': mMeanSums}  
    
beepStatistics = pd.DataFrame(dict)

#print(beepStatistics)

beepStatistics.to_csv(beepStatisticsFolder+"beepStatistics"+".csv", index=False)




