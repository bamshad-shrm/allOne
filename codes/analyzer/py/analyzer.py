import matplotlib.pyplot as plt 
import csv


# Settings:
# Folder's path of the LOD files.
filesFolder="C:/Users/blue/Documents/study/BI/Thesis/lod/first/"
# number of replicate
numberOfReplicate = 5
replicateNumbers = range(numberOfReplicate+1)
# reward modes:
rewardModes = [0]
# group modes:
groupModes = [0]
# End of settings

x = range(0, 4800, 100)



for rewardMode in rewardModes:
    for groupMode in groupModes:
        for replicateNumber in replicateNumbers:
            y = []
            #print(rewardMode," ",groupMode," ",replicateNumber,"\n")
            fileName = "LOD"+"_"+str(rewardMode)+"_"+str(groupMode)+"_"+str(replicateNumber)
            #print(filesFolder+fileName+".csv")
            with open(filesFolder+fileName+".csv", 'r',) as file:
                reader = csv.reader(file)
                next(reader, None)
                for row in reader:
                    y.append(row[2]) 
            plt.plot(x, y)


                
plt.show() 





  
