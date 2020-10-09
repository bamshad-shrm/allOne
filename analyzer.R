# This file go through all the LOD files, calculate the average of the socres and make the graphs

# Settings:
# Folder's path of the LOD files.
filesFolder = "/home/yellow/Desktop/Study/BI/Thesis/allOne/lod/first/"
# number of replicate
numberOfReplicate = 20
replicateNumbers = c(0:numberOfReplicate)
# reward modes:
rewardModes = c(0,1,2,3)
# group modes:
groupModes = c(0,1)
# End of settings

# Create a data frame for our average scores
averageOfScores <- data.frame("replicateNumber" = replicateNumbers)

# Going through all the files
for (rewardMode in rewardModes) {
  for (groupMode in groupModes) {
    #cat(paste(filesFolder, "LOD_",rewardMode,"_",groupMode, sep = ""),"\n")
    newClmnName = paste("LOD_",rewardMode,"_",groupMode, sep = "")
    averageOfScores[newClmnName] = 0 
    for (replicateNumber in replicateNumbers) {
      #cat(paste(filesFolder, "LOD_",rewardMode,"_",groupMode,"_",replicateNumber, sep = ""),"\n")
      currentLodFile <- read.csv(file = paste(filesFolder, "LOD_",rewardMode,"_",groupMode,"_",replicateNumber,".csv", sep = ""), header = TRUE)
      #cat(paste(filesFolder, "LOD_",rewardMode,"_",groupMode,"_",replicateNumber, sep = ""),"mean socre: ",mean(currentLodFile$score),"\n")
      averageOfScores[replicateNumber+1,newClmnName] = mean(currentLodFile$score)
    }
  }
}

print(averageOfScores)


 


