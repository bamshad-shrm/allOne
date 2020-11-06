import matplotlib.pyplot as plt 
import pandas as pd

# Settings:
# Folder's path of the LOD files.
filesFolder="C:/Users/blue/Documents/study/BI/Thesis/allOne/lod/first/"
# Folder's path of the graphs
graphFolder="C:/Users/blue/Documents/study/BI/Thesis/allOne/graph/first/"
# number of replicate
numberOfReplicate = 20
replicateNumbers = range(numberOfReplicate+1)
# reward modes:
rewardModes = [0,1,2,3]
# group modes:
groupModes = [0,1]
# End of settings

lodLengths = []
maxLodLengths = 0

for rewardMode in rewardModes:
    for groupMode in groupModes:
        for replicateNumber in replicateNumbers:
            fileName = "LOD"+"_"+str(rewardMode)+"_"+str(groupMode)+"_"+str(replicateNumber)
            df = pd.read_csv(filesFolder+fileName+".csv")
            lodLengths.append(len(df.index))

maxLodLengths = max(lodLengths)
x = range(0, maxLodLengths*100, 100)

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




  
